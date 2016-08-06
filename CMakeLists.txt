cmake_minimum_required(VERSION 3.4)
project(pkg6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(tinyxml2 rapidjson libpkg)
set(LIBPKG_SOURCE_FILES
        libpkg/image/Image.cpp libpkg/image/Image.h
        libpkg/image/LicenseInfo.cpp libpkg/image/LicenseInfo.h
        libpkg/pkgdefs.h
        libpkg/package/PackageCategory.cpp libpkg/package/PackageCategory.h
        libpkg/package/PackageInfo.cpp libpkg/package/PackageInfo.h
        libpkg/history/History.cpp libpkg/history/History.h
        libpkg/history/HistoryOperation.h
        libpkg/catalog/Catalog.cpp libpkg/catalog/Catalog.h
        libpkg/image/ImageConfig.cpp libpkg/image/ImageConfig.cpp
        libpkg/Progress.h
        libpkg/manifest/Manifest.cpp libpkg/manifest/Manifest.h
        libpkg/catalog/parser/V1BaseHandler.h libpkg/catalog/parser/V1DependencySummaryHandler.h
        libpkg/action/AttributeAction.cpp libpkg/action/AttributeAction.h libpkg/action/Exception.h libpkg/action/DependAction.cpp libpkg/action/DependAction.h libpkg/action/DirectoryAction.cpp libpkg/action/DirectoryAction.h)
add_library(pkg6 SHARED ${LIBPKG_SOURCE_FILES})
add_library(tinyxml2 SHARED tinyxml2/tinyxml2.h tinyxml2/tinyxml2.cpp)
add_executable(pkg main.cpp)
target_link_libraries(pkg6 debug tinyxml2 boost_filesystem boost_system)
target_link_libraries(pkg debug pkg6)