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
    string opcode;
};

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

int binaryToDec(string binary, bool IsUnsigned)
{
    bool isNeg = false;
    int decimal = 0;
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
        registers[inst.rd] = "";
        registers[inst.rd].append(31, '0');
        registers[inst.rd] += (rs1 < rs2) ? "1" : "0";
        break;
    case 3:
        int rs1 = binaryToDec(registers[inst.rs1], true);
        int rs2 = binaryToDec(registers[inst.rs2], true);
        registers[inst.rd] = "";
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

void executeIType(Instruction inst, string registers[], string memory[], int &pc)
{
    if (inst.rd != 0)
    {
        if (inst.opcode == "0010011")
        {
            switch (inst.func3)
            {
            case 0:
                int rs1 = binaryToDec(registers[inst.rs1], false);
                int addi = rs1 + inst.immediate;
                registers[inst.rd] = decToBinary(addi);
                break;
            case 1:
                int rs1 = binaryToDec(registers[inst.rs1], false);
                int slli = rs1 * pow(2, inst.immediate);
                registers[inst.rd] = decToBinary(slli);
                break;
            case 2:
                int rs1 = binaryToDec(registers[inst.rs1], false);
                registers[inst.rd] = "";
                registers[inst.rd].append(31, '0');
                registers[inst.rd] += (rs1 < inst.immediate) ? "1" : "0";
                break;
            case 3:
                int rs1 = binaryToDec(registers[inst.rs1], true);
                registers[inst.rd] = "";
                registers[inst.rd].append(31, '0');
                registers[inst.rd] += (rs1 < inst.immediate) ? "1" : "0";
                break;
            case 4:
                int rs1 = binaryToDec(registers[inst.rs1], true);
                int XORI = rs1 ^ inst.immediate;
                registers[inst.rd] = decToBinary(XORI);
                break;
            case 5:
                if (inst.func7 == 0)
                    registers[inst.rd] = SRL(registers[inst.rs1], inst.immediate);
                else
                {
                    int rs1 = binaryToDec(registers[inst.rs1], false);
                    int srai = rs1 / pow(2, inst.immediate);
                    registers[inst.rd] = decToBinary(srai);
                }
                break;
            case 6:
                int rs1 = binaryToDec(registers[inst.rs1], true);
                int ORI = rs1 | inst.immediate;
                registers[inst.rd] = decToBinary(ORI);
                break;
            case 7:
                int rs1 = binaryToDec(registers[inst.rs1], true);
                int ANDI = rs1 & inst.immediate;
                registers[inst.rd] = decToBinary(ANDI);
                break;
            default:
                break;
            }
        }
        else if (inst.opcode == "0000011")
        {
            registers[inst.rd] = "";
            int rs1 = binaryToDec(registers[inst.rs1], true);

            switch (inst.func3)
            {
            case 0:
                registers[inst.rd].append(24, memory[rs1 + inst.immediate][0]);
                registers[inst.rd] += memory[inst.rs1 + inst.immediate];
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
        else if (inst.opcode == "1100111")
        {
            registers[inst.rd] = pc + ((inst.isCompressed) ? 2 : 4);
            pc = binaryToDec(registers[inst.rs1], true) + inst.immediate;
        }
    }

    if (inst.opcode == "1110011")
    {
        string message = "";
        switch (binaryToDec(registers[17], true))
        {
        case 1:
            cout << binaryToDec(memory[binaryToDec(registers[10], true)], false);
            break;
        case 4:
            int baseAddress = binaryToDec(registers[10], true);
            int value = binaryToDec(memory[baseAddress], false);
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
    int rs1 = binaryToDec(registers[inst.rs1], true);
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
    int rs1 = binaryToDec(registers[inst.rs1], false);
    int rs2 = binaryToDec(registers[inst.rs2], false);
    int rs1U = binaryToDec(registers[inst.rs1], true);
    int rs2U = binaryToDec(registers[inst.rs2], true);

    switch (inst.func3)
    {
    case 0:
        if (rs1 == rs2)
            pc += inst.immediate;
        break;
    case 1:
        if (rs1 != rs2)
            pc += inst.immediate;
        break;
    case 4:
        if (rs1 < rs2)
            pc += inst.immediate;
        break;
    case 5:
        if (rs1 >= rs2)
            pc += inst.immediate;
        break;
    case 6:
        if (rs1U < rs2U)
            pc += inst.immediate;
        break;
    case 7:
        if (rs1U >= rs2U)
            pc += inst.immediate;
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
        registers[inst.rd] = pc + binaryToDec(imm, true);
    }
}

void executeJType(Instruction inst, string registers[], string memory[], int &pc)
{
    if (inst.rd != 0)
    {
        registers[inst.rd] = pc + ((inst.isCompressed) ? 2 : 4);
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