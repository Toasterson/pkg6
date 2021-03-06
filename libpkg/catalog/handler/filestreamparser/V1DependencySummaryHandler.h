//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_CATALOGDEPENDENCYHANDLER_H
#define PKG6_CATALOGDEPENDENCYHANDLER_H
#include "rapidjson/reader.h"
#include <iostream>
#include <string>
#include <map>
#include <catalog/handler/storage/V1CatalogStorage.h>

using namespace std;
using namespace rapidjson;

namespace pkg {
    //TODO Refactor to be more usefull for a PKG5 Catalog Storage Interface
    struct V1DependencySummaryHandler : public BaseReaderHandler<UTF8<>, V1DependencySummaryHandler> {
        V1DependencySummaryHandler(V1CatalogStorage& v1CatalogStorage, Progress& progress) : state_(kExpectObjectStart), v1CatalogStorage(v1CatalogStorage), progress(progress) {}

        V1CatalogStorage v1CatalogStorage;

        Progress progress;

        PackageInfo package_ = PackageInfo();

        std::string publisher_name_;

        std::string package_name_;

        bool StartObject() {
            switch (state_) {
                case kExpectObjectStart:
                    state_ = kExpectPublisherName;
                    return true;
                case kExpectPublisherObjectStart:
                    state_ = kExpectPackageNameOrPublisherEnd;
                    return true;
                case kExpectPackageVersionObjectStart:
                    state_= kExpectPackageVersionName;
                    return true;
                case kExpectPackageVersionObjectEndOrNext:
                    state_= kExpectPackageVersionName;
                    return true;
                default:
                    return false;
            }
        }

        bool StartArray() {
            switch (state_) {
                case kExpectPackageVersionArrayStart:
                    state_ = kExpectPackageVersionObjectStart;
                    return true;
                case kExpectPackageVersionActionArrayStart:
                    state_ = kExpectPackageVersionActionArrayValueOrEnd;
                    return true;
                default:
                    return false;
            }
        }

        bool String(const char *str, SizeType length, bool) {
            switch (state_) {
                case kExpectPublisherName:
                    publisher_name_ = string(str, length);
                    state_ = kExpectPublisherObjectStart;
                    return true;
                case kExpectPackageNameOrPublisherEnd:
                    package_name_ = string(str, length);
                    state_ = kExpectPackageVersionArrayStart;
                    return true;
                case kExpectPackageVersionName:
                    package_.name = package_name_;
                    package_.publisher = publisher_name_;
                    state_ = kExpectPackageVersionValue;
                    return true;
                case kExpectPackageVersionValue:
                    package_.version = string(str, length);
                    state_ = kExpectPackageVersionActionName;
                    return true;
                case kExpectPackageVersionActionName:
                    state_ = kExpectPackageVersionActionArrayStart;
                    return true;
                case kExpectPackageVersionActionArrayValueOrEnd:
                    package_.addAction(string(str, length));
                    return true;
                default:
                    return false;
            }
        }

        bool EndArray(SizeType length){
            switch (state_) {
                case kExpectPackageVersionObjectEndOrNext:
                    state_ = kExpectPackageNameOrPublisherEnd;
                    return true;
                case kExpectPackageVersionActionArrayValueOrEnd:
                    state_ = kExpectPackageVersionObjectEndOrNext;
                    return true;
                default:
                    return false;
            }
        }

        bool EndObject(SizeType length) {
            switch (state_) {
                case kExpectPackageVersionObjectEndOrNext:
                    v1CatalogStorage.addOrUpdatePackage(package_);
                    progress++;
                    package_ = PackageInfo();
                    return true;
                case kExpectPackageNameOrPublisherEnd:
                    state_ = kExpectObjectEnd;
                    return true;
                case kExpectObjectEnd:
                    return true;
                default:
                    return false;
            }
        }

        bool Default() { return false; } // All other events are invalid.
        enum State {
            kExpectObjectStart,
            kExpectPublisherName,
            kExpectPublisherObjectStart,
            kExpectPackageNameOrPublisherEnd,
            kExpectPackageVersionArrayStart,
            kExpectPackageVersionObjectStart,
            kExpectPackageVersionName,
            kExpectPackageVersionValue,
            kExpectPackageVersionActionName,
            kExpectPackageVersionActionArrayStart,
            kExpectPackageVersionActionArrayValueOrEnd,
            kExpectPackageVersionObjectEndOrNext,
            kExpectObjectEnd,
        } state_;
    };
}

#endif //PKG6_CATALOGDEPENDENCYHANDLER_H
