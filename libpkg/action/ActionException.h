//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_ACTIONEXCEPTION_H
#define PKG6_ACTIONEXCEPTION_H

#include <exception>
#include <string>

namespace pkg {
    namespace action {
        namespace exception {
            class InvalidActionException : public std::exception {
            private:
                std::string m_msg;
            public:
                InvalidActionException(const std::string& error)
                        : m_msg(std::string("invalid action ") + error)
                {}
                virtual const char *what() const throw() {
                    return m_msg.c_str();
                }
            };
        }
    }
}
#endif //PKG6_ACTIONEXCEPTION_H
