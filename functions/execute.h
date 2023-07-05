#ifndef __EXECUTE_H__
#define __EXECUTE_H__

#include <iostream>
#include <cmath>
#include "instructionStruct.h"
#include "utilities.h"
using namespace std;

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
    if (inst.rd == 0)
    {
        return;
    }

    long int rs1 = 0, rs2 = 0, sum = 0, sll = 0, XOR = 0, sra = 0, OR = 0, AND = 0;
    switch (inst.func3)
    {
    case 0:
        rs1 = binaryToDec(registers[inst.rs1], false);
        rs2 = binaryToDec(registers[inst.rs2], false);
        if (inst.func7 == 32)
            rs2 *= -1;
        sum = rs1 + rs2;
        registers[inst.rd] = decToBinary(sum);
        break;
    case 1:
        rs1 = binaryToDec(registers[inst.rs1], false);
        rs2 = binaryToDec(registers[inst.rs2], false);
        sll = rs1 * pow(2, rs2);
        registers[inst.rd] = decToBinary(sll);
        break;
    case 2:
        rs1 = binaryToDec(registers[inst.rs1], false);
        rs2 = binaryToDec(registers[inst.rs2], false);
        registers[inst.rd] = "";
        registers[inst.rd].append(31, '0');
        registers[inst.rd] += (rs1 < rs2) ? "1" : "0";
        break;
    case 3:
        rs1 = binaryToDec(registers[inst.rs1], true);
        rs2 = binaryToDec(registers[inst.rs2], true);
        registers[inst.rd] = "";
        registers[inst.rd].append(31, '0');
        registers[inst.rd] += (rs1 < rs2) ? "1" : "0";
        break;
    case 4:
        rs1 = binaryToDec(registers[inst.rs1], true);
        rs2 = binaryToDec(registers[inst.rs2], true);
        cout << rs1 << " " << rs2 << endl;
        XOR = rs1 xor rs2;
        registers[inst.rd] = decToBinary(XOR);
        break;
    case 5:
        rs2 = binaryToDec(registers[inst.rs2], false);
        if (inst.func7 == 32)
        {
            rs1 = binaryToDec(registers[inst.rs1], false);
            sra = rs1 / pow(2, rs2);
            registers[inst.rd] = decToBinary(sra);
        }
        else
        {
            registers[inst.rd] = SRL(registers[inst.rs1], rs2);
        }
        break;
    case 6:
        rs1 = binaryToDec(registers[inst.rs1], true);
        rs2 = binaryToDec(registers[inst.rs2], true);
        OR = rs1 | rs2;
        registers[inst.rd] = decToBinary(OR);
        break;
    case 7:
        rs1 = binaryToDec(registers[inst.rs1], true);
        rs2 = binaryToDec(registers[inst.rs2], true);
        AND = rs1 & rs2;
        registers[inst.rd] = decToBinary(AND);
        break;
    default:
        break;
    }
}

