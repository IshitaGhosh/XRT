# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2024-2025 Advanced Micro Devices, Inc.  All rights reserved.
#

# =======================================================================
# This builds the Gen Asm Elf Plugin to experiment asm, elf generation and execution
# the start. It is currently built on Client and VE2
# =========================================================================

if (XDP_CLIENT_BUILD_CMAKE STREQUAL "yes")
  set(IMPL_DIR "${PROFILE_DIR}/plugin/gen_asm_elf/clientDev")

elseif (XDP_VE2_BUILD_CMAKE STREQUAL "yes")
  set(IMPL_DIR "${PROFILE_DIR}/plugin/gen_asm_elf/ve2")
  
endif()

file(GLOB XDP_ASM_ELF_PLUGIN_FILES
  "${PROFILE_DIR}/plugin/gen_asm_elf/*.h"
  "${PROFILE_DIR}/plugin/gen_asm_elf/*.cpp"
  "${IMPL_DIR}/*.h"
  "${IMPL_DIR}/*.cpp"
)

file(GLOB XDP_DEVICE_COMMON_FILES
  "${PROFILE_DIR}/device/common/*.h"
  "${PROFILE_DIR}/device/common/*.cpp"
)

if (XDP_CLIENT_BUILD_CMAKE STREQUAL "yes")
  add_library(xdp_gen_asm_elf_plugin MODULE ${XDP_ASM_ELF_PLUGIN_FILES} ${XDP_DEVICE_COMMON_FILES})
  add_dependencies(xdp_gen_asm_elf_plugin xdp_core xrt_coreutil)
  target_link_libraries(xdp_gen_asm_elf_plugin PRIVATE xdp_core xrt_coreutil xaiengine)
  target_compile_definitions(xdp_gen_asm_elf_plugin PRIVATE XDP_CLIENT_BUILD=1 -DXAIE_FEATURE_MSVC)
  target_include_directories(xdp_gen_asm_elf_plugin PRIVATE ${AIERT_DIR}/include)
  set_target_properties(xdp_gen_asm_elf_plugin PROPERTIES VERSION ${XRT_VERSION_STRING} SOVERSION ${XRT_SOVERSION})

  install (TARGETS xdp_gen_asm_elf_plugin
    LIBRARY DESTINATION ${XDP_PLUGIN_INSTALL_DIR} COMPONENT ${XRT_COMPONENT}
  )

elseif (XDP_VE2_BUILD_CMAKE STREQUAL "yes")
	add_library(xdp_gen_asm_elf_plugin MODULE ${XDP_ASM_ELF_PLUGIN_FILES})
  add_dependencies(xdp_gen_asm_elf_plugin xdp_core xrt_coreutil)

  target_include_directories(xdp_gen_asm_elf_plugin PRIVATE ${AIERT_DIR}/include)
  target_link_libraries(xdp_gen_asm_elf_plugin PRIVATE xdp_core xrt_coreutil xaiengine)
  #target_link_libraries(xdp_gen_asm_elf_plugin PRIVATE xdp_core xrt_coreutil)
  target_compile_definitions(xdp_gen_asm_elf_plugin PRIVATE XDP_VE2_BUILD=1)

  set_target_properties(xdp_gen_asm_elf_plugin PROPERTIES VERSION ${XRT_VERSION_STRING} SOVERSION ${XRT_SOVERSION})

  install (TARGETS xdp_gen_asm_elf_plugin
    LIBRARY DESTINATION ${XDP_PLUGIN_INSTALL_DIR} COMPONENT ${XRT_COMPONENT}
  )

# Else, on edge-aarch64 don't build at all

endif()
