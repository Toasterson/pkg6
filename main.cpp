#include <iostream>
#include <ImageConfig.h>

using namespace std;
using namespace pkg;

int main() {
    ImageConfig config = ImageConfig("/home/toast/workspace/cpp/pkg6/sample_data/pkg5");
    //cout<<config.getVariant("variant.arch");
    cout<<"\nAll Done!\n";
    return 0;
}