
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include "ship.h"
#include "loc.h"


// ====== NOTATION: X is ship, # is water ======

//================ HELPER FUNCTION ==========================================

void print_bd (std::vector<std::string> const &my_bd, std::vector<int> const &num_ship_each_row, std::vector<int> const&num_ship_each_col) {
	//std::cout << "Print Board For Testing " << "\n";
	//==============Begin=====================
	std::cout << '+';
	for (unsigned int i=0; i<num_ship_each_col.size(); ++i) {
		std::cout<< '-';
	}
	std::cout << '+' << '\n';
	//==============Content=====================
	for (unsigned int j=0; j<my_bd.size(); ++j) {
		std::cout << '|' ;
		for (unsigned int i=0; i<my_bd[0].size(); ++i) {
			std::cout << my_bd[j][i];
		}
		std::cout << '|' << num_ship_each_row[j] << std::endl;
	}
	//==============End=====================
	std::cout << '+';
	for (unsigned int i=0; i<num_ship_each_col.size(); ++i) {
		std::cout<< '-';
	}
	std::cout << '+' << '\n';
	std::cout << ' ';
	for (unsigned int j=0; j<num_ship_each_col.size(); ++j) {
		std::cout << num_ship_each_col[j];
	}
	std::cout << ' ' << '\n';
}

void print_sol (std::vector<ship> const &solution) {
	if (solution.size()==0) {
		return;
	}
	else {
		std::cout << "Solution:" << "\n";
		for (unsigned int j=0; j<solution.size(); ++j) {
			std::cout << std::setw(11) << std::left << solution[j].get_type() << std::setw(2) << std::left << solution[j].get_loc_row() << std::setw(3) << std::left << solution[j].get_loc_col() << solution[j].get_direction() << "\n";
		}
	}
}

void format_bd (std::vector<std::string> &my_bd, std::vector<ship> const &solution, std::vector<int> &num_ship_each_row, std::vector<int> &num_ship_each_col) {
	for (int i=0; i<my_bd.size(); ++i) {
		for (int j=0; j<my_bd[0].size(); ++j) {
			if (my_bd[i][j] == '#') {
				my_bd[i][j] = ' ';
			}
		}
	}
	for (int i=0; i< solution.size(); ++i) {
		int x= solution[i].get_loc_row();
		int y= solution[i].get_loc_col();
		int l= solution[i].get_length();
		if (solution[i].get_direction() == " ") {
			my_bd[x][y] = 'o';
		}
		else if (solution[i].get_direction() == "horizontal") {
			int count = l;
			while(count >0) {
				if (count == l) {
					my_bd[x][y] = '<';
					count --;
				}
				else if (count == 1) {
					my_bd[x][y+l-1] = '>';
					count --;
				}
				else {
					my_bd[x][y+l-count] = 'X';
					count--;
				}
			}
		}
		else {
			int count = l;
			while(count >0) {
				if (count == l) {
					my_bd[x][y] = '^';
					count --;
				}
				else if (count == 1) {
					my_bd[x+l-1][y] = 'v';
					count --;
				}
				else {
					my_bd[x+l-count][y] = 'X';
					count--;
				}
			}
		}
	}
}

void fix_row_col (std::vector<std::string> const&my_bd, std::vector<int> &num_ship_each_row, std::vector<int> &num_ship_each_col) {
	for (int i=0; i<num_ship_each_row.size(); ++i) {
		if (num_ship_each_row[i] == -1) {
			int count=0;
			for (int j=0; j<num_ship_each_col.size(); j++) {
				if (my_bd[i][j] != ' ') {
					count++;
				}
			}
			num_ship_each_row[i] = count;
		}
	}
	for (int j=0; j<num_ship_each_col.size(); ++j) {
		if (num_ship_each_col[j] == -1) {
			int count=0;
			for (int i=0; i<num_ship_each_row.size(); i++) {
				if (my_bd[i][j] != ' ') {
					count++;
				}
			}
			num_ship_each_col[j] = count;
		}
	}
}

