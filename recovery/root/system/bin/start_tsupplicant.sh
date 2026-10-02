#!/sbin/sh
# start_tsupplicant.sh - start the TEE TA supplier
#
# The TEE pulls gatekeeper.elf through vendor.tsupplicant; stock runs it as a
# class core service, which recovery never starts. TWRP never mounts odm and
# nothing else mounts it here, so do it up front instead of waiting.
#
# On Unisoc T765 (ums9621) the GateKeeper TA is embedded in the trustos
# firmware rather than shipped as a standalone .elf file, so gatekeeper.elf
# will not be found - that is expected and harmless.

LOG=/tmp/start_tsupplicant.log
exec > "$LOG" 2>&1

echo "start_tsupplicant: starting"

# Ensure vendor is mounted (needed for tsupplicant binary and libraries)
if ! mount | grep -q ' /vendor '; then
    mount -t erofs -o ro /dev/block/by-name/vendor /vendor 2>/dev/null || \
    mount -t erofs -o ro /dev/block/dm-5 /vendor 2>/dev/null || \
    mount -o ro /dev/block/by-name/vendor /vendor 2>/dev/null
fi

# Try to mount odm so tsupplicant can find any .elf TAs stored there
if ! mount | grep -q ' /odm '; then
    mount -t erofs -o ro /dev/block/by-name/odm /odm 2>/dev/null || \
    mount -t erofs -o ro /dev/block/dm-0 /odm 2>/dev/null || \
    mount -t erofs -o ro /dev/block/mapper/odm_a /odm 2>/dev/null || \
    mount -o ro /dev/block/by-name/odm /odm 2>/dev/null
fi

# Check firmware paths (informational only - GK TA may be in trustos firmware)
for path in /odm/firmware/gatekeeper.elf /vendor/firmware/gatekeeper.elf; do
    if [ -e "$path" ]; then
        echo "Found TA: $path"
    fi
done

# Start the TA supplier - passes search dirs as args
setprop vendor.sprd.tsupplicant.enabled 1

# Wait for tsupplicant to register with TEE before signalling ready.
# Increase to 3s to give Trusty IPC time to fully establish on slower boots.
sleep 3
setprop twrp.tsupplicant.ready 1
echo "supplier started"
