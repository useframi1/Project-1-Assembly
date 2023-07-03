#include "functions/ISS.h"
#include "functions/readFile.h"

int main(int argc, char *argv[])
{
    try
    {
        string machineCode = readFile(argv, argc - 1);
        ISS(machineCode);
    }
    catch (string e)
    {
        cerr << e << endl;
    }
}