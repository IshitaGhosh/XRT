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

#include "core/common/api/bo_int.h"
#include "core/common/api/hw_context_int.h"
#include "core/common/device.h"
#include "core/common/message.h"

#include "core/include/xrt/experimental/xrt_elf.h"
#include "core/include/xrt/experimental/xrt_ext.h"
#include "core/include/xrt/experimental/xrt_module.h"

#include "core/include/xrt/xrt_bo.h"
#include "core/include/xrt/xrt_kernel.h"

#include "xdp/profile/plugin/gen_asm_elf/ve2/gen_asm_elf.h"
#include "xdp/profile/plugin/vp_base/utility.h"

extern "C" {
  #include <xaiengine.h>
  #include <xaiengine/xaiegbl_params.h>
}

namespace xdp {

  class ResultBOContainer
  {
    public:
      xrt::bo  mBO;
      ResultBOContainer(void* hwCtxImpl, uint32_t sz)
      {
        mBO = xrt_core::bo_int::create_debug_bo(
                xrt_core::hw_context_int::create_hw_context_from_implementation(hwCtxImpl),
                sz);
      }
      ~ResultBOContainer() {}

      void
      syncFromDevice()
      {
        mBO.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
      }
      uint32_t*
      map()
      {
        return mBO.map<uint32_t*>();
      }
  };

  GenAsmElfVE2Impl::GenAsmElfVE2Impl(VPDatabase*dB)
    : GenAsmElfImpl(dB)
  {
  }

  void GenAsmElfVE2Impl::updateDevice(void* hwCtxImpl)
  {
    xrt_core::message::send(xrt_core::message::severity_level::debug, "XRT",
              "In GenAsmElfVE2Impl::updateDevice");
    std::unique_ptr<ResultBOContainer> mResultBOHolder;
    try {
      mResultBOHolder = std::make_unique<xdp::ResultBOContainer>(hwCtxImpl, 0x30000);
      memset(mResultBOHolder->map(), 0, 0x30000);

    } catch (std::exception& e) {
      std::stringstream msg;
      msg << "Unable to create/initialize result buffer of size 0x30000 "
          << " Bytes for Record Timer Values. Cannot get info. "
          << e.what() << std::endl;
      xrt_core::message::send(xrt_core::message::severity_level::warning, "XRT", msg.str());
      return;
    }
    xrt_core::message::send(xrt_core::message::severity_level::debug, "XRT",
              "Allocated buffer In MLTimelineVE2Impl::updateDevice");

    xrt_core::message::send(xrt_core::message::severity_level::debug, "XRT",
              "New Control Code In MLTimelineVE2Impl::updateDevice");

//XAie_Config ve2Cfg {5, 2199023255552, 25, 20, 7, 36, 0, 1, 2, 3, 4, {0}};
/*
        "driver_config": {
            "hw_gen": 5,
            "base_address": 2199023255552,
            "npi_address": 4141154304,
            "column_shift": 25,
            "row_shift": 20,
            "num_rows": 7,
            "num_columns": 36,
            "shim_row": 0,
            "mem_tile_row_start": 1,
            "mem_tile_num_rows": 2,
            "aie_tile_row_start": 3,
            "aie_tile_num_rows": 4
        }
*/



XAie_DevInst aieDevInst = {0};

XAie_Config ve2Cfg {5, 2199023255552, 25, 20, 7, 36, 0, 1, 2, 3, 4, {0}};
// old XAie_Config ve2Cfg {5, 2199023255552, 25, 20, 7, 36, 0, 1, 2, 3, 4, {0}};
    
//    XAie_SetupConfig(ve2Cfg, 5, 2199023255552, 25, 20, 36, 7, 0, 1, 2, 3, 4);
//    XAie_InstDeclare(aieDevInst, &ve2Cfg);
//      XAie_CfgInitialize(&(aieDevInst), &ve2Cfg);
#if 0
      XAie_SetupConfig(ConfigPtr, HW_GEN, X_BASE_ADDR, X_COL_SHIFT,
                       X_ROW_SHIFT, X_NUM_COLS, X_NUM_ROWS,
                       X_SHIM_ROW, X_MEM_TILE_ROW_START,
                       X_MEM_TILE_NUM_ROWS, X__TILE_ROW_START,
                       X__TILE_NUM_ROWS);

      XAie_CfgInitialize(&(DevInst), &ConfigPtr)
#endif



 XAie_CfgInitialize(&aieDevInst, &ve2Cfg);
#if 0
auto RC = XAie_CfgInitialize(&aieDevInst, &ve2Cfg);
if (RC != X_OK)
  xrt_core::message::send(xrt_core::message::severity_level::warning, "XRT", " Driver Initialization Failed.");
#endif

  XAie_SetIOBackend(&aieDevInst, XAIE_IO_BACKEND_CONTROLCODE);
  XAie_OpenControlCodeFile(&aieDevInst, "gen5svt.asm", 0x2000);
  XAie_StartNewJob(&aieDevInst);
  XAie_ControlCodeSaveTimestamp(&aieDevInst, 1212);
  XAie_ControlCodeSaveTimestamp(&aieDevInst, 150);
  XAie_ControlCodeSaveTimestamp(&aieDevInst, 44);
  XAie_ControlCodeSaveTimestamp(&aieDevInst, 8787);
  XAie_ControlCodeSaveTimestamp(&aieDevInst, 6565);
  XAie_EndJob(&aieDevInst);
  XAie_EndPage(&aieDevInst);
  XAie_CloseControlCodeFile(&aieDevInst);

    xrt_core::message::send(xrt_core::message::severity_level::debug, "XRT",
              "New Control Code ASM gen5svt.asm should have been generated");

#if 0
  std::vector<char> control_code_buf; // fill with asm content
    std::string inputCtrlCode = xrt_core::config::get_aie_halt_settings_control_code();
    if (inputCtrlCode.empty()) {
      xrt_core::message::send(xrt_core::message::severity_level::debug, "XRT",
                "No input control code file for  GenAsmElf provided. Defaulting to \"aieGenAsmElf4x4.elf\".");
      inputCtrlCode = "aieGenAsmElf4x4.elf";
    }
    
    xrt::hw_context hwContext = xrt_core::hw_context_int::create_hw_context_from_implementation(hwCtxImpl);

    xrt::elf haltElf;
    try {
      haltElf = xrt::elf(inputCtrlCode);
    } catch (...) {
      std::string msg = "Failed to load " + inputCtrlCode + ". Cannot configure  to halt.";
      xrt_core::message::send(xrt_core::message::severity_level::warning, "XRT", msg);
      return;
    }

    xrt::module mod{haltElf};
    xrt::kernel krnl;
    try {
      krnl = xrt::ext::kernel{hwContext, mod, "XDP_KERNEL:{IPUV1CNN}"};
    } catch (...) {
      xrt_core::message::send(xrt_core::message::severity_level::warning, "XRT",
                "XDP_KERNEL not found in HW Context. Cannot configure  to halt.");
      return;
    }

    xrt_core::message::send(xrt_core::message::severity_level::debug, "XRT",
              "In GenAsmElfVE2Impl New Kernel Object for XDP_KERNEL created for running control code Elf");      

    xrt::run rn{krnl};
    rn.start();
    xrt_core::message::send(xrt_core::message::severity_level::debug, "XRT",
              "In GenAsmElfVE2Impl run start, going to wait");  

    rn.wait2();
#endif
  }

  void GenAsmElfVE2Impl::finishflushDevice(void* /*hwCtxImpl*/)
  {
  }
}
