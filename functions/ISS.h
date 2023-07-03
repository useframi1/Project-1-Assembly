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
        // for (int i = 65528; i < 66000; i++)
        // {
        //     cout << memory[i] << endl;
        // }

        runDisplayAll(memory, instructions, registers);
        // runDisplayAtExecution(memory, instructions, registers);
    }
    catch (string e)
    {
        cerr << e << endl;
    }
    // for (int i = 0; i < 32; i++)
    // {
    //     cout << i << ": " << registers[i] << endl;
    // }
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

void runDisplayAll(string memory[], Instruction instructions[], string registers[])
{
    cout << "Assembly code" << endl;
    int i = 0;
    int pc = 0;
    while (i < 65536)
    {
        if ((memory[i + 1] + memory[i]) != "0000000000000000")
        {
            string word = checkForDecompression(memory, instructions[i], i);
            instructions[i].opcode = word.substr(25, 7);
            instructions[i].type = getType(instructions[i].opcode);
            translate(instructions[i], word);
            cout << i << ":\t";
            displayInst(instructions[i]);
        }
        i += instructions[i].isCompressed ? 2 : 4;
    }

    cout << "Execution" << endl;
    while (pc < 65536)
    {
        if ((memory[pc + 1] + memory[pc]) == "0000000000000000")
        {
            string error = "Program exited with no termination";
            throw error;
        }
        execute(instructions[pc], registers, memory, pc);
    }
}

void runDisplayAtExecution(string memory[], Instruction instructions[], string registers[])
{
    int pc = 0;
    while (pc < 65536)
    {
        if ((memory[pc + 1] + memory[pc]) == "0000000000000000")
        {
            string error = "Program exited with no termination";
            throw error;
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

#endif // __ISS_H__