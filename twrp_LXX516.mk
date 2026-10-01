#
# Copyright (C) 2026 The Android Open Source Project
#
# SPDX-License-Identifier: Apache-2.0
#

DEVICE_PATH := device/lava/LXX516

# Inherit from those products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/base.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)

# Inherit common TWRP / OrangeFox stuff
$(call inherit-product, vendor/twrp/config/common.mk)

# Inherit OrangeFox specific settings
$(call inherit-product-if-exists, $(DEVICE_PATH)/fox_LXX516.mk)

# Inherit from LXX516 device configuration
$(call inherit-product, $(DEVICE_PATH)/device.mk)

PRODUCT_DEVICE := LXX516
PRODUCT_NAME := twrp_LXX516
PRODUCT_BRAND := lava
PRODUCT_MODEL := LXX516
PRODUCT_MANUFACTURER := lava
