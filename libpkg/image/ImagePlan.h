//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_IMAGEPLAN_H
#define PKG6_IMAGEPLAN_H


#include <package/PackageInfo.h>
#include "ImageConfig.h"

using namespace std;

namespace pkg {
    class ImagePlan {
    private:
        vector<pkg::PackageInfo> packages;
        string ROOT;
        string CACHE_ROOT;
        ImageConfig config;
        void getPackageMetadata();
    public:

        ImagePlan(const string& root, const std::string& cache, const ImageConfig& config):ROOT{root},CACHE_ROOT{cache},config{config} {}
        ImagePlan(const std::vector<pkg::PackageInfo> &packages, const string& root, const std::string& cache, const ImageConfig& config):
                ROOT{root},
                CACHE_ROOT{cache},
                packages{packages},
                config{config}
        {}

        void add(const pkg::PackageInfo &pkg);

        void add(const std::vector<pkg::PackageInfo> &pkgs);

        bool contains(const pkg::PackageInfo &pkg);

        bool contains(const std::string &fmri);

        void download();

        void install();

        bool empty(){
            return packages.empty();
        }
    };
}

#endif //PKG6_IMAGEPLAN_H
