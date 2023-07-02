#include <iostream>
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
    bool isTranslated;
};

void initialize(string registers[], Instruction instructions[]);
void instDivider(string machineCode, string memory[]);
string decompressor(string halfword);
void translate(Instruction &inst, string word);
void displayInst(Instruction inst);
void execute(Instruction inst, string registers[], string memory[], int &pc);
char getType(string opcode);

void ISS(string machineCode)
{
    string memory[80000];
    Instruction instructions[64000];
    string registers[32];
    int pc = 0;

    initialize(registers, instructions);

    try
    {
        instDivider(machineCode, memory);

        while (pc < 64000)
        {
            string halfword = memory[pc + 1] + memory[pc];
            string word;
            if (halfword.substr(14, 2) != "11")
            {
                word = decompressor(halfword);
                instructions[pc].isCompressed = true;
            }
            else
            {
                word = memory[pc + 3] + memory[pc + 2] + memory[pc + 1] + memory[pc];
                instructions[pc].isCompressed = false;
            }

            instructions[pc].opcode = word.substr(25, 7);
            instructions[pc].type = getType(instructions[pc].opcode);

            if (!instructions[pc].isTranslated)
            {
                translate(instructions[pc], word);
                displayInst(instructions[pc]);
            }
            execute(instructions[pc], registers, memory, pc);
        }
    }
    catch (string e)
    {
        cerr << e << endl;
    }
}

char getType(string opcode)
{
    if (opcode == "0110011")
    {
        return 'r';
    }
    else if (opcode == "0010011" || opcode == "0000011" || opcode == "1100111" || opcode == "1110011")
    {
        return 'i';
    }
    else if (opcode == "0100011")
    {
        return 's';
    }
    else if (opcode == "1100011")
    {
        return 'b';
    }
    else if (opcode == "0110111" || opcode == "0010111")
    {
        return 'u';
    }
    else if (opcode == "1101111")
    {
        return 'j';
    }
}

void initialize(string registers[], Instruction instructions[])
{
    for (int i = 0; i < 32; i++)
    {
        registers[i] = "";
        registers[i].append(32, '0');
    }

    for (int i = 0; i < 64000; i++)
    {
        instructions[i].isTranslated = false;
    }
}