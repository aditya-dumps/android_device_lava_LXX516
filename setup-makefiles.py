#!/usr/bin/env -S PYTHONPATH=../../../tools/extract-utils python3
#
# SPDX-FileCopyrightText: The Z2577 Project
# SPDX-License-Identifier: Apache-2.0
#
# Regenerate the vendor makefiles, then drop the prebuilt prefer flags. A
# prebuilt that claims an AOSP module of the same name on another partition is
# what Soong rejects, and where the platform builds a vendor variant that copy
# is the right one anyway. tools/preflight.py checks the result.
#

import os
import pathlib
import re
import subprocess
import sys

HERE = pathlib.Path(__file__).resolve().parent
VENDOR_DIR = (HERE / '../../../vendor/lava/LXX516').resolve()
VENDOR_BP = VENDOR_DIR / 'Android.bp'
VENDOR_MK = VENDOR_DIR / 'Android.mk'
VENDOR_PRODUCT_MK = VENDOR_DIR / 'LXX516-vendor.mk'

PREFER = re.compile(r'^    prefer: true,\n', re.M)

subprocess.run(
    [sys.executable, str(HERE / 'extract-files.py'), '--regenerate_makefiles'],
    env=os.environ,
    check=True,
)

bp = VENDOR_BP.resolve()
text = bp.read_text()
text, count = PREFER.subn('', text)
bp.write_text(text)

if count:
    print(f'dropped {count} prefer: true lines from {bp}')

# Convert overlay APKs from PRODUCT_COPY_FILES to BUILD_PREBUILT in Android.mk.
# build/make/core/Makefile forbids prebuilt apks in PRODUCT_COPY_FILES.
if VENDOR_PRODUCT_MK.exists():
    vendor_mk_text = VENDOR_PRODUCT_MK.read_text()

    copy_pattern = re.compile(
        r'^\s*vendor/lava/LXX516/(proprietary/(?:vendor|odm)/([^\s:]+?\.apk)):\$\(TARGET_COPY_OUT_(VENDOR|ODM)\)/([^\s\\]+)\s*(?:\\?)\n',
        re.M,
    )
    matches = copy_pattern.findall(vendor_mk_text)

    if matches:
        new_vendor_mk, removed_count = copy_pattern.subn('', vendor_mk_text)

        mk_rules = []
        pkg_names = []
        for full_src, rel_apk, part, dest in matches:
            filename = os.path.basename(rel_apk)
            stem = filename.removesuffix('.apk')
            # Avoid collision with platform modules
            mod_name = stem if stem != 'NetworkStackOverlay' else f'{stem}_vendor'
            dest_dir = os.path.dirname(dest)
            dest_path = f'$(TARGET_OUT_{part})/{dest_dir}' if dest_dir else f'$(TARGET_OUT_{part})'

            rule = [
                'include $(CLEAR_VARS)',
                f'LOCAL_MODULE := {mod_name}',
            ]
            if mod_name != stem:
                rule += [
                    f'LOCAL_MODULE_STEM := {stem}',
                    f'LOCAL_INSTALLED_MODULE_STEM := {filename}',
                ]
            rule += [
                'LOCAL_MODULE_TAGS := optional',
                f'LOCAL_SRC_FILES := {full_src}',
                'LOCAL_MODULE_CLASS := APPS',
                'LOCAL_MODULE_SUFFIX := $(COMMON_ANDROID_PACKAGE_SUFFIX)',
                'LOCAL_CERTIFICATE := PRESIGNED',
                'LOCAL_DEX_PREOPT := false',
                'LOCAL_ENFORCE_USES_LIBRARIES := false',
                f'LOCAL_{part}_MODULE := true',
                f'LOCAL_MODULE_PATH := {dest_path}',
                'include $(BUILD_PREBUILT)',
            ]
            mk_rules.append('\n'.join(rule))
            pkg_names.append(mod_name)

        # Update Android.mk
        android_mk_text = VENDOR_MK.read_text() if VENDOR_MK.exists() else ''
        if 'LOCAL_PATH := $(call my-dir)' not in android_mk_text:
            android_mk_text = '#\n# Automatically generated file. DO NOT MODIFY\n#\n\nLOCAL_PATH := $(call my-dir)\n'
        android_mk_text = android_mk_text.rstrip() + '\n\n' + '\n\n'.join(mk_rules) + '\n'
        VENDOR_MK.write_text(android_mk_text)

        # Update LXX516-vendor.mk PRODUCT_PACKAGES
        pkg_lines = ' \\\n    '.join(pkg_names)
        new_vendor_mk = new_vendor_mk.rstrip() + f'\n\nPRODUCT_PACKAGES += \\\n    {pkg_lines}\n'
        VENDOR_PRODUCT_MK.write_text(new_vendor_mk)

        print(f'Converted {removed_count} overlay APKs from PRODUCT_COPY_FILES to BUILD_PREBUILT in Android.mk')

