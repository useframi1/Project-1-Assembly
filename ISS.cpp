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
    string decompressedWord;
};

void initialize(string registers[], Instruction instructions[]);
void loadMemory(string machineCode, string memory[]);
string decompressor(string halfword);
string checkForDecompression(string memory[], Instruction &inst, int pc);
char getType(string opcode);
void translate(Instruction &inst, string word);
void displayInst(Instruction inst);
void execute(Instruction inst, string registers[], string memory[], int &pc);

void ISS(string machineCode)
{
    string memory[80000];
    Instruction instructions[64000];
    string registers[32];
    int pc = 0;

    initialize(registers, instructions);

    try
    {
        loadMemory(machineCode, memory);
        for (int i = 0; i < 64000; i++)
        {
            string word = checkForDecompression(memory, instructions[i], pc);
            instructions[i].opcode = word.substr(25, 7);
            instructions[i].type = getType(instructions[i].opcode);
            translate(instructions[i], word);
            displayInst(instructions[i]);
        }

        while (pc < 64000)
        {
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
        instructions[i].isCompressed = false;
    }
}

string checkForDecompression(string memory[], Instruction &inst, int pc)
{
    string halfword = memory[pc + 1] + memory[pc];
    string word;
    if (halfword.substr(14, 2) == "11" && !inst.isCompressed)
    {
        word = decompressor(halfword);
        inst.decompressedWord = word;
        inst.isCompressed = true;
    }
    else
    {
        word = inst.isCompressed ? inst.decompressedWord : memory[pc + 3] + memory[pc + 2] + memory[pc + 1] + memory[pc];
    }

    return word;
}