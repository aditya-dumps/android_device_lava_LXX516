#!/sbin/sh
# prepdecrypt.sh - wait for the trusty IPC nodes, then signal crypto.ready
#
# keymint and gatekeeper need the trusty modules TWRP loads through
# TW_LOAD_VENDOR_BOOT_MODULES.

LOG=/tmp/prepdecrypt.log
exec > "$LOG" 2>&1

TIMEOUT=30
WAITED=0

while [ $WAITED -lt $TIMEOUT ]; do
    if ls /dev/trusty-ipc* 2>/dev/null | grep -q .; then
        echo "trusty ready after ${WAITED}s"
        break
    fi
    sleep 1
    WAITED=$((WAITED + 1))
done

if [ $WAITED -ge $TIMEOUT ]; then
    echo "timed out waiting for /dev/trusty-ipc*"
fi

# let recovery know keystore/gatekeeper can start
setprop crypto.ready 1
