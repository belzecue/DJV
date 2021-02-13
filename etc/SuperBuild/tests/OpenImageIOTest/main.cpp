#include <OpenImageIO/imageio.h>

#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "OIIO version: " << OIIO::openimageio_version() << std::endl;
    return 0;
}
