/**
 * Copyright (C) 2025 Advanced Micro Devices, Inc. - All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You may
 * not use this file except in compliance with the License. A copy of the
 * License is located at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

#define XDP_PLUGIN_SOURCE

#include "xdp/profile/plugin/gen_asm_elf/gen_asm_elf_cb.h"
#include "xdp/profile/plugin/gen_asm_elf/gen_asm_elf_plugin.h"

namespace xdp {

  static GenAsmElfPlugin genAsmElfPluginInstance;

  static void updateDeviceGenAsmElf(void* hwCtxImpl)
  {
    if (GenAsmElfPlugin::alive()) {
      genAsmElfPluginInstance.updateDevice(hwCtxImpl);
    } 
  } 

  static void finishflushDeviceGenAsmElf(void* hwCtxImpl)
  {
    if (GenAsmElfPlugin::alive()) {
      genAsmElfPluginInstance.finishflushDevice(hwCtxImpl);
    } 
  } 

} // end namespace xdp

extern "C"
void updateDeviceGenAsmElf(void* hwCtxImpl)
{
  xdp::updateDeviceGenAsmElf(hwCtxImpl);
}

extern "C"
void finishflushDeviceGenAsmElf(void* hwCtxImpl)
{
  xdp::finishflushDeviceGenAsmElf(hwCtxImpl);
}