bool same_bd (std::vector<std::string> const &my_bd1, std::vector<std::string> const &my_bd2) {
	if (my_bd1.size() != my_bd2.size()) {
		return false;
	}
	else if (my_bd1[0].size() != my_bd2[0].size()) {
		return false;
	}
	else {
		for (int i=0; i<my_bd1.size(); ++i) {
			for (int j=0; j<my_bd1[0].size(); ++j) {
				if (my_bd1[i][j] != my_bd2[i][j]) {
					return false;
				}
			}
		}
	}
	return true;
}

bool is_duplicate (std::vector<std::vector<std::string> > const &vec_bd, std::vector<std::string> const &my_bd) {
	for (int i=0; i<vec_bd.size(); ++i) {
		if (same_bd (vec_bd[i], my_bd)) {
			return true;
		}
	}
	return false;
}

bool fit_constraint (std::vector<std::string> &my_bd, std::vector<char> const &constraint, std::vector<loc> const &constraint_loc) {
	if (constraint.size()==0) {
		return true;
	}
	else {
		for (int i=0; i<constraint.size(); ++i) {
			int row = constraint_loc[i].row;
			int col = constraint_loc[i].col;
			if (my_bd[row][col] != constraint[i]) {
				return false;
			}
		}
	}
	return true;
}

void fill_water (std::vector<std::string> &my_bd, std::vector<int> &num_ship_each_row, std::vector<int> &num_ship_each_col) {
	//========== FILL ADJACENT CELL OF SHIP WITH WATER ======================
	int bd_row=num_ship_each_row.size();
	int bd_col=num_ship_each_col.size();
	for (int i=0; i< bd_row; ++i) {
		for (int j=0; j< bd_col; ++j) {
			if (my_bd[i][j] == ' ') {
				//======== UPPER LEFT ===========
				if ((i==0) && (j==0)) {
					if ((my_bd[i+1][j] == 'X') ||(my_bd[i][j+1] == 'X') || (my_bd[i+1][j+1] == 'X')) {
						my_bd[i][j] = '#';
					}
				}
				//======== UPPER RIGHT ===========
				else if ((i==0) && (j==bd_col-1)) {
					if ((my_bd[i+1][j] == 'X') ||(my_bd[i][j-1] == 'X') || (my_bd[i+1][j-1] == 'X')) {
						my_bd[i][j] = '#';
					}
				}
				//======== LOWER LEFT ===========
				else if ((i==bd_row-1) && (j==0)) {
					if ((my_bd[i-1][j] == 'X') ||(my_bd[i][j+1] == 'X') || (my_bd[i-1][j+1] == 'X')) {
						my_bd[i][j] = '#';
					}
				}
				//======== LOWER RIGHT ===========
				else if ((i==bd_row-1) && (j==bd_col-1)) {
					if ((my_bd[i-1][j] == 'X') ||(my_bd[i][j-1] == 'X') || (my_bd[i-1][j-1] == 'X')) {
						my_bd[i][j] = '#';
					}
				}
				//======== TOP ===========
				else if (i==0) {
					if ((my_bd[i+1][j] == 'X') ||(my_bd[i][j-1] == 'X') || (my_bd[i][j+1] == 'X') || (my_bd[i+1][j-1] == 'X') || (my_bd[i+1][j+1] == 'X')) {
						my_bd[i][j] = '#';
					}
				}
				//======== BOT ===========
				else if (i==bd_row-1) {
					if ((my_bd[i-1][j] == 'X') ||(my_bd[i][j-1] == 'X') || (my_bd[i][j+1] == 'X') || (my_bd[i-1][j-1] == 'X') || (my_bd[i-1][j+1] == 'X')) {
						my_bd[i][j] = '#';
					}
				}
				//======== LEFT ===========
				else if (j==0) {
					if ((my_bd[i-1][j] == 'X') ||(my_bd[i+1][j] == 'X') || (my_bd[i][j+1] == 'X') || (my_bd[i-1][j+1] == 'X') || (my_bd[i+1][j+1] == 'X')) {
						my_bd[i][j] = '#';
					}
				}
				//======== RIGHT ===========
				else if (j==bd_col-1) {
					if ((my_bd[i-1][j] == 'X') ||(my_bd[i][j-1] == 'X') || (my_bd[i+1][j] == 'X') || (my_bd[i+1][j-1] == 'X') || (my_bd[i-1][j-1] == 'X')) {
						my_bd[i][j] = '#';
					}
				}
				//======== NORMAL CASES ===========
				else {
					if ((my_bd[i-1][j] == 'X') || (my_bd[i+1][j] == 'X') || (my_bd[i][j-1] == 'X') || (my_bd[i][j+1] == 'X') || (my_bd[i-1][j-1] == 'X') || (my_bd[i-1][j+1] == 'X') || (my_bd[i+1][j-1] == 'X') || (my_bd[i+1][j+1] == 'X')) {
						my_bd[i][j] = '#'; 
					}
				}
			}
		}
	}
}


