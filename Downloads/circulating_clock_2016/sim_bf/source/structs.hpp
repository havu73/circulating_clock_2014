#ifndef STRUCTS_HPP
#define STRUCTS_HPP
///INCLUDE HEADER FILES
#include <cstdlib> // Needed for cmath
#include <cstring> // Needed for strlen, memset, memcpy
#include <iostream> // Needed for cout
#include <bitset> // Needed for bitset
#include <fstream> // Needed for ofstream
#include <string>
#include <map> // Needed for map
#include "macros.hpp"
#include "memory.hpp"


//use standard name_space here
using namespace std;


struct terminal {
	// Escape codes
	const char* code_blue;
	const char* code_red;
	const char* code_reset;
	
	// Colors
	char * blue;
	char * red;
	char * reset;
	
	// Verbose stream
	streambuf* verbose_streambuf;
	ostream* verbose_stream;
	
	terminal () {
		this->code_blue = "\x1b[34m";
		this->code_red = "\x1b[31m";
		this->code_reset = "\x1b[0m";
		this->blue = new char [10];
		this->red = new char [10];
		this->reset = new char [10];
		strcpy(this->blue, this->code_blue);
		strcpy(this->red, this->code_red);
		strcpy(this->reset, this->code_reset);
		this->verbose_stream = new ostream(cout.rdbuf());
	}
	
	~terminal () {
		delete []this->blue;
		delete []this->red;
		delete []this->reset;
		delete verbose_stream;
	}
	
	// Indicates a task is done (pass terminal->verbose() into this function to print only with verbose mode on)
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
	
	// Indicates the program couldn't read from a pipe
	void failed_pipe_read () {
		cout << this->red << "Couldn't read from the pipe!" << this->reset << endl;
	}
	
