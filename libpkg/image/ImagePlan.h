//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_IMAGEPLAN_H
#define PKG6_IMAGEPLAN_H


#include <package/PackageInfo.h>

namespace pkg {
    class ImagePlan {
        //TODO more action classes to be saved here
        std::vector<pkg::PackageInfo> packages;
    public:

        ImagePlan(){}
        ImagePlan(const std::vector<pkg::PackageInfo> &packages);

        void add(const pkg::PackageInfo &pkg);

        void add(const std::vector<pkg::PackageInfo> &pkgs);

        bool contains(const pkg::PackageInfo &pkg);

        bool contains(const std::string &fmri);

        void install();

        bool empty(){
            return packages.empty();
        }
    };
}

#endif //PKG6_IMAGEPLAN_H