void fill_remains (std::vector<std::string> &my_bd, std::vector<int> &num_ship_each_row, std::vector<int> &num_ship_each_col) {
	//========== FILL THOSE IDENTIFIABLE CELLS WITH WATER ======================
	int bd_row=num_ship_each_row.size();
	int bd_col=num_ship_each_col.size();
	for (unsigned int i=0; i<num_ship_each_row.size(); ++i) {
		if (num_ship_each_row[i] == 0) {
			for (int p=0; p<bd_col; ++p) {
				if (my_bd[i][p] == ' ') {
					my_bd[i][p] = '#';
				}
			}
		}
	}
	for (unsigned int j=0; j<num_ship_each_col.size(); ++j) {
		if (num_ship_each_col[j] == 0) {
			for (int q=0; q<bd_row; ++q) {
				if (my_bd[q][j] == ' ') {
					my_bd[q][j] = '#';
				}
			}
		}
	}
}

bool trigger_function_h (std::vector<std::string> &my_bd, std::vector<ship> &solution, int max, loc location, std::vector<int> &num_ship_each_row, std::vector<int> &num_ship_each_col, std::vector<std::vector<std::string> > &vec_bd, std::vector<std::vector<ship> > &vec_solutions){
	int i = location.row;
	int j = location.col;
	for (int p=0; p<max; p++) { 
		if ( (j+p) >= num_ship_each_col.size()) {  //TOUCH THE BOUNDARY________
			return false;
		}
		else {
			if (my_bd[i][j+p]!=' ') {
				return false;
			}
		}
	}
//=======================
	for (int p=0; p<max; p++) {
		my_bd[i][j+p] = 'X';
		num_ship_each_row[i] --;
		num_ship_each_col[j+p] --;
	}
//=======================
	int index;
	for (int i=0; i<solution.size(); ++i) {
		if ((solution[i].get_length()==max)&&(solution[i].get_loc_row() == -1)) {
			index = i;
			break;
		}
	}
	solution[index].set_loc(i,j);
	if (max != 1) {
		solution[index].set_direction("horizontal");
	}
	return true;
}


bool trigger_function_v (std::vector<std::string> &my_bd, std::vector<ship> &solution, int max, loc location, std::vector<int> &num_ship_each_row, std::vector<int> &num_ship_each_col, std::vector<std::vector<std::string> > &vec_bd, std::vector<std::vector<ship> > &vec_solutions){
	int i = location.row;
	int j = location.col;
	for (int q=0; q<max; q++) { 
		if ( (i+q) >= num_ship_each_row.size()) {  //TOUCH THE BOUNDARY________
			return false;
		}
		else {
			if (my_bd[i+q][j]!=' ') {
				return false;
			}
		}
	}
//=======================
	for (int q=0; q<max; q++) {
		my_bd[i+q][j] = 'X';
		num_ship_each_row[i+q] --;
		num_ship_each_col[j] --;
	}
//=======================
	int index;
	for (int i=0; i<solution.size(); ++i) {
		if ((solution[i].get_length()==max)&&(solution[i].get_loc_row() == -1)) {
			index = i;
			break;
		}
	}
	solution[index].set_loc(i,j);
	if (max != 1) {
		solution[index].set_direction("vertical");
	}
	return true;
}

