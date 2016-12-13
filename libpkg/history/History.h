//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_HISTORY_H
#define PKG6_HISTORY_H

#include <string>
#include <history/HistoryOperation.h>
#include <map>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <tinyxml2/tinyxml2.h>
#include <pkgdefs.h>
#define BOOST_SYSTEM_NO_DEPRECATED
#include "boost/filesystem.hpp"
#include <iomanip>

namespace pkg {
    namespace history {
        // Constants for the (outcome, reason) combination for operation result
        // and reason.  The first field, 'outcome' should be a single word to allow easy
        // extraction from 'pkg history' but 'reason' may be a phrase.
        // Indicates that the user canceled the operation.
        const std::string RESULT_CANCELED = "Canceled, None";
        // Indicates that the operation had no work to perform or didn't need to make
        // any changes to the image.
        const std::string RESULT_NOTHING_TO_DO = "Ignored, Nothing to do";
        // Indicates that the operation succeeded.
        const std::string RESULT_SUCCEEDED = "Succeeded, None";
        // Indicates that the user or client provided bad information which resulted in
        // operation failure.
        const std::string RESULT_FAILED_BAD_REQUEST = "Failed, Bad Request";
        // Indicates that the operation failed due to a configuration error (such as an
        // invalid SSL Certificate, etc.).
        const std::string RESULT_FAILED_CONFIGURATION = "Failed, Configuration";
        // Indicates that the operation failed due to package constraints or because of
        // a restriction enforced by the client (e.g. pkg(5) out of date).
        const std::string RESULT_FAILED_CONSTRAINED = "Failed, Constrained";
        // Indicates an operation failed because the image was already in use.
        const std::string RESULT_FAILED_LOCKED = "Failed, Locked";
        // Indicates that a search operation failed.
        const std::string RESULT_FAILED_SEARCH = "Failed, Search";
        // Indicates that there was a problem reading, writing, or accessing a file.
        const std::string RESULT_FAILED_STORAGE = "Failed, Storage";
        // Indicates that a transport error caused the operation to fail.
        const std::string RESULT_FAILED_TRANSPORT = "Failed, Transport";
        // Indicates that the operation failed due to an actuator problem
        const std::string RESULT_FAILED_ACTUATOR = "Failed, Actuator";
        // Indicates that the operation failed due to not enough memory
        const std::string RESULT_FAILED_OUTOFMEMORY = "Failed, Out of Memory";
        // Indicates that the operation failed because of conflicting actions
        const std::string RESULT_CONFLICTING_ACTIONS = "Failed, Conflicting Actions";
        // Indicates that the operation failed for an unknown reason.
        const std::string RESULT_FAILED_UNKNOWN = "Failed, Unknown";

        // Operations that are discarded, not saved, when recorded by history.
        const std::string DISCARDED_OPERATIONS[] = {"contents", "info", "list"};

        //TODO Refactor into interface
        class History {
            /*
             * A History object is a representation of data about a pkg(5) client
             * and about operations that the client is executing or has executed.  It
             * uses the HistoryOperation class to represent the data about an operation.
             */
        private:
            // The directory where the history directory can be found (or
            // created if it doesn't exist).
            std::string root_dir;
            // The name of the client (e.g. pkg, etc.)
            std::string client_name;
            // The version of the client (e.g. 093ca22da67c).
            std::string client_version;
            // How the client was invoked (e.g. 'pkg install -n foo').
            std::string client_args;
            //The Operations performed during this history action
            HistoryOperation operation;

            void importpkg5();

            const char *ISO8601_PARSE_STRING = "%Y%m%dT%H%M%SZ";
            const char *SNAPSHOT_PARSE_STRING = "%Y-%m-%d-%H:%M:%S";

        public:
            History(const std::string &root_dir):
                    root_dir(root_dir),
                    client_name(""),
                    client_version(""),
                    client_args(""),
                    operation(HistoryOperation()) { }

            History(const std::string &root_dir, const std::string &filename):
                    root_dir(root_dir)
            {
                load(filename);
            }

            const std::string &getClient_name() const {
                return client_name;
            }

            const std::string &getClient_version() const {
                return client_version;
            }

            const std::string &getClient_args() const {
                return client_args;
            }

            void setClient_name(const std::string &client_name) {
                History::client_name = client_name;
            }

            void setClient_version(const std::string &client_version) {
                History::client_version = client_version;
            }

            void setClient_args(const std::string &client_args) {
                History::client_args = client_args;
            }

            /*
             * The directory where history files will be written to or read from.
             */
            std::string path();

            /*
             * Returns the pathname that the history information was read
             * from or will attempted to be written to.  Returns None if no
             * operation has started yet or if no operation has been loaded.
             */
            std::string pathname();

            /*
             * Generates the lines of release notes for this operation.
             * If no release notes are present, no output occurs.
             */
            std::string notes();

            /*
             * Discards all information related to the current history
             * object.
             */
            void clear();

            /*
             * Removes all history information by deleting the directory
             * indicated by the value "path" and then creates a new history
             * entry to record that this purge occurred.
             */
            void purge(const std::string &be_name = "", const std::string &be_uuid = "");

            /*
             * Intended to be used by the client during top-level error
             * handling to indicate that an unrecoverable error occurred
             * during the current operation(s).  This allows History to end
             * all of the current operations properly and handle any possible
             * error that might be encountered in History itself.
             */
            void abort(const std::string &result);

            /**
             * Marks the start of an operation to be recorded in image history.
             */
            void logStart(const std::string &name, const std::string &be_name, const std::string &be_uuid);

            /*
             * Marks the end of an operation to be recorded in image history.
             * 'result' should be a pkg.client.history constant value
             * representing the outcome of an operation.  If not provided,
             * and 'error' is provided, the final result of the operation will
             * be based on the class of 'error' and 'error' will be recorded
             * for the current operation.  If 'result' and 'error' is not
             * provided, success is assumed.
             */
            //TODO Check if instead of string a error class setup would be better i think
            void logEnd(const std::string &error = "", const std::string &result = "", const std::string &release_notes = "");

            /*
             * Adds an error to the list of error to be recorded in image
             * history for the current operation
             */
            //TODO Check if instead of string a error class setup would be better
            void logError(const std::string &error = "");

            //TODO Snapshot functions

            void load(const std::string &filename);

            void save();

            void upgrade_format(const std::string &newRoot = "");

        };
    };
};


#endif //PKG6_HISTORY_H
