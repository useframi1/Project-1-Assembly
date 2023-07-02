#include "ISS.cpp"
#include "readFile.cpp"

int main(char *args[])
{
    string machineCode = readFile(args);
    ISS(machineCode);
}