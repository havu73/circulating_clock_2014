#include <unistd.h> // Needed for getpid
#include <cmath>
#include <string>

#include "init.hpp" // Function declarations
#include "io.hpp"
#include "main.hpp"

using namespace std;
terminal * term = NULL;
void init_terminal () {
	term = new terminal();
}

inline bool option_set (const char* option, const char* short_name, const char* long_name) {
	return strcmp(option, short_name) == 0 || strcmp(option, long_name) == 0;
}

void ensure_nonempty (const char* option, const char* arg) {
	if (arg == NULL) {
		char* message = (char*)mallocate(strlen("Missing the argument for the '' option.") + strlen(option) + 1);
		sprintf(message, "Missing the argument for the '%s' option.", option);
		usage(message);
	}
}

void accept_input_params (int num_args, char** args, input_params& ip) {
	string o;
	string v;
		for (int i = 1; i < num_args; i += 2){
			//Process option and values 
			o = args[i];
			if (i < num_args - 1) {
				v = args[i + 1];
			} else {
				v = "";
			}
			
			char * option = new char [o.length() +1];
			char * value = new char [v.length() +1];
			strcpy(option, o.c_str());
			if (v.length() != 0){
				strcpy(value, v.c_str());
			}else {
				value = NULL; 
			}
			
			// Accept command-line arguments in both short and long form
			if (option_set(option, "-o", "--print-passed")) {
				ensure_nonempty(option, value);
				store_filename(&(ip.passed_file), value);
				ip.print_passed = true;
			}else if (option_set(option, "-D", "--directory-path")) {
				ensure_nonempty(option, value);
				store_filename(&(ip.dir_path), value);
			}else if (option_set(option, "-t", "--print-cons")) {
				ip.print_cons = true;
				i--;
			}else if (option_set(option, "-B", "--binary-cons-output")) {
				ip.binary_cons_output = true;
				i--;
			}else if (option_set(option, "-g", "--big-granularity")) {
				ensure_nonempty(option, value);
				ip.big_gran = atoi(value);
				if (ip.big_gran < 1) {
					usage("The big granularity to simulate with must be a positive number of time steps. Set -b or --big-granularity to at least 1.");
				}
			}else if (option_set(option, "-S", "--step-size")) {
				ensure_nonempty(option, value);
				ip.step_size = atof(value);
				if (ip.step_size <= 0) {
					usage("The time step for Euler's method must be a positive real number. Set -S or --time-step to be greater than 0.");
				}
			}else if (option_set(option, "-m", "--minutes")) {
				ensure_nonempty(option, value);
				ip.time_total = atoi(value);
				if (ip.time_total < 1) {
					usage("The simulation must be run for positive number of minutes. Set -m or --minutes to at least 1.");
				}
			}else if (option_set(option, "-p", "--parameter-sets")) {
				ensure_nonempty(option, value);
				ip.num_sets = atoi(value);
				if (ip.num_sets < 1) {
					usage("The number of parameters to run must be a positive integer. Set -p or --parameter-sets to at least 1.");
				}
			}else if (option_set(option, "-s", "--seed")) {
				ensure_nonempty(option, value);
				ip.seed = atoi(value);
				if (ip.seed <= 0) {
					usage("The seed to generate random numbers must be a positive integer. Set -s or --seed to at least 1.");
				}
				ip.reset_seed = true;
			}else if (option_set(option, "-X", "--reset-seed")) {
				ip.reset_seed = true;
				i--;
			}else if (option_set(option, "-d", "--parameters-seed")) {
				ensure_nonempty(option, value);
				ip.pseed = atoi(value);
				ip.store_pseed = true;
				if (ip.pseed <= 0) {
					usage("The seed to generate random parameters must be a positive integer. Set -d or --parameters-seed to at least 1.");
				}
			}else if (option_set(option, "-e", "--print-seeds")) {
				ensure_nonempty(option, value);
				store_filename(&(ip.seed_file), value);
				ip.print_seeds = true;
			}else if (option_set(option, "-C", "--short-circuit")) {
				ip.short_circuit = true;
				i--;
			} else if (option_set(option, "-I", "--pipe-in")) {
				ensure_nonempty(option, value);
				ip.piping = true;
				ip.pipe_in = atoi(value);
				if (ip.pipe_in <= 0) {
					usage("The file descriptor to pipe data from must be a positive integer. Set -I or --pipe-in to be at least 1.");
				}
			} else if (option_set(option, "-O", "--pipe-out")) {
				ensure_nonempty(option, value);
				ip.piping = true;
				ip.pipe_out = atoi(value);
				if (ip.pipe_out <= 0) {
					usage("The file descriptor to pipe data into must be a positive integer. Set -O or --pipe-out to be at least 1.");
				}
			} else if (option_set(option, "-c", "--no-color")) {
				free(term->blue);
				free(term->red);
				free(term->reset);
				strcpy(term->blue, "");
				strcpy(term->red, "");
				strcpy(term->reset, "");
				i--;
			} else if (option_set(option, "-v", "--verbose")) {
				if (!ip.verbose) {
					ip.verbose = true;
				}
				i--;
			}else if (option_set(option, "-u" , "--uper-bound")){
				ensure_nonempty(option, value);	
				ip.ub = atoi(value);
			}else if (option_set(option, "-b" , "--lower-bound")){
				ensure_nonempty(option, value);	
				ip.lb = atoi(value);
			}else if (option_set(option, "-q", "--quiet")) {
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
			} else { // Do not ignore invalid arguments; exit with an error to let the user know this argument is problematic
				const char* message_0 = "'";
				const char* message_1 = "' is not a valid option! Please check that every argument matches one available in the following usage information.";
				char* message = (char*)mallocate(sizeof(char) * (strlen(message_0) + strlen(option) + strlen(message_1) + 1));
				sprintf(message, "%s%s%s", message_0, option, message_1);
				usage(message);
			}
			delete [] option;
			delete [] value;
		}
}

