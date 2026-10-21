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

extern "C" {

std::string _ZN7android4base8BasenameERKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEE(
    const std::string& path) {
    return ::android::base::Basename(std::string_view(path));
}

bool _ZN7android4base15WriteStringToFdERKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEENS0_11borrowed_fdE(
    const std::string& content, borrowed_fd fd) {
    return ::android::base::WriteStringToFd(std::string_view(content), fd);
}

std::string _ZN7android4base4JoinINSt3__16vectorINS2_12basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS7_IS9_EEEEcEES9_RKT_T0_(
    const std::vector<std::string>& list, char delimiter) {
    return ::android::base::Join(list, delimiter);
}

} // extern "C"

} // namespace base
} // namespace android
