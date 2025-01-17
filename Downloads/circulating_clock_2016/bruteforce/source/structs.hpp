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
structs.hpp contains every struct used in the program.
*/

#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <cstring> // Needed for strlen, strcpy, strcmp
#include <iostream> // Needed for cout
#include <fstream> // Needed for ofstream


#include "macros.hpp"
#include "memory.hpp"

using namespace std;

char* copy_str(const char*); // init.h cannot be included because it requires this file, structs.h, creating a cyclical dependency; therefore, copy_str, declared in init.h, must be declared in this file as well in order to use it here

/* terminal contains colors, streams, and common messages for terminal output
	notes:
		There should be only one instance of terminal at any time.
	todo:
*/
struct terminal {
	// Escape codes
	const char* code_blue;
	const char* code_red;
	const char* code_yellow;
	const char* code_reset;
	
	// Colors
	char* blue;
	char* red;
	char* yellow;
	char* reset;
	
	// Verbose stream
	streambuf* verbose_streambuf;
	ostream* verbose_stream;
	
	terminal () {
		this->code_blue = "\x1b[34m";
		this->code_red = "\x1b[31m";
		this->code_yellow = "\x1b[33m";
		this->code_reset = "\x1b[0m";
		this->blue = copy_str(this->code_blue);
		this->red = copy_str(this->code_red);
		this->yellow = copy_str(this->code_yellow);
		this->reset = copy_str(this->code_reset);
		this->verbose_stream = new ostream(cout.rdbuf());
	}
	
	~terminal () {
		mfree(this->blue);
		mfree(this->red);
		mfree(this->yellow);
		mfree(this->reset);
		delete verbose_stream;
	}
	
	// Prints two spaces and then the given MPI rank in parentheses (pass terminal->verbose() into this function to print only with verbose mode on)
	void rank (int rank, ostream& stream) {
		stream << this->yellow << "(" << rank << ") " << this->reset;
	}
	
	// Prints two spaces and then the given MPI rank in parentheses
	void rank (int rank) {
		this->rank(rank, cout);
	}
	
	// Indicates a process is done (pass terminal->verbose() into this function to print only with verbose mode on)
	void done (ostream& stream) {
		stream << this->blue << "Done" << this->reset << endl;
	}
	
	// Indicates a task is done
	void done () {
		done(cout);
	}
	
	// Indicates the program is out of memory
	void no_memory () {
		cout << this->red << "Not enough memory!" << this->reset << endl;
	}
	
	// Indicates the program couldn't remove the given file
	void failed_file_remove (const char* filename) {
		cout << this->red << "Couldn't remove '" << filename << "'! Make sure files are not moved or removed during the simulations." << this->reset << endl;
	}
	
	// Indicates the program couldn't create a pipe
	void failed_pipe_create () {
		cout << this->red << "Couldn't create a pipe!" << this->reset << endl;
	}
	
	// Indicates the program couldn't read from a pipe
	void failed_pipe_read () {
		cout << this->red << "Couldn't read from the pipe!" << this->reset << endl;
	}
	
	// Indicates the program couldn't read from a pipe
	void failed_pipe_write () {
		cout << this->red << "Couldn't write to the pipe!" << this->reset << endl;
	}
	
	// Indicates the program couldn't fork a child process
	void failed_fork () {
		cout << this->red << "Couldn't fork a child process!" << this->reset << endl;
	}
	
	// Indicates the program couldn't execute an external program
	void failed_exec () {
		cout << this->red << "Couldn't execute an external program!" << this->reset << endl;
	}
	
	// Indicates a child process encountered an error and did not exit properly
	void failed_child () {
		cout << this->red << "A child process encountered an error!" << this->reset << endl;
	}
	
	// Returns the verbose stream that prints only when verbose mode is on
	ostream& verbose () {
		return *(this->verbose_stream);
	}
	
	// Sets the stream buffer for verbose mode
	void set_verbose_streambuf (streambuf* sb) {
		this->verbose_stream->rdbuf(sb);
		this->verbose_streambuf = this->verbose_stream->rdbuf();
	}
};

/* input_params contains all of the program's input parameters (i.e. the given command-line arguments) as well as data associated with them
	notes:
		There should be only one instance of input_params at any time.
		Variables should be initialized to the values indicated in the usage information.
	todo:
*/
struct input_params {
	
	// Input and output files' paths and names (either absolute or relative
	char* sim_file; // The relative filename of the simulation executable
	
	char* good_sets_file;
	bool print_good_sets;
	ofstream* good_set_stream;
	int num_sets;
	// Simulation parameters
	char** sim_args; // Arguments to be passed to the simulation
	int num_sim_args; // The number of arguments to be passed to the simulation
	
	// Output stream data
	bool verbose; // Whether or not the program is verbose, i.e. prints many messages about program and simulation state, default=false
	bool quiet; // Whether or not the program is quiet, i.e. redirects cout to /dev/null, default=false
	streambuf* cout_orig; // cout's original buffer to be restored at program completion
	ofstream* null_stream; // A stream to /dev/null that cout is redirected to if quiet mode is set
	int seed; // The seed used in the evolutionary strategy, default=current UNIX time
	
	input_params () {
		this->sim_file = copy_str("../simulation/simulation");
		this->seed = time(0);
		this->print_good_sets = true;
		this->sim_args = NULL;
		this->num_sim_args = 0;
		this->verbose = false;
		this->quiet = false;
		this->cout_orig = NULL;
		this->null_stream = new ofstream("/dev/null");
		this->num_sets = 10000;
		this->good_sets_file = copy_str("set.txt");
		this->good_set_stream = new ofstream();
	}
	
	~input_params () {
		mfree(this->sim_file);
		if (this->good_sets_file != NULL){
			mfree(this->good_sets_file);
		}
		if (this->sim_args != NULL) {
			for (int i = 0; i < this->num_sim_args; i++) {
				mfree(this->sim_args[i]);
			}
			mfree(sim_args);
		}
		delete this->null_stream;
	}
};

struct parameters {
	//rates bases and rates for mutants
	double** data;  // Base rates taken from the current parameter set
	int num_sets; 
	explicit parameters (int num_sets) {
		this->num_sets = num_sets;
		this->data = new double* [num_sets];
		for (int i = 0; i < num_sets; i ++){
			this->data[i] = new double[DIM];
			memset(this->data[i], 0, sizeof(double) * DIM);
		}
	}
	
	void clear (){
		for (int i = 0; i < this->num_sets; i++) {
				delete[] this->data[i];
		}
		delete[] this->data;
	}
	
	~parameters () {
		this->clear();
	}
};



#endif

