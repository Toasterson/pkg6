//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_PACKAGEINFO_H
#define PKG6_PACKAGEINFO_H
#define RAPIDJSON_HAS_STDSTRING 1
#include <string>
#include <vector>
#include <ctime>
#include "PackageCategory.h"
#include "image/LicenseInfo.h"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <action/Action.h>
#include <action/AttributeAction.h>
#include <action/DependAction.h>
#include <action/DirectoryAction.h>
#include <action/FileAction.h>
#include <sstream>
#include <iomanip>

using namespace rapidjson;
using namespace  pkg::action;

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
        static const int INCORPORATED = -2;
        static const int EXCLUDES = -3;
        //For other States see pkgdefs.h
        //TODO Check if these two constants need to be moved to pkgdefs.h
    public:
        std::vector<pkg::PackageCategory> categories;
        std::vector<int> states;
        std::string publisher;
        std::string version;
        std::string signature;
        std::string build_release;
        std::string branch;
        std::tm packaging_date;
        std::string summary;
        std::string description;
        bool obsolete;
        bool renamed;
        std::vector<std::string> classification;
        std::string humanversion;

        int size;
        int csize;

        std::string name;
        std::vector<LicenseInfo> licenses;
        std::tm last_update;
        std::tm last_install;

        std::vector<AttributeAction> attrs;
        std::vector<DependAction> dependencies;
        std::vector<DirectoryAction> dirs;
        std::vector<AttributeAction> links;
        std::vector<FileAction> files;

        PackageInfo(){}
        PackageInfo(const std::string& publisher, const std::string& name, const std::string& version, const std::string& branch, const std::string& build, const std::string& packaging_date):
                publisher{publisher},
                name{name},
                version{version},
                build_release{build},
                branch{branch}
        {
            setPackagingDate(packaging_date);
        }
        PackageInfo(const PackageInfo& origin):
                publisher{origin.publisher},
                name{origin.name},
                version{origin.version},
                branch{origin.branch},
                build_release{origin.build_release}
        {
            setPackagingDate(origin.getPackagingDate());
        }
        PackageInfo(const std::string& FMRI){
            setFmri(FMRI);
        }

        std::string getFmri() const {
            return publisher + "/" + name + "@" + version + "," + build_release + "-" + branch + ":" +
                    getPackagingDate();
        }

        string getPackagingDate() const {
            char buffer[80];
            strftime(buffer, 80, "%Y%m%dT%H%M%SZ", &packaging_date);
            return string(buffer);
        }

        void setPackagingDate(const string &datestring){
            stringstream ss(datestring);
            ss >> get_time(&packaging_date, "%Y%m%dT%H%M%SZ");
        }

        void setFmri(const std::string& fmri);

        void addAction(const std::string& action_string);

        void commitManifest(istream& manifest);

        void markObsolete();

        void markRenamed();

        bool hasAttr(const AttributeAction &alternate, const bool &exact = false);

        bool hasDependency(const DependAction &alternate);

        PackageInfo operator+=(const PackageInfo& alternate);
        bool operator==(PackageInfo& alternate);
    };

};
#endif //PKG6_PACKAGEINFO_H
