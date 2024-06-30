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


#include "core/common/device.h"
#include "core/common/message.h"

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
        = std::make_unique<aie::ClientTransaction>(mHwContext, "AIE Halt");
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

    XAie_StartTransaction(&aieDevInst, XAIE_TRANSACTION_DISABLE_AUTO_FLUSH);

    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(0, 2));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(0, 3));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(0, 4));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(0, 5));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(1, 2));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(1, 3));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(1, 4));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(1, 5));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(2, 2));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(2, 3));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(2, 4));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(2, 5));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(3, 2));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(3, 3));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(3, 4));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(3, 5));
#if 0
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(0, 2));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(0, 3));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(0, 4));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(0, 5));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(1, 2));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(1, 3));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(1, 4));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(1, 5));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(2, 2));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(2, 3));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(2, 4));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(2, 5));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(3, 2));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(3, 3));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(3, 4));
    XAie_CoreDebugHalt(&aieDevInst, XAie_TileLoc(3, 5));
#endif

    uint8_t* txnBin = XAie_ExportSerializedTransaction(&aieDevInst, 1, 0);

    std::ofstream txnBinF("coreHalt.bin", std::ios::binary);
    XAie_TxnHeader *txnHdr = (XAie_TxnHeader*)txnBin;
    txnBinF.write((const char*)txnBin, txnHdr->TxnSize);

    if (!txnHandler->submitTransaction(txnBin))
      return;

    std::cout << " 2 Wait for user input " << std::endl;
    std::cin >> a;
    std::cout << " Got input " << a << std::endl;


    XAie_ClearTransaction(&aieDevInst);

  #if 0
    try {

      /* Use a container for Debug BO for results to control its lifetime.
       * The result BO should be deleted after reading out recorded data in
       * finishFlushDevice so that AIE Profile/Debug Plugins, if enabled,
       * can use their own Debug BO to capture their data.
       */
      mResultBOHolder = new ResultBOContainer(mHwContext, mBufSz);

    } catch (std::exception& e) {
      std::stringstream msg;
      msg << "Unable to create result buffer of size "
          << std::hex << mBufSz << std::dec
          << " Bytes for Record Timer Values. Cannot get ML Timeline info. " 
          << e.what() << std::endl;
      xrt_core::message::send(xrt_core::message::severity_level::warning, "XRT", msg.str());
      return;
    }
    xrt_core::message::send(xrt_core::message::severity_level::debug, "XRT", 
              "Allocated buffer In MultiPDIProfClientDevImpl::updateDevice");
  #endif
  }

  void MultiPDIProfClientDevImpl::finishflushDevice(void* /*hwCtxImpl*/)
  {
#if 0
    xrt_core::message::send(xrt_core::message::severity_level::debug, "XRT", 
              "Using Allocated buffer In MultiPDIProfClientDevImpl::finishflushDevice");
              
    mResultBOHolder->syncFromDevice();    
    uint32_t* ptr = mResultBOHolder->map();
      
    boost::property_tree::ptree ptTop;
    boost::property_tree::ptree ptHeader;
    boost::property_tree::ptree ptRecordTimerTS;

    // Header for JSON 
    ptHeader.put("date", xdp::getCurrentDateTime());
    ptHeader.put("time_created", xdp::getMsecSinceEpoch());

    boost::property_tree::ptree ptSchema;
    ptSchema.put("major", "1");
    ptSchema.put("minor", "0");
    ptSchema.put("patch", "0");
    ptHeader.add_child("schema_version", ptSchema);
    ptHeader.put("device", "Client");
    ptHeader.put("clock_freq_MHz", 1000);
    ptTop.add_child("header", ptHeader);

    // Record Timer TS in JSON
    // Assuming correct Stub has been called and Write Buffer contains valid data
    
    uint32_t max_count = mBufSz / (2*sizeof(uint32_t));
    // Each record timer entry has 32bit ID and 32bit AIE Timer low value.

    uint32_t numEntries = max_count;
    std::stringstream msg;
    msg << " A maximum of " << numEntries << " record can be accommodated in given buffer of bytes size"
        << std::hex << mBufSz << std::dec << std::endl;
    xrt_core::message::send(xrt_core::message::severity_level::debug, "XRT", msg.str());

    if (numEntries <= max_count) {
      for (uint32_t i = 0 ; i < numEntries; i++) {
        boost::property_tree::ptree ptIdTS;
        ptIdTS.put("id", *ptr);
        ptr++;
        if (0 == *ptr) {
          // Zero value for Timestamp in cycles indicates end of recorded data
          std::string msgEntries = " Got " + std::to_string(i) + " records in buffer";
          xrt_core::message::send(xrt_core::message::severity_level::debug, "XRT", msgEntries);
          break;
        }
        ptIdTS.put("cycle", *ptr);
        ptr++;

        ptRecordTimerTS.push_back(std::make_pair("", ptIdTS));
      }
    }    

    if (ptRecordTimerTS.empty()) {
      boost::property_tree::ptree ptEmpty;
      ptRecordTimerTS.push_back(std::make_pair("", ptEmpty));
    }
    ptTop.add_child("record_timer_ts", ptRecordTimerTS);

    // Write output file
    std::ostringstream oss;
    boost::property_tree::write_json(oss, ptTop);

    // Remove quotes from value strings
    std::regex reg("\\\"((-?[0-9]+\\.{0,1}[0-9]*)|(null)|())\\\"(?!\\:)");
    std::string result = std::regex_replace(oss.str(), reg, "$1");

    std::ofstream fOut;
    fOut.open("record_timer_ts.json");
    fOut << result;
    fOut.close();

    xrt_core::message::send(xrt_core::message::severity_level::debug, "XRT", 
              "Finished writing record_timer_ts.json in MultiPDIProfClientDevImpl::finishflushDevice");

    /* Delete the result BO so that AIE Profile/Debug Plugins, if enabled,
     * can use their own Debug BO to capture their data.
     */
    delete mResultBOHolder;
    mResultBOHolder = nullptr;
#endif
  }
}

