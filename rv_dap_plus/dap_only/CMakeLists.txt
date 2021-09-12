set(BSP_COMMON_DIR ${CMAKE_SOURCE_DIR}/bsp/bsp_common)
set(CMSIS_DAP_DIR ${CMAKE_CURRENT_LIST_DIR}/../DAP)
set(TARGET_REQUIRED_LIBS usb_stack)
list(APPEND ADD_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
list(APPEND ADD_INCLUDE "${CMSIS_DAP_DIR}/Include")

set(TARGET_REQUIRED_SRCS ${BSP_COMMON_DIR}/usb/usb_dc.c
                         ${CMAKE_CURRENT_LIST_DIR}/usb.c
                         ${CMSIS_DAP_DIR}/Source/DAP.c
                         ${CMSIS_DAP_DIR}/Source/JTAG_DP.c
                         ${CMSIS_DAP_DIR}/Source/SW_DP.c)

set(mains main.c)
generate_bin()
