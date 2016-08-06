//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_LICENSEINFO_H
#define PKG6_LICENSEINFO_H

#include <string>
namespace pkg {
    class LicenseInfo {
    public:
        LicenseInfo() :
                alt_pub(""),
                fmri(""),
                img(""),
                text(""),
                keyword(""),
                must_accept(false),
                must_display(false) { }

        std::string alt_pub;
        std::string fmri;
        std::string img;
        std::string text;
        std::string keyword;
        bool must_accept;
        bool must_display;
    };
};

#endif //PKG6_LICENSEINFO_H
