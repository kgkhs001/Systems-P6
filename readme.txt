Author: Krishna Garg

Program -> fed2cs2303

Description:

Usage: ./fed2cs2303 input_file.csv output_file.csv (or txt not exclusive)

This program takes in a list of zipcode data from a database or csv file, parses the data to only print certain columns and only print rows for certain states (in this case Massachusetts), and prints 
the data either to the 
terminal or the inputted file at output file (depends on the version of print() that we use).

Linking:
fed2cs2303: fed2cs2303.o zipfed.o
	g++ -g fed2cs2303.o zipfed.o -o fed2cs2303
	
Compiling:
fed2cs2303.o: fed2cs2303.c zipfed.hpp
	g++ -g -c fed2cs2303.c




Program -> zipcode

Usage: ./zipcode input_file.csv

This program takes in a database of parsed data, saves each row into a linked list, and then allows you to search for the zip code(s) for a specific city in the list. If there are multiple zip codes 
for a given city this program will output all of the zip codes associated with that city. When you run the program you will be prompted for a city. When you type the city you will get all the zip codes 
associated with that city. 

Linking: 
zipcode: zipcode.o zipfed.o
	g++ -g zipcode.o zipfed.o -o zipcode
	
Compiling:
zipcode.o: zipcode.c zipfed.hpp
	g++ -g -c zipcode.c
	

Other Notes:
Currently there is no way to prove that the linked list is alphabetically sorted. However I have built in a method to prove this. If you look in the zipcode.c file there is a commented out 
loop that you can uncomment and then run the program. This will print all of the list onto the command line. 
