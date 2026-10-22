/*
 * SPDX-FileCopyrightText: The LXX516 Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include <malloc.h>
#include <dlfcn.h>
#include <sys/prctl.h>

#ifndef PR_SET_TAGGED_ADDR_CTRL
#define PR_SET_TAGGED_ADDR_CTRL 55
#endif

#if defined(__aarch64__)
__attribute__((constructor)) static void disable_pointer_tagging() {
    mallopt(M_BIONIC_SET_HEAP_TAGGING_LEVEL, M_HEAP_TAGGING_LEVEL_NONE);
    prctl(PR_SET_TAGGED_ADDR_CTRL, 0, 0, 0, 0);
}

// Intercept CameraRequestAssistant::processCaptureResult to avoid SIGSEGV when
// num_output_buffers == 0 or output_buffers points to an invalid/null address.
extern "C" void _ZN10sprdcamera10camera_hal22CameraRequestAssistant20processCaptureResultER22camera3_capture_result(
    void* thisptr, void* result) {
    if (!result) return;

    uint32_t num_output_buffers = *(uint32_t*)((char*)result + 0x10);
    uintptr_t output_buffers = *(uintptr_t*)((char*)result + 0x18);

    if (num_output_buffers == 0 || output_buffers < 0x10000) {
        void* adapter = *(void**)((char*)thisptr + 0x110);
        if (adapter) {
            void** cb_ops = *(void***)((char*)adapter + 0x220);
            if (cb_ops && *cb_ops) {
                typedef void (*pfn_process_res)(const void*, const void*);
                pfn_process_res cb = (pfn_process_res)(*cb_ops);
                cb(cb_ops, result);
            }
        }
        return;
    }

    uintptr_t stream = *(uintptr_t*)output_buffers;
    if (stream < 0x10000) {
        void* adapter = *(void**)((char*)thisptr + 0x110);
        if (adapter) {
            void** cb_ops = *(void***)((char*)adapter + 0x220);
            if (cb_ops && *cb_ops) {
                typedef void (*pfn_process_res)(const void*, const void*);
                pfn_process_res cb = (pfn_process_res)(*cb_ops);
                cb(cb_ops, result);
            }
        }
        return;
    }

    typedef void (*pfn_orig)(void*, void*);
    static pfn_orig orig_fn = (pfn_orig)dlsym(
        RTLD_NEXT,
        "_ZN10sprdcamera10camera_hal22CameraRequestAssistant20processCaptureResultER22camera3_capture_result");
    if (orig_fn) {
        orig_fn(thisptr, result);
    }
}

// Suppress fatal abort when HIDL return status is unchecked or transport fails
namespace android {
namespace hardware {
namespace details {
extern "C" void _ZNK7android8hardware7details13return_status8assertOkEv(const void* /*thisptr*/) {
    // No-op: Prevent camera provider crash on dropped frames / BAD_VALUE transport error
}
}
}
}

// Fallback stubs for Unisoc FaceDetect/MNN algorithms
extern "C" {
int faceAlgInterfaceInit(void** handle, void*) {
    if (handle && !*handle) {
        *handle = calloc(1, 256);
    }
    return 0;
}
int faceAlgInterfaceDeInit(void** handle, int) {
    if (handle && *handle) {
        free(*handle);
        *handle = nullptr;
    }
    return 0;
}
int faceAlgInterfaceRun(void*, void*, void*) { return 0; }
int faceAlgInterfaceGetFDInfo(void*, void*) { return 0; }
int faceAlgInterfaceGetFAInfo(void*, void*) { return 0; }
int faceAlgInterfaceGetFARInfo(void*, void*) { return 0; }
int faceAlgInterfaceGetFA2DLandMarkInfo(void*, void*) { return 0; }
int faceAlgInterfaceGetLibInfoFD(void*) { return 0; }
int faceAlgInterfaceGetAlgLibInfoFAR(void*) { return 0; }
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

} // namespace android