bool trigger_function (bool constraint_found, int argc, std::vector<std::string> &my_bd, std::vector<ship> &solution, std::vector<ship> &vec_ships, std::vector<int> &num_ship_each_row, std::vector<int> &num_ship_each_col, std::vector<std::vector<std::string> > &vec_bd, std::vector<std::vector<ship> > &vec_solutions, std::vector<char> const &constraint, std::vector<loc> const &constraint_loc) {
	// ==============================================================
	bool check_4=false; 
	bool check_5=false; 
	bool check1, check2, check3; 
	int bd_row=num_ship_each_row.size();
	int bd_col=num_ship_each_col.size();
	// ========= BASE CASE ==========================================
	if (!constraint_found) {
		if ((argc==2)&&(vec_bd.size()==1)) {
			return true; /////////////////////////////////////// BUT NOT FOR THE CONSTRAINTS
		}
	}
	int sum_ship_size =0;
	for (int i=0; i<vec_ships.size(); ++i) {
		sum_ship_size += vec_ships[i].get_length();
	}
	int sum_row=0;
	for (int i=0; i<num_ship_each_row.size(); ++i) {
		sum_row += num_ship_each_row[i];
	}
	int sum_col=0;
	for (int i=0; i<num_ship_each_col.size(); ++i) {
		sum_col += num_ship_each_col[i];
	}
	if (sum_col != sum_row) {
		return false;
	}
	if (sum_ship_size != sum_row) {
		return false;
	}

	check1 =true;
	for (int i=0; i<bd_row; ++i) {
		for (int j=0; j<bd_col; ++j) {
			if (my_bd[i][j] == ' ') {
				check1 = false;
				break;
			}
		}
	}
	check2 =true;
	for (int i=0; i<bd_row; ++i) {
		if (num_ship_each_row[i]!=0) {
			check2 =false;
			break;
		}
	}
	check3 =true;
	for (int j=0; j<bd_col; ++j) {
		if (num_ship_each_col[j]!=0) {
			check3 =false;
			break;
		}
	}

	if ((check1==true)&&(check2==true)&&(check3==true)&&(vec_ships.size()==0)) { //
		format_bd(my_bd, solution, num_ship_each_row, num_ship_each_col);
		if (!is_duplicate (vec_bd, my_bd)) {
		}
		if (fit_constraint(my_bd, constraint, constraint_loc)) {
		}
		if ((!is_duplicate (vec_bd, my_bd)) && fit_constraint(my_bd, constraint, constraint_loc)) {	
			vec_solutions.push_back(solution);
			vec_bd.push_back(my_bd);
		}		
		//std::cout << "==============SUCESS====================" << std::endl<< std::endl<< std::endl;
		return true;
	}
	int max_length = vec_ships[vec_ships.size()-1].get_length();
	//================= CHECK EACH ROW =============================================================	
	for (int i=0; i<bd_row; ++i) {
		if (num_ship_each_row[i] >= max_length) {
			for (int j=0; j<bd_col; ++j) {
				if (my_bd[i][j] == ' '){
					std::vector<std::string> my_bd_h = my_bd;
					std::vector<ship> solution_h = solution;
					std::vector<int> num_ship_each_row_h = num_ship_each_row;
					std::vector<int> num_ship_each_col_h = num_ship_each_col;
					std::vector<ship> solution = vec_ships;
					if (trigger_function_h(my_bd_h, solution_h, max_length, loc(i,j), num_ship_each_row_h, num_ship_each_col_h, vec_bd, vec_solutions)) {
						std::vector<ship> vec_ships_h = vec_ships;
						vec_ships_h.pop_back();
						fill_water (my_bd_h, num_ship_each_row_h, num_ship_each_col_h);
						fill_remains (my_bd_h, num_ship_each_row_h, num_ship_each_col_h);					
						if (trigger_function (constraint_found, argc, my_bd_h, solution_h, vec_ships_h, num_ship_each_row_h, num_ship_each_col_h, vec_bd, vec_solutions, constraint, constraint_loc)) {
							check_4 = true;
						}
					}
				}
			}
		}
	}
	//================= CHECK EACH COLUMN ==============================================================
	for (int q=0; q<bd_col; ++q) {
		
		if (num_ship_each_col[q] >= max_length) {
			for (int p=0; p<bd_row; ++p) {
				if (my_bd[p][q] == ' '){
					std::vector<std::string> my_bd_v = my_bd;
					std::vector<ship> solution_v = solution;
					std::vector<int> num_ship_each_row_v = num_ship_each_row;
					std::vector<int> num_ship_each_col_v = num_ship_each_col;
					std::vector<ship> solution = vec_ships;
					if (trigger_function_v(my_bd_v, solution_v, max_length, loc(p,q), num_ship_each_row_v, num_ship_each_col_v,vec_bd, vec_solutions)) {					
						std::vector<ship> vec_ships_v = vec_ships;
						vec_ships_v.pop_back();
						fill_water (my_bd_v, num_ship_each_row_v, num_ship_each_col_v);
						fill_remains (my_bd_v, num_ship_each_row_v, num_ship_each_col_v);
						if (trigger_function (constraint_found, argc, my_bd_v, solution_v, vec_ships_v, num_ship_each_row_v, num_ship_each_col_v, vec_bd, vec_solutions, constraint, constraint_loc)) {
							check_5 = true;
						}
					}
				}
			}
		}
	}
	if ((check_4 == true)||(check_5 == true)) {
		//std::cout << "==============SUCESS====================" << std::endl<< std::endl<< std::endl;
		return true;
	}
	//std::cout << "==============FAIL====================" << std::endl<< std::endl<< std::endl;
	return false;
} 

