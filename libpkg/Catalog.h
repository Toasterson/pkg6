//
// Created by toast on 13.04.16.
//

#ifndef PKG6_CATALOGPARTBASE_H
#define PKG6_CATALOGPARTBASE_H

#include <string>
#include <chrono>

class Catalog {
public:
    std::string name;
    std::string sign;
    std::string signatures;
    std::string root;
    std::tm last_modified;
    void destroy();
    bool exists();
    std::string load();
    std::string fullpath();
    bool save(std::string& data);

};


#endif //PKG6_CATALOGPARTBASE_H
