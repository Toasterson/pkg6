//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_PACKAGEINFO_H
#define PKG6_PACKAGEINFO_H

#include <string>
#include <vector>
#include <ctime>
#include "PackageCategory.h"
#include "LicenseInfo.h"

namespace pkg {
/*
 * A class capturing the information about packages that a client
 * could need. The fmri is guaranteed to be set. All other values may
 * be None, depending on how the PackageInfo instance was created.
 */
    class PackageInfo {
        /*
         Possible package states; these constants should match the values used
         by the Image class.  Constants with negative values are not currently
         available.
         */
        const int INCORPORATED = -2;
        const int EXCLUDES = -3;
        //For other States see pkgdefs.h
        //TODO Check if these two constants need to be moved to pkgdefs.h
    public:
        PackageInfo(std::string fmri);
        std::string summary;
        std::vector<pkg::PackageCategory> categories;
        std::vector<int> states;
        std::string publisher;
        std::string version;
        std::string build_release;
        std::string branch;
        std::tm packaging_date;
        std::string signature;
        //TODO check what type these could be
        std::string size;
        std::string csize;
        ////
        std::string fmri;
        std::vector<LicenseInfo> licenses;
        std::vector<std::string> links;
        std::vector<std::string> hardlinks;
        std::vector<std::string> files;
        std::vector<std::string> dirs;
        std::vector<std::string> dependencies;
        std::vector<std::string> attrs;
        std::string description;
        std::tm last_update;
        std::tm last_install;

        void load();
        void save();

    };

};
#endif //PKG6_PACKAGEINFO_H
