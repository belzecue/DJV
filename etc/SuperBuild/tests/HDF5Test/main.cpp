#include <H5Cpp.h>

#include <iostream>

int main(int argc, char* argv[])
{
    unsigned major = 0;
    unsigned minor = 0;
    unsigned release = 0;
    H5::H5Library::getLibVersion(major, minor, release);
    std::cout << "HDF5 version: " << major << "." << minor << "." << release << std::endl;
    return 0;
}
