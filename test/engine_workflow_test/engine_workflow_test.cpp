#include "workflow.h"
#include <iostream>
#include <unistd.h>

using namespace std;
using namespace breeze::engine;

int main()
{
    auto workflow = Singleton<Workflow>::Instance();

    char path[1024] = {0};

    string config_path = "/home/breeze/exercise/web/config/workflow.xml";
    
    workflow -> load(config_path);

    const string& input = "echo_plugin request";
    string output;

    workflow -> run(1, input, output);

    std::cout << output << std::endl;
    
    return 0;
}