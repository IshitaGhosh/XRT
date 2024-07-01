/**
 * Copyright (C) 2023-2024 Advanced Micro Devices, Inc. - All rights reserved
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

#include "core/common/api/hw_context_int.h"
#include "core/common/device.h"
#include "core/common/message.h"
#include "core/common/query.h"
#include "core/common/query_requests.h"

#include "xdp/profile/database/database.h"
#include "xdp/profile/database/static_info/aie_util.h"
#include "xdp/profile/database/static_info/aie_constructs.h"
#include "xdp/profile/database/static_info/filetypes/base_filetype_impl.h"
#include "xdp/profile/device/common/client_transaction.h"

#include "xdp/profile/plugin/multi_pdi_prof/clientDev/multi_pdi_prof.h"
#include "xdp/profile/plugin/vp_base/utility.h"

#include <iostream>

extern "C" {
  #include <xaiengine.h>
  #include <xaiengine/xaiegbl_params.h>
}

namespace xdp {


  MultiPDIProfClientDevImpl::MultiPDIProfClientDevImpl(VPDatabase*dB)
    : MultiPDIProfImpl(dB)
  {
    xrt_core::message::send(xrt_core::message::severity_level::debug, "XRT", 
              "Created AIE Halt Plugin for Client Device.");
  }

  void MultiPDIProfClientDevImpl::updateDevice(void* /*hwCtxImpl*/)
  {
    xrt_core::message::send(xrt_core::message::severity_level::debug, "XRT", 
              "In MultiPDIProfClientDevImpl::updateDevice");

    std::cout << " 1 Wait for user input " << std::endl;
    uint32_t a;
    std::cin >> a;
    std::cout << " Got input " << a << std::endl;

    std::unique_ptr<aie::ClientTransaction> txnHandler
        = std::make_unique<aie::ClientTransaction>(mHwContext, "Multi PDI Prof");
    if (!txnHandler->initializeKernel("XDP_KERNEL"))
      return;

    const aie::BaseFiletypeImpl* metadataReader = (db->getStaticInfo()).getAIEmetadataReader();
    if (!metadataReader)
      return;

    xdp::aie::driver_config meta_config = metadataReader->getDriverConfig();

    XAie_Config cfg {
      meta_config.hw_gen,
      meta_config.base_address,
      meta_config.column_shift,
      meta_config.row_shift,
      meta_config.num_rows,
      meta_config.num_columns,
      meta_config.shim_row,
      meta_config.mem_row_start,
      meta_config.mem_num_rows,
      meta_config.aie_tile_row_start,
      meta_config.aie_tile_num_rows,
      {0} // PartProp
    };

    XAie_DevInst aieDevInst = {0};
    auto RC = XAie_CfgInitialize(&aieDevInst, &cfg);
    if (RC != XAIE_OK) {
      xrt_core::message::send(xrt_core::message::severity_level::warning, "XRT", "AIE Driver Initialization Failed.");
      return;
    }

    std::vector<profile_data_t> op_profile_data;
    //Start recording the transaction
    XAie_StartTransaction(&aieDevInst, XAIE_TRANSACTION_DISABLE_AUTO_FLUSH);
    // Profiling is 3rd custom OP
    XAie_RequestCustomTxnOp(&aieDevInst);
    XAie_RequestCustomTxnOp(&aieDevInst);
    XAie_RequestCustomTxnOp(&aieDevInst);  // Read Reg
    XAie_RequestCustomTxnOp(&aieDevInst);  // Record Timer
    XAie_RequestCustomTxnOp(&aieDevInst);  // Merge Sync
    
    auto storeAIEConfigOpCode = XAie_RequestCustomTxnOp(&aieDevInst);
    aie_profile_op_t* opAddresses;

    try {
      auto device = xrt_core::hw_context_int::get_core_device(mHwContext);
      if (!device)
        return;

      auto aiePartitionInfo = xrt_core::device_query<xrt_core::query::aie_partition_info>(device.get());
      if (aiePartitionInfo.empty()) {
        xrt_core::message::send(xrt_core::message::severity_level::info, "XRT", "No AIE partition information found.");
        return;
      }
      // For now assuming only 1
      for (auto& info : aiePartitionInfo) {
        /*
        auto startCol = static_cast<uint8_t>(info.start_col);
        xrt_core::message::send(xrt_core::message::severity_level::info, "XRT",
            "Partition shift of " + std::to_string(startCol) +
            " was found, number of columns: " + std::to_string(info.num_cols));
        */
        // Only for Core Tiles for now
        size_t opSize = sizeof(aie_profile_op_t) + (sizeof(profile_data_t)*((info.num_cols*4) - 1));
        opAddresses = (aie_profile_op_t*)malloc(opSize);

        size_t idx = 0;
        uint64_t col = info.start_col;
        for (uint64_t i = 0; i < info.num_cols; i++, col++) {
          for(uint64_t row = 2; row < 4; row++) {
            opAddresses->profile_data[idx].perf_address = (col << 25) + (row << 20);
            idx++;
          }
        }
        XAie_AddCustomTxnOp(&aieDevInst, (uint8_t)storeAIEConfigOpCode, (void*)opAddresses, opSize);
        break; // For now assuming only 1
      }
    }
    catch(...) {
      // Query not available
      xrt_core::message::send(xrt_core::message::severity_level::info, "XRT", "Unable to query AIE partition information.");
      return;
    }
    uint8_t *txnBin = XAie_ExportSerializedTransaction(&aieDevInst, 1, 0);
    // If we haven't properly initialized the transaction handler
    if (!txnHandler)
      return;
    txnHandler->setTransactionName("Multi PDI Profile Store Config");
    if (!txnHandler->submitTransaction(txnBin))
      return;

    std::cout << " 2 Wait for user input " << std::endl;
    std::cin >> a;
    std::cout << " Got input " << a << std::endl;

    XAie_ClearTransaction(&aieDevInst);
    free(opAddresses);
  }

  void MultiPDIProfClientDevImpl::finishflushDevice(void* /*hwCtxImpl*/)
  {
#if 0
#endif
  }
}

