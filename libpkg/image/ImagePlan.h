//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_IMAGEPLAN_H
#define PKG6_IMAGEPLAN_H


#include <package/PackageInfo.h>

namespace pkg {
    class ImagePlan {
        //TODO more action classes to be included here
        std::vector<std::string> packages;
    public:

        void add(const pkg::PackageInfo &pkg);

        void contains(const pkg::PackageInfo &pkg);

        void install();
    };
}

#endif //PKG6_IMAGEPLAN_H
