/*
 * SPDX-FileCopyrightText: The LXX516 Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ui/GraphicBufferMapper.h>
#include <ui/Rect.h>
#include <cutils/native_handle.h>

namespace android {

extern "C" {

status_t _ZN7android19GraphicBufferMapper4lockEPK13native_handlejRKNS_4RectEPPvPiS9_(
    GraphicBufferMapper* thisptr, buffer_handle_t handle, uint32_t usage,
    const Rect& bounds, void** vaddr, int32_t* outBytesPerPixel, int32_t* outBytesPerStride) {
    if (outBytesPerPixel) *outBytesPerPixel = -1;
    if (outBytesPerStride) *outBytesPerStride = -1;
    if (thisptr != nullptr) {
        return thisptr->lock(handle, usage, bounds, vaddr);
    }
    return GraphicBufferMapper::getInstance().lock(handle, usage, bounds, vaddr);
}

status_t _ZN7android19GraphicBufferMapper6unlockEPK13native_handle(
    GraphicBufferMapper* thisptr, buffer_handle_t handle) {
    if (thisptr != nullptr) {
        return thisptr->unlock(handle, nullptr);
    }
    return GraphicBufferMapper::getInstance().unlock(handle, nullptr);
}

} // extern "C"

} // namespace android
