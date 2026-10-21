/*
 * SPDX-FileCopyrightText: The LXX516 Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stddef.h>
#include <stdint.h>

struct cbs_st {
    const uint8_t *data;
    size_t len;
};

extern "C" {

void* OPENSSL_sk_dup(const void* sk);

void* sk_dup(const void* sk) {
    return OPENSSL_sk_dup(sk);
}

void CBS_init(struct cbs_st *cbs, const uint8_t *data, size_t len) {
    if (cbs != nullptr) {
        cbs->data = data;
        cbs->len = len;
    }
}

}
