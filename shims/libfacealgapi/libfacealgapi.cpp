/*
 * SPDX-FileCopyrightText: The LXX516 Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include <stdlib.h>

extern "C" {

int FACE_ALG_API_logLevelCtr = 0;

void initLogLevel() {
    FACE_ALG_API_logLevelCtr = 0;
}

uint64_t getCurrentTime() {
    return 0;
}

int faceAlgInterfaceInit(void** handle, void* /*options*/) {
    if (handle) {
        if (!*handle) {
            *handle = calloc(1, 256);
        }
    }
    return 0;
}

int faceAlgInterfaceDeInit(void** handle, int /*flag*/) {
    if (handle && *handle) {
        free(*handle);
        *handle = nullptr;
    }
    return 0;
}

int faceAlgInterfaceRun(void* /*handle*/, void* /*in*/, void* /*out*/) {
    return 0;
}

int faceAlgInterfaceGetFDInfo(void* /*handle*/, void* /*info*/) {
    return 0;
}

int faceAlgInterfaceGetFAInfo(void* /*handle*/, void* /*info*/) {
    return 0;
}

int faceAlgInterfaceGetFARInfo(void* /*handle*/, void* /*info*/) {
    return 0;
}

int faceAlgInterfaceGetFA2DLandMarkInfo(void* /*handle*/, void* /*info*/) {
    return 0;
}

int faceAlgInterfaceGetLibInfoFD(void* /*info*/) {
    return 0;
}

int faceAlgInterfaceGetAlgLibInfoFAR(void* /*info*/) {
    return 0;
}

}
