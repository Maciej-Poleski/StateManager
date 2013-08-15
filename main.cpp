#include <iostream>
#include <string>

#include <boost/filesystem/path.hpp>

#include "src/StateManager.hxx"

using namespace std;
using namespace boost::filesystem;

int main(int argc, char* argv[])
{
    if(argc!=2)
    {
        cerr<<argv[0]<<" [path]\n";
        return 1;
    }

    StateManager stateManager(argv[1]);

    return 0;
}
