/*
 * Interface for running skip list demo and tests
 */

#include <iostream>

#include "tests.h"

int main(int argc, char* argv[])
{
	std::cout << "\n******************************************************************************************************"<< std::endl;
	std::cout << "\n Skip list implementation for CMPUT 403 Winter 2022" << std::endl;
	std::cout << " Based on https://en.wikipedia.org/wiki/Skip_list" << std::endl;
	std::cout << " Author: Mike Greber" << std::endl;

	char c;
	bool loop = true;
	do
	{
		std::cout << "\n******************************************************************************************************"<< std::endl;
		std::cout << "\n Main Menu:\n" << std::endl;
		std::cout << " Enter p to run performance test" << std::endl;
		std::cout << " Enter c to run correctness test" << std::endl;
		std::cout << " Enter f to run free test" << std::endl;
		std::cout << " Enter q to quit" << std::endl;
		std::cout << "\n ";
		std::cin >> c;

		switch(c)
		{
		case 'p':
			{
				run_performance_test();
				break;
			}
		case 'c':
			{
				run_correctness_test();
				break;
			}
		case 'f':
			{
				run_free_test();
				break;
			}
		case 'q':
			{
				loop = false;
				break;
			}
		default:
			{
				std::cout << "\n Invalid input" << std::endl;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		}
		
	} while (loop);
	
	std::cout << "\n******************************************************************************************************"<< std::endl;
	std::cout << "\n Quit" << std::endl;
	std::cout << "\n******************************************************************************************************\n"<< std::endl;
	
	return 0;
}
