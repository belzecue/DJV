#include <pxr/usd/usd/timeCode.h>

#include <iostream>

int main(int argc, char* argv[])
{
    pxr::UsdTimeCode timecode(0.0);
    std::cout << "Timecode 0.0: " << timecode << std::endl;
    return 0;
}
