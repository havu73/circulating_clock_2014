/*
Stochastically ranked evolutionary strategy sampler for zebrafish segmentation
Copyright (C) 2013 Ahmet Ay, Jack Holland, Adriana Sperlea, Sebastian Sangervasi

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
init.cpp contains initialization functions used before any simulations start.
*/

#include <cmath> // Needed for log10
#include <stdio.h>

#include "init.hpp" // Function declarations

#include "io.hpp"
#include "macros.hpp"
#include "main.hpp"

using namespace std; 
extern input_params ip; // Declared in main.cpp
terminal* term; // The global terminal struct

/* copy_str copies the given string, allocating enough memory for the new string
	parameters:
		str: the string to copy
	returns: a pointer to the new string
	notes:
	todo:
*/
char* copy_str (const char* str) { 
	if (str == NULL) {
		return NULL;
	} else {
		char* newstr = (char*)mallocate(sizeof(char) * (strlen(str) + 1));
		return strcpy(newstr, str);
	}
}

/* init_terminal creates and initializes a new terminal struct
	parameters:
	returns: nothing
	notes:
	todo:
*/
void init_terminal () {
	if (term != NULL) {
		delete term;
	}
	term = new terminal();
}

/* free_terminal frees the terminal from memory and resets the terminal text color to its default value
	parameters:
	returns: nothing
	notes:
	todo:
*/
void free_terminal () {
	cout << term->reset;
	delete term;
}

/* accept_input_params fills the given input_params with values from the given command-line arguments
	parameters:
		num_args: the number of command-line arguments (i.e. argc)
		args: the array of command-line arguments (i.e. argv)
		ip: the program's input parameters
	returns: nothing
	notes:
		Ensure the usage function's message in main.cpp matches the arguments this function accepts whenever editing or adding command-line argument acceptance.
	todo:
*/
void accept_input_params (int num_args, char** args, input_params& ip) {
	if (num_args > 1) { // If arguments were given (the 0th argument is the program name)
		for (int i = 1; i < num_args; i += 2) { // Iterate through each argument pair (if an argument does not have an accompanying value, i-- should be called when found)
			char* option = args[i];
			char* value;
			if (i < num_args - 1) {
				value = args[i + 1];
			} else {
				value = NULL;
			}
			// Accept command-line arguments in both short and long form
			if (option_set(option, "-f", "--simulation")) {
				ensure_nonempty(option, value);
				store_filename(&(ip.sim_file), value);
			}else if (option_set(option, "-s", "--seed")) {
				ensure_nonempty(option, value);
				ip.seed = atoi(value);
				if (ip.seed <= 0) {
					usage("The seed to generate random numbers must be a positive integer. Set -s or --seed to at least 1.");
				}
			}else if (option_set(option, "-n", "--num_sets")) {
				ensure_nonempty(option, value);
				ip.num_sets = atoi(value);
				if (ip.seed <= 0) {
					usage("The number of sets to simulate need to be at least 1. ");
				}
			}else if (option_set(option, "-o", "--print-good-sets")) {
                ensure_nonempty(option, value);
                store_filename(&(ip.good_sets_file), value);
                ip.print_good_sets = true;
            } else if (option_set(option, "-a", "--arguments")) {
				ensure_nonempty(option, value);
				++i;
				ip.num_sim_args = num_args - i + NUM_IMPLICIT_SIM_ARGS;
				ip.sim_args = (char**)mallocate(sizeof(char*) * (ip.num_sim_args));
				for (int j = 1; j < ip.num_sim_args - (NUM_IMPLICIT_SIM_ARGS - 1); j++) {
					char* arg = args[i + j - 1];
					ip.sim_args[j] = (char*)mallocate(sizeof(char) * (strlen(arg) + 1));
					sprintf(ip.sim_args[j], "%s", arg);
				}
				i = num_args;
			} else if (option_set(option, "-c", "--no-color")) {
				mfree(term->blue);
				mfree(term->red);
				mfree(term->yellow);
				mfree(term->reset);
				term->blue = copy_str("");
				term->red = copy_str("");
				term->yellow = copy_str("");
				term->reset = copy_str("");
				i--;
			} else if (option_set(option, "-v", "--verbose")) {
				if (!ip.verbose) {
					ip.verbose = true;
				}
				i--;
			} else if (option_set(option, "-q", "--quiet")) {
				if (!ip.quiet) {
					ip.quiet = true;
					ip.cout_orig = cout.rdbuf();
					cout.rdbuf(ip.null_stream->rdbuf());
					term->set_verbose_streambuf(ip.null_stream->rdbuf());
				}
				i--;
			} else if (option_set(option, "-h", "--help")) {
				usage("");
				i--;
			} else if (option_set(option, "-l", "--licensing")) { 
				licensing();
				i--;
			} else {
				const char* message_0 = "'";
				const char* message_1 = "' is not a valid option! Please check that every argument matches one available in the following usage information.";
				char* message = (char*)mallocate(sizeof(char) * (strlen(message_0) + strlen(option) + strlen(message_1) + 1));
				sprintf(message, "%s%s%s", message_0, option, message_1);
				usage(message);
			}
		}
	}
}

