#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include "loc.h"
#include "ship.h"


//ship::ship(std::string type, loc begin_loc, std::string direction, int length);
/*ship::ship(){
	type = ' ';
	direction = ' ';
	length = 0;
}*/


ship::ship() {
	type = ' ';
	loc my_begin_loc(-1,-1);
	begin_loc = my_begin_loc;
	direction = ' ';
	length = 0;
}

std::string ship::get_type() const {
	return type;
}

std::string ship::get_direction() const {
	return direction;
}

int ship::get_loc_row() const {
	return begin_loc.row;
}

int ship::get_loc_col() const {
	return begin_loc.col;
}

int ship::get_length() const {
	return length;
}

//================================================================

void ship::set_type(std::string atype) {
	type = atype;
	if (type == "submarine") {
		length =1;
	}
	else if (type == "destroyer") {
		length =2;
	}
	else if (type == "cruiser") {
		length =3;
	}
	else if (type == "battleship") {
		length =4;
	}
	else if (type == "carrier") {
		length =5;
	}
	else if (type == "cargo") {
		length =6;
	}
	else if (type == "tanker") {
		length =7;
	}
}

void ship::set_direction(std::string adirection) {
	direction = adirection;
}

void ship::set_loc(int r, int c) {
	begin_loc.row = r;
	begin_loc.col = c;
}

//==================================================================================


bool ship_length_compare(const ship &s1, const ship &s2) {
	return (s1.get_length() < s2.get_length());
}