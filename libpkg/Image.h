//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_IMAGE_H
#define PKG6_IMAGE_H

#include <string>
#include <vector>
#include "History.h"
#include "ImageConfig.h"

namespace pkg {
    class Image {
        /*
         * An Image object is a directory tree containing the laid-down contents
            of a self-consistent graph of Packages.

            An Image has a root path.

            An Image of type IMG_ENTIRE does not have a parent Image.  Other Image
            types must have a parent Image.  The external state of the parent Image
            must be accessible from the Image's context, or duplicated within the
            Image (IMG_PARTIAL for zones, for instance).

            The parent of a user Image can be a partial Image.  The parent of a
            partial Image must be an entire Image.

            An Image of type IMG_USER stores its external state at self.root +
            ".org.opensolaris,pkg".

            An Image of type IMG_ENTIRE or IMG_PARTIAL stores its external state at
            self.root + "/var/pkg".

            An Image needs to be able to have a different repository set than the
            system's root Image.

            For image format details, see section 5.3 of doc/on-disk-format.txt
            in the pkg(5) gate.
         */
    private:

        bool allow_ondisk_upgrade;
        bool blocking_locks;
        pkg::ImageConfig config;
        pkg::history::History history;
        int version;
        bool locked;


        //TODO Check Type
        std::string transport;
        std::string linkded;
        std::string imageplan;
        std::string image_prefix;
        std::string index_dir;
        std::string plandir;
    public:
        const std::string CATALOG_KNOWN = "known";
        const std::string CATALOG_INSTALLED = "installed";
        //The absolute path of the image's location.
        std::string getImgRoot();

        //The absolute path of the image's metadata.
        std::string getImgDir();

        //A boolean value indicating whether the image is currently locked.
        bool isLocked();

        std::string getWriteCachePath();

        Image(const std::string &root, const bool &allow_ondisk_upgrade = false);

        void lock(const bool &allow_unprevileged = false);
        void unlock();
        int getImageType();
        void upgrade_format(const bool &allow_unprevileged);
        /*
         * Creates a new image with the given attributes if it does not
           exist; should not be used with an existing image.

            'is_zone' is a boolean indicating whether the image is a zone.

            'publishers' is a list of Publisher objects to configure the image
            with.

            'refresh_allowed' is an optional boolean indicating that
            network operations (such as publisher data retrieval) are
            allowed.

            'properties' is an optional map of image properties to seed from

            'variants' is an optional map of variant names and values.

            'facets' is an optional map of facet names and values.
         */
        void create(const bool &is_zone = false,
                    const std::vector<std::string> &publishers = std::vector<std::string>(),
                    const bool &refresh_allowed = false,
                    const std::map<std::string, std::string> &properties = std::map<std::string, std::string>(),
                    const std::map<std::string, std::string> &variants = std::map<std::string, std::string>(),
                    const std::map<std::string, std::string> &facets = std::map<std::string, std::string>()
        );

        std::string getArch();
        std::tm getLastModified();


    };
};

#endif //PKG6_IMAGE_H
