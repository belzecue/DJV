#include <MaterialXCore/Util.h>

#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "MaterialX version: " << MaterialX::getVersionString() << std::endl;
    return 0;
}
