#include <iostream>
#include "geesespotter_lib.h"

char* createBoard(std::size_t xdim, std::size_t ydim) {

	// this goes on the heap and doesn't get lost
	char *board = new char[xdim * ydim] { 0 };
	return board;
}

void computeNeighbors(char *board, std::size_t xdim, std::size_t ydim) {

	int index = 0; //this is the field we are checking

	for (int y { 0 }; y < ydim; y++) { // rows
		for (int x { 0 }; x < xdim; x++) { // columns
			int north { index - xdim }; // goes up one row
			int south { index + xdim }; //goes down one row etc...
			int east { index + 1 };
			int west { index - 1 };
			int north_east { north + 1 };
			int north_west { north - 1 };
			int south_east { south + 1 };
			int south_west { south - 1 };

			if (board[index] != 9) {

				if ((y > 0) && ((board[north]) == 9)) {
					board[index] += 1; //if I'm not at the first row (I have a north) and my north is a goose ---> increment index by 1.
				}

				if ((y < (ydim - 1)) && (board[south] == 9)) {
					board[index] += 1; //if I'm not at the last row (I have a south) and my south is a goose ---> increment index by 1.
				}

				if ((x < (xdim - 1)) && (board[east] == 9)) {
					board[index] += 1;
				}

				if ((x > 0) && (board[west] == 9)) {
					board[index] += 1;
				}

				if ((y > 0) && (x < (xdim - 1)) && (board[north_east] == 9)) {
					board[index] += 1;
				}

				if ((y > 0) && (x > 0) && (board[north_west] == 9)) {
					board[index] += 1;
				}

				if ((x < (xdim - 1)) && (y < (ydim - 1))
						&& (board[south_east] == 9)) {
					board[index] += 1;
				}

				if ((x > 0) && (y < (ydim - 1)) && (board[south_west] == 9)) {
					board[index] += 1;
				}

			}
			index++; // after we checked what is my board[index] I move on to the next by incrementing by 1.
		}

	}

	/*board[0] = 0;
	 board[23] = hiddenBit();
	 board[24] = hiddenBit() | 9; // this is a hidden field with a goose*/

}

void hideBoard(char *board, std::size_t xdim, std::size_t ydim) {
	for (int index { 0 }; index < xdim * ydim; index++) {
		board[index] = board[index] | hiddenBit(); // this turns on the hidden bit*/
	}
}

void cleanBoard(char *board) {

	delete[] board;
	board = nullptr;
}

void printBoard(char *board, std::size_t xdim, std::size_t ydim) {

	/*for (int i { 0 }; i < xdim * ydim; i++) {
	 std::cout << board[i] << " ";
	 }*/

	for (int location_index { 0 }; location_index <= (xdim * ydim - 1);
			location_index++) {

		if ((board[location_index] & markedBit()) == markedBit()) //Marked has to be before hidden as can't be hidden and marked
			std::cout << 'M';

		else if ((board[location_index] & hiddenBit()) == hiddenBit()) //location_index will be defined later on and hiddenBit() here is 0x20 which is the value for the bit when it is hidden, the & is a bitwise AND (see Lab 6)
			//Other ways to write this ----> if (board[location_index] & hiddenBit() > 0)
			//if (board[location_index] & hiddenBit() != 0)
			//if (board[location_index] & hiddenBit()) ---> if this is zero it will evaluate to false, otherwise true
			std::cout << '*';

		else
			std::cout << int(board[location_index]);

		if ((location_index % (xdim)) == (xdim - 1)) {
			std::cout << std::endl;
		}
	}
}

int reveal(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc,
		std::size_t yloc) {
	//return 1 if field is already marked
	// return 2 if field is already revealed
	// reveal a hidden field
	// return 9 if a field contains a goose

	//otherwise, 0, if 0 also reveal neighbors (compute neighbors)

	int index { xdim * yloc + xloc }; //this is the field we are checking and is an alternative to the for loop because we don't need to iterate through whole matrix

	if ((board[index] & markedBit()) == markedBit()) { // checks if field is already marked and means it's already marked
		return 1;
	}

	else if ((board[index] & valueMask()) == board[index]) { // checks if field is already revealed
		return 2;
	}

	else if ((board[index] & 0x09) == 0x09) { // checks if field contains a goose
		board[index] = board[index] & valueMask(); // removing all the flags and leaving neighboring bits
		return 9;
	}

	board[index] = board[index] & valueMask();

	if (board[index] == 0) { // reveal neighbors if the field I'm at == 0
		index = 0;
		for (int y { 0 }; y < ydim; y++) { // rows
			for (int x { 0 }; x < xdim; x++) { // columns
				if ((x == xloc) && (y == yloc)) {
					int north { index - xdim }; // goes up one row
					int south { index + xdim }; //goes down one row etc...
					int east { index + 1 };
					int west { index - 1 };
					int north_east { north + 1 };
					int north_west { north - 1 };
					int south_east { south + 1 };
					int south_west { south - 1 };
					if ((y > 0)) {
						board[north] = (board[north] & valueMask()); //if I'm not at the first row (I have a north) and my north is a goose ---> increment index by 1.
					}

					if ((y < (ydim - 1))) {
						board[south] = (board[south] & valueMask()); //if I'm not at the last row (I have a south) and my south is a goose ---> increment index by 1.
					}

					if ((x < (xdim - 1))) {
						board[east] = (board[east] & valueMask());
					}

					if ((x > 0)) {
						board[west] = (board[west] & valueMask());
					}

					if ((y > 0) && (x < (xdim - 1))) {
						board[north_east] = (board[north_east] & valueMask());
					}

					if ((y > 0) && (x > 0)) {
						board[north_west] = (board[north_west] & valueMask());
					}

					if ((x < (xdim - 1)) && (y < (ydim - 1))) {
						board[south_east] = (board[south_east] & valueMask());
					}

					if ((x > 0) && (y < (ydim - 1))) {
						board[south_west] = (board[south_west] & valueMask());
					}

				}
				index++;
			}

		}
	}

	return 0;
}

int mark(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc) {

	int index { xdim * yloc + xloc }; //this is the field we are checking and is an alternative to the for loop because we don't need to iterate through whole matrix

	if (valueMask() == (board[index] | valueMask())) { // checks if field is already revealed
		return 2;
	}

	else
		board[index] = board[index] ^ markedBit(); //turns on marked bit
	return 0;
}
//board[index] = (board[index] & valueMask(). This reveals or can check if revealed
//(board[index] & markedBit()) == markedBit() checks if field is already marked and this means it's marked

bool isGameWon(char *board, std::size_t xdim, std::size_t ydim) {

	bool isgamewon = true;

	for (unsigned int index{0}; index < (xdim*ydim); index++) {
		if (((board[index] & 0x09) != 0x09) && ((board[index] & valueMask()) != board[index])) { //if fields are not a goose and fields are all revealed
		isgamewon = false;
		break;

		}
	}

	return isgamewon;

}






































