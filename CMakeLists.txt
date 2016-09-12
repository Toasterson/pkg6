cmake_minimum_required(VERSION 3.4)
project(pkg6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(lib libpkg lib/beast/include)
link_directories(/usr/local/lib)
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
        libpkg/action/AttributeAction.cpp libpkg/action/AttributeAction.h libpkg/action/ActionException.h
        libpkg/action/DependAction.cpp libpkg/action/DependAction.h
        libpkg/action/DirectoryAction.cpp libpkg/action/DirectoryAction.h
        libpkg/action/Action.cpp libpkg/action/Action.h
        libpkg/vectoradd.h
        libpkg/package/PackageException.h libpkg/image/ImagePlan.cpp
        libpkg/image/ImagePlan.h libpkg/catalog/CatalogError.h
        libpkg/http_client/HttpClient.cpp libpkg/http_client/HttpClient.h
        libpkg/http_client/HTTPException.h libpkg/image/ImageException.h
        libpkg/publisher/Publisher.h
        libpkg/action/FileAction.cpp libpkg/action/FileAction.h)
add_library(pkg6 SHARED ${LIBPKG_SOURCE_FILES})
add_library(tinyxml2 SHARED lib/tinyxml2/tinyxml2.h lib/tinyxml2/tinyxml2.cpp)
add_executable(pkg pkg/main.cpp)
target_link_libraries(pkg6 debug tinyxml2 boost_filesystem boost_system)
target_link_libraries(pkg debug pkg6 boost_program_options)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(pkg_tests
        libpkg/action_tests/AttributeActionTest.cpp
        libpkg/package_tests/PackageInfoTest.cpp
        libpkg/action_tests/DependActionTest.cpp

        libpkg/image_tests/ImagePlanTest.cpp
        libpkg/image_tests/ImageConfigTest.cpp
        libpkg/action_tests/FileActionTest.cpp)
target_link_libraries(pkg_tests ${GTEST_BOTH_LIBRARIES} pkg6)
add_test(AllTestInPKG6 pkg_tests)
