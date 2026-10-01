# OrangeFox Recovery Device Tree for Lava Shark 5G (LXX516)

```
#
# Copyright (C) 2026 The Android Open Source Project
# Copyright (C) 2026 The OrangeFox Recovery Project
#
# SPDX-License-Identifier: Apache-2.0
#
```

## Device Specifications

| Feature | Details |
| :--- | :--- |
| Device | Lava Shark 5G |
| Codename | LXX516 |
| Chipset | Unisoc T765 (UMS9620 / UMS9230 architecture) |
| CPU | Octa-core (2x Cortex-A75 + 6x Cortex-A55) |
| Architecture | arm64-v8a |
| Screen Resolution | 720 x 1600 / 1640 |
| Partition Scheme | Virtual A/B (VAB) with Dynamic Partitions |
| Recovery Location | vendor_boot (Boot Header v4) |

## Build Instructions (GitHub Actions)

This device tree is configured to be built remotely with [Actions-Build-OrangeFox](https://github.com/aditya-dumps/Actions-Build-OrangeFox):
- **Manifest Branch**: `14.1`
- **Device Tree**: `https://github.com/aditya-dumps/device_lava_LXX516_recovery.git`
- **Device Branch**: `fox-14.1`
- **Device Path**: `device/lava/LXX516`
- **Device Name**: `LXX516`
- **Build Target**: `vendorboot`