void executeIType(Instruction inst, string registers[], string memory[], int &pc)
{
    if (inst.rd != 0)
    {
        long int rs1 = 0, rs2 = 0, addi = 0, slli = 0, XORI = 0, srai = 0, ORI = 0, ANDI = 0;
        if (inst.opcode == "0010011")
        {
            switch (inst.func3)
            {
            case 0:
                rs1 = binaryToDec(registers[inst.rs1], false);
                addi = rs1 + inst.immediate;
                registers[inst.rd] = decToBinary(addi);
                break;
            case 1:
                rs1 = binaryToDec(registers[inst.rs1], false);
                slli = rs1 * pow(2, inst.immediate);
                registers[inst.rd] = decToBinary(slli);
                break;
            case 2:
                rs1 = binaryToDec(registers[inst.rs1], false);
                registers[inst.rd] = "";
                registers[inst.rd].append(31, '0');
                registers[inst.rd] += (rs1 < inst.immediate) ? "1" : "0";
                break;
            case 3:
                rs1 = binaryToDec(registers[inst.rs1], true);
                registers[inst.rd] = "";
                registers[inst.rd].append(31, '0');
                registers[inst.rd] += (rs1 < inst.immediate) ? "1" : "0";
                break;
            case 4:
                rs1 = binaryToDec(registers[inst.rs1], true);
                XORI = rs1 ^ inst.immediate;
                registers[inst.rd] = decToBinary(XORI);
                break;
            case 5:
                if (inst.func7 == 0)
                    registers[inst.rd] = SRL(registers[inst.rs1], inst.immediate);
                else
                {
                    rs1 = binaryToDec(registers[inst.rs1], false);
                    srai = rs1 / pow(2, inst.immediate);
                    registers[inst.rd] = decToBinary(srai);
                }
                break;
            case 6:
                rs1 = binaryToDec(registers[inst.rs1], true);
                ORI = rs1 | inst.immediate;
                registers[inst.rd] = decToBinary(ORI);
                break;
            case 7:
                rs1 = binaryToDec(registers[inst.rs1], true);
                ANDI = rs1 & inst.immediate;
                registers[inst.rd] = decToBinary(ANDI);
                break;
            default:
                break;
            }
        }
        else if (inst.opcode == "0000011")
        {
            registers[inst.rd] = "";
            rs1 = binaryToDec(registers[inst.rs1], true);

            switch (inst.func3)
            {
            case 0:
                registers[inst.rd].append(24, memory[rs1 + inst.immediate][0]);
                registers[inst.rd] += memory[rs1 + inst.immediate];
                break;
            case 1:
                registers[inst.rd].append(16, memory[rs1 + inst.immediate + 1][0]);
                registers[inst.rd] += memory[rs1 + inst.immediate + 1] + memory[rs1 + inst.immediate];
                break;
            case 2:
                registers[inst.rd] = memory[rs1 + inst.immediate + 3] + memory[rs1 + inst.immediate + 2] + memory[rs1 + inst.immediate + 1] + memory[rs1 + inst.immediate];
                break;
            case 4:
                registers[inst.rd].append(24, '0');
                registers[inst.rd] += memory[rs1 + inst.immediate];
                break;
            case 5:
                registers[inst.rd].append(16, '0');
                registers[inst.rd] += memory[rs1 + inst.immediate + 1] + memory[rs1 + inst.immediate];
                break;

            default:
                break;
            }
        }
    }

    if (inst.opcode == "1100111")
    {
        if (inst.rd != 0)
        {
            registers[inst.rd] = decToBinary(pc + ((inst.isCompressed) ? 2 : 4));
        }
        pc = binaryToDec(registers[inst.rs1], true) + inst.immediate;
    }

    if (inst.opcode == "1110011")
    {
        int baseAddress = 0, value = 0;
        string message = "";
        switch (binaryToDec(registers[17], true))
        {
        case 1:
            value = binaryToDec(registers[10], false);
            cout << value << endl;
            break;
        case 4:
            baseAddress = binaryToDec(registers[10], true);
            value = binaryToDec(memory[baseAddress], false);
            while (value != 0)
            {
                message += value;
                baseAddress++;
                value = binaryToDec(memory[baseAddress], false);
            }
            cout << message << endl;
            break;
        case 10:
            message = "Program ended with exit code 0";
            throw message;
        default:
            break;
        }
    }
}

void executeSType(Instruction inst, string registers[], string memory[])
{
    string toStore = "";
    string rs2 = registers[inst.rs2];
    long int rs1 = binaryToDec(registers[inst.rs1], true);
    switch (inst.func3)
    {
    case 0:
        toStore = rs2.substr(24, 8);
        memory[rs1 + inst.immediate] = toStore;
        break;
    case 1:
        toStore = rs2.substr(24, 8);
        memory[rs1 + inst.immediate] = toStore;
        toStore = rs2.substr(16, 8);
        memory[rs1 + inst.immediate + 1] = toStore;
        break;
    case 2:
        toStore = rs2.substr(24, 8);
        memory[rs1 + inst.immediate] = toStore;
        toStore = rs2.substr(16, 8);
        memory[rs1 + inst.immediate + 1] = toStore;
        toStore = rs2.substr(8, 8);
        memory[rs1 + inst.immediate + 2] = toStore;
        toStore = rs2.substr(0, 8);
        memory[rs1 + inst.immediate + 3] = toStore;
        break;

    default:
        break;
    }
}

