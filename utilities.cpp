#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <iostream>
using namespace std;

string getTwosComplement(string binary)
{
    bool foundOne = false;
    string temp = binary;
    for (int i = binary.length() - 1; i >= 0; i--)
    {
        if (foundOne)
        {
            if (binary[i] == '0')
                temp[i] = '1';
            else
                temp[i] = '0';
        }
        if (binary[i] == '1' && !foundOne)
        {
            foundOne = true;
        }
    }

    return temp;
}

long int binaryToDec(string binary, bool IsUnsigned)
{
    bool isNeg = false;
    long int decimal = 0;
    string binaryTemp = binary;
    if (!IsUnsigned && binary[0] == '1')
    {
        binaryTemp = getTwosComplement(binary);
        isNeg = true;
    }

    for (int i = binaryTemp.length() - 1; i >= 0; i--)
    {
        decimal = decimal + (binaryTemp[i] - 48) * pow(2, binaryTemp.length() - i - 1);
    }

    return (isNeg) ? -decimal : decimal;
}

string decToBinary(int num)
{
    string binary = "";
    bool isNeg = false;
    if (num < 0)
    {
        isNeg = true;
        num *= -1;
    }

    for (int i = 31; i >= 0; i--)
    {
        int k = num >> i;
        if (k & 1)
            binary += '1';
        else
            binary += '0';
    }

    if (isNeg)
        binary = getTwosComplement(binary);

    return binary;
}
#endif // __UTILITIES_H__