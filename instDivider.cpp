#include <iostream>
using namespace std;

/// @brief takes the whole machine code and divides it into bytes then stores every byte in the memory
/// @param machineCode the whole binary code fetched from the file
/// @param memory the memory array of size 80 000
/// @param error the error message
void instDivider(string machineCode, string memory[], string &error)
{
    // checks if the number of bytes is greater than the text section capacity (64kbytes)
    if (machineCode.length() / 8 >= 64000)
    {
        error = "Memory Overflow detected!";
        throw error;
    }
    else
    {
        // initialize memory
        for (int i = 0; i < 80000; i++)
        {
            memory[i] = "";
        }

        // store every 8 bits in a memory index
        int j = 0;
        for (int i = 0; i < machineCode.length(); i++)
        {
            if (memory[j].length() == 8)
            {
                j++;
            }
            memory[j] += machineCode[i];
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