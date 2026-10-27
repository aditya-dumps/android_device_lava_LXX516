/*
 * SPDX-FileCopyrightText: The LXX516 Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include <malloc.h>
#include <sys/prctl.h>

#ifndef PR_SET_TAGGED_ADDR_CTRL
#define PR_SET_TAGGED_ADDR_CTRL 55
#endif

#if defined(__aarch64__)
__attribute__((constructor)) static void disable_pointer_tagging() {
    mallopt(M_BIONIC_SET_HEAP_TAGGING_LEVEL, M_HEAP_TAGGING_LEVEL_NONE);
    prctl(PR_SET_TAGGED_ADDR_CTRL, 0, 0, 0, 0);
}
#endif

struct native_handle;

namespace android {

class Rect;
class GraphicBufferMapper;

extern "C" {
int32_t _ZN7android19GraphicBufferMapper4lockEPK13native_handlejRKNS_4RectEPPv(
    GraphicBufferMapper* thisptr, const struct native_handle* handle,
    uint32_t usage, const Rect& bounds, void** vaddr);

int32_t _ZN7android19GraphicBufferMapper6unlockEPK13native_handlePNS_4base14unique_fd_implINS4_13DefaultCloserEEE(
    GraphicBufferMapper* thisptr, const struct native_handle* handle, void* outFence);
}

extern "C" {

int32_t _ZN7android19GraphicBufferMapper4lockEPK13native_handlejRKNS_4RectEPPvPiS9_(
    GraphicBufferMapper* thisptr, const struct native_handle* handle, uint32_t usage,
    const Rect& bounds, void** vaddr, int32_t* outBytesPerPixel, int32_t* outBytesPerStride) {
    if (outBytesPerPixel) *outBytesPerPixel = -1;
    if (outBytesPerStride) *outBytesPerStride = -1;
    return _ZN7android19GraphicBufferMapper4lockEPK13native_handlejRKNS_4RectEPPv(
        thisptr, handle, usage, bounds, vaddr);
}

int32_t _ZN7android19GraphicBufferMapper6unlockEPK13native_handle(
    GraphicBufferMapper* thisptr, const struct native_handle* handle) {
    return _ZN7android19GraphicBufferMapper6unlockEPK13native_handlePNS_4base14unique_fd_implINS4_13DefaultCloserEEE(
        thisptr, handle, nullptr);
}

} // extern "C"

// Suppress fatal abort when HIDL return status is unchecked or transport fails
namespace hardware {
namespace details {
extern "C" void _ZNK7android8hardware7details13return_status8assertOkEv(const void* /*thisptr*/) {
    // No-op: Prevent camera provider crash on dropped frames / BAD_VALUE transport error
}
}
}

} // namespace android