void executeBType(Instruction inst, string registers[], string memory[], int &pc)
{
    long int rs1 = binaryToDec(registers[inst.rs1], false);
    long int rs2 = binaryToDec(registers[inst.rs2], false);
    long int rs1U = binaryToDec(registers[inst.rs1], true);
    long int rs2U = binaryToDec(registers[inst.rs2], true);

    switch (inst.func3)
    {
    case 0:
        if (rs1 == rs2)
            pc += inst.immediate;
        else
            pc += inst.isCompressed ? 2 : 4;
        break;
    case 1:
        if (rs1 != rs2)
            pc += inst.immediate;
        else
            pc += inst.isCompressed ? 2 : 4;
        break;
    case 4:
        if (rs1 < rs2)
            pc += inst.immediate;
        else
            pc += inst.isCompressed ? 2 : 4;
        break;
    case 5:
        if (rs1 >= rs2)
            pc += inst.immediate;
        else
            pc += inst.isCompressed ? 2 : 4;
        break;
    case 6:
        if (rs1U < rs2U)
            pc += inst.immediate;
        else
            pc += inst.isCompressed ? 2 : 4;
        break;
    case 7:
        if (rs1U >= rs2U)
            pc += inst.immediate;
        else
            pc += inst.isCompressed ? 2 : 4;
        break;

    default:
        break;
    }
}

void executeUType(Instruction inst, string registers[], string memory[], int pc)
{
    if (inst.rd == 0)
    {
        return;
    }

    string imm = decToBinary(inst.immediate);
    imm.append(12, '0');
    imm.erase(0, 12);

    if (inst.opcode == "0110111")
    {
        registers[inst.rd] = imm;
    }
    else if (inst.opcode == "0010111")
    {
        registers[inst.rd] = decToBinary(pc + binaryToDec(imm, true));
    }
}

void executeJType(Instruction inst, string registers[], string memory[], int &pc)
{
    if (inst.rd != 0)
    {
        registers[inst.rd] = decToBinary(pc + ((inst.isCompressed) ? 2 : 4));
    }
    pc += inst.immediate;
}

void execute(Instruction inst, string registers[], string memory[], int &pc)
{
    switch (inst.type)
    {
    case 'r':
        executeRType(inst, registers, memory);
        break;
    case 'i':
        executeIType(inst, registers, memory, pc);
        break;
    case 's':
        executeSType(inst, registers, memory);
        break;
    case 'b':
        executeBType(inst, registers, memory, pc);
        break;
    case 'u':
        executeUType(inst, registers, memory, pc);
        break;
    case 'j':
        executeJType(inst, registers, memory, pc);
        break;

    default:
        break;
    }
    if (inst.opcode != "1100111" && inst.type != 'j' && inst.type != 'b')
    {
        pc += inst.isCompressed ? 2 : 4;
    }
}

// int main()
// {
//     Instruction inst;
//     inst.opcode = "0010111";
//     inst.type = 'j';
//     inst.rd = 5;
//     inst.func3 = 4;
//     inst.rs1 = 2;
//     inst.rs2 = 10;
//     inst.immediate = 5;
//     inst.func7 = 0;

//     string registers[32];
//     string memory[80000];
//     int pc = 5;
//     for (int i = 0; i < 32; i++)
//     {
//         registers[i] = "";
//         registers[i].append(32, '0');
//     }

//     memory[0] = "01000001";
//     memory[1] = "01000010";
//     memory[2] = "01000011";
//     memory[3] = "01000100";

//     registers[inst.rs2] = "";
//     registers[inst.rs2].append(31, '1');
//     registers[inst.rs2] += "0";

//     // registers[inst.rs1].erase(29, 3);
//     // registers[inst.rs1] += "111";

//     registers[inst.rs1].erase(29, 3);
//     registers[inst.rs1] += "101";

//     execute(inst, registers, memory, pc);

//     for (int i = 0; i < 32; i++)
//     {
//         cout << i << ":\t" << registers[i] << endl;
//     }

//     for (int i = 0; i < 4; i++)
//     {
//         cout << memory[i] << endl;
//     }

//     cout << pc << endl;
// }
#endif // __EXECUTE_H__