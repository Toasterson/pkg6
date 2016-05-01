//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_PKGDEFS_H
#define PKG6_PKGDEFS_H


namespace pkg {
    // pkg exit codes
    static int EXIT_OK = 0; // Command succeeded.
    static int EXIT_OOPS = 1; // An error occurred.
    static int EXIT_BADOPT = 2; // Invalid command line options were specified.
    static int EXIT_PARTIAL = 3; // Multiple ops were requested, but not all succeeded.
    static int EXIT_NOP = 4; // No changes were made - nothing to do.
    static int EXIT_NOTLIVE = 5; // The requested op cannot be performed on a live image.
    static int EXIT_LICENSE = 6; // License acceptance required for requested op.
    static int EXIT_LOCKED = 7; // Image is currently locked by another process
    static int EXIT_ACTUATOR = 8; // Actuator timed out

    // private pkg exit codes
    static int EXIT_EACCESS = 51; // Can't access requested image
    static int EXIT_DIVERGED = 52; // Image is not in sync with its constraints
    static int EXIT_NOPARENT = 53; // Image is not linked to a parent image
    static int EXIT_PARENTOP = 54; // Linked operation must be done from parent

    // package operations
    namespace PKG_OP {
        static std::string ATTACH = "attach-linked";
        static std::string AUDIT_LINKED = "audit-linked";
        static std::string CHANGE_FACET = "change-facet";
        static std::string CHANGE_VARIANT = "change-variant";
        static std::string DEHYDRATE = "dehydrate";
        static std::string DETACH = "detach-linked";
        static std::string EXACT_INSTALL = "exact-install";
        static std::string FIX = "fix";
        static std::string INFO = "info";
        static std::string INSTALL = "install";
        static std::string LIST = "list";
        static std::string LIST_LINKED = "list-linked";
        static std::string PROP_LINKED = "property-linked";
        static std::string PUBCHECK = "pubcheck-linked";
        static std::string PUBLISHER_LIST = "publisher";
        static std::string REHYDRATE = "rehydrate";
        static std::string REVERT = "revert";
        static std::string SET_MEDIATOR = "set-mediator";
        static std::string SET_PUBLISHER = "set-publisher";
        static std::string SET_PROP_LINKED = "set-property-linked";
        static std::string SYNC = "sync-linked";
        static std::string UNINSTALL = "uninstall";
        static std::string UNSET_PUBLISHER = "unset-publisher";
        static std::string UPDATE = "update";
        static std::string VERIFY = "verify";
    };

    namespace API_OP {
        static std::string ATTACH = "attach-linked";
        static std::string CHANGE_FACET = "change-facet";
        static std::string CHANGE_VARIANT = "change-variant";
        static std::string DEHYDRATE = "dehydrate";
        static std::string DETACH = "detach-linked";
        static std::string EXACT_INSTALL = "exact-install";
        static std::string FIX = "fix";
        static std::string INSTALL = "install";
        static std::string REHYDRATE = "rehydrate";
        static std::string REPAIR = "repair";
        static std::string REVERT = "revert";
        static std::string MEDIATOR = "set-mediator";
        static std::string SYNC = "sync-linked";
        static std::string UNINSTALL = "uninstall";
        static std::string UPDATE = "update";
        static std::string PURGE_HISTORY = "purge-history";
    };

    namespace API_STAGE {
        static std::string DEFAULT = "default";
        static std::string PLAN = "plan";
        static std::string PREPARE = "prepare";
        static std::string EXECUTE = "execute";
    };

    namespace PKG_STATE {
        /*
         Please note that the values of these PKG_STATE constants should not
         be changed as it would invalidate existing catalog data stored in the
         image.  This means that if a constant is removed, the values of the
         other constants should not change, etc.

         This state indicates that a package is present in a repository
         catalog.
        */
        static int KNOWN = 0;

        /*
         This is a transitory state used to indicate that a package is no
         longer present in a repository catalog; it is only used to clear
         PKG_STATE_KNOWN.
         */
        static int UNKNOWN = 1;

        // This state indicates that a package is installed.
        static int INSTALLED = 2;

        /*
         This is a transitory state used to indicate that a package is no
         longer installed; it is only used to clear PKG_STATE_INSTALLED.
         */
        static int UNINSTALLED = 3;
        static int UPGRADABLE = 4;

        /*
         These states are used to indicate the package's related catalog
         version.  This is helpful to consumers of the catalog data so that
         they can be aware of what metadata may not immediately available
         (require manifest retrieval) based on the catalog version.
         */
        static int V0 = 6;
        static int V1 = 7;

        //Newly Introduced by pkg6
        static int V2 = 12;

        static int OBSOLETE = 8;
        static int RENAMED = 9;

        /*
         These states are used to indicate why a package was rejected and
         is not available for packaging operations.
         */
        static int UNSUPPORTED = 10;      // Package contains invalid or unsupported metadata.

        // This state indicates that this package is frozen.
        static int FROZEN = 11;

        /*
         This is a transitory state used for temporary package sources to
         indicate that the package entry should be removed if it does not
         also have PKG_STATE_INSTALLED.  This state must not be written
         to disk.
         */
        static int ALT_SOURCE = 99;
    };

    namespace MSG_TYPE {
        // Message types
        static int ERROR = 0;
        static int WARNING = 1;
        static int INFO = 2;
    };


};
#endif //PKG6_PKGDEFS_H