/* option_set checks if the given string matches either given version (short or long) of an option
	parameters:
		option: the string to check
		short_name: the short version of the option
		long_name: the long version of the option
	returns: true if the string matches a version, false otherwise
	notes:
	todo:
*/
inline bool option_set (const char* option, const char* short_name, const char* long_name) {
	return strcmp(option, short_name) == 0 || strcmp(option, long_name) == 0;
}

/* ensure_nonempty ensures that an option that should have an associated value has one or exits with an error
	parameters:
		option: the option to check
		arg: the value to check for
	returns: nothing
	notes:
	todo:
*/
void ensure_nonempty (const char* option, const char* arg) {
	if (arg == NULL) {
		char* message = (char*)mallocate(strlen("Missing the argument for the '' option.") + strlen(option) + 1);
		sprintf(message, "Missing the argument for the '%s' option.", option);
		usage(message);
	}
}


/* init_verbosity sets the verbose stream to /dev/null if verbose mode is not enabled
	parameters:
		ip: the program's input parameters
	returns: nothing
	notes:
	todo:
*/
void init_verbosity (input_params& ip) {
	if (!ip.verbose) {
		term->set_verbose_streambuf(ip.null_stream->rdbuf());
	}
}

/* init_sim_args initializes the arguments to be passed into every simulation
	parameters:
		ip: the program's input parameters
	returns: nothing
	notes:
	todo:
*/
void init_sim_args (input_params& ip) {
	if (ip.num_sim_args == 0) { // If the arguments were not initialized in accept_input_params (i.e. the user did not specify simulation arguments with -a or --arguments)
		ip.num_sim_args = NUM_IMPLICIT_SIM_ARGS; // "simulation --pipe-in x --pipe-out y" takes 5 terms and the final NULL element makes the sum 6
		ip.sim_args = (char**)mallocate(sizeof(char*) * NUM_IMPLICIT_SIM_ARGS);
	}
	
	// Initialize the implicit arguments
	ip.sim_args[0] = copy_str("simulation");
	ip.sim_args[ip.num_sim_args - 5] = copy_str("--pipe-in");
	ip.sim_args[ip.num_sim_args - 4] = copy_str("0");
	ip.sim_args[ip.num_sim_args - 3] = copy_str("--pipe-out");
	ip.sim_args[ip.num_sim_args - 2] = copy_str("0");
	ip.sim_args[ip.num_sim_args - 1] = NULL;
}

/* copy_args copies the given array of arguments
	parameters:
		args: the array of arguments to copy
		num_args: the number of elements in the array of arguments
	returns: the new array of arguments
	notes:
	todo:
*/
char** copy_args (char** args, int num_args) {
	char** new_args = (char**)mallocate(sizeof(char*) * num_args);
	for (int i = 0; i < num_args; i++) {
		new_args[i] = copy_str(args[i]);
	}
	return new_args;
}



/* store_pipe stores the given pipe file descriptor into the given index in the array of arguments
	parameters:
		args: the array of arguments
		index: the index in the array to place the pipe file descriptor
		pipe: the file descriptor of the pipe
	returns: nothing
	notes:
	todo:
*/
void store_pipe (char** args, int index, int pipe) {
	mfree(args[index]);
	int int_size = log10(pipe > 0 ? pipe : 1) + 1; // How many bytes the ASCII representation of pipe takes
	args[index] = (char*)mallocate(sizeof(char) * (int_size + 1));
	sprintf(args[index], "%d", pipe);
}

/* reset_cout resets the cout buffer to its original stream if quiet mode was on and cout was therefore redirected to /dev/null
	parameters:
		ip: the program's input parameters
	returns: nothing
	notes:
	todo:
*/
void reset_cout (input_params& ip) {
	if (ip.quiet) {
		cout.rdbuf(ip.cout_orig);
	}
}

void generate_parameters(parameters& pr){
	for (int i = 0; i < pr.num_sets; i ++){
		for (int j = 0; j < DIM; j ++){
			pr.data[i][j] = random_parameters(0, 1000);
		}
	}
}

double random_parameters(int min, int max){
	double f = (double) rand()/(double)(RAND_MAX);
	return min + f * (max - min);
}

void create_good_set_file(){
	if (ip.print_good_sets){
		open_file(ip.good_set_stream, ip.good_sets_file, false);
	}
}
