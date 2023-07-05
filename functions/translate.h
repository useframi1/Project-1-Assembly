#ifndef __TRANSLATE_H__
#define __TRANSLATE_H__

// Translate function
#include <iostream>
#include <cmath>
#include <vector>
#include "instructionStruct.h"
#include "utilities.h"
using namespace std;

void translateR(string stringInst, Instruction &inst)
{
	string subpart;

	subpart = stringInst.substr(20, 5); // rd
	inst.rd = binaryToDec(subpart, 1);

	subpart = stringInst.substr(17, 3); // func3
	inst.func3 = binaryToDec(subpart, 1);

	subpart = stringInst.substr(12, 5); // rs1
	inst.rs1 = binaryToDec(subpart, 1);

	subpart = stringInst.substr(7, 5); // rs2
	inst.rs2 = binaryToDec(subpart, 1);

	subpart = stringInst.substr(0, 7); // func7
	inst.func7 = binaryToDec(subpart, 1);
}

void translateI(string stringInst, Instruction &inst)
{
	string subpart;

	subpart = stringInst.substr(20, 5); // rd
	inst.rd = binaryToDec(subpart, 1);

	subpart = stringInst.substr(12, 5); // rs1
	inst.rs1 = binaryToDec(subpart, 1);

	subpart = stringInst.substr(17, 3); // func3
	inst.func3 = binaryToDec(subpart, 1);

	// now we handle the three shifting operations

	if (inst.func3 == 1 || inst.func3 == 5)
	{									   // checks if shifting
		subpart = stringInst.substr(0, 7); // func7
		inst.func7 = binaryToDec(subpart, 1);

		subpart = stringInst.substr(7, 5); // imm unsigned
		inst.immediate = binaryToDec(subpart, 1);
	}
	else
	{										// not shifting
		subpart = stringInst.substr(0, 12); // imm signed
		inst.immediate = binaryToDec(subpart, 0);
	}
}

void translateS(string stringInst, Instruction &inst)
{
	string subpart;

	subpart = stringInst.substr(17, 3); // func3
	inst.func3 = binaryToDec(subpart, 1);

	subpart = stringInst.substr(12, 5); // rs1
	inst.rs1 = binaryToDec(subpart, 1);

	subpart = stringInst.substr(7, 5); // rs2
	inst.rs2 = binaryToDec(subpart, 1);

	subpart = stringInst.substr(0, 7);	 // imm [11:5]
	subpart += stringInst.substr(20, 5); // imm += imm[4:0]
	inst.immediate = binaryToDec(subpart, 0);
}

void translateB(string stringInst, Instruction &inst)
{
	string subpart;

	subpart = stringInst.substr(17, 3); // func3
	inst.func3 = binaryToDec(subpart, 1);

	subpart = stringInst.substr(12, 5); // rs1
	inst.rs1 = binaryToDec(subpart, 1);

	subpart = stringInst.substr(7, 5); // rs2
	inst.rs2 = binaryToDec(subpart, 1);

	subpart = stringInst[0];														// imm[12]
	subpart += stringInst[24] + stringInst.substr(1, 6) + stringInst.substr(20, 4); // imm+= 7agat kiteer ba2a (NOT MULTIPLIED BY 2)
	inst.immediate = binaryToDec(subpart, 0) * 2;
}

void translateJ(string stringInst, Instruction &inst)
{
	string subpart;

	subpart = stringInst.substr(20, 5); // rd
	inst.rd = binaryToDec(subpart, 1);

	subpart = stringInst[0];														 // imm[20]
	subpart += stringInst.substr(12, 8) + stringInst[11] + stringInst.substr(1, 10); // imm+= 7agat kiteer ba2a (NOT MULTIPLIED BY 2)
	inst.immediate = binaryToDec(subpart, 0) * 2;
}

void translateU(string stringInst, Instruction &inst)
{
	string subpart;

	subpart = stringInst.substr(20, 5); // rd
	inst.rd = binaryToDec(subpart, 1);

	subpart = stringInst.substr(0, 20); // imm
	inst.immediate = binaryToDec(subpart, 0);
}

void translate(Instruction &inst, string stringInst)
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

#endif // __TRANSLATE_H__