void produce_unknown_row_type(std::vector<int> &num_ship_each_row, std::vector<int> const &num_ship_each_col, std::vector<std::vector<int> > &vec_num_row) {
	bool check= true;
	for (int i=0; i<num_ship_each_row.size(); ++i) {
		if (num_ship_each_row[i] == -1) {
			check= false;
		}
	}
	if (check) {
		vec_num_row.push_back(num_ship_each_row);
	}

	for (int i=0; i<num_ship_each_row.size(); ++i) {
		if (num_ship_each_row[i] == -1) {
			for (int j=0; j<num_ship_each_col.size()+1;++j) {
				std::vector<int> new_num_ship_each_row = num_ship_each_row;
				new_num_ship_each_row[i] = j;
				produce_unknown_row_type(new_num_ship_each_row, num_ship_each_col, vec_num_row);
			}
			break;
		}
	}
}

void produce_unknown_col_type(std::vector<int> const &num_ship_each_row, std::vector<int> &num_ship_each_col, std::vector<std::vector<int> > &vec_num_col) {
	bool check= true;
	for (int i=0; i<num_ship_each_col.size(); ++i) {
		if (num_ship_each_col[i] == -1) {
			check= false;
		}
	}
	if (check) {
		vec_num_col.push_back(num_ship_each_col);
	}

	for (int i=0; i<num_ship_each_col.size(); ++i) {
		if (num_ship_each_col[i] == -1) {
			for (int j=0; j<num_ship_each_row.size()+1;++j) {
				std::vector<int> new_num_ship_each_col = num_ship_each_col;
				new_num_ship_each_col[i] = j;
				produce_unknown_col_type(num_ship_each_row, new_num_ship_each_col, vec_num_col);
			}
			break;
		}
	}
}

