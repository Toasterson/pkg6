//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_CATALOGDEPENDENCYHANDLER_H
#define PKG6_CATALOGDEPENDENCYHANDLER_H
#include "reader.h"
#include "error/en.h"
#include <iostream>
#include <string>
#include <map>
#include <Catalog.h>

using namespace std;
using namespace rapidjson;

namespace pkg {
    struct CatalogActionHandler : public BaseReaderHandler<UTF8<>, CatalogActionHandler> {
        CatalogActionHandler(Catalog& catalog, Progress& progress) : state_(kExpectObjectStart), catalog(catalog), progress(progress) {}

        Catalog catalog;

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
                    catalog.addOrUpdatePackage(package_);
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
