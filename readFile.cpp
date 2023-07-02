#ifndef __READFILE_H__
#define __READFILE_H__

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <bitset>
using namespace std;

// this project is to create a simulator of a disassembler
// the disassembler will take in a binary file that contains machine code and output the assembly code
// however this file should be read from the command line

// the decompressor function will check the opcode of every 16 bit instruction and if decompression is needed it will translate to 32 bits

string readFile(char *argv[])
{
	ifstream inFile(argv[1], ios::binary);
	string machineCode;

	if (inFile.is_open())
	{
		char byte;
		while (inFile.get(byte))
		{
			bitset<8> bits(byte); // Convert the byte to a bitset

			// Read each bit from the bitset and append it to the binary string
			for (int i = 7; i >= 0; i--)
			{
				machineCode += bits[i] ? '1' : '0';
			}
		}

		// cout << "Machine Code: " << machineCode << endl;
		inFile.close();
	}
	else
	{
		cout << "Error opening input file." << endl;
	}
	return machineCode;
}

#endif // __READFILE_H__