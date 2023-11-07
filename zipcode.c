/** Program to read in public zip code information and store as needed
 * for cs2301 assignment needs.
 *
 * @author Blake Nelson
 * @editor Krishna Garg
 */
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <string>
#include <forward_list>
#include "zipfed.hpp"

// size of fully qualified path/file name with null terminator
#define SZ_FILENAME (129)
#define CITY_NAME (500)
/** Function to read a line from the US FED and return pointer to the 
 * line of data represented as a C-String (i.e. NULL terminated string)
 *
 * @lineptr is a pointer to the dynamically allocated buffer to fill.
 *   If the pointer is NULL, a buffer will be allocated to hold the line.
 *   If the buffer is too small to holsd the line, realloc will be called
 *   to allocate a larger buffer. If the buffer is reallocated, n will
 *   be set to new size.
 * @n is a pointer to the size of the buffer pointer to be lineptr. If
 *   memory is reallocated for lineptr, the size of n will be incremented
 *   accordingly. The parameter n will always specify the size of buffer
 *   pointed at by lineptr.
 * @fptr is the FILE pointer to the open file to read.
 * @return number bytes (chars) read or -1 on error
 */
ssize_t readln_cs2303 (char **lineptr, size_t *n, FILE *stream) {
  ssize_t bytes_read = -1;  // -1 will signify error reading line
  const int delim = '\n';   // each line terminated with \n

  // Verify the file is open
  if (stream == NULL) {
    return (ssize_t) -1;
  }

  // Each line of the FED ZIP CSV file is terminated by \n\r, so we
  // read up to and including the \r for the entire line
  bytes_read = getdelim (lineptr, n, delim, stream);

  // Remove the \n at the end of the line by replacing with NULL terminator
  // Expect length of at least 2 for the \n\r at end of a line
  for (int i = bytes_read - 1; i >= 0; i--) {
    if ((*lineptr)[i] == '\n') {
      (*lineptr)[i] = '\0';
    }
  }
  //should return a linked list of zipfed
  return bytes_read;
}


/** Comparator function to compare two city names of two parsed lines
* @zip1 is a Zipfed pointer
* @zip2 is a Zipfed pointer
* @returns true if first inputs city name would come before the second inputs city name in an alphabetical list
*/
bool alphabet(Zipfed * zip1, Zipfed * zip2){
  	std::string city1 = zip1->get_city();
  	std::string city2 = zip2->get_city();
  	return city1 < city2;
}



/** main function to drive program. Input and output file names specified
 * on command line.
 *
 * usage:
 *    fed2cs2301 input_file output_file
 *
 * The input file must already exist. Output file will be created.
 *
 * Approach:
 * 1) Open both input and output files, or fail with error
 * 2) Read a line from input. This is a record of federal zip code data
 *    Note: the first line of the file is header info, not data, ignore it
 * 3) Parse the input line to populate a ZIP_FEDERAL structure
 * 4) Write the ZIP_FEDERAL structure to output file 1 record / line
 *    Note: no column header information will be written out, just  data fields
 * 5) Close output and input files and exit cleanly
 *
 * @param argc is the number of input strings - will be 2
 * @param argv is array of cmd line args -
 *        fed2cs2301 existing_input_file output_file_to_create
 * @return 0 for success. non-zero for error
 */
int main (int argc, char *argv[]) {
  char infile[SZ_FILENAME];  // Path/name of input file
  FILE *fdIn;
  
  ssize_t chars_read;        // number of chars read for line of input
  char *inbuf = NULL;        // input file has 1 record per line to buffer
  size_t sz_inbuf = 0;       // current size of the input record
  

  std::forward_list<Zipfed *> llist; // singly linked list of pointers to Zipfed instances
  
  /* Open input file specified on command line
   * Common sense error checking on cmd line parameters
  */
  if (argc != 2) {
    fprintf(stderr, "usage: %s input_file\n", argv[0]);
    return -1;
  }
  
  strncpy(infile, argv[1], SZ_FILENAME-1);

  /* Open input files - return error on failure
   * input for reading
   */
  
  fdIn = fopen(infile, "r");
  if (fdIn == NULL) {
    fprintf(stderr, "cannot open %s for input - exiting\n", infile);
    return -2;
  }
  
  /* Now a loop to read each line of the input file formatted and structured
   * as downloaded from US Government. Then, each line is loaded into our
   * zip code data structure before writing it out to the output file.
   *
   * Notes: 
   *  - each line of the input is a zip code record.
   *    the first line of the input is column hedaders, not data. skip it.
   *  - each record will be written to ouput on its own line
   *  - read until EOF on input or error either reading or writing
   */
  
  //chars_read = readln_cs2303 (&inbuf, &sz_inbuf, fdIn);
  // Now loop to process each line of zip code data in CSV format
  while ((chars_read = readln_cs2303 (&inbuf, &sz_inbuf, fdIn)) != EOF) {
    if (chars_read == 0) {  // not EOF, but nothing to process
      continue;
    }

    // write the buffer to stdout for debugging
    #ifdef DEBUG
      printf("inbuf: %s\n", inbuf);
    #endif
    // for each input line we create a new object instance of Zipfed, have the object
    // parse the input record and initialize itself with the parse result
    // Nonzero return indicates error parsing
    Zipfed *pZipfed = new Zipfed();
    if(pZipfed->parse_zip_cs2303(inbuf) != 0) {
      fprintf (stderr, "failed to process input record - exiting\n");
      fclose (fdIn);
      return -4;
    }
    
    // write the structure to stdout to verify processing during development
    /* printf ("DEBUG: "); */
    /* pZipfed->print(); */

    // Add the zip code record into the linked list
    llist.push_front(pZipfed);
  }
  
  //sort the list by alphabetical order based on their city names
  llist.sort(alphabet);
  
  
  //find the zip code of specific cities
  printf("Enter the names of the cities whose zip codes you want to find. Make sure your input is ALL CAPS...\n");
  std::string input;
  
  while(getline(std::cin, input)){		//keep going until prompted to stop
  	if(std::cin.eof()){			//if the input is ctrl-d or end of file
  		break;				//exit the loop
  	}
  	
  	for(std::forward_list<Zipfed *>::iterator it = llist.begin(); it != llist.end(); it++) {
      		Zipfed * pTmpZipfed = *it;
      		if(pTmpZipfed->get_city().compare(input) == 0){			//if the Zipfed field's city name you are currently at is what we're looking for
      			std::cout << pTmpZipfed->get_zip() << std::endl;	//output the zip for that city
      		}
 	}
  }
  
  /* If you want to see the alphabetically sorted list of Zipfed then you should take these comments out
  for(std::forward_list<Zipfed *>::iterator it = llist.begin(); it != llist.end(); it++) {
      		Zipfed * pTmpZipfed = *it;
      		pTmpZipfed->print();
  }
  std::cout << distance(llist.begin(), llist.end()) << std::endl;		//this line shows that there were no cut fields from the file.
  */
  
  
  /* Free memory before exiting
   * keep deleting the head until list is empty.
   * this code is responsible for freeing the zip code data struct
   * the linked list will free the node form the linked list
   */
  while (!llist.empty()) {
    llist.pop_front();
  }
  
  return 0;
}
