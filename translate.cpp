//Translate function
#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

struct Instruction {
	int func7;
	int rd;
	int rs1;
	int rs2;
	int func3;
	int immediate;
	int type;
	bool isCompressed;
	string opcode;
};

string getTwosComplement(string  binary_number) 
{
	bool foundOne = false;
	string temp = binary_number;
	for (int i = binary_number.length() - 1; i >= 0; i--) 
	{
		if (foundOne) 
		{
			if (binary_number[i] == '0')
				temp[i] = '1';
			else
				temp[i] = '0';
		}
		if (binary_number[i] == '1' && !foundOne) 
			foundOne = true;
	}
	return temp;
}

int binaryToDec(string binary_number,bool isUnsigned)
{
	int decimal_number = 0;
	bool isNeg = false;
	if (binary_number[0] == '1' && !isUnsigned) 
	{
		isNeg = true;
		binary_number = getTwosComplement(binary_number);
	}
	for (int i = 0; i < binary_number.length(); i++) 
	{
		decimal_number += (binary_number[i] - '0') * pow(2, binary_number.length() - i - 1);
	}
	if(isNeg)
		return -1*decimal_number;
	else
		return decimal_number;
}

void translateR(string stringInst, Instruction & inst)
{
	string subpart;

	subpart = stringInst.substr(20, 5); //rd
	inst.rd = binaryToDec(subpart,1);

	subpart = stringInst.substr(17, 3); //func3
	inst.func3 = binaryToDec(subpart,1);

	subpart = stringInst.substr(12, 5); //rs1
	inst.rs1 = binaryToDec(subpart,1);

	subpart = stringInst.substr(7, 5); //rs2
	inst.rs2 = binaryToDec(subpart,1);

	subpart = stringInst.substr(0, 7); //func7
	inst.func7 = binaryToDec(subpart,1);
}

void translateI(string stringInst, Instruction& inst) 
{
	string subpart;

	subpart = stringInst.substr(20, 5); //rd
	inst.rd= binaryToDec(subpart,1);

	subpart = stringInst.substr(12, 5); //rs1
	inst.rs1 = binaryToDec(subpart,1);

	subpart = stringInst.substr(17, 3); //func3
	inst.func3 = binaryToDec(subpart, 1);

	//now we handle the three shifting operations

	if (inst.func3 == 1 || inst.func3 == 5) 
	{  //checks if shifting
		subpart = stringInst.substr(0, 7); //func7
		inst.func7 = binaryToDec(subpart, 1);

		subpart = stringInst.substr(7,5); //imm unsigned
		inst.immediate = binaryToDec(subpart, 1);
	}
	else 
	{ //not shifting
		subpart = stringInst.substr(0, 12); //imm signed
		inst.immediate = binaryToDec(subpart, 0);
	}
}

void translateS(string stringInst, Instruction& inst) 
{ 
	string subpart;

	subpart = stringInst.substr(17, 3); //func3
	inst.func3 = binaryToDec(subpart,1);

	subpart = stringInst.substr(12, 5); //rs1
	inst.rs1 = binaryToDec(subpart,1);

	subpart = stringInst.substr(7, 5); //rs2
	inst.rs2 = binaryToDec(subpart,1);

	subpart = stringInst.substr(0, 7); //imm [11:5]
	subpart += stringInst.substr(20, 5); //imm += imm[4:0]
	inst.immediate = binaryToDec(subpart,0);
}

void translateB(string stringInst, Instruction& inst) 
{ 
	string subpart;

	subpart = stringInst.substr(17, 3); //func3
	inst.func3 = binaryToDec(subpart,1);

	subpart = stringInst.substr(12, 5); //rs1
	inst.rs1 = binaryToDec(subpart,1);

	subpart = stringInst.substr(7, 5); //rs2
	inst.rs2 = binaryToDec(subpart,1);

	subpart = stringInst[0];//imm[12]
	subpart += stringInst[24]+stringInst.substr(1, 6)+ stringInst.substr(20, 4); //imm+= 7agat kiteer ba2a (NOT MULTIPLIED BY 2)
	inst.immediate = binaryToDec(subpart,0)*2;
}

