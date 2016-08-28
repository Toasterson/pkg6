//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_PACKAGEXCEPTION_H
#define PKG6_PACKAGEXCEPTION_H
#include <exception>
#include <string>

namespace pkg {
    namespace exception {
        class InvalidPackageException : public std::exception {
        private:
            std::string m_msg;
        public:
            InvalidPackageException(const std::string& name, const std::string& error)
                    : m_msg(std::string("Invalid package ") + name + std::string("  ") + error)
            {}
            virtual const char *what() const throw() {
                return m_msg.c_str();
            }
        };

        class InvalidFMRIException : public std::exception {
        private:
            std::string m_msg;
        public:
            InvalidFMRIException(const std::string& fmri)
                    : m_msg(std::string("Invalid package FMRI ") + fmri)
            {}
            virtual const char *what() const throw() {
                return m_msg.c_str();
            }
        };
    }
}
#endif //PKG6_PACKAGEXCEPTION_H
