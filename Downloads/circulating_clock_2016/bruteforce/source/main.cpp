
#include "main.hpp" // Function declarations

#include "init.hpp"
#include "macros.hpp"
#include "io.hpp"
using namespace std;
input_params ip;
extern terminal* term; // Declared in init.cpp

/* main is called when the program is run and performs all program functionality
	parameters:
		argc: the number of command-line arguments
		argv: the array of command-line arguments
	returns: 0 on success, a positive integer on failure
	notes:
		Main should only delegate functionality; let the functions it calls handle specific tasks. This keeps the function looking clean and helps maintain the program structure.
	todo:
*/
int main (int argc, char** argv) {

	// Initialize the program's terminal functionality and input parameters
	init_terminal();
	accept_input_params(argc, argv, ip);
	init_verbosity(ip);
	init_sim_args(ip);
	cout << ip.sim_args[1] << endl;
	
	//Generate random parameter sets
	parameters pr (ip.num_sets);
	generate_parameters(pr);
	cout << "in Main: first parameters " << pr.data[0][0] << endl;
	
	//Create passed file stream to write
	create_good_set_file();
	
	//Call to simulation
	simulate_set(pr);
	// Free used memory, etc.
	#if defined(MEMTRACK)
		print_heap_usage();
	#endif
	close_if_open(*ip.good_set_stream);
	free_terminal();
	reset_cout(ip);
	return 0;
}

/* usage prints the usage information and, optionally, an error message and then exits
	parameters:
		message: an error message to print before the usage information (set message to NULL or "\0" to not print any error)
	returns: nothing
	notes:
		This function exits after printing the usage information.
		Note that accept_input_params in init.cpp handles actual command-line input and that this information should be updated according to that function.
	todo:
		TODO somehow free memory even with the abrupt exit
*/
void usage (const char* message) {
	cout << endl;
	bool error = message != NULL && message[0] != '\0';
	if (error) {
		cout << term->red << message << term->reset << endl << endl;
	}
	cout << "Usage: [-option [value]]. . . [--option [value]]. . ." << endl;
	cout << "-f, --simulation         [filename]   : the relative filename of the simulation executable, default=../simulation/simulation" << endl;
	cout << "-s, --seed               [int]        : the seed used in the evolutionary strategy (not simulations), min=1, default=time" << endl;
	cout << "-n, --num_sets           [int]        : the number of parameters sets to generate and run on simulation" << endl;
	cout << "-o, --print-good-sets    [int]        : the name of a file containing sets of parameters that do not produce negative or NaN" << endl;
	cout << "-a, --arguments          [N/A]        : every argument following this will be sent to the simulation" << endl;
	cout << "-c, --no-color           [N/A]        : disable coloring the terminal output, default=unused" << endl;
	cout << "-v, --verbose            [N/A]        : print detailed messages about the program state" << endl;
	cout << "-q, --quiet              [N/A]        : hide the terminal output, default=unused" << endl;
	cout << "-l, --licensing          [N/A]        : view licensing information (no simulations will be run)" << endl;
	cout << "-h, --help               [N/A]        : view usage information (i.e. this)" << endl;
	cout << endl << term->blue << "Example: ./sres-sampler " << term->reset << endl << endl;
	if (error) {
		exit(EXIT_INPUT_ERROR);
	} else {
		exit(EXIT_SUCCESS);
	}
}

/* licensing prints the program's copyright and licensing information and then exits
	parameters:
	returns: nothing
	notes:
	todo:
*/
void licensing () {
	cout << endl;
	cout << "Stochastically ranked evolutionary strategy sampler for zebrafish segmentation" << endl;
	cout << "Copyright (C) 2013 Ahmet Ay (aay@colgate.edu), Jack Holland (jholland@colgate.edu), Adriana Sperlea (asperlea@colgate.edu), Sebastian Sangervasi (ssangervasi@colgate.edu)" << endl;
	cout << "This program comes with ABSOLUTELY NO WARRANTY" << endl;
	cout << "This is free software, and you are welcome to redistribute it under certain conditions;" << endl;
	cout << "You can use this code and modify it as you wish under the condition that you refer to the article: \"Short-lived Her proteins drive robust synchronized oscillations in the zebrafish segmentation clock\" (Development 2013 140:3244-3253; doi:10.1242/dev.093278)" << endl;
	cout << endl;
	exit(EXIT_SUCCESS);
}

