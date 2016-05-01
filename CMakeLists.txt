cmake_minimum_required(VERSION 3.4)
project(pkg6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(tinyxml2 libpkg)
set(LIBPKG_SOURCE_FILES
        libpkg/Image.cpp libpkg/Image.h
        libpkg/LicenseInfo.cpp libpkg/LicenseInfo.h
        libpkg/pkgdefs.h
        libpkg/PackageCategory.cpp libpkg/PackageCategory.h
        libpkg/PackageInfo.cpp libpkg/PackageInfo.h
        libpkg/History.cpp libpkg/History.h
        libpkg/HistoryOperation.h
        libpkg/Catalog.cpp libpkg/Catalog.h
        libpkg/ImageConfig.cpp libpkg/ImageConfig.cpp
        libpkg/Progress.h
        libpkg/Manifest.cpp libpkg/Manifest.h libpkg/Action.cpp libpkg/Action.h)
add_library(pkg6 SHARED ${LIBPKG_SOURCE_FILES})
add_library(tinyxml2 SHARED tinyxml2/tinyxml2.h tinyxml2/tinyxml2.cpp)
add_executable(pkg main.cpp)
target_link_libraries(pkg6 debug tinyxml2 boost_filesystem boost_system)
target_link_libraries(pkg debug pkg6)