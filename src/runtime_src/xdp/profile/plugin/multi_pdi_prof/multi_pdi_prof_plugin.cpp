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

#include<regex>
#include<string>

#include "core/common/device.h"
#include "core/common/message.h"
#include "core/common/api/hw_context_int.h"

#include "xdp/profile/plugin/multi_pdi_prof/multi_pdi_prof_plugin.h"
#include "xdp/profile/plugin/vp_base/info.h"
#include "xdp/profile/plugin/vp_base/utility.h"

#ifdef XDP_CLIENT_BUILD
#include "xdp/profile/plugin/multi_pdi_prof/clientDev/multi_pdi_prof.h"
#endif

namespace xdp {

  bool MultiPDIProfPlugin::live = false;

  MultiPDIProfPlugin::MultiPDIProfPlugin()
    : XDPPlugin()
  {
    MultiPDIProfPlugin::live = true;

    db->registerPlugin(this);
    db->registerInfo(info::multi_pdi_prof);
  }

  MultiPDIProfPlugin::~MultiPDIProfPlugin()
  {
    if (VPDatabase::alive()) {
      try {
        writeAll(false);
      }
      catch (...) {
      }
      db->unregisterPlugin(this);
    }

    MultiPDIProfPlugin::live = false;
  }

  bool MultiPDIProfPlugin::alive()
  {
    return MultiPDIProfPlugin::live;
  }

  void MultiPDIProfPlugin::updateDevice(void* hwCtxImpl)
  {
#ifdef XDP_CLIENT_BUILD
    if (mHwCtxImpl) {
      // For client device flow, only 1 device and xclbin is supported now.
      return;
    }
    mHwCtxImpl = hwCtxImpl;

    xrt_core::message::send(xrt_core::message::severity_level::debug, "XRT", 
          "Retrieving hw_context.");

    xrt::hw_context hwContext = xrt_core::hw_context_int::create_hw_context_from_implementation(mHwCtxImpl);
    std::shared_ptr<xrt_core::device> coreDevice = xrt_core::hw_context_int::get_core_device(hwContext);

    // Only one device for Client Device flow
    uint64_t deviceId = db->addDevice("win_device");
    (db->getStaticInfo()).updateDeviceClient(deviceId, coreDevice);
    (db->getStaticInfo()).setDeviceName(deviceId, "win_device");

    DeviceDataEntry.valid = true;
    DeviceDataEntry.implementation = std::make_unique<MultiPDIProfClientDevImpl>(db);
    DeviceDataEntry.implementation->setHwContext(hwContext);
    //DeviceDataEntry.implementation->setBufSize(mBufSz);
    DeviceDataEntry.implementation->updateDevice(mHwCtxImpl);
#endif
  }

  void MultiPDIProfPlugin::finishflushDevice(void* /*hwCtxImpl*/)
  {
#ifdef XDP_CLIENT_BUILD
#if 0
    if (!mHwCtxImpl || !DeviceDataEntry.valid) {
      return;
    }

    if (hwCtxImpl != mHwCtxImpl) {
      xrt_core::message::send(xrt_core::message::severity_level::debug, "XRT", 
          "Cannot retrieve ML Timeline data as a new HW Context Implementation is passed.");
      return;
    } 
    DeviceDataEntry.valid = false;
    DeviceDataEntry.implementation->finishflushDevice(mHwCtxImpl);
#endif
#endif
  }

  void MultiPDIProfPlugin::writeAll(bool /*openNewFiles*/)
  {
#ifdef XDP_CLIENT_BUILD
#if 0
    if (!mHwCtxImpl || !DeviceDataEntry.valid) {
      return;
    }
    DeviceDataEntry.valid = false;
    DeviceDataEntry.implementation->finishflushDevice(mHwCtxImpl);
#endif
#endif
  }

}
