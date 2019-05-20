#ifndef __ship_h_
#define __ship_h_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include "loc.h"

class ship {
public:
	ship();

	// ACCESSORS
	std::string get_type() const;
	std::string get_direction() const;
	int get_loc_row() const;
	int get_loc_col() const;
	int get_length() const;

	// MODIFIERS
	void set_type(std::string atype);
	void set_direction(std::string adirection);
	void set_loc(int r, int c);

private:
	std::string type;
	loc begin_loc;
	std::string direction;
	int length;
};
//other member fuctions that operate on player objects;
bool ship_length_compare(const ship &s1, const ship &s2);

#endif 
