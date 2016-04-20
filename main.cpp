#include <iostream>
#include <Catalog.h>

using namespace std;
using namespace pkg;

int main() {
    Catalog installed = Catalog("/home/toast/workspace/cpp/pkg6/sample_data/pkg5/state/installed");
    installed.save();
    cout<<"\nAll Done!\n";
    return 0;
}