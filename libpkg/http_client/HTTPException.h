//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_HTTPEXCEPTION_H
#define PKG6_HTTPEXCEPTION_H
#include <exception>
#include <string>

namespace pkg {
        namespace exception {
            class HTTPBadResponseException : public std::exception {
            private:
                std::string m_msg;
            public:
                HTTPBadResponseException(const int& code)
                        : m_msg(std::string("HTTP Status code: ") + std::to_string(code))
                {}
                virtual const char *what() const throw() {
                    return m_msg.c_str();
                }
            };
        }
}
#endif //PKG6_HTTPEXCEPTION_H
