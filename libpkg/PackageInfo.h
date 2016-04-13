//
// Created by toast on 26.03.16.
//

#ifndef PKG6_PACKAGEINFO_H
#define PKG6_PACKAGEINFO_H

#include "pkgdefs.h"
#include <string>
#include <pgtypes_date.h>
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
        std::string pkg_stem;
        std::string summary;
        pkg::PackageCategory categories[];
        int states[];
        std::string publisher;
        std::string version;
        std::string build_release;
        std::string branch;
        date packaging_date;
        //TODO check what type these could be
        std::string size;
        std::string csize;
        ////
        std::string fmri;
        LicenseInfo licenses[];
        //TODO check what types these could be
        std::string links;
        std::string hardlinks;
        std::string files;
        std::string dirs;
        std::string dependencies;
        std::string attrs[];
        ////
        std::string description;
        date last_update;
        date last_install;

    };

};
#endif //PKG6_PACKAGEINFO_H
