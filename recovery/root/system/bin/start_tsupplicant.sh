#!/sbin/sh
# start_tsupplicant.sh - start the TEE TA supplier once odm is mounted
#
# The TEE pulls gatekeeper.elf through vendor.tsupplicant; stock runs it as a
# class core service, which recovery never starts. The mount has to exist
# first, and the supplier has to be up before the gatekeeper HAL asks.

LOG=/tmp/start_tsupplicant.log
exec > "$LOG" 2>&1

TA=/odm/firmware/gatekeeper.elf
TIMEOUT=300
WAITED=0

while [ $WAITED -lt $TIMEOUT ]; do
    if [ -e "$TA" ]; then
        echo "odm mounted after ${WAITED}s"
        break
    fi
    sleep 1
    WAITED=$((WAITED + 1))
done

if [ $WAITED -ge $TIMEOUT ]; then
    echo "$TA missing after ${TIMEOUT}s; trying supplier anyway"
fi

setprop vendor.sprd.tsupplicant.enabled 1
sleep 1
setprop twrp.tsupplicant.ready 1
