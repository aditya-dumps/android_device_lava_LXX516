/*
 * SPDX-FileCopyrightText: The LXX516 Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android-base/file.h>
#include <android-base/strings.h>
#include <string>
#include <vector>

namespace android {
namespace base {

std::string Basename_compat(const std::string& path) asm("_ZN7android4base8BasenameERKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEE");

std::string Basename_compat(const std::string& path) {
    return ::android::base::Basename(std::string_view(path));
}

bool WriteStringToFd_compat(const std::string& content, borrowed_fd fd) asm("_ZN7android4base15WriteStringToFdERKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEENS0_11borrowed_fdE");

bool WriteStringToFd_compat(const std::string& content, borrowed_fd fd) {
    return ::android::base::WriteStringToFd(std::string_view(content), fd);
}

std::string Join_compat(const std::vector<std::string>& list, char delimiter) asm("_ZN7android4base4JoinINSt3__16vectorINS2_12basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS7_IS9_EEEEcEES9_RKT_T0_");

std::string Join_compat(const std::vector<std::string>& list, char delimiter) {
    return ::android::base::Join(list, delimiter);
}

} // namespace base
} // namespace android