void produce_unknown_ship_type(std::vector<ship> &vec_ships, std::vector<ship> &solution, std::vector<std::vector<ship> > &vec_vec_ship, std::vector<std::vector<ship> > &vec_vec_sol) {
	bool check=true;
	for (int i=0; i<vec_ships.size(); ++i) {
		if (vec_ships[i].get_type()=="unknown") {
			check = false;
		}
	}
	for (int i=0; i<solution.size(); ++i) {
		if (solution[i].get_type()=="unknown") {
			check = false;
		}
	}
	if (check) {
		vec_vec_ship.push_back(vec_ships);
		vec_vec_sol.push_back(solution);
	}

	for (int i=0; i<vec_ships.size(); ++i) {
		if (vec_ships[i].get_type()=="unknown") {
			for (int j=1; j<8; ++j) {
				if (j==1) {
					std::vector<ship> new_vec_ships = vec_ships;
					std::vector<ship> new_solution = solution;
					new_vec_ships[i].set_type("submarine");
					new_solution[i].set_type("submarine");
					produce_unknown_ship_type(new_vec_ships, new_solution, vec_vec_ship, vec_vec_sol);
				}
				else if (j==2) {
					std::vector<ship> new_vec_ships = vec_ships;
					std::vector<ship> new_solution = solution;
					new_vec_ships[i].set_type("destroyer");
					new_solution[i].set_type("destroyer");
					produce_unknown_ship_type(new_vec_ships, new_solution, vec_vec_ship, vec_vec_sol);
				}
				else if (j==3) {
					std::vector<ship> new_vec_ships = vec_ships;
					std::vector<ship> new_solution = solution;
					new_vec_ships[i].set_type("cruiser");
					new_solution[i].set_type("cruiser");
					produce_unknown_ship_type(new_vec_ships, new_solution, vec_vec_ship, vec_vec_sol);
				}
				else if (j==4) {
					std::vector<ship> new_vec_ships = vec_ships;
					std::vector<ship> new_solution = solution;
					new_vec_ships[i].set_type("battleship");
					new_solution[i].set_type("battleship");
					produce_unknown_ship_type(new_vec_ships, new_solution, vec_vec_ship, vec_vec_sol);
				}
				else if (j==5) {
					std::vector<ship> new_vec_ships = vec_ships;
					std::vector<ship> new_solution = solution;
					new_vec_ships[i].set_type("carrier");
					new_solution[i].set_type("carrier");
					produce_unknown_ship_type(new_vec_ships, new_solution, vec_vec_ship, vec_vec_sol);
				}
				else if (j==6) {
					std::vector<ship> new_vec_ships = vec_ships;
					std::vector<ship> new_solution = solution;
					new_vec_ships[i].set_type("cargo");
					new_solution[i].set_type("cargo");
					produce_unknown_ship_type(new_vec_ships, new_solution, vec_vec_ship, vec_vec_sol);
				}
				else {
					std::vector<ship> new_vec_ships = vec_ships;
					std::vector<ship> new_solution = solution;
					new_vec_ships[i].set_type("tanker");
					new_solution[i].set_type("tanker");
					produce_unknown_ship_type(new_vec_ships, new_solution, vec_vec_ship, vec_vec_sol);
				}
			}
			break;
		}
	}
}

