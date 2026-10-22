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

// Intercept queueResult to prevent out-of-bounds iteration over result vector
extern "C" void _ZN10sprdcamera10camera_hal22CameraRequestAssistant11queueResultER18camera3_notify_msgR22camera3_capture_resultP17camera_frame_type(
    void* thisptr, void* msg, void* result, void* frame_type) {
    if (!thisptr) return;

    char* assistant = (char*)thisptr;
    void* head = assistant + 0xb8;
    void** node_ptr = (void**)(assistant + 0xc0);
    if (node_ptr) {
        void* node = *node_ptr;
        while (node && node != head && (uintptr_t)node > 0x10000) {
            char* req = *(char**)((char*)node + 0x10);
            if (req && (uintptr_t)req > 0x10000) {
                uintptr_t begin = *(uintptr_t*)(req + 0x50);
                uintptr_t finish = *(uintptr_t*)(req + 0x58);
                if (finish >= begin && begin > 0x10000) {
                    uint32_t count = (uint32_t)((finish - begin) / 0x40);
                    uint32_t* expected = (uint32_t*)(req + 0xa0);
                    if (*expected > count) {
                        *expected = count;
                    }
                }
            }
            node = *(void**)((char*)node + 0x8);
        }
    }

    typedef void (*pfn_queue)(void*, void*, void*, void*);
    static pfn_queue orig_queue = (pfn_queue)dlsym(
        RTLD_NEXT,
        "_ZN10sprdcamera10camera_hal22CameraRequestAssistant11queueResultER18camera3_notify_msgR22camera3_capture_resultP17camera_frame_type");
    if (orig_queue) {
        orig_queue(thisptr, msg, result, frame_type);
    }
}

// Safe implementation of lwp::Port::unlinkAll to prevent null dereference during node teardown
namespace lwp {
class Port {
public:
    void unlinkAll();
};

void Port::unlinkAll() {
    char* self = (char*)this;
    if (!self) return;

    uint32_t type = *(uint32_t*)(self + 0x50);
    void** start = *(void***)(self + 0x38);
    void** finish = *(void***)(self + 0x40);

    if (!start || !finish || start >= finish) {
        *(void***)(self + 0x40) = start;
        return;
    }

    if (type == 1) {
        Port* target = *(Port**)start;
        if (target && (uintptr_t)target > 0x10000) {
            target->unlinkAll();
        }
    } else {
        for (void** it = start; it < finish; ++it) {
            void* connected = *it;
            if (connected && (uintptr_t)connected > 0x10000) {
                char* conn_ptr = (char*)connected;
                *(void**)(conn_ptr + 0x40) = *(void**)(conn_ptr + 0x38);
            }
        }
    }
    *(void***)(self + 0x40) = start;
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
