#include "context.h"
#include <iostream>

using namespace std;
using namespace breeze::engine;

int main()
{
    Context ctx;

    ctx.set("name", "jack");
    ctx.set("age", 18);

    string name = ctx.get<string>("name");
    int age = ctx.get<int>("age");

    cout << "name =" << name << ", age = " << age << endl;

    return 0;
}