void translateJ(string stringInst, Instruction& inst) 
{ 
	string subpart;

	subpart = stringInst.substr(20, 5); //rd
	inst.rd = binaryToDec(subpart, 1);

	subpart = stringInst[0];//imm[20]
	subpart += stringInst.substr(12, 8) + stringInst[11]+ stringInst.substr(1, 10); //imm+= 7agat kiteer ba2a (NOT MULTIPLIED BY 2)
	inst.immediate = binaryToDec(subpart, 0)*2;
}

void translateU(string stringInst, Instruction& inst) 
{ 
	string subpart;

	subpart = stringInst.substr(20, 5); //rd
	inst.rd = binaryToDec(subpart, 1);

	subpart = stringInst.substr(0, 20); //imm
	inst.immediate = binaryToDec(subpart, 0);
}

void Translate(string stringInst, Instruction & inst) 
{
		switch (inst.type) 
		{
		case 'r':
			translateR(stringInst, inst);
			break;
		case 'i':
			translateI(stringInst, inst);
			break;
		case 's':
			translateS(stringInst, inst);
			break;
		case 'b':
			translateB(stringInst, inst);
			break;
		case 'j':
			translateJ(stringInst, inst);
			break;
		case 'u':
			translateU(stringInst, inst);
			break;
		}
}

int main()
{
	Instruction tester;

	//testing r type

	/*
	tester.type = 'r';
	string s = "00000000101010011000100100110011";
	Translate(s,tester);
	cout << tester.rd << " " << tester.func3 << " " << tester.rs1 << " " << tester.rs2 << " "
		<< tester.func7 << endl;
	*/

	//testing i type

	/*
	tester.type = 'i';
	cout << "test1: " << endl;
	string s = "11111100111000001000011110010011";
	Translate(s, tester);
	cout << tester.rd << " " << tester.func3 << " " << tester.rs1 << " " << tester.immediate <<  endl;
	cout << "test2: load" << endl;
	string s2 = "00000000100000010010011100000011";
	Translate(s2, tester);
	cout << tester.rd << " " << tester.func3 << " " << tester.rs1 << " " << tester.immediate << endl;
	cout << "test3: shift op shamt=8" << endl;
	string s3 = "00000000100000010001011100010011";
	Translate(s3, tester);
	cout << tester.rd << " " << tester.func3 << " " << tester.rs1 << " " << tester.func7 <<" "
		<<tester.immediate<<endl;
	cout << "test4: shift op shamt=8" << endl;
	string s4 = "01000001100001100101011000010011";
	Translate(s4, tester);
	cout << tester.rd << " " << tester.func3 << " " << tester.rs1 << " " << tester.func7 << " "
		<< tester.immediate << endl;
		*/

	//testing s type

	/*
	tester.type = 's';
	string s = "00000000111000010010010000100011";
	Translate(s, tester);
	cout << tester.func3 << " " << tester.rs1 << " " << tester.rs2 << " " 
		<< tester.immediate <<  endl;
	*/

	//testing b type

	/*
	tester.type = 'b';
	cout << "test1: " << endl;
	string s = "00000000101010011000100001100011";
	Translate(s, tester);
	cout << tester.func3 << " " << tester.rs1 << " " << tester.rs2 << " "
		<< tester.immediate <<  endl;
	cout << "test2: " << endl;
	string s2 = "11111110101010011000111111100011";
	Translate(s2, tester);
	cout << tester.func3 << " " << tester.rs1 << " " << tester.rs2 << " "
		<< tester.immediate << endl;
	*/

	//testing j type

	/*
	tester.type = 'j';
	cout << "test1: " << endl;
	string s = "00000001111000000000001001101111"; //rd=4 and imm= 15
	Translate(s, tester);
	cout << tester.rd <<" "<< tester.immediate << endl;

	cout << "test2: " << endl;
	string s2 = "11111111111111111111100001101111"; //rd=16 imm=-1
	Translate(s2, tester);
	cout << tester.rd<<" "<< tester.immediate << endl;
	*/

	//testing u type

	/*
	tester.type = 'u';
	cout << "test1: " << endl;
	string s = "00000000010000000001001001101111"; //rd=4 and imm= 1025
	Translate(s, tester);
	cout << tester.rd <<" "<< tester.immediate << endl;

	cout << "test2: " << endl;
	string s2 = "11111111111111111111100001101111"; //rd=16 imm=-1
	Translate(s2, tester);
	cout << tester.rd<<" "<< tester.immediate << endl;
	*/

	
	
}