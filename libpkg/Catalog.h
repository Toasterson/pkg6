//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_CATALOGPARTBASE_H
#define PKG6_CATALOGPARTBASE_H

#include <string>
#include <chrono>
#include <map>
#include "PackageInfo.h"

namespace pkg {

    class Catalog {
        const std::string FILE_NAME = "catalog.json";
        const std::string SIG_FILE_NAME = "signatures.json";

    private:
        bool batch_mode;
        bool read_only;
        bool do_sign;
        std::string signature;
        std::string root_dir;
        std::tm last_modified;
        std::map<std::string,pkg::PackageInfo> packages;
    public:

        Catalog(bool batch_mode=false,
                std::string root="",
                bool read_only,
                bool do_sign);


        bool isRead_only() const {
            return read_only;
        }

        const std::string &getSignature() const {
            return signature;
        }

        const tm &getLast_modified() const {
            return last_modified;
        }

        const std::map<std::string, PackageInfo> &getPackages() const {
            return packages;
        }

        void destroy();

        bool exists();

        bool load();

        bool importpkg5();

        std::string fullpath();

        bool save();

        bool finalize();


    };
};


#endif //PKG6_CATALOGPARTBASE_H
