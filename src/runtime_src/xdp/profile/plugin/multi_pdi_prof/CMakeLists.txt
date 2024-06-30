# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2024 Advanced Micro Devices, Inc.  All rights reserved.
#

# ========================================================================================
# This builds the Plugin to test Multi PDI Profile . It is currently built on Windows Only.
# ========================================================================================

if (XDP_CLIENT_BUILD_CMAKE STREQUAL "yes")
  set(IMPL_DIR "${PROFILE_DIR}/plugin/multi_pdi_prof/clientDev")
endif()

file(GLOB MULTI_PDI_PROF_PLUGIN_FILES
  "${PROFILE_DIR}/plugin/multi_pdi_prof/*.h"
  "${PROFILE_DIR}/plugin/multi_pdi_prof/*.cpp"
  "${IMPL_DIR}/*.h"
  "${IMPL_DIR}/*.cpp"
)

file(GLOB PROF_DEVICE_COMMON_FILES
  "${PROFILE_DIR}/device/common/*.h"
  "${PROFILE_DIR}/device/common/*.cpp"
)

if (XDP_CLIENT_BUILD_CMAKE STREQUAL "yes")
  add_library(xdp_multi_pdi_prof_plugin MODULE ${MULTI_PDI_PROF_PLUGIN_FILES} ${PROF_DEVICE_COMMON_FILES})
  add_dependencies(xdp_multi_pdi_prof_plugin xdp_core xrt_coreutil)

  target_link_libraries(xdp_multi_pdi_prof_plugin PRIVATE xdp_core xrt_coreutil xaiengine)

  target_compile_definitions(xdp_multi_pdi_prof_plugin PRIVATE XDP_CLIENT_BUILD=1 -DXAIE_FEATURE_MSVC)

  target_include_directories(xdp_multi_pdi_prof_plugin PRIVATE ${AIERT_DIR}/include)

  set_target_properties(xdp_multi_pdi_prof_plugin PROPERTIES VERSION ${XRT_VERSION_STRING} SOVERSION ${XRT_SOVERSION})

  install (TARGETS xdp_multi_pdi_prof_plugin
    LIBRARY DESTINATION ${XDP_PLUGIN_INSTALL_DIR}
  )

endif()
