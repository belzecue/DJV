#include <OpenColorIO/OpenColorIO.h>

#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "OpenColorIO version: " << OCIO_NAMESPACE::GetVersion() << std::endl;
    return 0;
}
