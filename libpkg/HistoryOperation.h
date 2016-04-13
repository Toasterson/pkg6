//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_HISTORYOPERATION_H
#define PKG6_HISTORYOPERATION_H

#include <string>
#include <chrono>

namespace pkg {
    namespace history {
        class HistoryOperation {
            /*
             *  A HistoryOperation object is a representation of data about an
             *  operation that a pkg(5) client has performed.  This class is private
             *  and not intended for use by classes other than History.
             *  This class provides an abstraction layer between the stack of
             *  operations that History manages should these values need to be
             *  manipulated as they are set or retrieved.
             */
        private:
            // All "time" values should be in UTC, using ISO 8601 as the format.
            // Name of the operation performed (e.g. install, update, etc.).
            std::string name;
            // When the operation started.
            std::tm start_time;
            // When the operation ended.
            std::tm end_time;
            // The starting state of the operation (e.g. image plan pre-evaluation).
            std::string start_state;
            // The ending state of the operation (e.g. image plan post-evaluation).
            std::string end_state;
            // Errors encountered during an operation.
            std::string error;
            // username of the user that performed the operation.
            std::string username;
            // id of the user that performed the operation.
            std::string userid;
            // The boot environment on which the user performed the operation
            std::string be;
            // The current name of the boot environment.
            std::string current_be;
            // The uuid of the BE on which the user performed the operation
            std::string be_uuid;
            // The new boot environment that was created as a result of the operation
            std::string new_be;
            // The current name of the new boot environment.
            std::string current_new_be;
            // The uuid of the boot environment that was created as a result of the operation
            std::string new_be_uuid;
            // The name of the file containing the release notes, or None.
            std::string release_notes;

            // The snapshot that was created while running this operation
            // set to None if no snapshot was taken, or destroyed after successful completion
            std::tm snapshot;

            // The result of the operation (must be a list indicating (outcome, reason)).
            std::string result;


            //Function returning the Result of an operation
            //TODO may be removed as its origin may be some python stuff
            //std::string result_text();
            friend class History;

            HistoryOperation(){
                name = "";
                start_time = tm();
                end_time = tm();
                start_state = "";
                end_state = "";
                error = "";
                username = "";
                userid = "";
                be = "";
                current_be = "";
                current_new_be = "";
                be_uuid = "";
                be = "";
                new_be = "";
                new_be_uuid = "";
                release_notes = "";
            }
        };
    };
};


#endif //PKG6_HISTORYOPERATION_H
