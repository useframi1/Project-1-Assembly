#include <iostream>
#include <vector>
#include <string>
using namespace std;

//Assume that wordTye is lowercase char 
void displayInst(char wordType, vector<int> partsOfTheWord);
void displayR(vector<int> partsOfTheWord);
void displayI(char wordType, vector<int> partsOfTheWord);
void displayS(vector<int> partsOfTheWord);
void displayB(vector<int> partsOfTheWord);
void displayU(vector<int> partsOfTheWord);
void displayJ(vector<int> partsOfTheWord);

int main()
{
    vector<int> InstR = {8,0,8,9,0}; //rd, funct3, rs1, rs2, funct7
    vector<int> InstI = {6,5,8,1038}; //rd, funct3, rs1, imm[11:0]
    vector<int> Instl = {6,5,8,8}; //rd, funct3, rs1, imm[11:0]
    vector<int> Inste = {6,0,8,0}; //rd, funct3, rs1, imm[11:0]
    vector<int> InstS = {2,30,28,16}; //funct3, rs1, rs2, imm[11:0]
    vector<int> InstB = {1,0,0,500}; //funct3, rs1, rs2, imm[12:1]
    vector<int> InstU = {55,5,554580}; //opcode, rd, imm[31:12]
    vector<int> InstJ = {1,1}; //rd, imm[20:0]
    displayInst('r',InstR);
    displayInst('i',InstI);
    displayInst('l',Instl);
    displayInst('e',Inste);
    displayInst('s',InstS);
    displayInst('b',InstB);
    displayInst('u',InstU);
    displayInst('j',InstJ);
    return 0;
}

void displayInst(char wordType, vector<int> partsOfTheWord)
{
    switch(wordType){
        case 'r':
            displayR(partsOfTheWord);
            break;
        case 'i':
        case 'l':
        case 'e':
            displayI(wordType, partsOfTheWord);
            break;
        case 's':
            displayS(partsOfTheWord);
            break;
        case 'b':
            displayB(partsOfTheWord);
            break;
        case 'u':
            displayU(partsOfTheWord);
            break;
        case 'j':
            displayJ(partsOfTheWord);
            break;
    }
}

void displayR(vector<int> partsOfTheWord)
{   //rd, funct3, rs1, rs2, funct7
string print = " x" + to_string(partsOfTheWord[0]) + ", x" + to_string(partsOfTheWord[2]) + ", x" + to_string(partsOfTheWord[3]);
    switch(partsOfTheWord[1])
    {
        case 0:
            if(partsOfTheWord[4] == 0)
                print = "add" + print;
            else
                print = "sub" + print;
            break;
        
        case 4:
            print = "xor" + print;
            break;
        case 6:
            print = "or" + print;
            break;
        case 7:
            print = "and" + print;
            break;
        case 1:
            print = "sll" + print;
            break;
        case 5:
            if(partsOfTheWord[4] == 0)
                print = "srl" + print;
            else
                print = "sra" + print;
            break;
        case 2:
            print = "slt" + print;
            break;
        case 3:
            print = "sltu" + print;
            break;
        default:
            cout << "ERROR: Type unknown";
            return;
    }
    cout << print << endl;
}

void displayI(char wordType, vector<int> partsOfTheWord)
{   //rd, funct3, rs1, imm[11:0]
    string print;
    switch(wordType)
    {
        case 'i':
            print = " x" + to_string(partsOfTheWord[0]) + ", x" + to_string(partsOfTheWord[2]) + ", " + to_string(partsOfTheWord[3]);
            switch(partsOfTheWord[1])
            {
                case 0:
                    print = "addi" + print;
                    break;
                case 4:
                    print = "xori" + print;
                    break;
                case 6:
                    print = "ori" + print;
                    break;
                case 7:
                    print = "andi" + print;
                    break;
                case 1:
                    print = "slli" + print;
                    break;
                case 5:
                    if(partsOfTheWord[3] > 32)
                        print = "srai x" + to_string(partsOfTheWord[0]) + ", x" + to_string(partsOfTheWord[2]) + ", " + to_string(partsOfTheWord[3]-1024);
                    else
                        print = "srli" + print;
                    break;
                case 2:
                    print = "slti" + print;
                    break;
                case 3:
                    print = "sltui" + print;
                    break;
            }
            break;
        
        case 'l':
            print = " x" + to_string(partsOfTheWord[0]) + ", (" + to_string(partsOfTheWord[3]) + ")x" + to_string(partsOfTheWord[2]);
            switch(partsOfTheWord[1])
            {
                case 0:
                    print = "lb" + print;
                    break;
                case 1:
                    print = "lh" + print;
                    break;
                case 2:
                    print = "lw" + print;
                    break;
                case 4:
                    print = "lbu" + print;
                    break;
                case 5:
                    print = "lhu" + print;
                    break;
            }
            break;

        case 'e':
            switch(partsOfTheWord[3])
            {
                case 0:
                    print = "ecall";
                    break;
                case 1:
                    print = "ebreak";
                    break;
            }
            break;
    }

    cout << print << endl;
}

void displayS(vector<int> partsOfTheWord)
{ //funct3, rs1, rs2, imm[11:0]
    string print = " x" + to_string(partsOfTheWord[2]) + ", (" + to_string(partsOfTheWord[3]) + ")x" + to_string(partsOfTheWord[1]);
    switch(partsOfTheWord[0])
    {
        case 0:
            print = "sb" + print;
            break;
        case 1:
            print = "sh" + print;
            break;
        case 2:
            print = "sw" + print;
            break;
    }

    cout << print << endl;
}

void displayB(vector<int> partsOfTheWord)
{    //funct3, rs1, rs2, imm[12:1]
    string print = " x" + to_string(partsOfTheWord[1]) + ", x" + to_string(partsOfTheWord[2]) + ", " + to_string(partsOfTheWord[3]*2);
    switch(partsOfTheWord[0])
    {
        case 0:
            print = "beq" + print;
            break;
        case 1:
            print = "bne" + print;
            break;
        case 4:
            print = "blt" + print;
            break;
        case 5:
            print = "bge" + print;
            break;
        case 6:
            print = "bltu" + print;
            break;
        case 7:
            print = "bgeu" + print;
            break;
    }
    cout << print << endl;
}

void displayU(vector<int> partsOfTheWord)
{    //opcode, rd, imm[31:12]
    string print = " x" + to_string(partsOfTheWord[1]) + ", " + to_string(partsOfTheWord[2]);
    switch(partsOfTheWord[0])
    {
        case 55:
            print = "lui" + print;
            break;
        case 23:
            print = "auipc" + print;
            break;
    }
    cout << print << endl;
}

void displayJ(vector<int> partsOfTheWord)
{   //rd, imm[20:0]
    string print = "jal x" + to_string(partsOfTheWord[0]) + ", " + to_string(partsOfTheWord[1]);
    cout << print << endl;
}