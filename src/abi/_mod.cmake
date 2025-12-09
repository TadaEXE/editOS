set(MODULE_NAME "abi_runtime")

set(MODULE_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/gcc/int_arithmetic.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/icxx/atexit.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/icxx/guard.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/icxx/pure_virtual.cpp"
)

set(MODULE_INCLUDE_DIRS
    "${CMAKE_CURRENT_LIST_DIR}"
)

