//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_CATALOGERROR_H
#define PKG6_CATALOGERROR_H

#include <exception>
#include <string>

namespace pkg{
    namespace exception{
        class PackageResolveException : public std::exception {
        private:
            std::string m_msg;
        public:
            PackageResolveException(const std::string& pkg)
                    : m_msg(std::string("cannot resolve ") + pkg)
            {}
            virtual const char *what() const throw() {
                return m_msg.c_str();
            }
        };

        class PackageNotExistException : public std::exception {
        private:
            std::string m_msg;
        public:
            PackageNotExistException(const std::string& pkg)
                    : m_msg(pkg + std::string(" does not exist"))
            {}
            virtual const char *what() const throw() {
                return m_msg.c_str();
            }
        };
    }
}
#endif //PKG6_CATALOGERROR_H
