/**
 * Copyright (C) 2022-2023 Advanced Micro Devices, Inc. - All rights reserved
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

#ifndef AIE_PROFILE_UTIL_DOT_H
#define AIE_PROFILE_UTIL_DOT_H

#include <cstdint>
#include "xdp/profile/plugin/aie_profile/aie_profile_defs.h"
#include "xdp/profile/database/static_info/aie_constructs.h"

extern "C" {
#include <xaiengine.h>
#include <xaiengine/xaiegbl_params.h>
}

namespace xdp::aie::profile {

  const std::vector<XAie_Events> sSEventList = {
    XAIE_EVENT_PORT_RUNNING_0_CORE,
    XAIE_EVENT_PORT_STALLED_0_CORE,
    XAIE_EVENT_PORT_RUNNING_0_PL,
    XAIE_EVENT_PORT_RUNNING_0_MEM_TILE,
    XAIE_EVENT_PORT_STALLED_0_MEM_TILE,
    XAIE_EVENT_PORT_TLAST_0_MEM_TILE
  };

  const std::map<xdp::module_type, uint16_t> counterBases = {
    {module_type::core,     static_cast<uint16_t>(0)},
    {module_type::dma,      BASE_MEMORY_COUNTER},
    {module_type::shim,     BASE_SHIM_COUNTER},
    {module_type::mem_tile, BASE_MEM_TILE_COUNTER}
  };

  const std::vector<XAie_ModuleType> falModuleTypes = {
    XAIE_CORE_MOD,
    XAIE_MEM_MOD,
    XAIE_PL_MOD,
    XAIE_MEM_MOD
  };

  /**
   * @brief   Get metric sets for core modules
   * @param   hwGen integer representing the hardware generation
   * @return  Map of core metric set names with vectors of event IDs
   */
  std::map<std::string, std::vector<XAie_Events>> getCoreEventSets(int hwGen);

  /**
   * @brief   Get metric sets for memory modules
   * @param   hwGen integer representing the hardware generation
   * @return  Map of memory metric set names with vectors of event IDs
   */
  std::map<std::string, std::vector<XAie_Events>> getMemoryEventSets(int hwGen);
 
  /**
   * @brief   Get metric sets for interface modules
   * @return  Map of interface metric set names with vectors of event IDs
   */
  std::map<std::string, std::vector<XAie_Events>> getInterfaceTileEventSets(int hwGen);

  /**
   * @brief   Get metric sets for memory tile modules
   * @return  Map of memory tile metric set names with vectors of event IDs
   */
  std::map<std::string, std::vector<XAie_Events>> getMemoryTileEventSets();

  /**
   * @brief  Modify configured events
   * @param type module type 
   * @param subtype plio/gmio type
   * @param events vector of events to replace with the respective channel 1 events
   * @param hwGen AIE hardware generation
   */
  void modifyEvents(module_type type, uint16_t subtype, uint8_t channel,
                                         std::vector<XAie_Events>& events, int hwGen);
  /**
   * @brief  Check if event is generated by a stream switch monitor port
   * @param  event Event ID to check
   * @return True if given event is from a stream switch port
   */
  bool isStreamSwitchPortEvent(const XAie_Events event);

  /**
   * @brief  Check if event is a port running event
   * @param  event Event ID to check
   * @return True if given event is a port running event
   */
  bool isPortRunningEvent(const XAie_Events event);


  /**
   * @brief Get XAie module enum at the module index 
   * @param moduleIndex module index
   * @return  
   */
  XAie_ModuleType getFalModuleType(int moduleIndex);

  /**
   * @brief Get base event number for a module
   * @param type module type
   * @return  
   */
  uint16_t getCounterBase(xdp::module_type type);

}  // namespace xdp::aie::profile

#endif