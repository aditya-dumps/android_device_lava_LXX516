# Device tree for the Lava Shark 5G (LXX516)

The Lava Shark 5G (codenamed "LXX516") is a smartphone from LAVA International.

## Device specifications

| Basic                   | Spec                                                        |
| ----------------------- | :---------------------------------------------------------- |
| SoC                     | Unisoc UMS9621S                                             |
| CPU                     | Octa-core (2x Cortex-A76 & 6x Cortex-A55)                   |
| GPU                     | Mali-G57                                                    |
| Memory                  | 4 GB / 6 GB LPDDR4X                                         |
| Shipped Android version | 15                                                          |
| Storage                 | 64 GB / 128 GB eMMC 5.1                                     |
| MicroSD                 | microSDXC                                                   |
| Battery                 | Non-removable Li-Polymer 5000 mAh                           |
| Display                 | IPS LCD, 720 x 1600 pixels                                  |
| Rear Camera             | 13 MP, f/2.2                                                |
| Front Camera            | 5 MP                                                        |

## Device Picture

![Lava Shark 5G](docs/lxx516.jpg)

## Special Thanks & Credits

A huge thank you and massive credits go to **[@XTENSEI](https://github.com/XTENSEI)**!

He did most of the heavy lifting, foundational bringup work, reverse engineering, and tooling for this Unisoc platform base tree and vendor configuration. This port builds directly upon his extensive efforts and expertise.

## Extracting the blobs

`proprietary-files.txt` is the list and the vendor repo is its generated output.
Both need a LineageOS / crDroid checkout, with this tree at `device/lava/LXX516` and the
blobs at `vendor/lava/LXX516`:

    ./extract-files.py
    ./setup-makefiles.py

Soong only accepts letters, digits and `_ . @ + -` in a module name, and
`build/tools/extract-utils` builds one straight from the path, so
`vendor/bin/[` becomes `vendor_bin_[`, which Soong rejects. File names have the
same problem: `lib{bar}.so` would be named `lib{bar}`.
`docs/extract-utils-module-name.patch` covers both, leaving legal names alone.

The patch is lost on every `repo sync`, because `build/tools/extract-utils`
belongs to the LineageOS checkout and not to either repo, so the one symlink it
affects is declared by hand in `Android.bp`. The blob list no longer carries
that entry, so a regeneration never competes with it.
