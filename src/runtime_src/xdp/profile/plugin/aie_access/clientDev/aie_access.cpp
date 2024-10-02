/**
 * Copyright (C) 2024 Advanced Micro Devices, Inc. - All rights reserved
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

#include <boost/algorithm/string.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "core/common/device.h"
#include "core/common/message.h"
#include "core/common/api/hw_context_int.h"

#include "core/common/api/xclbin_int.h"
#include "core/include/xclbin.h"

#include "xdp/profile/database/database.h"
#include "xdp/profile/database/static_info/aie_util.h"
#include "xdp/profile/database/static_info/aie_constructs.h"
#include "xdp/profile/device/common/client_transaction.h"
#include "xdp/profile/plugin/aie_access/clientDev/aie_access.h"
#include "xdp/profile/plugin/vp_base/utility.h"

#ifdef _WIN32
# pragma warning ( disable : 4244 4456)
#endif

namespace xdp {


  AIEAccessClientDevImpl::AIEAccessClientDevImpl(VPDatabase*dB)
    : AIEAccessImpl(dB)
  {
  }

  void AIEAccessClientDevImpl::updateDevice(void* hwCtxImpl)
  {
    (void)hwCtxImpl;
    auto device = xrt_core::hw_context_int::get_core_device(mHwContext);
    auto aieDevice = std::make_unique<xrt::aie::device>(0);

    uint64_t ctxId = 0, col = 0, row = 0, startCol = 0, numCols = 0, offset = 0;
    boost::property_tree::ptree aiePartitionPt = xdp::aie::getAIEPartitionInfoClient(hwCtxImpl);
    for (const auto& e : aiePartitionPt) {
      startCol = e.second.get<uint64_t>("start_col");
      numCols  = e.second.get<uint64_t>("num_cols");
      ctxId = e.second.get<uint64_t>("context_id");
      // Currently, assuming only one Hw Context is alive at a time
      break;
    }

    givenAddress = xrt_core::config::get_aie_access_settings();
    std::vector<std::string> addresses;
    boost::split(addresses, givenAddress, boost::is_any_of(":"));
    std::cout <<" given address " << givenAddress << " size " << addresses.size() << std::endl;
    if (2 != addresses.size()) {
      std::cout << " size not 2" << std::endl;
      return;
    }
    std::vector<std::string> loc;
    boost::split(loc, addresses[0], boost::is_any_of(","));
    if (2 != loc.size()) {
      std::cout << " did not get col row relative address" << std::endl;
      return;
    }
    col = std::stoul(loc[0], nullptr, 10);
    row = std::stoul(loc[1], nullptr, 10);
    offset = std::stoul(addresses[1], nullptr, 16);

    std::cout << " Context Id " << ctxId << " Parsed col " << col << " row " << row << "register address " << offset << std::endl;

    uint32_t value = aieDevice.get()->read_aie_reg(ctxId, col, row, offset);
    std::cout << " Get read value in update" << value << std::endl;
    aieDevice.get()->write_aie_reg(ctxId, col, row, offset, 444);
    value = aieDevice.get()->read_aie_reg(ctxId, col, row, offset);
    std::cout << " Get read value 2 :: " << value << std::endl;

  }

  void AIEAccessClientDevImpl::finishflushDevice(void* hwCtxImpl)
  {
    auto device = xrt_core::hw_context_int::get_core_device(mHwContext);
    auto aieDevice = std::make_unique<xrt::aie::device>(0);

    uint64_t ctxId = 0, col = 0, row = 0, startCol = 0, numCols = 0, offset = 0;

    std::cout << " Just to halt : get ctx " << std::endl;
    std::cin >> ctxId;
    std::cout << " got " << ctxId << std::endl;

    boost::property_tree::ptree aiePartitionPt = xdp::aie::getAIEPartitionInfoClient(hwCtxImpl);
    for (const auto& e : aiePartitionPt) {
      startCol = e.second.get<uint64_t>("start_col");
      numCols  = e.second.get<uint64_t>("num_cols");
      ctxId = e.second.get<uint64_t>("context_id");
      // Currently, assuming only one Hw Context is alive at a time
      break;
    }

    std::string givenAddress1 = xrt_core::config::get_aie_access_settings();

    std::vector<std::string> addresses;
    boost::split(addresses, givenAddress, boost::is_any_of(":"));
    std::cout <<" given address old " << givenAddress << " size " << addresses.size() << " new given addr "  << givenAddress1 << std::endl;
    if (2 != addresses.size()) {
      std::cout << " size not 2" << std::endl;
      return;
    }

    std::vector<std::string> loc;
    boost::split(loc, addresses[0], boost::is_any_of(","));
    if (2 != loc.size()) {
      std::cout << " did not get col row relative address" << std::endl;
      return;
    }
    col = std::stoul(loc[0], nullptr, 10);
    row = std::stoul(loc[1], nullptr, 10);
    offset = std::stoul(addresses[1], nullptr, 16);

    std::cout << " Context Id " << ctxId << " Parsed col " << col << " row " << row << "register address " << offset << std::endl;

    uint32_t value = aieDevice.get()->read_aie_reg(ctxId, col, row, offset);
    std::cout << " Get read value again in finish" << value << std::endl;
    aieDevice.get()->write_aie_reg(ctxId, col, row, offset, 222);
    value = aieDevice.get()->read_aie_reg(ctxId, col, row, offset);
    std::cout << " Get read value 2 :: " << value << std::endl;
  }
}