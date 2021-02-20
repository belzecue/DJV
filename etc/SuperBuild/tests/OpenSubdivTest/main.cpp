#include <opensubdiv/far/error.h>

int main(int argc, char* argv[])
{
    OpenSubdiv::Far::SetErrorCallback(nullptr);
    return 0;
}
