#include "Errors.h"

#include <iostream>
#include <SDL/SDL.h>

namespace GTEngine {

	// Prints out an error message and exits the game
	void fatalError(std::string errorString){
		std::cout << errorString << std::endl;
		std::cout << "Enter any key to quit...";
		int userInput;
		std::cin >> userInput;
		SDL_Quit();
		exit(1);
	}

}