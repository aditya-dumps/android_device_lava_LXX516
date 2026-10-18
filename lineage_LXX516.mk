#
# SPDX-FileCopyrightText: The LXX516 Project
# SPDX-License-Identifier: Apache-2.0
#

# Inherit from those products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)

# Inherit from the device configuration
$(call inherit-product, $(LOCAL_PATH)/device.mk)

# The stock system is Android 15, the vendor is API 33
PRODUCT_SHIPPING_API_LEVEL := 35

## Device identifier, this must come after all inclusions
PRODUCT_BRAND := LAVA
PRODUCT_DEVICE := LXX516
PRODUCT_MANUFACTURER := LAVA
PRODUCT_MODEL := LAVA LXX516
PRODUCT_NAME := lineage_LXX516

PRODUCT_SYSTEM_DEFAULT_LANGUAGE := en-IN
PRODUCT_SYSTEM_DEFAULT_COUNTRY := IN

PRODUCT_GMS_CLIENTID_BASE := android-lava
