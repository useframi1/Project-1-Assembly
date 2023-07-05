#ifndef __LOADMEMORY_H__
#define __LOADMEMORY_H__

#include <iostream>
using namespace std;

/// @brief takes the whole machine code and divides it into bytes then stores every byte in the memory
/// @param machineCode the whole binary code fetched from the file
/// @param memory the memory array of size 80 000
/// @param error the error message
void loadMemory(string machineCode, string memory[])
{
    // checks if the number of bytes is greater than the text section capacity (64kbytes)
    if (machineCode.length() / 8 >= 81921)
    {
        string error = "Memory Overflow detected!";
        throw error;
    }
    else
    {
        // initialize memory
        for (int i = 0; i < 81920; i++)
        {
            memory[i] = "";
            memory[i].append(8, '0');
        }

        // store every 8 bits in a memory index
        int j = 0;
        int k = 0;
        for (int i = 0; i < machineCode.length(); i++)
        {
            if (k == 8)
            {
                j++;
                k = 0;
            }
            memory[j][k] = machineCode[i];
            k++;
        }
    }
}

// int main()
// {
//     string memory[80000];
//     string error = "";
//     try
//     {
//         instDivider("01010101010101010101010101010101", memory, error);
//         for (int i = 0; i < 10; i++)
//         {
//             cout << memory[i] << endl;
//         }
//     }
//     catch (string e)
//     {
//         cerr << e << endl;
//     }
// }
#endif // __LOADMEMORY_H__