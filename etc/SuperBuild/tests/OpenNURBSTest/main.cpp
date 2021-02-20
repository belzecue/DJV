#include <opennurbs/opennurbs.h>

#include <iostream>

int main(int argc, char* argv[])
{
    ON_UUID uuid;
    ON_CreateUuid(uuid);
    return 0;
}
