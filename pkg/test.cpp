//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <boost/filesystem.hpp>
#include <iostream>
#include <boost/algorithm/string.hpp>

namespace fs = boost::filesystem;
using namespace std;

int main(int argc, const char** argv) {
    string version1 = "10.0.1f";
    int major1, major2, minor1, minor2, revision1, revision2, result;
    char revChar;
    result = sscanf(version1.c_str(), "%d.%d.%d%c", &major1, &minor1, &revision1, &revChar);
    if(result > 3){
        revision1 += revChar;
    }
    printf("%d.%d.%d", major1, minor1, revision1);
    cout << "\n" << result << "\n";
}