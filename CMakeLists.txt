cmake_minimum_required(VERSION 3.4)
project(pkg6)

find_package(Boost COMPONENTS system filesystem program_options REQUIRED )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(tinyxml2 libpkg simpleini ${Boost_INCLUDE_DIR})
set(LIBPKG_SOURCE_FILES
        libpkg/Image.cpp libpkg/Image.h
        libpkg/LicenseInfo.cpp libpkg/LicenseInfo.h
        libpkg/pkgdefs.h
        libpkg/PackageCategory.cpp libpkg/PackageCategory.h
        libpkg/PackageInfo.cpp libpkg/PackageInfo.h
        libpkg/History.cpp libpkg/History.h
        libpkg/HistoryOperation.h
        libpkg/Catalog.cpp libpkg/Catalog.h
        libpkg/ImageConfig.cpp libpkg/ImageConfig.h
        simpleini/SimpleIni.h
        tinyxml2/tinyxml2.h tinyxml2/tinyxml2.cpp
        main.cpp)
add_executable(pkg ${LIBPKG_SOURCE_FILES})
link_libraries(pkg boost_filesystem boost_system)