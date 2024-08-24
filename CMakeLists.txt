cmake_minimum_required(VERSION 3.21...3.30)

# Default build type value.
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the build type")

project(pxr-tf
    HOMEPAGE_URL "https://github.com/untwine/pxr-tf"
    LANGUAGES C CXX
)

include(GNUInstallDirs)

if (NOT "${CMAKE_CXX_STANDARD}")
    set(CMAKE_CXX_STANDARD 17 CACHE STRING "Default C++ standard")
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default options.
option(BUILD_TESTS "Build tests" ON)
option(BUILD_SHARED_LIBS "Build Shared Library" ON)
option(BUILD_PYTHON_BINDINGS "Build Python Bindings" ON)
option(ENABLE_PRECOMPILED_HEADERS "Enable precompiled headers." OFF)

if (NOT BUILD_SHARED_LIBS)
    add_compile_definitions(PXR_STATIC)
endif()

if (WIN32)
    # Prevent WinDef.h from defining min/max macros.
    add_definitions(-DNOMINMAX)

    # Exclude unnecessary Windows APIs for faster builds and fewer macro conflicts.
    add_definitions(-DWIN32_LEAN_AND_MEAN)

    # Disable /Zc:inline in VS 2019+ to retain "arch_ctor_<name>" symbols in release.
    # https://developercommunity.visualstudio.com/t/914943
    if (MSVC_VERSION GREATER_EQUAL 1920)
        set(CMAKE_CXX_FLAGS "/Zc:inline- ${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "/Zc:inline ${CMAKE_CXX_FLAGS}")
    endif()

    # Disable harmless Visual Studio warning C4003 (non-standard preprocessor).
    # https://developercommunity.visualstudio.com/t/364698
    set(CMAKE_CXX_FLAGS "/wd4003 ${CMAKE_CXX_FLAGS}")
endif()

# Update build type from environment for CMake < 3.22
if (DEFINED ENV{CMAKE_BUILD_TYPE})
    set(CMAKE_BUILD_TYPE $ENV{CMAKE_BUILD_TYPE}
        CACHE STRING "Specifies the build type" FORCE)
endif()

find_package(pxr-arch REQUIRED)
find_package(TBB 2017.0 REQUIRED)

if(BUILD_PYTHON_BINDINGS)
    add_compile_definitions(PXR_PYTHON_SUPPORT_ENABLED=1)

    find_package(Python COMPONENTS Interpreter Development REQUIRED)

    set(_py_version ${Python_VERSION_MAJOR}${Python_VERSION_MINOR})
    set(_py_version_dot ${Python_VERSION_MAJOR}.${Python_VERSION_MINOR})

    find_package(Boost 1.80.0 COMPONENTS "python${_py_version}" REQUIRED)

    # Define generic target for Boost Python if necessary.
    if (NOT TARGET Boost::python)
        add_library(Boost::python ALIAS "Boost::python${_py_version}")
    endif()

    # Set variable to identify the path to install and test python libraries.
    set(PYTHON_DESTINATION
        "${CMAKE_INSTALL_LIBDIR}/python${_py_version_dot}/site-packages"
        CACHE INTERNAL "Python library path.")
endif()

add_subdirectory(src)

# Build and setup tests if required.
if (BUILD_TESTS)
    # Python is needed to run the tests.
    find_package(Python COMPONENTS Interpreter Development REQUIRED)

    enable_testing()
    add_subdirectory(test)
endif()

include(CMakePackageConfigHelpers)

configure_package_config_file(
    "cmake/pxr-tf-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/pxr-tf-config.cmake"
    INSTALL_DESTINATION share/cmake/pxr-tf
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/pxr-tf-config.cmake"
    DESTINATION share/cmake/pxr-tf
)