	// Indicates the program couldn't write to a pipeinput_params
	void failed_pipe_write () {
		cout << this->red << "Couldn't write to the pipe!" << this->reset << endl;
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


struct input_params {
	// Input and output files' paths and names (either absolute or relative)
	char* params_file; // The path and name of the parameter sets file, default=input.params
	bool read_params; // Whether or not the read the parameter sets file, default=false
	char* ranges_file; // The path and name of the parameter ranges file, default=none
	bool read_ranges; // Whether or not to read the ranges file, default=false
	char* perturb_file; // The path and name of the perturbations file, default=none
	bool read_perturb; // Whether or not to read the perturbations file, default=false
	char* passed_file; // The path and name of the passed file, default=output.passed
	bool print_passed; // Whether or not to print the passed file, default=false
	char* dir_path; // The path of the output directory for concentrations or oscillation features, default=none
	bool print_cons; // Whether or not to print concentrations, default=false
	bool binary_cons_output; // Whether or not to print the binary or ASCII value of numbers in the concentrations output files
	char* features_file; // The path and file of the features file, default=none
	bool print_features; // Whether or not to print general features file, default=false
	
	// Sets
	int num_sets; // The number of parameter sets to simulate, default=1
	

	// Time
	int time_total; // The number of minutes to run each simulation for, default=1200
	double step_size; // The time step in minutes used for Euler's method, default=0.01
	int big_gran; // The granularity in time steps with which to store data, default=1
	int small_gran; // The granularit in time steps with which to simulate data, default=1
	
	//Seed
	int seed; // The seed, used for generating random numbers, default=generated from the time and process ID
	bool reset_seed; // Whether or not to reset the seed after each parameter set, default=false
	int pseed; // The seed, used for generating random parameter sets, default=generated from the time and process ID
	bool store_pseed; // Whether or not to store the parameter generation seed, pseed, default=false
	char* seed_file; // Default=none
	bool print_seeds; // Whether or not to print the seeds used to the seed file
	
	//mutant management
	bool short_circuit; // Whether or not to stop simulating a parameter set after a mutant fails
	int num_active_mutants;
	
	// Piping data
	bool piping; // Whether or not input and output should be piped (as opposed to written to disk), default=false
	int pipe_in; // The file descriptor to pipe data from, default=none (0)
	int pipe_out; // The file descriptor to pipe data into, default=none (0)
	
	// Output stream data
	bool verbose; // Whether or not the program is verbose, i.e. prints many messages about program and simulation state, default=false
	bool quiet; // Whether or not the program is quiet, i.e. redirects cout to /dev/null, default=false
	streambuf* cout_orig; // cout's original buffer to be restored at program completion
	ofstream* null_stream; // A stream to /dev/null that cout is redirected to if quiet mode is set

	
	input_params () {
		//input and output files
		this->params_file = new char[30];
		this->read_params = false;
		this->ranges_file = new char [30];
		this->read_ranges = false;
		this->perturb_file = new char [30];
		this->read_perturb = false;
		this->passed_file = new char [30];
		this->print_passed = false;
		this->dir_path = new char [30];
		this->print_cons = false;
		this->binary_cons_output = false;
		this->features_file = new char [30];
		this->print_features = false;
		
		//timing
		this->num_sets = 1;
		this->step_size = 0.01;
		this->time_total = 200;
		this->big_gran = 1;
		
		//seeds
		this->seed = 0;
		this->reset_seed = false;
		this->pseed = 0;
		this->store_pseed = false;
		this->seed_file = new char [30];
		this->print_seeds = false;
		
		//mutant management
		this->short_circuit = false;
		this->num_active_mutants = 1;
		
		//piping data
		this->piping = false;
		this->pipe_in = 0;
		this->pipe_out = 0;
		
		//output stream data
		this->verbose = false;
		this->quiet = false;
		this->cout_orig = NULL;
		this->null_stream = new ofstream("/dev/null");
		
	}
	
	~input_params () {
		delete [] this->params_file;
		delete [] this->passed_file;
		delete [] this->dir_path;
		delete [] this->features_file;
		delete [] this->seed_file;
		delete this->null_stream;
	}
};

struct rates {
	//rates bases and rates for mutants
	double* rates_base;  // Base rates taken from the current parameter set
	double* factors_perturb;  // Perturbations (as percentages with 1=100%) taken from the perturbations input file
	
	explicit rates () {
		this->rates_base = new double [NUM_RATES];
		this->factors_perturb = new double [NUM_RATES];
		memset(this->rates_base, 0, sizeof(double) * NUM_RATES);
		memset(this->factors_perturb, 0, sizeof(double) * NUM_RATES);
	}
	
	void clear (){
		delete [](this->rates_base);
		delete [](this->factors_perturb);
	}
	
	~rates () {
		delete [](this->rates_base);
		delete [](this->factors_perturb);
	}
};

struct con_levels {
	bool initialized; // Whether or not this struct's data have been initialized
	int num_con_levels; // The number of concentration levels this struct stores (not necessarily the total number of concentration levels)
	int time_steps; // The number of time steps this struct stores concentrations for
	double ** data; // A two dimensional array that stores [concentration levels][time steps] in that order
	
	con_levels () {
		this->initialized = false;
	}
	
	con_levels (int num_con_levels, int time_steps) {
		this->initialized = false;
		initialize(num_con_levels, time_steps);
	}
	
	// Initializes the struct with the given number of concentration levels, time steps, and cells
	void initialize (int num_con_levels, int time_steps) {
		// If the current size is big enough to fit the new size then reuse the memory, otherwise allocate the required memory
		if (this->initialized && this->num_con_levels >= num_con_levels && this->time_steps >= time_steps) {
			this->reset();
		} else {
			this->num_con_levels = num_con_levels;
			this->time_steps = time_steps;
			
			this->data = new double*[num_con_levels];
			for (int i = 0; i < num_con_levels; i++) {
				this->data[i] = new double[time_steps];
				for (int j = 0; j < time_steps; j++) {
					this->data[i][j] = 0;
				}
			}
			
			this->initialized = true;
		}
	}
	
	// Sets every value in the struct to 0 but does not free any memory
	void reset () {
		if (this->initialized) {
			for (int i = 0; i < this->num_con_levels; i++) {
				for (int j = 0; j < this->time_steps; j++) {
					this->data[i][j] = 0;
				}
			}
		}
	}
	
	// Frees the memory used by the struct
	void clear () {
		if (this->initialized) {
			for (int i = 0; i < this->num_con_levels; i++) {
				delete[] this->data[i];
			}
			delete[] this->data;
			this->initialized = false;
		}
	}
	
	~con_levels () {
		this->clear();
	}
};

struct growin_array {
	int* array; // The array of integers
	int size; // The size of the array

	growin_array() {}

	void initialize(int size) {
		array = new int[size];
		this->size = size;
	}
	
	explicit growin_array (int size) {
		this->initialize(size);
	}

	int& operator[] (int index) {
		if (index >= this->size) {
			int* new_array = new int[2 * this->size];
			for (int i = 0; i < this->size; i++) {
				new_array[i] = array[i];
			}
			delete[] array;
			this->size = 2 * this->size;
			array = new_array;
		}
		return array[index];
	}
	
	void reset(int new_size) {
		this->size = new_size;
	}
	
	int get_size() {
		return this->size;
	}
	
	~growin_array () {
		delete[] array;
	}
};


struct sim_data {
	// Times and timing
	double step_size; // The step size in minutes
	int time_total; // The number of minutes to run for
	int steps_total; // The number of time steps to simulate (total time / step size)
	
	// Granularities
	int big_gran; // The granularity in time steps with which to analyze and store data
	int small_gran; // The granularit in time steps with which to simulate data
	
	// Cutoff values
	int max_delay_size; // The maximum number of time steps any delay in the current parameter set takes plus 1 (so that baby_cl and each mutant know how many minutes to store)
	
	// Mutants and condition scores
	int num_active_mutants; // The number of mutants to simulate for each parameter set

	explicit sim_data (input_params& ip) {
		//timing
		this->step_size = ip.step_size;
		this->time_total = ip.time_total;
		this->steps_total = ip.time_total / ip.step_size;
		
		//granularities
		this->big_gran = ip.big_gran;
		this->small_gran = ip.small_gran;
		
		//cutoff values
		this->max_delay_size = 0;
		
		//mutants and conditions scores
		this->num_active_mutants = ip.num_active_mutants;
	}
};

struct features {
	double period[NUM_FEATURE_CONS];
	double amplitude[NUM_FEATURE_CONS];
	
	features () {
		memset(period, 0, sizeof(period));
		memset(amplitude, 0, sizeof(amplitude));
	}
	
	void reset () {
		memset(period, 0, sizeof(period));
		memset(amplitude, 0, sizeof(amplitude));
	}
};


struct mutant_data {
	int index; // The index, i.e. how many mutants run before this one
	char* print_name; // The mutant's name for printing output
	char* dir_name; // The mutant's name for making its directory
	int num_knockouts; // The number of knockouts required to achieve this mutant
	int knockouts[2]; // The indices of the concentrations to knockout (num_knockouts determines how many indices to knockout)
	features feat; // The oscillation features this mutant produced when run
	int print_con; // The index of the concentration that should be printed (usually mh1)
	
	mutant_data () {
		this->index = 0;
		this->print_name = NULL;
		this->dir_name = NULL;
		this->num_knockouts = 0;
	}
	
	~mutant_data () {
		free(this->print_name);
		free(this->dir_name);
	}
};

struct input_data {
	char* filename; // The path and name of the file
	char* buffer; // A buffer to store the file's contents
	int size; // The number of bytes the file's contents take up
	int index; // The current index to access the buffer from
	
	explicit input_data (char* filename) {
		this->filename = filename;
		this->buffer = NULL;
		this->size = 0;
		this->index = 0;
	}
	void clear(){
		free(this->buffer);
	}
	
	~input_data () {
		free(this->buffer);
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
			this->data[i] = new double[NUM_RATES];
			memset(this->data[i], 0, sizeof(double) * NUM_RATES);
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
