/*
 * SPDX-FileCopyrightText: The LXX516 Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <processgroup/processgroup.h>
#include <string>
#include <vector>

bool SetTaskProfiles_compat(int tid, const std::vector<std::string>& profiles, bool use_fd_cache) asm("SetTaskProfiles");

bool SetTaskProfiles_compat(int tid, const std::vector<std::string>& profiles, bool use_fd_cache) {
    return SetTaskProfiles(tid, profiles, use_fd_cache);
}
