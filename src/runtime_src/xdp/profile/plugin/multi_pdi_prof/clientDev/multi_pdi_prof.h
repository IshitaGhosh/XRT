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

#ifndef XDP_PLUGIN_MULTI_PDI_PROF_CLIENTDEV_IMPL_H
#define XDP_PLUGIN_MULTI_PDI_PROF_CLIENTDEV_IMPL_H

#include "xdp/config.h"
#include "xdp/profile/plugin/multi_pdi_prof/multi_pdi_prof_impl.h"

namespace xdp {

  class MultiPDIProfClientDevImpl : public MultiPDIProfImpl
  {

    public :
      MultiPDIProfClientDevImpl(VPDatabase* dB);

      ~MultiPDIProfClientDevImpl() = default;

      virtual void updateDevice(void* hwCtxImpl);
      virtual void finishflushDevice(void* hwCtxImpl);
  };

}

#endif

