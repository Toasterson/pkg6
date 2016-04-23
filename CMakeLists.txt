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
        libpkg/Progress.h)
add_library(libpkg6 ${LIBPKG_SOURCE_FILES})
add_library(libtinyxml2 tinyxml2/tinyxml2.h tinyxml2/tinyxml2.cpp)
add_executable(pkg main.cpp)
target_link_libraries(libpkg6 libtinyxml2 boost_system)
target_link_libraries(pkg libpkg6)