void init_verbosity (input_params& ip) {
	if (!ip.verbose) {
		term->set_verbose_streambuf(ip.null_stream->rdbuf());
	}
}
/*
void check_input_params (input_params& ip){
	if (ip.piping && (ip.pipe_in == 0 || ip.pipe_out == 0)) {
		usage("If one end of a pipe is specified, the other must be as well. Set the file descriptors for both the pipe in (-I or --pipe-in) and the pipe out (-O or --pipe-out).");
	}
	if (!(ip.piping || ip.read_params || ip.read_ranges)) {
		usage("Parameter must be piped in via -I or --pipe-in, read from a file via -i or --params-file, or generated from a ranges file and number of sets via -R or --ranges-file and -p or --parameter-sets, respectively.");
	}
	if (!ip.dir_path && ip.print_cons) {
		usage("An output directory must be specified to print concentrations. ");
	}
	if (ip.reset_seed) {
		init_seeds(ip, 0, false, false);
	}
}
*/
void init_seeds (input_params& ip, int set_num, bool append, bool indent_message) {
	// If the file is being created then generate the parameter set seed
	if (!append && ip.store_pseed) {
		ip.pseed = generate_seed();
		if (indent_message) {
			term->verbose() << "  ";
		}
		term->verbose() << term->blue << "Using seed " << term->reset << ip.pseed << " for parameter set generation" << endl;
	}
	
	// If the seed was not specified by command-line argument or a new one should be generated, generate one
	if (ip.seed == 0 || append) {
		ip.seed = generate_seed();
		if (indent_message) {
			term->verbose() << "  ";
		}
		term->verbose() << term->blue << "Using seed " << term->reset << ip.seed << " for each run" << endl;
	}
	
	// If the user specified printing seeds to a file then print them to the specified file
	if (ip.print_seeds) {
		ofstream seed_file;
		term->verbose() << "  ";
		open_file(&seed_file, ip.seed_file, append);
		if (!append && ip.store_pseed) {
			seed_file << "pseed: " << ip.pseed << endl;
		}
		seed_file << "seed " << set_num << ": " << ip.seed << endl;
	}
}
/*
void read_sim_params (input_params& ip, input_data& params_data, parameters& pr, input_data& ranges_data) {
	cout << term->blue;
	if (ip.piping) { // If the user specified piping
		cout << "Reading pipe " << term->reset << "(file descriptor " << ip.pipe_in << ") . . . "<< endl;
		read_pipe(pr, ip);
		term->done();
	} else if (ip.read_params) { // If the user specified a parameter sets input file
		read_file(&params_data);
		for (int i = 0; i < ip.num_sets; i++) {
			if (params_data.index < params_data.size) { // Parse only as many lines as specified, even if the file is longer
				memset(pr.data[i], 0, sizeof(double) * NUM_RATES);
				if(!parse_param_line(pr.data[i], params_data.buffer, params_data.index)) { // Parse each line until the file is empty or the required number of sets have been found
					ip.num_sets = i;
				}
			} else {
				ip.num_sets = i;
			}
		}
	} else if (ip.read_ranges) { // If the user specified a ranges input file to generate random numbers from
		cout << "Generating " << term->reset << ip.num_sets << " random parameter sets according to the ranges in " << ranges_data.filename << " . . ." << endl;
		cout << "  ";
		read_file(&ranges_data);
		pair <double, double> ranges[NUM_RATES];
		parse_ranges_file(ranges, ranges_data.buffer);
		srand(ip.pseed);
		for (int i = 0; i < ip.num_sets; i++) {
			for (int j = 0; j < NUM_RATES; j++) {
				pr.data[i][j] = random_double(ranges[j]);
			}
		}
		term->done();
	} else {
		usage("Parameter must be piped in via -I or --pipe-in, read from a file via -i or --params-file, or generated from a ranges file and number of sets via -R or --ranges-file and -p or --parameter-sets, respectively.");
	} 
}
*/

