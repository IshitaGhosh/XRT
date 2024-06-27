# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2024 Advanced Micro Devices, Inc.  All rights reserved.
#

# ===========================================================================
# This builds the AIE Halt plugin to add Debug Halts for all AIE Cores at 
# the start. It is currently built on Windows Only.
# ===========================================================================

if (XDP_CLIENT_BUILD_CMAKE STREQUAL "yes")
  set(IMPL_DIR "${PROFILE_DIR}/plugin/aie_halt/clientDev")
endif()

file(GLOB AIE_HALT_PLUGIN_FILES
  "${PROFILE_DIR}/plugin/aie_halt/*.h"
  "${PROFILE_DIR}/plugin/aie_halt/*.cpp"
  "${IMPL_DIR}/*.h"
  "${IMPL_DIR}/*.cpp"
)

if (XDP_CLIENT_BUILD_CMAKE STREQUAL "yes")
  add_library(xdp_aie_halt_plugin MODULE ${AIE_HALT_PLUGIN_FILES})
  add_dependencies(xdp_aie_halt_plugin xdp_core xrt_coreutil)

  target_link_libraries(xdp_aie_halt_plugin PRIVATE xdp_core xrt_coreutil)

  target_compile_definitions(xdp_aie_halt_plugin PRIVATE XDP_CLIENT_BUILD=1)

  set_target_properties(xdp_aie_halt_plugin PROPERTIES VERSION ${XRT_VERSION_STRING} SOVERSION ${XRT_SOVERSION})

  install (TARGETS xdp_aie_halt_plugin
    LIBRARY DESTINATION ${XDP_PLUGIN_INSTALL_DIR}
  )

endif()
