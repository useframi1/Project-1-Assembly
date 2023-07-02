#include <iostream>
using namespace std;

string adjust_reg(string comp_reg)
{
	string reg;
	if (comp_reg == "000")
		reg = "01000";
	else if (comp_reg == "001")
		reg = "01001";
	else if (comp_reg == "010")
		reg = "01010";
	else if (comp_reg == "011")
		reg = "01011";
	else if (comp_reg == "100")
		reg = "01100";
	else if (comp_reg == "101")
		reg = "01101";
	else if (comp_reg == "110")
		reg = "01110";
	else if (comp_reg == "111")
		reg = "01111";
	return reg;
}
string decompressor(string compInst)
{
	string decompInst;
	string func7_R1 = "0000000";
	string func7_R2 = "0100000";
	
	//  decompress compressed RISC321C-V instructions to 32 bits according to their type CR,CI,CSS,CIW,CL,CS,CB,or CJ
	// already know that the instruction coming to me is compressed so i don't need to check if it is compressed or not
	
	if (compInst.substr(14, 2) == "00")
	{
		if (compInst.substr(0, 3) == "110")
		{
			// c.sw -> sw rs2,offset(rs1)
			decompInst = "00000"+compInst.substr(10,1)+compInst.substr(3,1)+ adjust_reg(compInst.substr(11,3)) + adjust_reg(compInst.substr(6, 3)) + "010" + compInst.substr(4, 2)+ compInst.substr(9,1)+ "00" + "0100011";
		}
		else if (compInst.substr(0, 3) == "010")
		{
			// c.lw -> lw rd,offset(rs1)
			decompInst = "00000" + compInst.substr(10, 1) + compInst.substr(3, 3) + compInst.substr(9, 1)+ "00" + adjust_reg(compInst.substr(6, 3)) + "010" + adjust_reg(compInst.substr(11, 3))  + "0000011";
		}
		else if (compInst.substr(0, 3) == "000")
		{
			// c.addi4spn -> addi rd',x2,imm[9:2]
			//decompInst = "000000" + 
			decompInst = "00" + compInst.substr(5, 4) + compInst.substr(3, 2) + compInst.substr(10, 1) + compInst.substr(9, 1)+ "00" + "00010"+ "000"+ adjust_reg(compInst.substr(11,3)) + "0010011";
		}
	}

	if (compInst.substr(14, 2) == "01")
	{
		if (compInst.substr(0, 3) == "000")
		{
			if (compInst.substr(4, 5) == "00000")
			{
				//c.nop instruction, decompresses to addi x0,x0,0
				decompInst = "00000000000000000000000000010011";
			}
			else
			{
				//c.addi instruction, decompresses to addi rd,rd,imm
				string neg = "111111";
				string pos = "000000";
				if (compInst.substr(3, 1) == "0")
					decompInst = pos + compInst.substr(3, 1) + compInst.substr(9, 5) + compInst.substr(4, 5) + "000" + compInst.substr(4, 5) + "0010011";
				else if (compInst.substr(3, 1) == "1")
					decompInst = neg + compInst.substr(3, 1) + compInst.substr(9, 5) + compInst.substr(4, 5) + "000" + compInst.substr(4, 5) + "0010011";
			}
		}
		if (compInst.substr(0, 3) == "001")
		{
			//c.jal instruction, decompresses to jal x1,imm
			if (compInst.substr(3, 1) == "0")
				decompInst = "0" + compInst.substr(7, 1) + compInst.substr(5, 2) + compInst.substr(9, 1) + compInst.substr(8, 1) + compInst.substr(12, 1) + compInst.substr(4, 1) + compInst.substr(10, 3) + compInst.substr(3, 1) + "00000000" + "00001" + "1101111";
			else if (compInst.substr(3, 1) == "1")
				decompInst = "1" + compInst.substr(7, 1) + compInst.substr(5, 2) + compInst.substr(9, 1) + compInst.substr(8, 1) + compInst.substr(12, 1) + compInst.substr(4, 1) + compInst.substr(10, 3) + compInst.substr(3, 1) + "00000000" + "00001" + "1101111";
		}
		if (compInst.substr(0, 3) == "101")
		{
			//c.j instruction, decompresses to jal x0,imm
			
			if (compInst.substr(3, 1) == "0")
				decompInst = "0" + compInst.substr(7, 1) + compInst.substr(5, 2) + compInst.substr(9, 1) + compInst.substr(8, 1) + compInst.substr(12, 1) + compInst.substr(4, 1) + compInst.substr(10, 3) + compInst.substr(3, 1) + "00000000" + "00000" + "1101111";
			else if (compInst.substr(3, 1) == "1")
				decompInst = "1" + compInst.substr(7, 1) + compInst.substr(5, 2) + compInst.substr(9, 1) + compInst.substr(8, 1) + compInst.substr(12, 1) + compInst.substr(4, 1) + compInst.substr(10, 3) + compInst.substr(3, 1) + "00000000" + "00000" + "1101111";

		}
		// c.beqz ->110; c.bnez ->111
		if (compInst.substr(0, 3) == "110")
		{
			//c.beqz instruction, decompresses to beq rs1,x0,imm
			if (compInst.substr(3, 1) == "0")

				decompInst = "000" + compInst.substr(3, 1) + compInst.substr(9, 2) + compInst.substr(13, 1) + "00000" + adjust_reg(compInst.substr(6, 3)) + "000" + compInst.substr(4, 2) + compInst.substr(11, 2) + "0" + "1100011";
			else if (compInst.substr(3, 1) == "1")
				decompInst = "111" + compInst.substr(3, 1) + compInst.substr(9, 2) + compInst.substr(13, 1) + "00000" + adjust_reg(compInst.substr(6, 3)) + "000" + compInst.substr(4, 2) + compInst.substr(11, 2) + "1" + "1100011";
		}
		if (compInst.substr(0, 3) == "111")
		{
			//c.bnez instruction, decompresses to beq rs1,x0,imm
			if (compInst.substr(3, 1) == "0")
				decompInst = "000" + compInst.substr(3, 1) + compInst.substr(9, 2) + compInst.substr(13, 1) + "00000" + adjust_reg(compInst.substr(6, 3)) + "001" + compInst.substr(4, 2) + compInst.substr(11, 2) + "0" + "1100011";
			else if (compInst.substr(3, 1) == "1")
				decompInst = "111" + compInst.substr(3, 1) + compInst.substr(9, 2) + compInst.substr(13, 1) + "00000" + adjust_reg(compInst.substr(6, 3)) + "001" + compInst.substr(4, 2) + compInst.substr(11, 2) + "1" + "1100011";
		}
		if (compInst.substr(0, 3) == "100")
		{
			if (compInst.substr(9, 2) == "00" && compInst.substr(4, 2) == "11")
			{
				// c.sub -> sub rsd,rsd,rs2
				decompInst = func7_R2 + adjust_reg(compInst.substr(11, 3)) + adjust_reg(compInst.substr(6, 3)) + "000" + adjust_reg(compInst.substr(6, 3)) + "0110011";

			}
			else if (compInst.substr(9, 2) == "01" && compInst.substr(4, 2) == "11")
			{
				// c.xor -> xor rsd,rsd,rs2
				decompInst = func7_R1 + adjust_reg(compInst.substr(11, 3)) + adjust_reg(compInst.substr(6, 3)) + "100" + adjust_reg(compInst.substr(6, 3)) + "0110011";

			}
			else if (compInst.substr(9, 2) == "10" && compInst.substr(4, 2) == "11")
			{
				// c.or -> or rsd,rsd,rs2
				decompInst = func7_R1 + adjust_reg(compInst.substr(11, 3)) + adjust_reg(compInst.substr(6, 3)) + "110" + adjust_reg(compInst.substr(6, 3)) + "0110011";

			}
			else if (compInst.substr(9, 2) == "11" && compInst.substr(4, 2) == "11")
			{
				// c.and -> and rsd,rsd,rs2
				decompInst = func7_R1 + adjust_reg(compInst.substr(11, 3)) + adjust_reg(compInst.substr(6, 3)) + "111" + adjust_reg(compInst.substr(6, 3)) + "0110011";

			} // sub,cor,or,and
			if (compInst.substr(4, 2) == "00")
			{
				// c.srli -> srli rsd,rsd,shamt
				decompInst = "000000" + compInst.substr(3, 1) + compInst.substr(9, 5) + adjust_reg(compInst.substr(6, 3)) + "101" + adjust_reg(compInst.substr(6, 3)) + "0010011";

			}
			else if (compInst.substr(4, 2) == "01")
			{
				// c.srai -> srai rsd,rsd,shamt
				decompInst = "010000" + compInst.substr(3, 1) + compInst.substr(9, 5) + adjust_reg(compInst.substr(6, 3)) + "101" + adjust_reg(compInst.substr(6, 3)) + "0010011";

			}
			else if (compInst.substr(4, 2) == "10")
			{
				// c.andi -> andi rsd,rsd,imm
				decompInst = "000000" + compInst.substr(3, 1) + compInst.substr(9, 5) + adjust_reg(compInst.substr(6, 3)) + "111" + adjust_reg(compInst.substr(6, 3)) + "0010011";

			}
		}
		if (compInst.substr(0, 3) == "010")
		{
			// c.li -> addi rd,0,imm
			if (compInst.substr(3, 1) == "0")
				decompInst = "000000" + compInst.substr(3, 1) + compInst.substr(9, 5) + "00000" + "000" + compInst.substr(4, 5) + "0010011";
			else if (compInst.substr(3, 1) == "1")
				decompInst = "111111" + compInst.substr(3, 1) + compInst.substr(9, 5) + "00000" + "000" + compInst.substr(4, 5) + "0010011";

		}
		if (compInst.substr(0, 3) == "011")
		{
			if ((compInst.substr(4, 5) != "00000" && compInst.substr(4, 5) != "00010"))
			{
				string pos = "00000000000000";
				string neg = "11111111111111";
				// c.lui -> lui rd,imm
				if (compInst.substr(3, 1) == "0")
					decompInst = pos + compInst.substr(3, 1) + compInst.substr(9, 5)  + compInst.substr(4, 5) + "0110111";
				else if (compInst.substr(3, 1) == "1")
					decompInst = neg + compInst.substr(3, 1) + compInst.substr(9, 5) + compInst.substr(4, 5) + "0110111";

			}
			else if (compInst.substr(4, 5) == "00010")
			{
				// c.addi16sp -> addi rd,sp,imm
				if (compInst.substr(3, 1) == "0")
					decompInst = "00" + compInst.substr(3, 1) + compInst.substr(11, 2) + compInst.substr(10, 1) + compInst.substr(13, 1) + compInst.substr(9, 1) + "0000" + "00010" + "000" + compInst.substr(4, 5) + "0010011";
				else if (compInst.substr(3, 1) == "1")
					decompInst = "11" + compInst.substr(3, 1) + compInst.substr(11, 2) + compInst.substr(10, 1) + compInst.substr(13, 1) + compInst.substr(9, 1) + "0000" + "00010" + "000" + compInst.substr(4, 5) + "0010011";
			}
		}

	}

	if (compInst.substr(14, 2) == "10")
	{
		if (compInst.substr(0, 4) == "1001")
		{
			if (compInst.substr(9, 5) != "00000" && compInst.substr(4, 5) != "00000")
			{
				//c.add instruction
				// if i have 1001_11001_00010_10 then it should decompress to 0000000_00010_11001_000_11001_0110011 // x25+x2=x25
				decompInst = func7_R1 + compInst.substr(9, 5) + compInst.substr(4, 5) + "000" + compInst.substr(4, 5) + "0110011";
			}
			else if (compInst.substr(9, 5) == "00000" && compInst.substr(4, 5) != "00000")
			{
				// c.jalr instruction, decompresses to jalr x1,rs1,0
				decompInst = "000000000000" + compInst.substr(4, 5) + "000" + "00001" + "1100111";
			}
		}
		if (compInst.substr(0, 4) == "1000")
		{
			if (compInst.substr(9, 5) == "00000" && compInst.substr(4, 5) != "00000")
			{
				// c.jr decompresses to jalr x0,rs1,0
				decompInst = "000000000000" + compInst.substr(4, 5) + "000" + "00000" + "1100111";
			}
			else
			{
				//c.mv instruction, decompresses to add rd,rs2,x0
				decompInst = "0000000" + compInst.substr(9, 5) + "00000" + "000" + compInst.substr(4, 5) + "0110011";
			}
		}
		if (compInst.substr(0, 3) == "000" && compInst.substr(9, 5) != "00000")
		{
			//c.slli instruction, decompresses to slli rd,rs1,shamt
			decompInst = "000000" + compInst.substr(3, 1) + compInst.substr(9, 5) + compInst.substr(4, 5) + "001" + compInst.substr(4, 5) + "0010011";
		}
		if (compInst.substr(0, 3) == "010" && compInst.substr(4, 5) != "00000")
		{
			// c.lwsp instruction, decompresses to lw rd,imm(rs2)
			decompInst = "0000" + compInst.substr(12, 2) + compInst.substr(3, 1) + compInst.substr(9, 3) + "00" + compInst.substr(4, 5) + "010" + compInst.substr(4, 5) + "0000011";
		}
		if (compInst.substr(0, 3) == "110")
		{
			// c.swsp instruction, decompresses to sw rs2,imm(rs1)
			decompInst = "0000" + compInst.substr(7, 2) + compInst.substr(3, 1) + compInst.substr(9, 5) + "00010" + "010" + compInst.substr(4, 3) + "00" + "0100011";
		}
	}

	return decompInst;
}