if(NOT DEFINED KERNEL_TARGET)
  set(KERNEL_TARGET kernel)
endif()

# Modules that must be linked into the kernel once KERNEL_TARGET exists
set(_PENDING_KERNEL_MODULES)

function(_add_kernel_module_internal kind)
  if(ARGC LESS 2 OR ARGC GREATER 3)
    message(FATAL_ERROR "_add_kernel_module_internal(${kind} <module_dir> [<override_name>])")
  endif()

  # ARGV0 = kind, ARGV1 = dir, ARGV2 = optional override name
  set(dir "${ARGV1}")
  get_filename_component(dir_abs "${dir}" ABSOLUTE)
  get_filename_component(dir_name "${dir_abs}" NAME)

  set(cmake_file "${dir_abs}/_mod.cmake")

  # Kind-based location checks
  if(kind STREQUAL "kernel")
    string(FIND "${dir_abs}" "/kernel/modules/" _pos)
    if(_pos EQUAL -1)
      message(FATAL_ERROR
        "add_kernel_module expects module_dir under 'src/kernel/modules', got '${dir_abs}'")
    endif()
  elseif(kind STREQUAL "module")
    string(FIND "${dir_abs}" "/kernel/modules/" _pos)
    if(NOT _pos EQUAL -1)
      message(FATAL_ERROR
        "add_module expects module_dir outside 'src/kernel/modules', got '${dir_abs}'")
    endif()
  endif()

  if(NOT EXISTS "${cmake_file}")
    set(template
"set(MODULE_NAME \"${dir_name}\")

# Either:
# set(MODULE_SOURCES
#     \"\${CMAKE_CURRENT_LIST_DIR}/your_source.cpp\"
# )
#
# Or for a header-only module:
# set(MODULE_SOURCES_EMPTY ON)
#
# Optionally:
# set(MODULE_INCLUDE_DIRS
#     \"\${CMAKE_CURRENT_LIST_DIR}\"
# )
")
    message(FATAL_ERROR
      "Module directory '${dir_abs}' does not contain '_mod.cmake'.\n"
      "Create '${cmake_file}' with something like:\n\n${template}")
  endif()

  # Per-module variables
  set(MODULE_NAME "")
  set(MODULE_SOURCES "")
  set(MODULE_INCLUDE_DIRS "")
  set(MODULE_SOURCES_EMPTY OFF)

  include("${cmake_file}")

  if(NOT MODULE_SOURCES AND NOT MODULE_SOURCES_EMPTY)
    message(FATAL_ERROR
      "Module '${cmake_file}' must set either MODULE_SOURCES or MODULE_SOURCES_EMPTY ON")
  endif()

  if(MODULE_SOURCES AND MODULE_SOURCES_EMPTY)
    message(FATAL_ERROR
      "Module '${cmake_file}' sets both MODULE_SOURCES and MODULE_SOURCES_EMPTY; pick one")
  endif()

  if(ARGC EQUAL 3)
    set(name "${ARGV2}")
  elseif(MODULE_NAME)
    set(name "${MODULE_NAME}")
  else()
    set(name "${dir_name}")
  endif()

  # Build the module library and attach the right compile config
  if(MODULE_SOURCES_EMPTY)
    add_library(${name} INTERFACE)

    if(MODULE_INCLUDE_DIRS)
      target_include_directories(${name} INTERFACE ${MODULE_INCLUDE_DIRS})
    endif()

    if(kind STREQUAL "kernel")
      target_link_libraries(${name} INTERFACE kernel_compile_config)
    else()
      target_link_libraries(${name} INTERFACE module_compile_config)
    endif()
  else()
    add_library(${name} STATIC ${MODULE_SOURCES})

    if(MODULE_INCLUDE_DIRS)
      target_include_directories(${name} PUBLIC ${MODULE_INCLUDE_DIRS})
    endif()

    if(kind STREQUAL "kernel")
      target_link_libraries(${name} PRIVATE kernel_compile_config)
    else()
      target_link_libraries(${name} PRIVATE module_compile_config)
    endif()
  endif()

  # Link into the kernel (now or later)
  if(TARGET ${KERNEL_TARGET})
    target_link_libraries(${KERNEL_TARGET} PRIVATE ${name})
  else()
    list(APPEND _PENDING_KERNEL_MODULES "${name}")
    set(_PENDING_KERNEL_MODULES "${_PENDING_KERNEL_MODULES}" PARENT_SCOPE)
  endif()
endfunction()

# Kernel feature modules under src/kernel/modules/*
function(add_kernel_module)
  _add_kernel_module_internal("kernel" ${ARGV})
endfunction()

# Support / runtime / generic modules (not under src/kernel/modules)
function(add_module)
  _add_kernel_module_internal("module" ${ARGV})
endfunction()

function(flush_pending_kernel_modules)
  if(NOT TARGET ${KERNEL_TARGET})
    message(FATAL_ERROR "flush_pending_kernel_modules: KERNEL_TARGET '${KERNEL_TARGET}' does not exist")
  endif()

  if(DEFINED _PENDING_KERNEL_MODULES)
    foreach(mod IN LISTS _PENDING_KERNEL_MODULES)
      target_link_libraries(${KERNEL_TARGET} PRIVATE "${mod}")
    endforeach()
  endif()
endfunction()

