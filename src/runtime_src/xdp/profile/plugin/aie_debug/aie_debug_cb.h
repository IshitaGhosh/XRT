// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2023 Advanced Micro Devices, Inc. All rights reserved.

#ifndef XDP_PLUGIN_AIE_DEBUG_CB_H
#define XDP_PLUGIN_AIE_DEBUG_CB_H

#include "xdp/config.h"
#include <cstdint>

extern "C"
XDP_PLUGIN_EXPORT
void updateAIEDebugDevice(void* handle);

extern "C"
XDP_PLUGIN_EXPORT
void endAIEDebugRead(void* handle);

extern "C"
XDP_PLUGIN_EXPORT
void scheduleConfigTxn(void* handle, uint64_t pdiId);

extern "C"
XDP_PLUGIN_EXPORT
void scheduleDataFlushTxn(void* handle, uint64_t pdiId);

#endif
