#ifndef __READFILE_H__
#define __READFILE_H__

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <bitset>
using namespace std;

void readTextSection(string &machineCode, char *arg)
{
	ifstream file(arg, ios::binary);
	if (file.is_open())
	{
		char byte;
		while (file.get(byte))
		{
			bitset<8> bits(byte); // Convert the byte to a bitset

			// Read each bit from the bitset and append it to the binary string
			for (int i = 7; i >= 0; i--)
			{
				machineCode += bits[i] ? '1' : '0';
			}
		}

		file.close();
	}
	else
	{
		string error = "Error opening input file.";
		throw error;
	}

	if (machineCode.length() / 8 > 65536)
	{
		machineCode = machineCode.substr(0, 65536 * 8);
	}
	else if (machineCode.length() / 8 < 65536)
	{
		int difference = 65536 - machineCode.length() / 8;
		machineCode.append(difference * 8, '0');
	}
}

void readDataSection(string &machineCode, char *arg)
{
	string dataCode = "";
	ifstream file(arg, ios::binary);
	if (file.is_open())
	{
		char byte;
		while (file.get(byte))
		{
			bitset<8> bits(byte); // Convert the byte to a bitset

			// Read each bit from the bitset and append it to the binary string
			for (int i = 7; i >= 0; i--)
			{
				dataCode += bits[i] ? '1' : '0';
			}
		}

		file.close();
	}
	else
	{
		string error = "Error opening input file.";
		throw error;
	}
	if (dataCode.length() / 8 > 16384)
	{
		dataCode = dataCode.substr(0, 16384 * 8);
	}
	else if (dataCode.length() / 8 < 16384)
	{
		int difference = 16384 - dataCode.length() / 8;
		dataCode.append(difference * 8, '0');
	}
	machineCode += dataCode;
}

string readFile(char *argv[], int numberOfFiles)
{
	string machineCode = "";

	readTextSection(machineCode, argv[1]);
	if (numberOfFiles > 1)
		readDataSection(machineCode, argv[2]);

	return machineCode;
}

#endif // __READFILE_H__