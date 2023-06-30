#include <iostream>
#include <cmath>
using namespace std;

struct Instruction
{
    int func7;
    int rd;
    int rs1;
    int rs2;
    int func3;
    int immediate;
    char type;
    bool isCompressed;
};

void getTwosComplement(string &binary)
{
    bool foundOne = false;
    for (int i = binary.length() - 1; i >= 0; i--)
    {
        if (foundOne)
        {
            if (binary[i] == '0')
                binary[i] = '1';
            else
                binary[i] = '0';
        }
        if (binary[i] == '1' && !foundOne)
        {
            foundOne = true;
        }
    }
}

int binaryToDec(string binary, bool IsUnsigned)
{
    bool isNeg = false;
    int decimal = 0;
    if (!IsUnsigned && binary[0] == 1)
    {
        getTwosComplement(binary);
        isNeg = true;
    }

    for (int i = binary.length() - 1; i >= 0; i--)
    {
        decimal = decimal + (binary[i] - 48) * pow(2, i);
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
        getTwosComplement(binary);

    return binary;
}

string SRL(string rs1, int rs2)
{
    string temp = rs1;
    for (int i = 0; i < rs2; i++)
    {
        for (int j = 31; j > 0; j--)
        {
            temp[j] = rs1[j - 1];
        }
        temp[0] = '0';
    }
    return temp;
}

void executeRType(Instruction inst, string registers[], string memory[])
{
    switch (inst.func3)
    {
    case 0:
        int rs1 = binaryToDec(registers[inst.rs1], false);
        int rs2 = binaryToDec(registers[inst.rs2], false);
        if (inst.func7 == 32)
            rs2 *= -1;
        int sum = rs1 + rs2;
        registers[inst.rd] = decToBinary(sum);
        break;
    case 1:
        int rs1 = binaryToDec(registers[inst.rs1], false);
        int rs2 = binaryToDec(registers[inst.rs2], false);
        int sll = rs1 * pow(2, rs2);
        registers[inst.rd] = decToBinary(sll);
        break;
    case 2:
        int rs1 = binaryToDec(registers[inst.rs1], false);
        int rs2 = binaryToDec(registers[inst.rs2], false);
        registers[inst.rd].append(31, '0');
        registers[inst.rd] += (rs1 < rs2) ? "1" : "0";
        break;
    case 3:
        int rs1 = binaryToDec(registers[inst.rs1], true);
        int rs2 = binaryToDec(registers[inst.rs2], true);
        registers[inst.rd].append(31, '0');
        registers[inst.rd] += (rs1 < rs2) ? "1" : "0";
        break;
    case 4:
        int rs1 = binaryToDec(registers[inst.rs1], true);
        int rs2 = binaryToDec(registers[inst.rs2], true);
        int XOR = rs1 ^ rs2;
        registers[inst.rd] = decToBinary(XOR);
        break;
    case 5:
        int rs2 = binaryToDec(registers[inst.rs2], false);
        if (inst.func7 == 32)
        {
            int rs1 = binaryToDec(registers[inst.rs1], false);
            int sra = rs1 / pow(2, rs2);
            registers[inst.rd] = decToBinary(sra);
        }
        else
        {
            registers[inst.rd] = SRL(registers[inst.rs1], rs2);
        }
        break;
    case 6:
        int rs1 = binaryToDec(registers[inst.rs1], true);
        int rs2 = binaryToDec(registers[inst.rs2], true);
        int OR = rs1 | rs2;
        registers[inst.rd] = decToBinary(OR);
        break;
    case 7:
        int rs1 = binaryToDec(registers[inst.rs1], true);
        int rs2 = binaryToDec(registers[inst.rs2], true);
        int AND = rs1 & rs2;
        registers[inst.rd] = decToBinary(AND);
        break;
    default:
        break;
    }
}

void executeIType(Instruction inst, string registers[], string memory[])
{
}

void executeSType(Instruction inst, string registers[], string memory[])
{
}

void executeBType(Instruction inst, string registers[], string memory[])
{
}

void executeUType(Instruction inst, string registers[], string memory[])
{
}

void executeJType(Instruction inst, string registers[], string memory[])
{
}

void execute(Instruction inst, string registers[], string memory[], int &pc)
{
    switch (inst.type)
    {
    case 'r':
        executeRType(inst, registers, memory);
        break;
    case 'i':
        executeIType(inst, registers, memory);
        break;
    case 's':
        executeSType(inst, registers, memory);
        break;
    case 'b':
        executeBType(inst, registers, memory);
        break;
    case 'u':
        executeUType(inst, registers, memory);
        break;
    case 'j':
        executeJType(inst, registers, memory);
        break;

    default:
        break;
    }

    pc += inst.isCompressed ? 2 : 4;
}