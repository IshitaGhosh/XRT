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

#ifndef XDP_PLUGIN_AIE_HALT_IMPL_H
#define XDP_PLUGIN_AIE_HALT_IMPL_H

#include "core/include/xrt/xrt_hw_context.h"

namespace xdp {

  class VPDatabase;

  class AieHaltImpl
  {
    protected :
      VPDatabase* db = nullptr;
      xrt::hw_context mHwContext;
      //uint32_t mBufSz;

    public:
      AieHaltImpl(VPDatabase* dB)
        : db(dB)
      {}

      AieHaltImpl() = delete;

      virtual ~AieHaltImpl() {}

      virtual void updateDevice(void*) = 0;
      virtual void finishflushDevice(void*) = 0;

      void setHwContext(xrt::hw_context ctx)
      {
        mHwContext = std::move(ctx);
      }
      
      #if 0
      void setBufSize(uint32_t sz)
      {
        mBufSz = sz;
      }
      #endif
  };

}
#endif

