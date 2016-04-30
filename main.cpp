#include <iostream>
#include <Catalog.h>

using namespace std;
using namespace pkg;

int main() {
    Catalog installed = Catalog("/home/toast/workspace/cpp/pkg6/sample_data/pkg5/state/installed", "installed.db");
    installed.upgrade_format("/home/toast/workspace/cpp/pkg6/sample_data/pkg6/state");

    cout << "All Done!" << endl;
    return 0;
}