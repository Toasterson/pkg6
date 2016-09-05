//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_IMAGEEXCEPTION_H
#define PKG6_IMAGEEXCEPTION_H

#include <exception>
#include <string>

namespace pkg {
        namespace exception {
            class ImageConfigException : public std::exception {
            private:
                std::string m_msg;
            public:
                ImageConfigException(const std::string& error)
                        : m_msg(std::string("Error parsing configuration file at: ") + error)
                {}
                virtual const char *what() const throw() {
                    return m_msg.c_str();
                }
            };
        }
}
#endif //PKG6_IMAGEEXCEPTION_H