/* random_double generates a random double in the range specified by the given pair of doubles
	parameters:
		range: a pair of doubles that specifies the lower and upper bounds in that order
	returns: the random double
	notes:
	todo:
*/
double random_double (double first, double second) {
	return first + (second - first) * rand() / (RAND_MAX + 1.0);
}


/* fill_perturbations fills the factors_perturb array in the given rates struct based on the given perturbations input buffer
	parameters:
		rs: the current simulation's rates to fill
		perturbs: the perturbations buffer taken from the input file
	returns: nothing
	notes:
		The buffer should contain one perturbation factor per line in the format 'factor start end' where 'start' is the index of the concentration to start applying the perturbation and 'end' is the index of the concentration after which to stop applying the perturbation. A perturbation with a maximum absolute percentage of 'factor' is applied to every concentration from 'start' to 'end'.
	todo:
		TODO allow comments in files
*/
void fill_perturbations (rates& rs, char* perturbs) {
	if (perturbs != NULL) {
		static const char* usage_message = "There was an error reading the given perturbations file.";
		int index = 0;
		while (perturbs[index] != '\0') {
			double factor = 0; // The perturbation factor
			int con_start = 0; // The starting concentration
			int con_end = 0; // The ending concentration
			
			// Read the perturbations
			if (sscanf(perturbs + index, "%lf %d %d", &factor, &con_start, &con_end) == 3) {
				if (factor < 0) {
					usage("The given perturbations file includes at least one factor less than 0. Adjust the perturbations file given with -u or --perturb-file.");
				}
				if (con_start < 0 || con_start >= NUM_RATES || con_end < 0 || con_end >= NUM_RATES) {
					usage("The given perturbations file includes rates outside of the valid range. Adjust the perturbations file given with -u or --perturb-file or add the appropriate rates by editing the macros file and recompiling.");
				}
				
				// If the given factor and index range is valid then fill the current simulation's rates with the perturbations
				factor /= 100;
				for (int i = con_start; i <= con_end; i++) {
					rs.factors_perturb[i] = factor;
				}
			} else {
				cout << sscanf(perturbs + index, "%lf %d %d", &factor, &con_start, &con_end);
				usage(usage_message);
			}
			while (not_EOL(perturbs[index++])) {} // Skip to the next line
		}
	}
}

