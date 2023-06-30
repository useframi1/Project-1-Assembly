#include <iostream>
#include <vector>
#include <string>
using namespace std;

//Assume that wordTye is lowercase char 
void displayInst(char wordType, vector<int> partsOfTheWord);
void displayR(vector<int> partsOfTheWord, string ABIname[]);
void displayI(char wordType, vector<int> partsOfTheWord, string ABIname[]);
void displayS(vector<int> partsOfTheWord, string ABIname[]);
void displayB(vector<int> partsOfTheWord, string ABIname[]);
void displayU(vector<int> partsOfTheWord, string ABIname[]);
void displayJ(vector<int> partsOfTheWord, string ABIname[]);

int main()
{
    vector<int> InstR = {8,0,8,9,0}; //rd, funct3, rs1, rs2, funct7
    vector<int> InstI = {6,5,8,1038}; //rd, funct3, rs1, imm
    vector<int> Instl = {6,5,8,8}; //rd, funct3, rs1, imm
    vector<int> Inste = {6,0,8,0}; //rd, funct3, rs1, imm
    vector<int> InstS = {2,30,28,16}; //funct3, rs1, rs2, imm
    vector<int> InstB = {1,0,0,500}; //funct3, rs1, rs2, imm
    vector<int> InstU = {55,5,554580}; //opcode, rd, imm
    vector<int> InstJ = {1,1}; //rd, imm
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
    string ABIname[32] = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", 
                            "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", 
                            "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", 
                            "t5", "t6"};
    switch(wordType){
        case 'r':
            displayR(partsOfTheWord, ABIname);
            break;
        case 'i':
        case 'l':
        case 'e':
        case 'q':
            displayI(wordType, partsOfTheWord, ABIname);
            break;
        case 's':
            displayS(partsOfTheWord, ABIname);
            break;
        case 'b':
            displayB(partsOfTheWord, ABIname);
            break;
        case 'u':
            displayU(partsOfTheWord, ABIname);
            break;
        case 'j':
            displayJ(partsOfTheWord, ABIname);
            break;
        default:
            cout << "ERROR: Type unknown";
            return;
    }
}

void displayR(vector<int> partsOfTheWord, string ABIname[])
{   //rd, funct3, rs1, rs2, funct7
string print = " " + ABIname[partsOfTheWord[0]] + ", " + ABIname[partsOfTheWord[2]] + ", " + ABIname[partsOfTheWord[3]];
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

void displayI(char wordType, vector<int> partsOfTheWord, string ABIname[])
{   //rd, funct3, rs1, imm
    string print;
    switch(wordType)
    {
        case 'i':
        case 'q':
            print = " " + ABIname[partsOfTheWord[0]] + ", " + ABIname[partsOfTheWord[2]] + ", " + to_string(partsOfTheWord[3]);
            switch(partsOfTheWord[1])
            {
                case 0:
                    if(wordType == 'i')
                        print = "addi" + print;
                    else
                        print = "jalr" + print;
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
                        print = "srai " + ABIname[partsOfTheWord[0]] + ", " + ABIname[partsOfTheWord[2]] + ", " + to_string(partsOfTheWord[3]-1024);
                    else
                        print = "srli" + print;
                    break;
                case 2:
                    print = "slti" + print;
                    break;
                case 3:
                    print = "sltui" + print;
                    break;
                default:
                    cout << "ERROR: Type unknown";
                    return;
            }
            break;
        
        case 'l':
            print = " " + ABIname[partsOfTheWord[0]] + ", (" + to_string(partsOfTheWord[3]) + ")" + ABIname[partsOfTheWord[2]];
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
                default:
                    cout << "ERROR: Type unknown";
                    return;
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
                default:
                    cout << "ERROR: Type unknown";
                    return;
            }
            break;
        default:
            cout << "ERROR: Type unknown";
            return;
    }

    cout << print << endl;
}

void displayS(vector<int> partsOfTheWord, string ABIname[])
{ //funct3, rs1, rs2, imm
    string print = " " + ABIname[partsOfTheWord[2]] + ", (" + to_string(partsOfTheWord[3]) + ")" + ABIname[partsOfTheWord[1]];
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
        default:
            cout << "ERROR: Type unknown";
            return;
    }

    cout << print << endl;
}

void displayB(vector<int> partsOfTheWord, string ABIname[])
{    //funct3, rs1, rs2, imm
    string print = " " + ABIname[partsOfTheWord[1]] + ", " + ABIname[partsOfTheWord[2]] + ", " + to_string(partsOfTheWord[3]);
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
        default:
            cout << "ERROR: Type unknown";
            return;
    }
    cout << print << endl;
}

void displayU(vector<int> partsOfTheWord, string ABIname[])
{    //opcode, rd, imm
    string print = " " + ABIname[partsOfTheWord[1]] + ", " + to_string(partsOfTheWord[2]);
    switch(partsOfTheWord[0])
    {
        case 55:
            print = "lui" + print;
            break;
        case 23:
            print = "auipc" + print;
            break;
        default:
            cout << "ERROR: Type unknown";
            return;
    }
    cout << print << endl;
}

void displayJ(vector<int> partsOfTheWord, string ABIname[])
{   //rd, imm
    string print = "jal " + ABIname[partsOfTheWord[0]] + ", " + to_string(partsOfTheWord[1]);
    cout << print << endl;
}