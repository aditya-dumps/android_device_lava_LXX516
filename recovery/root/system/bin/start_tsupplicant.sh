#!/sbin/sh
# start_tsupplicant.sh - setup partitions & start TEE services for FBE decrypt
#
# On Unisoc T765 (ums9621_1h10):
# 1. Gatekeeper TA (gatekeeper.elf) resides in /odm/firmware/
# 2. Gatekeeper failure records are stored in /mnt/vendor/productinfo/sprd_ss (prodnv partition)
# 3. Trusty storage proxies (ns, prodnv, teens) + rpmbserver are mandatory for Gatekeeper and KeyMint

LOG=/tmp/start_tsupplicant.log
exec > "$LOG" 2>&1

echo "start_tsupplicant: starting storage & TEE setup"

# 1. Mount /vendor (needed for binaries & libraries)
mkdir -p /vendor
if ! mount | grep -q ' /vendor '; then
    mount -t erofs -o ro /dev/block/mapper/vendor_a /vendor 2>/dev/null || \
    mount -t erofs -o ro /dev/block/mapper/vendor_b /vendor 2>/dev/null || \
    mount -t erofs -o ro /dev/block/by-name/vendor /vendor 2>/dev/null || \
    mount -t erofs -o ro /dev/block/dm-5 /vendor 2>/dev/null || \
    mount -o ro /dev/block/by-name/vendor /vendor 2>/dev/null
fi

# 2. Mount /odm (contains gatekeeper.elf)
mkdir -p /odm
if ! mount | grep -q ' /odm '; then
    mount -t erofs -o ro /dev/block/mapper/odm_a /odm 2>/dev/null || \
    mount -t erofs -o ro /dev/block/mapper/odm_b /odm 2>/dev/null || \
    mount -t erofs -o ro /dev/block/by-name/odm /odm 2>/dev/null || \
    mount -t erofs -o ro /dev/block/dm-0 /odm 2>/dev/null || \
    mount -o ro /dev/block/by-name/odm /odm 2>/dev/null
fi

# 3. Mount /mnt/vendor (prodnv partition: holds GateKeeper failure records)
mkdir -p /mnt/vendor
if ! mount | grep -q ' /mnt/vendor '; then
    mount -t ext4 /dev/block/by-name/prodnv /mnt/vendor 2>/dev/null || \
    mount -t ext4 /dev/block/mmcblk0p1 /mnt/vendor 2>/dev/null
fi

# 4. Start RPMB server
if [ -e /dev/mmcblk0rpmb ] && [ -x /vendor/bin/rpmbserver ]; then
    /vendor/bin/rpmbserver -r /dev/mmcblk0rpmb &
fi

# 5. Start secure storage proxies
if [ -x /vendor/bin/sprdstorageproxyd ]; then
    # Non-secure storage proxy
    mkdir -p /data/vendor/sprd_ss
    /vendor/bin/sprdstorageproxyd -f ns -d /dev/trusty-ipc-dev0 -p /data/vendor/sprd_ss &

    # Prodnv storage proxy (holds GateKeeper failure records)
    mkdir -p /mnt/vendor/productinfo/sprd_ss
    /vendor/bin/sprdstorageproxyd -f prodnv -d /dev/trusty-ipc-dev0 -p /mnt/vendor/productinfo/sprd_ss &

    # GP TEE storage proxy
    mkdir -p /data/vendor/sprd_tee_ss
    /vendor/bin/sprdstorageproxyd -f teens -d /dev/trusty-ipc-dev0 -p /data/vendor/sprd_tee_ss &
fi

# 6. Start tsupplicant (loads gatekeeper.elf from /odm/firmware)
setprop vendor.sprd.tsupplicant.enabled 1

# 7. Wait for TEE sessions to establish, then signal ready
sleep 3
setprop twrp.tsupplicant.ready 1
echo "start_tsupplicant: all TEE services started successfully"
