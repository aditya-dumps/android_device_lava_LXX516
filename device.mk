#
# SPDX-FileCopyrightText: The LXX516 Project
# SPDX-License-Identifier: Apache-2.0
#

# Inherit some common Lineage stuff
$(call inherit-product, vendor/lineage/config/common_full_phone.mk)

# Inherit from the LXX516 vendor blobs
$(call inherit-product, vendor/lava/LXX516/LXX516-vendor.mk)

# APEX
$(call inherit-product, $(SRC_TARGET_DIR)/product/updatable_apex.mk)

# Virtual A/B with compressed snapshots
PRODUCT_VIRTUAL_AB_COMPRESSION_METHOD := lz4
PRODUCT_VIRTUAL_AB_COMPRESSION_FACTOR := 65536
$(call inherit-product, $(SRC_TARGET_DIR)/product/virtual_ab_ota.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/generic_ramdisk.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/virtual_ab_ota/vabc_features.mk)

# Boot control
PRODUCT_PACKAGES += \
    android.hardware.boot@1.2-impl \
    android.hardware.boot@1.2-impl.recovery \
    android.hardware.boot@1.2-service

# Power stats, the platform builds the service the blob list used to ship
PRODUCT_PACKAGES += \
    android.hardware.power.stats-service.example

# DRM, health and sensors, the platform builds what the blob list used to ship
PRODUCT_PACKAGES += \
    android.hardware.drm-service.clearkey \
    android.hardware.health-service.example \
    android.hardware.sensors-service.multihal \
    rkp_factory_extraction_tool

# OTA
PRODUCT_PACKAGES += \
    update_engine \
    update_engine_sideload \
    update_verifier

PRODUCT_PACKAGES += \
    checkpoint_gc \
    otapreopt_script

AB_OTA_POSTINSTALL_CONFIG += \
    RUN_POSTINSTALL_system=true \
    POSTINSTALL_PATH_system=system/bin/otapreopt_script \
    FILESYSTEM_TYPE_system=erofs \
    POSTINSTALL_OPTIONAL_system=true

AB_OTA_POSTINSTALL_CONFIG += \
    RUN_POSTINSTALL_vendor=true \
    POSTINSTALL_PATH_vendor=bin/checkpoint_gc \
    FILESYSTEM_TYPE_vendor=erofs \
    POSTINSTALL_OPTIONAL_vendor=true

# fastbootd
PRODUCT_PACKAGES += \
    android.hardware.fastboot@1.1-impl-mock \
    fastbootd

# Kernel modules, the loader config and the boot mode lists
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/prebuilts/modules/init.insmod.cfg:vendor_dlkm/lib/modules/init.insmod.cfg \
    $(LOCAL_PATH)/prebuilts/modules/modules.load.cali:vendor_dlkm/lib/modules/modules.load.cali \
    $(LOCAL_PATH)/prebuilts/modules/modules.load.charger:vendor_dlkm/lib/modules/modules.load.charger

# Symlinks declared by hand: the module loader path, toybox [, and renamed _vendor prebuilts
PRODUCT_PACKAGES += \
    vendor_lib_modules \
    vendor_bin_lbracket \
    vendor_lib64_libkeymint \
    vendor_lib64_lib_android_keymaster_keymint_utils \
    vendor_lib64_libwifi_hal \
    vendor_lib_libwifi_hal \
    vendor_lib64_libbluetooth_audio_session_aidl \
    vendor_lib64_libbluetooth_audio_session \
    vendor_lib64_audio_common_types \
    vendor_lib64_graphics_common \
    vendor_lib64_libdav1d \
    vendor_lib64_libtrusty \
    vendor_lib_libtrusty \
    vendor_lib_libbluetooth_audio_session_aidl \
    vendor_lib_libbluetooth_audio_session \
    vendor_lib_audio_common_types \
    vendor_lib_graphics_common \
    vendor_lib_libdav1d \
    vendor_lib64_graphics_allocator \
    vendor_lib_graphics_allocator \
    vendor_lib64_graphics_mapper \
    vendor_lib_graphics_mapper \
    vendor_lib64_checkkeybox \
    hwservicemanager \
    android.hardware.security.rkp-V3-ndk \
    libpower_compat \
    libwifi_compat \
    libui_compat \
    libbase_compat


# Partitions
PRODUCT_USE_DYNAMIC_PARTITIONS := true

# Rootdir, the fstab has to sit in the first stage ramdisk to mount super
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/etc/fstab.ums9621_1h10:$(TARGET_COPY_OUT_RAMDISK)/fstab.ums9621_1h10 \
    $(LOCAL_PATH)/rootdir/etc/fstab.ums9621_1h10:$(TARGET_COPY_OUT_RAMDISK)/first_stage_ramdisk/fstab.ums9621_1h10 \
    $(LOCAL_PATH)/rootdir/etc/fstab.ums9621_1h10:$(TARGET_COPY_OUT_RAMDISK)/system/etc/fstab.ums9621_1h10 \
    $(LOCAL_PATH)/rootdir/etc/fstab.ums9621_1h10:$(TARGET_COPY_OUT_RAMDISK)/first_stage_ramdisk/system/etc/fstab.ums9621_1h10 \
    $(LOCAL_PATH)/rootdir/etc/fstab.ums9621_1h10:$(TARGET_COPY_OUT_VENDOR_RAMDISK)/first_stage_ramdisk/fstab.ums9621_1h10 \
    $(LOCAL_PATH)/rootdir/etc/fstab.ums9621_1h10:$(TARGET_COPY_OUT_VENDOR_RAMDISK)/fstab.ums9621_1h10 \
    $(LOCAL_PATH)/rootdir/etc/fstab.ums9621_1h10:$(TARGET_COPY_OUT_VENDOR_RAMDISK)/system/etc/fstab.ums9621_1h10 \
    $(LOCAL_PATH)/rootdir/etc/fstab.ums9621_1h10:$(TARGET_COPY_OUT_VENDOR_RAMDISK)/first_stage_ramdisk/system/etc/fstab.ums9621_1h10 \
    $(LOCAL_PATH)/rootdir/etc/fstab.ums9621_1h10:$(TARGET_COPY_OUT_VENDOR)/etc/fstab.ums9621_1h10 \
    $(LOCAL_PATH)/rootdir/etc/fstab.ums9621_1h10:$(TARGET_COPY_OUT_VENDOR)/etc/fstab.ums9621_1h10_3le \
    $(LOCAL_PATH)/rootdir/etc/fstab.ums9621_1h10:$(TARGET_COPY_OUT_VENDOR)/etc/fstab.ums9621_1h10_go \
    $(LOCAL_PATH)/rootdir/etc/fstab.ums9621_1h10:$(TARGET_COPY_OUT_VENDOR)/etc/fstab.ums9621_1h10_uob \
    $(LOCAL_PATH)/rootdir/etc/init.trusty.rc:$(TARGET_COPY_OUT_VENDOR)/etc/init/init.trusty.rc \
    $(LOCAL_PATH)/recovery/root/ueventd.ums9621_1h10.rc:$(TARGET_COPY_OUT_VENDOR_RAMDISK)/ueventd.ums9621_1h10.rc
