#ifndef __ISS_H__
#define __ISS_H__

#include "loadMemory.h"
#include "decompressor.h"
#include "translate.h"
#include "displayInst.h"
#include "execute.h"
#include "instructionStruct.h"

void initialize(string registers[], Instruction instructions[]);
string checkForDecompression(string memory[], Instruction &inst, int pc);
char getType(string opcode);
void runDisplayAll(string memory[], Instruction instructions[], string registers[]);
void runDisplayAtExecution(string memory[], Instruction instructions[], string registers[]);

void ISS(string machineCode)
{
    string memory[81920];
    Instruction instructions[65536];
    string registers[32];
    int pc = 0;

    initialize(registers, instructions);

    try
    {
        loadMemory(machineCode, memory);

        runDisplayAtExecution(memory, instructions, registers);
        // runDisplayAll(memory, instructions, registers);
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

    return ' ';
}

void initialize(string registers[], Instruction instructions[])
{
    for (int i = 0; i < 32; i++)
    {
        registers[i] = "";
        registers[i].append(32, '0');
    }
    registers[2] = "00000000000000010011111111111111";
    for (int i = 0; i < 65536; i++)
    {
        instructions[i].isTranslated = false;
        instructions[i].isCompressed = false;
    }
}

string checkForDecompression(string memory[], Instruction &inst, int pc)
{
    string halfword = memory[pc + 1] + memory[pc];
    string word = "";

    if (halfword.substr(14, 2) != "11" && !inst.isCompressed)
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

void runDisplayAtExecution(string memory[], Instruction instructions[], string registers[])
{
    int pc = 0;
    while (pc < 65535)
    {
        if ((memory[pc + 1] + memory[pc]) == "0000000000000000")
        {
            pc++;
            continue;
        }
        if (!instructions[pc].isTranslated)
        {
            string word = checkForDecompression(memory, instructions[pc], pc);
            instructions[pc].opcode = word.substr(25, 7);
            instructions[pc].type = getType(instructions[pc].opcode);
            translate(instructions[pc], word);
            cout << pc << ":\t";
            displayInst(instructions[pc]);
        }

        execute(instructions[pc], registers, memory, pc);
    }
}

void runDisplayAll(string memory[], Instruction instructions[], string registers[])
{
    int pc = 0;
    while (pc < 65535)
    {
        if ((memory[pc + 1] + memory[pc]) == "0000000000000000")
        {
            pc++;
            continue;
        }
        string word = checkForDecompression(memory, instructions[pc], pc);
        instructions[pc].opcode = word.substr(25, 7);
        instructions[pc].type = getType(instructions[pc].opcode);
        translate(instructions[pc], word);
        cout << pc << ":\t";
        displayInst(instructions[pc]);
        pc += instructions[pc].isCompressed ? 2 : 4;
    }
    pc = 0;
    while (pc < 65535)
    {
        execute(instructions[pc], registers, memory, pc);
    }
}

#endif // __ISS_H__