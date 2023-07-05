# **CSCE 2303 Project 1**
## How to build and use the simulator 
in the terminal type:
- `g++ main.cpp -o main`
- `./main <testcase-folder-name>/<text-section-file-name> <testcase-folder-name>/<data-section-file-name>`

## The simulator design
### [DisplayInst](functions/displayInst.h)
This function consists mainly of switch case statements that will eventually lead to the given instruction being output. It will initially check the type of instruction based on the type that is stored in the struct that we created, implemented using a switch case statement, which will then call the respective display(Type) function for it. Each sub function will create the format of the assembly instruction in the string and will utilize the aspects of the word, like func3, to differentiate between separate instructions to accordingly assign the name of the instruction to said string. For readability reasons, the registers are printed using their ABI names which was done by 1-1 mapping each ABI name to its corresponding number as its index. Immediates we’re assumed to have been shifted or fixed by the translate function therefore I immediately display them.

### [Execute](functions/execute.h)
Upon translating the instruction, and setting the value of all the word parts necessary for that specific instruction, an execute function is called to execute said instruction. The execute function takes as parameters the instruction object, the memory array, the registers array, as well as the program counter (pc). The function would then modify the values stored, whether in the memory or the registers, depending on the instruction. Additionally, the function updates the pc depending on whether the instruction is compressed or not, or maybe to jump to a specified memory location. In order to execute an instruction, the function checks the type of the instruction being executed based on the opcode. Then, executes a specific instruction depending on the func3 parameter already stored in the instruction object. Finally, the pc is updated and the function returns.

### [Decompressor](functions/decompressor.h)
The decompressor takes in a compressed instruction, an instruction written in 16 bits where the last 2 bits are not 11, and analyzes it according to either its opcode, func3 or value of rs1/rd to obtain its correct type. After it has obtained the correct type of the incoming instruction, it will change the instruction to its correct uncompressed 32 bit format. This is done by comparing what the original format of the half word was to what it should become as a decompressed full word and doing the required string manipulations to create this. 

### [ISS](functions/ISS.h)
This is the overhead function that will bring together all the functions that were created to simulate an ISS. It creates three arrays; memory, the instructions that we read and the registers. It will call loadMemory in order to store the given file in our created array and the call runDisplayAtExecution which will, inside a while loop, check if the half word we read needs decompression and call decompress if needed, then it will store the opcode and type of instruction that is being read in the instruction struct, we then display the instruction by calling displayInst and execute it by called execute. To optimize the code slightly we check if the half word is all 0s so that we can skip the word, otherwise we check if the word has been previously already translated in order not to do it again.

### [InstructionStruct](functions/instructionStruct.h)
In order to simplify our code design and  increase its readability, every instruction was allocated an instance of the Instruction struct. This struct allows us to organize every instruction’s word segments as distinct struct members. Instruction struct members include:
- int func7
- int rd
- int rs1
- int rs2
- int func3
- int immediate
- string opcode

In addition to segments of the instruction word, the struct contains additional members that ease maneuver and act as useful identifiers.
- char type
- bool isCompressed
- bool isTranslated
- string decompressedWord

### [LoadMemory](functions/loadMemory.h)
This function will receive the string from the readFile function and split it up into bytes to be stored in memory. Every 8 bits are split and stored consecutively in the memory array and any indices not used by the file data will be initialized by 0. As per the handout, the text section begins form index 1 and the data section starts at index 65536 (0x00010000). An exception will be thrown if the length of the string is found to be greater than the allocated size of memory. 

### [Translate](functions/translate.h)
The translate function is responsible for initializing the instruction struct members containing segments of the instruction word. The function takes a 32-bit instruction word alongside the struct instance and maps all instruction data such as the registers, operation identifiers (func3 and func7), offsets and immediates contained in the word to their corresponding members in the struct. 

The function consists primarily of a switch case statement which, according to the instruction type, calls a corresponding function that correctly extracts the data in the instruction word and assigns it to the appropriate members.

Moreover, the function handles the half-word immediates stored in the B and J type instructions, by converting the stored numerical offsets to their byte equivalents (unit change). Thus the offsets stored in the struct instance can be directly used by the display and execute functions to perform the sought jumps/branching

### [Utilities](functions/utilities.h)
Utilities contains the essential helper functions that are repeatedly used to facilitate the execution of the project code. These are mainly converter-functions that are at the heart of all other project functions. Those functions are:
- getTwosComplement
- binaryToDec 
- decToBinary

Where binaryToDec and decToBinary handle both signed and unsigned  conversions.

### [ReadFile](functions/readFile.h)
The function takes in string &machineCode and char *arg as parameters and the .text section files. It is important to note that in the function ios::binary tells the program to read the file’s content as binary numbers. Thus, if the file is open, read every byte and conver it to a bitset and then read each bit from the bitset and append it to the string called ‘machineCode’. Then the function will check if the bytes in the string machineCode are less than or greater than the size of the .text section which is 65536. If the bytes are greater than then we ignore the extra bytes;however, if the number of bytes is smaller than 65536, then we will append the “missing” bytes by 0.
## Challenges

## Contributers:
- @useframi1
- @NadSafwat
- @nkasaby
- @Mariam-ElG