/* create_passed_file creates a file to store parameter sets that passed
	parameters:
		ip: the program's input parameters
	returns: a pointer to the file output stream
	notes:
	todo:
*/
ofstream* create_passed_file (input_params& ip) {
	ofstream* file_passed = new ofstream();
	if (ip.print_passed) { // Print the passed sets only if the user specified it
		open_file(file_passed, ip.passed_file, false);
	}
	return file_passed;
}

/* delete_file closes the given file and frees it from memory
	parameters:
		file: a pointer to the output file stream to delete
	returns: nothing
	notes:
	todo:
*/
void delete_file (ofstream* file) {
	close_if_open(file);
	delete file;
}

/* delete_sets frees the given array of parameter sets from memory
	parameters:
		sets: the array of parameter sets
		ip: the program's input parameters
	returns: nothing
	notes:
	todo:
*/
void delete_sets (double** sets, input_params& ip) {
	for (int i = 0; i < ip.num_sets; i++) {
		delete[] sets[i];
	}
	delete[] sets;
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


/* generate_seed generates a seed based on the current UNIX time and process ID (PID) of the program instance
	parameters:
	returns: the generated seed
	notes:
		Generating a seed based on both the time and PID ensures concurrently executed versions of this program do not generated the same seed.
	todo:
*/
int generate_seed () {
	return abs(((time(0) * 181) * ((getpid() - 83) * 359)) % 805306457);
}


void update_initial_conditions(con_levels& cl){
	cl.data[GRB][0] = 0.787955;
	cl.data[G][0] = 0.210048;
	cl.data[GRSRB][0] = 0.558565;
	cl.data[GRS][0] = 0.416217;
	cl.data[GCR][0] = 0.804404;
	cl.data[GC][0] = 0.193886;
	cl.data[GBR][0] = 0.39359;
	cl.data[GB][0] = 0.599965;
	cl.data[GBRB][0] = 0.404247;
	cl.data[GBB][0] = 0.589308;
	cl.data[MNPO][0] = 4.18947;
	cl.data[MCPO][0] = 0.579108;
	cl.data[MNPT][0] = 8.34963;
	cl.data[MCPT][0] = 1.41168;
	cl.data[MNRT][0] = 12.3735;
	cl.data[MCRT][0] = 4.47054;
	cl.data[MNREV][0] = 31.869;
	cl.data[MCREV][0] = 161.144;
	cl.data[MNRO][0] = 3.76085;
	cl.data[MCRO][0] = 1.09547;
	cl.data[MNB][0] = 35.1842;
	cl.data[MCB][0] = 8.56043;
	cl.data[MNNP][0] = 0.520367;
	cl.data[MCNP][0] = 0.25596;
	cl.data[B][0] = 23.7741;
	cl.data[CL][0] = 48.0977;
	cl.data[BC][0] = 6.5815;
	cl.data[CYREV][0] = 123.95;
	cl.data[REVN][0] = 467.844;
	cl.data[CYREVG][0] = 0.193289;
	cl.data[REVNG][0] = 58.2391;
	cl.data[CYREVGP][0] = 0.0397137;
	cl.data[REVNGP][0] = 19.0662;
	cl.data[CYREVP][0] = 0.11012;
	cl.data[REVNP][0] = 67.6175;
	cl.data[GTO][0] = 1.13837;
	cl.data[X00001][0] = 0.342061;
	cl.data[X00011][0] = 8.17977;
	cl.data[X00100][0] = 50.5794;
	cl.data[X00110][0] = 3.08486;
	cl.data[X00200][0] = 0.0985802;
	cl.data[X00210][0] = 1.30846;
	cl.data[X01000][0] = 41.1658;
	cl.data[X01010][0] = 0.0156084;
	cl.data[X01011][0] = 8.72089;
	cl.data[X02000][0] = 46.5721;
	cl.data[X02010][0] = 0.0031953;
	cl.data[X02011][0] = 1.36799;
	cl.data[X10000][0] = 0.422898;
	cl.data[X10100][0] = 1.09254;
	cl.data[X20000][0] = 0.00181722;
	cl.data[X20010][0] = 5.19698e-05;
	cl.data[X20011][0] = 0.00512223;
	cl.data[X20100][0] = 0.106575;
	cl.data[X20110][0] = 0.000833073;
	cl.data[X20111][0] = 0.0566636;
	cl.data[X21000][0] = 0.00329174;
	cl.data[X21010][0] = 1.4996e-05;
	cl.data[X21011][0] = 0.00543134;
	cl.data[X21100][0] = 0.0876909;
	cl.data[X21110][0] = 0.000333744;
	cl.data[X21111][0] = 0.0548927;
	cl.data[X22000][0] = 0.00348546;
	cl.data[X22010][0] = 1.54312e-05;
	cl.data[X22011][0] = 0.00510457;
	cl.data[X22100][0] = 0.0938758;
	cl.data[X22110][0] = 0.000353611;
	cl.data[X22111][0] = 0.0535918;
	cl.data[X30000][0] = 0.914365;
	cl.data[X30100][0] = 0.664039;
	cl.data[X30200][0] = 0.0226162;
	cl.data[X30300][0] = 0.0241531;
	cl.data[X40000][0] = 0.00356643;
	cl.data[X40010][0] = 1.94469e-05;
	cl.data[X40011][0] = 0.000959363;
	cl.data[X40100][0] = 0.258442;
	cl.data[X40110][0] = 0.000985386;
	cl.data[X40111][0] = 0.0249991;
	cl.data[X40200][0] = 0.000157103;
	cl.data[X40210][0] = 1.1242e-05;
	cl.data[X40211][0] = 0.00132337;
	cl.data[X40300][0] = 0.0105914;
	cl.data[X40310][0] = 0.00023062;
	cl.data[X40311][0] = 0.025311;
	cl.data[X41000][0] = 0.00390694;
	cl.data[X41010][0] = 1.34882e-05;
	cl.data[X41011][0] = 0.000809309;
	cl.data[X41100][0] = 0.124712;
	cl.data[X41110][0] = 0.000416813;
	cl.data[X41111][0] = 0.0148919;
	cl.data[X41200][0] = 0.000188833;
	cl.data[X41210][0] = 1.79979e-06;
	cl.data[X41211][0] = 0.00109293;
	cl.data[X41300][0] = 0.00584769;
	cl.data[X41310][0] = 4.35837e-05;
	cl.data[X41311][0] = 0.0166107;
	cl.data[X42000][0] = 0.00428454;
	cl.data[X42010][0] = 1.47716e-05;
	cl.data[X42011][0] = 0.00086132;
	cl.data[X42100][0] = 0.137696;
	cl.data[X42110][0] = 0.000459811;
	cl.data[X42111][0] = 0.0160349;
	cl.data[X42200][0] = 0.0002068;
	cl.data[X42210][0] = 1.94018e-06;
	cl.data[X42211][0] = 0.0011516;
	cl.data[X42300][0] = 0.00644626;
	cl.data[X42310][0] = 4.76349e-05;
	cl.data[X42311][0] = 0.0177021;
	cl.data[X50000][0] = 0.00569806;
	cl.data[X50010][0] = 0.000190832;
	cl.data[X50011][0] = 0.0566834;
	cl.data[X50100][0] = 0.0287569;
	cl.data[X50110][0] = 0.000413832;
	cl.data[X50111][0] = 0.0494651;
	cl.data[X50200][0] = 0.00414505;
	cl.data[X50210][0] = 0.000386223;
	cl.data[X50211][0] = 0.124464;
	cl.data[X50300][0] = 0.00740796;
	cl.data[X50310][0] = 0.000654378;
	cl.data[X50311][0] = 0.104089;
	cl.data[X51000][0] = 0.00666974;
	cl.data[X51010][0] = 2.54221e-05;
	cl.data[X51011][0] = 0.00484524;
	cl.data[X51100][0] = 0.0828541;
	cl.data[X51110][0] = 0.000294564;
	cl.data[X51111][0] = 0.0323573;
	cl.data[X51200][0] = 0.0012142;
	cl.data[X51210][0] = 1.1508e-05;
	cl.data[X51211][0] = 0.00976858;
	cl.data[X51300][0] = 0.0087474;
	cl.data[X51310][0] = 7.98494e-05;
	cl.data[X51311][0] = 0.0607878;
	cl.data[X52000][0] = 0.00706385;
	cl.data[X52010][0] = 2.64292e-05;
	cl.data[X52011][0] = 0.00442251;
	cl.data[X52100][0] = 0.0852703;
	cl.data[X52110][0] = 0.000300906;
	cl.data[X52111][0] = 0.0301143;
	cl.data[X52200][0] = 0.00132641;
	cl.data[X52210][0] = 1.12497e-05;
	cl.data[X52211][0] = 0.00878991;
	cl.data[X52300][0] = 0.0091505;
	cl.data[X52310][0] = 7.78605e-05;
	cl.data[X52311][0] = 0.055586;
	cl.data[X60000][0] = 0.000792825;
	cl.data[X60010][0] = 2.57804e-05;
	cl.data[X60011][0] = 0.00334582;
	cl.data[X60100][0] = 0.0274125;
	cl.data[X60110][0] = 0.000385498;
	cl.data[X60111][0] = 0.0589937;
	cl.data[X60200][0] = 0.000125512;
	cl.data[X60210][0] = 4.99769e-05;
	cl.data[X60211][0] = 0.00735186;
	cl.data[X60300][0] = 0.00550233;
	cl.data[X60310][0] = 0.000631987;
	cl.data[X60311][0] = 0.12768;
	cl.data[X61000][0] = 0.00252101;
	cl.data[X61010][0] = 1.00779e-05;
	cl.data[X61011][0] = 0.00248103;
	cl.data[X61100][0] = 0.0581233;
	cl.data[X61110][0] = 0.000207429;
	cl.data[X61111][0] = 0.0239137;
	cl.data[X61200][0] = 0.000305307;
	cl.data[X61210][0] = 4.90217e-06;
	cl.data[X61211][0] = 0.00521949;
	cl.data[X61300][0] = 0.00753943;
	cl.data[X61310][0] = 6.39585e-05;
	cl.data[X61311][0] = 0.0471435;
	cl.data[X62000][0] = 0.00259772;
	cl.data[X62010][0] = 1.01709e-05;
	cl.data[X62011][0] = 0.00224807;
	cl.data[X62100][0] = 0.0602479;
	cl.data[X62110][0] = 0.000213449;
	cl.data[X62111][0] = 0.0225783;
	cl.data[X62200][0] = 0.000320067;
	cl.data[X62210][0] = 4.60372e-06;
	cl.data[X62211][0] = 0.004701;
	cl.data[X62300][0] = 0.00797183;
	cl.data[X62310][0] = 6.35261e-05;
	cl.data[X62311][0] = 0.0440794;
}

void generate_random_parameters(input_params& ip, parameters& pr){
	srand(ip.pseed);
	for (int i = 0; i < ip.num_sets; i++) {
		for (int j = 0; j < NUM_RATES; j++) {
			pr.data[i][j] = random_double((double)ip.lb, (double)ip.ub);
		}
	}
}
