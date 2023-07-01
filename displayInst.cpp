#include <iostream>
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
    string opcode;
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
    Instruction InstR = {0,8,8,9,0,0,'r',"0110011"};
    Instruction InstI0 = {0,6,8,0,5,1038,'i',"0010011"};
    Instruction InstI1 = {0,6,8,0,5,8,'i',"0000011"};
    Instruction InstI2 = {0,6,8,0,0,0,'i',"1110011"};
    Instruction InstS = {0,0,30,28,2,16,'s',"0100011"};
    Instruction InstB = {0,0,0,0,1,500,'b',"1100011"};
    Instruction InstU = {0,5,0,0,0,554580,'u',"0110111"};
    Instruction InstJ = {0,1,0,0,0,1,'j',"1101111"};
    displayInst(InstR);
    displayInst(InstI0);
    displayInst(InstI1);
    displayInst(InstI2);
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
    if (Inst.opcode == "0010011" || Inst.opcode == "1100111")
    {
        print = " " + ABIname[Inst.rd] + ", " + ABIname[Inst.rs1] + ", " + to_string(Inst.immediate);
        switch(Inst.func3)
        {
            case 0:
                if(Inst.opcode == "0010011")
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
    }
    else if (Inst.opcode == "0000011")
    {
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
    }
    else if (Inst.opcode == "1110011")
        print = "ecall";
    else
    {
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

    if(Inst.opcode == "0110111")
        print = "lui" + print;
    else if (Inst.opcode == "0010111")
        print = "auipc" + print;
    else
    {
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