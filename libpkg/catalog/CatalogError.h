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
        //TODO Refactor to generic class with informative error message and information
        class PackageResolveException : public std::exception {
        private:
            std::string m_msg;
        public:
            explicit PackageResolveException(const std::string& pkg)
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
            explicit PackageNotExistException(const std::string& pkg)
                    : m_msg(pkg + std::string(" does not exist"))
            {}
            virtual const char *what() const throw() {
                return m_msg.c_str();
            }
        };

        class PackageLoadException : public std::exception {
        private:
            std::string m_msg;
        public:
            explicit PackageLoadException(const std::string& pkg)
                    : m_msg(std::string("can not load ")+pkg)
            {}
            virtual const char *what() const throw() {
                return m_msg.c_str();
            }
        };

        class PackageSaveException : public std::exception {
        private:
            std::string m_msg;
        public:
            explicit PackageSaveException(const std::string& pkg)
                    : m_msg(std::string("can not save ")+pkg)
            {}
            virtual const char *what() const throw() {
                return m_msg.c_str();
            }
        };
    }
}
#endif //PKG6_CATALOGERROR_H
