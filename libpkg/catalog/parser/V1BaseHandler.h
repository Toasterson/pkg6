//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_CATALOGBASEHANDLER_H
#define PKG6_CATALOGBASEHANDLER_H
#include "rapidjson/reader.h"
#include <iostream>
#include <string>
#include <map>
#include <catalog/Catalog.h>

using namespace std;
using namespace rapidjson;

namespace pkg {
    struct V1BaseHandler : public BaseReaderHandler<UTF8<>, V1BaseHandler> {
        V1BaseHandler(Catalog& catalog, Progress& progress) : state_(kExpectObjectStart), catalog(catalog), progress(progress) {}

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
                case kExpectPackageVersionMetadataObjectStart:
                    state_ = kExpectPackageVersionMetadataStatesName;
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
                case kExpectPackageVersionMetadataStatesArrayStart:
                    state_ = kExpectPackageVersionMetadataStatesArrayValueOrEnd;
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
                    state_ = kExpectPackageVersionSignatureName;
                    return true;
                case kExpectPackageVersionSignatureName:
                    state_ = kExpectPackageVersionSignatureValue;
                    return true;
                case kExpectPackageVersionSignatureValue:
                    package_.signature = string(str, length);
                    state_ = kExpectPackageVersionMetadataName;
                    return true;
                case kExpectPackageVersionMetadataName:
                    state_ = kExpectPackageVersionMetadataObjectStart;
                    return true;
                case kExpectPackageVersionMetadataStatesName:
                    state_ = kExpectPackageVersionMetadataStatesArrayStart;
                    return true;
                default:
                    return false;
            }
        }

        bool EndArray(SizeType length){
            switch (state_) {
                case kExpectPackageVersionMetadataStatesArrayValueOrEnd:
                    state_ = kExpectPackageVersionMetadataObjectEnd;
                    return true;
                case kExpectPackageVersionObjectEndOrNext:
                    state_ = kExpectPackageNameOrPublisherEnd;
                    return true;
                default:
                    return false;
            }
        }

        bool EndObject(SizeType length) {
            switch (state_) {
                case kExpectPackageVersionMetadataObjectEnd:
                    state_ = kExpectPackageVersionObjectEndOrNext;
                    return true;
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

        bool Int(int nr) {
            switch (state_) {
                case kExpectPackageVersionMetadataStatesArrayValueOrEnd:
                    package_.states.push_back(nr);
                    return true;
                default:
                    return false;
            }
        }
        bool Uint(unsigned nr) {
            switch (state_) {
                case kExpectPackageVersionMetadataStatesArrayValueOrEnd:
                    package_.states.push_back(nr);
                    return true;
                default:
                    return false;
            }
        }
        bool Double(double nr) {
            switch (state_) {
                case kExpectPackageVersionMetadataStatesArrayValueOrEnd:
                    package_.states.push_back((const int &) nr);
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
            kExpectPackageVersionSignatureName,
            kExpectPackageVersionSignatureValue,
            kExpectPackageVersionMetadataName,
            kExpectPackageVersionMetadataObjectStart,
            kExpectPackageVersionMetadataStatesName,
            kExpectPackageVersionMetadataStatesArrayStart,
            kExpectPackageVersionMetadataStatesArrayValueOrEnd,
            kExpectPackageVersionMetadataObjectEnd,
            kExpectPackageVersionObjectEndOrNext,
            kExpectObjectEnd,
        } state_;
    };
}

#endif //PKG6_CATALOGBASEHANDLER_H
