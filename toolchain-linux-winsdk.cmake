set(CMAKE_SYSTEM_NAME Windows)

set(CMAKE_C_COMPILER clang-cl)
set(CMAKE_CXX_COMPILER clang-cl)
set(CMAKE_LINKER lld-link)

# Tell CMake the target architecture during compiler detection.
set(CMAKE_C_COMPILER_TARGET i686-pc-windows-msvc)
set(CMAKE_CXX_COMPILER_TARGET i686-pc-windows-msvc)

# Override CMake's default linker machine type.
set(CMAKE_EXE_LINKER_FLAGS_INIT "/machine:x86")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "/machine:x86")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "/machine:x86")

set(CMAKE_C_COMPILER_FRONTEND_VARIANT MSVC)
set(CMAKE_CXX_COMPILER_FRONTEND_VARIANT MSVC)

set(WINDOWS_SDK_PATH "" CACHE PATH "Path to Windows SDK root")
set(CMAKE_TRY_COMPILE_PLATFORM_VARIABLES WINDOWS_SDK_PATH)

message(STATUS "WINDOWS_SDK_PATH=${WINDOWS_SDK_PATH}")

if(NOT WINDOWS_SDK_PATH)
  if(DEFINED ENV{WINDOWS_SDK_PATH})
    set(WINDOWS_SDK_PATH "$ENV{WINDOWS_SDK_PATH}")
  else()
    find_program(_msbuild_exe msbuild)
    if(_msbuild_exe)
      get_filename_component(_msbuild_bin "${_msbuild_exe}" DIRECTORY)
      get_filename_component(_candidate "${_msbuild_bin}/.." ABSOLUTE)
      if(NOT IS_DIRECTORY "${_candidate}/vc")
        get_filename_component(_candidate "${_candidate}/.." ABSOLUTE)
      endif()
      message(STATUS "Found Windows SDK Path in ${_candidate}")
      set(WINDOWS_SDK_PATH "${_candidate}")
    else()
      set(WINDOWS_SDK_PATH "/opt/msvc")
      message(STATUS "Fallback to ${WINDOWS_SDK_PATH}")
    endif()
  endif()
endif()

if(NOT EXISTS "${WINDOWS_SDK_PATH}/kits/10/Include")
  message(FATAL_ERROR "Invalid WINDOWS_SDK_PATH: ${WINDOWS_SDK_PATH}")
endif()

file(GLOB _winsdk_versions
    LIST_DIRECTORIES TRUE
    "${WINDOWS_SDK_PATH}/kits/10/Include/*"
)

if(NOT _winsdk_versions)
  message(FATAL_ERROR "No Windows SDK versions found")
endif()

list(GET _winsdk_versions 0 _winsdk_dir)
get_filename_component(_winsdk_ver "${_winsdk_dir}" NAME)

add_compile_options(
    -fms-compatibility
    -fms-extensions
    -fdelayed-template-parsing
    /winsysroot "${WINDOWS_SDK_PATH}"
)

add_link_options(
    /machine:x86
    /winsysroot:"${WINDOWS_SDK_PATH}"
)

# Force x86 linker flags so CMake does not default to x64.
set(CMAKE_EXE_LINKER_FLAGS "/machine:x86" CACHE STRING "" FORCE)
set(CMAKE_SHARED_LINKER_FLAGS "/machine:x86" CACHE STRING "" FORCE)
set(CMAKE_MODULE_LINKER_FLAGS "/machine:x86" CACHE STRING "" FORCE)
set(CMAKE_STATIC_LINKER_FLAGS "/machine:x86" CACHE STRING "" FORCE)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
