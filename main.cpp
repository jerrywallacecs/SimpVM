#include <iostream> // console output
#include <fstream> // file input handling
#include <filesystem> // file error handling

//////////////////////////////////////////////////
//					DATA TYPE					//
//////////////////////////////////////////////////
struct address {
	int operation;
	int x;
	int y;

	// converts address into an integer value (0-999)
	int toInt() const
	{
		/*
			usage example:
			operation = 4;
			x = 2;
			y = 7;
			4 * 100 = 400
			2 * 10 = 20
			400 + 20 + 7 = 427
		*/
		return (operation * 100 + x * 10 + y);
	}

	// converts integer (0-999) to an address
	// note: this is static so that it does not require an instance
	// allowing us to call address::fromInt(int num)
	static address fromInt(int num)
	{
		/*
			usage example:
			num = 427;
			427 / 100 = 4.27 but is truncated due to being an integer
			427 / 10 % 10 = 2.7 but is truncated due to being an integer
			427 % 10 = 7
		*/
		return address(
			num / 100,
			num / 10 % 10,
			num % 10
		);
	}
};

//////////////////////////////////////////////////
//					GLOBALS						//
//////////////////////////////////////////////////

const int MEMORY_SIZE = 1000;
const int REGISTER_AMOUNT = 10;

bool running = true;
int programCounter = 0;
int instructionCounter = 0;

int registers[REGISTER_AMOUNT];
address memory[MEMORY_SIZE];


//////////////////////////////////////////////////
//				FUNCTION PROTOTYPES				//
//////////////////////////////////////////////////

void handleFile(std::string path);
std::string getUserInput();
void interpret(address memory[MEMORY_SIZE], int startingAddress);
void execute(address addr);
void displayStats();


//////////////////////////////////////////////////
//					ENTRY POINT					//
//////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	handleFile(getUserInput());
	interpret(memory, 0); // starting address of 0
	displayStats();

	return 0;
}

//////////////////////////////////////////////////
//				FUNCTION DEFINITIONS			//
//////////////////////////////////////////////////

// opens the file and loads memory, as well as some error handling
void handleFile(std::string path)
{
	std::ifstream fin(path);

	if (!fin)
	{
		std::cerr << "could not open file" << '\n' << "current working directory: " << std::filesystem::current_path() << '\n';
	}

	unsigned int index = 0;
	while (fin)
	{
		address unit = { 0, 0, 0 };
		int num;
		fin >> num;
		unit = address::fromInt(num);
		memory[index] = unit;
		index++;
	}
}

// prompts the user for a file path relative to the current directory
std::string getUserInput()
{
	std::string filePath;
	std::cout << "Please enter file path relative to current directory: ";
	std::cin >> filePath;

	return filePath;
}

// takes a starting address and a memory array and executes the instructions inside of memory
void interpret(address memory[MEMORY_SIZE], int startingAddress)
{
	programCounter = startingAddress;

	while (running)
	{

		// safety check
		if (programCounter < 0 || programCounter > MEMORY_SIZE)
		{
			std::cerr << "pc out of bounds." << '\n';
			return;
		}

		address currentInstruction = memory[programCounter];
		programCounter++;
		execute(currentInstruction);
		instructionCounter++;
	}
	return;
}

// executes on a given memory address with a pre-defined encoding
void execute(address addr)
{
	/*
	OPERATION ENCODING:
	100 : halt
	2dn : set register d to value n
	3dn : multiply register d by value n
	4dn : add value of n to register d
	5ds : set register d to value of register s
	6ds : multiply register d by value of register s
	7ds : add value of register s to register d
	8da : set register d to value in memory whose address is in register a
	9sa : set value in memory whose address is in register a to register s
	0ds : jump to location in register d unless register s contains 0
	*/

	switch (addr.operation)
	{
	case 0: // BRANCH
		if (registers[addr.y] != 0)
		{
			programCounter = registers[addr.x];
		}
		break;
	case 1: // HALT
		running = false;
		
		break;
	case 2: // SET TO VALUE
		registers[addr.x] = addr.y;
		
		break;
	case 3: // MULTIPLY BY VALUE
		registers[addr.x] = (registers[addr.x] * addr.y) % 1000;
		
		break;
	case 4: // ADD BY VALUE
		registers[addr.x] = (registers[addr.x] + addr.y) % 1000;
		
		break;
	case 5: // COPY BY VALUE
		registers[addr.x] = registers[addr.y] % 1000;
		
		break;
	case 6: // MULTIPLY BY REGISTER
		registers[addr.x] = (registers[addr.x] * registers[addr.y]) % 1000;
		
		break;
	case 7: // ADD BY REGISTER
		registers[addr.x] = (registers[addr.x] + registers[addr.y]) % 1000;
		
		break;
	case 8: // LOAD FROM MEMORY
		registers[addr.x] = memory[registers[addr.y]].toInt() % 1000;
		
		break;
	case 9: // WRITE TO MEMORY
		memory[registers[addr.y]] = address::fromInt(registers[addr.x] % 1000);
		
		break;
	}
}

// prints registers and number of instructions executed to the console
void displayStats()
{
	for (int i = 0; i < REGISTER_AMOUNT; i++)
	{
		std::cout << "Register " << i << ": " << registers[i] << '\n';
	}

	std::cout << "Number of instructions executed: " << instructionCounter << '\n';
}