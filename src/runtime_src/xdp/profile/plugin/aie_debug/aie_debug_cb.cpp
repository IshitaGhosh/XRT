// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2023 Advanced Micro Devices, Inc. All rights reserved.

#define XDP_PLUGIN_SOURCE

#include "aie_debug_cb.h"
#include "aie_debug_plugin.h"

namespace xdp {

  static AieDebugPlugin aieDebugPluginInstance;

  static void updateAIEDebugDevice(void* handle)
  {
    if (AieDebugPlugin::alive())
      aieDebugPluginInstance.updateAIEDevice(handle);
  }

  static void endAIEDebugRead(void* handle)
  {
    if (AieDebugPlugin::alive())
      aieDebugPluginInstance.endAIEDebugRead(handle);
  }

  static void scheduleConfigTxn(void* handle, uint64_t pdiId)
  {
    if (AieDebugPlugin::alive())
      aieDebugPluginInstance.scheduleConfigTxn(handle, pdiId);
  }

  static void scheduleDataFlushTxn(void* handle, uint64_t pdiId)
  {
    if (AieDebugPlugin::alive())
      aieDebugPluginInstance.scheduleDataFlushTxn(handle, pdiId);
  }

} // end namespace xdp


extern "C"
void scheduleConfigTxn(void* handle, uint64_t pdiId)
{
  xdp::scheduleConfigTxn(handle, pdiId);
}

extern "C"
void scheduleDataFlushTxn(void* handle, uint64_t pdiId)
{
  xdp::scheduleDataFlushTxn(handle, pdiId);
}

extern "C"
void updateAIEDebugDevice(void* handle)
{
  xdp::updateAIEDebugDevice(handle);
}

extern "C"
void endAIEDebugRead(void* handle)
{
  xdp::endAIEDebugRead(handle);
}
