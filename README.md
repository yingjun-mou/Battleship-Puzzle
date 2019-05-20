# BattleshipPuzzle
Yingjun Mou
CSCI 1200 - Data Structure - Spring 2016

A program in C++ which can not only solve a battleship puzzle, but can also generate a possible battle puzzle problem given the dimension of board.

# How to Play
## Input
The command line accept 1 or 2 line arguments, the first arguement is the name of a battleship puzzle board file.There is a __optional__ section arguement "find_all_solutions"
Example of command line:<br />

__*./main.exe*&nbsp;&nbsp;*puzzle_sample.txt*__<br />
or:<br />
__*./main.exe*&nbsp;&nbsp;*puzzle_sample.txt*&nbsp;&nbsp;*find_all_solutions*__<br />

Example of puzzle board input file as below:

board 4 5<br />
rows 4 0 2 1<br />
cols 1 2 1 2 1<br />
cruiser<br />
destroyer<br />
submarine<br />
submarine<br />

It begins with the dimensions of the board, in this case 4 rows and 5 columns. Next, we give the number of cells in each row and each column that are occupied by a ship. The other cells in the row are open water. Then, we have a simple list of the ships that must be placed on the board. All ships are 1 cell wide, but each ship types has a different length(# of cells): submarine = 1, destroyer = 2, cruiser =3, battleship =4, carrier =5, cargo = 6, and tanker = 7

i.e.<br />
submarine: o<br />
destroyer: \<\> <br />
cruiser: \<X\> <br />
battleship: \<XX\> <br />
carrier: \<XXX\> <br />
cargo: \<XXXX\> <br />
tanker: \<XXXXX\> <br />

## Output
Place the ships on the board satisfying the counts for each row while no two ships may occupy adjacent cells (including the diagonal). The sample output below shows one of the possible solutions:<br />

Solution:<br />
cruiser   0 0 horizontal<br />
submarine 0 4<br />
submarine 2 1<br />
destroyer 2 3 vertical<br />
+-----+<br />
|\<X\>&nbsp;o|4<br />
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|0<br />
|&nbsp;&nbsp;o &nbsp;^&nbsp;|2<br />
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;v&nbsp;|1<br />
+-----+<br />
&nbsp;12121<br />

## Inputs with Constraints
for example:<br />

board 4 5<br />
rows 4 0 2 1<br />
cols 1 2 1 2 1<br />
cruiser<br />
destroyer<br />
submarine<br />
submarine<br />
constraint 0 2 \<<br />

The last line specified that there must be a horizontal ship starting from location (0,2)

## Inputs with Unknown Sums or Unknown Ship Types
for example:<br />

board 4 5
rows ? 0 2 1
cols 1 2 1 ? 1
cruiser
destroyer
submarine
unknown
constraint 0 2 <

The ? might range from 1 to 7



# What I Learned
* Recursion
