#include <iostream>
#include <vector>
#include <string>
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
};

void displayInst(Instruction);
void displayR(Instruction, string ABIname[]);
void displayI(Instruction, string ABIname[]);
void displayS(Instruction, string ABIname[]);
void displayB(Instruction, string ABIname[]);
void displayU(Instruction, string ABIname[]);
void displayJ(Instruction, string ABIname[]);

int main()
{
    Instruction InstR = {0,8,8,9,0,0,'r'};
    Instruction InstI = {0,6,8,0,5,1038,'i'};
    Instruction Instl = {0,6,8,0,5,8,'l'};
    Instruction Inste = {0,6,8,0,0,0,'e'};
    Instruction InstS = {0,0,30,28,2,16,'s'};
    Instruction InstB = {0,0,0,0,1,500,'b'};
    Instruction InstU = {0,5,0,0,0,554580,'u'};
    Instruction InstJ = {0,1,0,0,0,1,'j'};
    displayInst(InstR);
    displayInst(InstI);
    displayInst(Instl);
    displayInst(Inste);
    displayInst(InstS);
    displayInst(InstB);
    displayInst(InstU);
    displayInst(InstJ);
    return 0;
}

void displayInst(Instruction Inst)
{
    string ABIname[32] = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", 
                            "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", 
                            "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", 
                            "t5", "t6"};
    switch(Inst.type){
        case 'r':
            displayR(Inst, ABIname);
            break;
        case 'i':
        case 'l':
        case 'e':
        case 'q':
            displayI(Inst, ABIname);
            break;
        case 's':
            displayS(Inst, ABIname);
            break;
        case 'b':
            displayB(Inst, ABIname);
            break;
        case 'u':
            displayU(Inst, ABIname);
            break;
        case 'j':
            displayJ(Inst, ABIname);
            break;
        default:
            cout << "ERROR: Type unknown";
            return;
    }
}

void displayR(Instruction Inst, string ABIname[])
{   
string print = " " + ABIname[Inst.rd] + ", " + ABIname[Inst.rs1] + ", " + ABIname[Inst.rs2];
    switch(Inst.func3)
    {
        case 0:
            if(Inst.func7 == 0)
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
            if(Inst.func7 == 0)
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

void displayI(Instruction Inst, string ABIname[])
{
    string print;
    switch(Inst.type)
    {
        case 'i':
        case 'q':
            print = " " + ABIname[Inst.rd] + ", " + ABIname[Inst.rs1] + ", " + to_string(Inst.immediate);
            switch(Inst.func3)
            {
                case 0:
                    if(Inst.type == 'i')
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
                    if(Inst.immediate > 32)
                        print = "srai " + ABIname[Inst.rd] + ", " + ABIname[Inst.rs1] + ", " + to_string(Inst.immediate-1024);
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
            print = " " + ABIname[Inst.rd] + ", (" + to_string(Inst.immediate) + ")" + ABIname[Inst.rs1];
            switch(Inst.func3)
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
            print = "ecall";
            break;
        default:
            cout << "ERROR: Type unknown";
            return;
    }

    cout << print << endl;
}

void displayS(Instruction Inst, string ABIname[])
{ 
    string print = " " + ABIname[Inst.rs2] + ", (" + to_string(Inst.immediate) + ")" + ABIname[Inst.rs1];
    switch(Inst.func3)
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

void displayB(Instruction Inst, string ABIname[])
{  
    string print = " " + ABIname[Inst.rs1] + ", " + ABIname[Inst.rs2] + ", " + to_string(Inst.immediate);
    switch(Inst.func3)
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

void displayU(Instruction Inst, string ABIname[])
{    //opcode, rd, imm
    string print = " " + ABIname[Inst.rd] + ", " + to_string(Inst.immediate);
    switch(Inst.func7)
    {
        case 1:
            print = "lui" + print;
            break;
        case 0:
            print = "auipc" + print;
            break;
        default:
            cout << "ERROR: Type unknown";
            return;
    }
    cout << print << endl;
}

void displayJ(Instruction Inst, string ABIname[])
{   //rd, imm
    string print = "jal " + ABIname[Inst.rd] + ", " + to_string(Inst.immediate);
    cout << print << endl;
}