int main(int argc, char* argv[]) {
	//============ JUST CHECKING ============================================
	if (argc < 2 || argc > 3) {
		std::cerr << "Invalid commands number" << "\n";
		return 1;
	}
	std::ifstream puzzle (argv[1]); 
	if (!puzzle.good()) {
		std::cerr << "Can not open the puzzle file " << argv[0] << "\n";
		return 1;		
	}

	//============ INITIALIZE VECTOR ============================================
	std::vector<ship> vec_ships;
	std::vector<int> num_ship_each_row, num_ship_each_col;

	//============ READING INPUT============================
	std::string str1, str2, str3, atype;
	int bd_row, bd_col;
	std::string bd_row_str, bd_col_str, num_ship_each_row_i, num_ship_each_col_j;
	puzzle >> str1 >> bd_row_str >> bd_col_str;
	bd_row = atoi(bd_row_str.c_str());
	bd_col = atoi(bd_col_str.c_str());
	puzzle >> str2;
	for (int i=0; i<bd_row; ++i) {
		puzzle >> num_ship_each_row_i;
		if (num_ship_each_row_i == "?") {
			num_ship_each_row.push_back(-1);
		}
		else {
			num_ship_each_row.push_back(atoi(num_ship_each_row_i.c_str()));
		}
	}
	puzzle >> str2;
	for (int j=0; j<bd_col; ++j) {
		puzzle >> num_ship_each_col_j;
		if ( num_ship_each_col_j == "?") {
			num_ship_each_col.push_back(-1);
		}
		else {
			num_ship_each_col.push_back(atoi(num_ship_each_col_j.c_str()));
		}
	}
	//================ INITIALIZE MY BOARD AND SHIPS ==============================
	std::vector<std::string> my_bd;
	std::vector<ship> solution;
	bool constraint_found=false;
	//===================================================================
	for (int i=0; i<bd_row; ++i) {
		std::string my_row;
		for (int j=0; j<bd_col; ++j) {
			my_row += ' ';
		}
		my_bd.push_back(my_row);
	}
	while ((puzzle >> atype)) {
		if (atype == "constraint") {
			constraint_found = true;
			break;
		}
		ship my_ship;
		my_ship.set_type(atype);
		vec_ships.push_back(my_ship);
	}
	std::sort(vec_ships.begin(), vec_ships.end(), ship_length_compare);
	solution = vec_ships;

	//================ CONSTRAINT INPUT ================================
	std::vector<char> constraint;
	std::vector<loc> constraint_loc;
	int row, col;
	std::string constraint_str, str_row, str_col, con;

	if (constraint_found==true) {
		puzzle >> str_row >> str_col >> con;
		row = atoi(str_row.c_str());
		col = atoi(str_col.c_str());
		loc con_loc(row, col);
		constraint.push_back(con[0]);
		constraint_loc.push_back(con_loc);
		while (puzzle >> constraint_str >> str_row >> str_col >> con) {
			row = atoi(str_row.c_str());
			col = atoi(str_col.c_str());
			loc con_loc(row, col);
			constraint.push_back(con[0]);
			constraint_loc.push_back(con_loc);
		}
	}
	//================ QUESTION 3 -------> UNKNOWN TYPES ================================
	std::vector<std::vector<ship> > vec_vec_ship, vec_vec_sol; 
	std::vector<std::vector<int> > vec_num_row, vec_num_col;
	for (int i=0; i<num_ship_each_row.size(); ++i) {
		if (num_ship_each_row[i] == -1) {
		}
	}
	for (int j=0; j<num_ship_each_col.size(); ++j) {
		if ( num_ship_each_col[j] == -1) {
		}
	}
	produce_unknown_ship_type(vec_ships, solution, vec_vec_ship, vec_vec_sol);
	produce_unknown_row_type(num_ship_each_row, num_ship_each_col, vec_num_row);
	produce_unknown_col_type(num_ship_each_row, num_ship_each_col, vec_num_col);
	//std::cout << "HOW MANY POSSIBILITIES " << vec_num_row.size() <<"row conditions " << vec_num_col.size() <<"col conditions " << vec_vec_ship.size() <<"ships conditions \n";
	
	//============ PREPARE FOR SOLUTION ============================
	std::vector<std::vector<ship> > vec_solutions;
	std::vector<std::vector<std::string> > vec_bd;

	//============ STUDENT TEST CASE ============================

	//================ QUESTION 3 -------> UNKNOWN TYPES ================================
	for (int i=0; i<vec_num_row.size(); ++i) {
		for (int j=0; j<vec_num_col.size(); ++j) {
			for (int p=0; p<vec_vec_ship.size(); ++p) {
				std::vector<std::string> new_board = my_bd;
				fill_remains (new_board, vec_num_row[i], vec_num_col[j]);
				trigger_function(constraint_found, argc, new_board, vec_vec_sol[p], vec_vec_ship[p], vec_num_row[i], vec_num_col[j], vec_bd, vec_solutions, constraint, constraint_loc);
			}
		}
	}
	

	//============ OUTPUT =========================================================
	if (vec_solutions.size()==0) {
		std:: cout << "No solutions" <<std::endl;
	}
	else {
		if ((argc == 3)&&(std::string(argv[2]) =="find_all_solutions")){
			for (int i=0; i<vec_bd.size(); ++i) {
				print_sol (vec_solutions[i]);
				fix_row_col (vec_bd[i], num_ship_each_row, num_ship_each_col);
				print_bd (vec_bd[i], num_ship_each_row, num_ship_each_col);     //num_ship_each_row CHANGE TO vec_num_row ?????????????????????????????????
				std::cout << std::endl;
			}
			std:: cout << "Found " << vec_solutions.size() << " solution(s)" <<std::endl;
		}
		else {
			print_sol (vec_solutions[0]);
			fix_row_col (vec_bd[0], num_ship_each_row, num_ship_each_col);
			print_bd (vec_bd[0], num_ship_each_row, num_ship_each_col);    //num_ship_each_row CHANGE TO vec_num_row ?????????????????????????????????
			std::cout << std::endl;
		}
	}
	//========================================================================================
	return 0;
}