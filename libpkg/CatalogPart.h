//
// Created by toast on 13.04.16.
//

#ifndef PKG6_CATALOGPART_H
#define PKG6_CATALOGPART_H


#include <vector>
#include <map>
#include "CatalogBase.h"

class CatalogPart: public CatalogBase {
    bool ordered;
    std::map<std::string,std::string> data;
    bool add();
    bool remove(std::string& fmri);
    std::string getEntry(std::string& fmri);
    int getPackageCountofPublisher(std::string& publisher);
    int getPackageVersionsofPublisher(std::string publisher);
    void sort(std::vector<std::string>& fmris);
};


#endif //PKG6_CATALOGPART_H
