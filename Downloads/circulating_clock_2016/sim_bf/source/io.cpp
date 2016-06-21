//for string processing
#include <cerrno> // Needed for errno, EEXIST
#include <cstdio> // Needed for fopen, fclose, fseek, ftell, rewind
#include <sys/stat.h> // Needed for mkdir
#include <unistd.h> // Needed for read, write, close
#include <stdio.h>
#include <string.h>

#include "io.hpp"
#include "main.hpp"
#include "macros.hpp"
extern terminal* term;
void store_filename (char** field, const char* value) {
	strcpy(*field, value);
}

void read_pipe (parameters& pr, input_params& ip) {
	// Read how many rates per set will be piped in

	int num_pars = 0;
	read_pipe_int(ip.pipe_in, &num_pars);
	if (num_pars != NUM_RATES) {
		cout << term->red << "An incorrect number of rates will be piped in! This simulation requires " << NUM_RATES << " rates per set but the sampler is sending " << num_pars << " per set." << term->reset << endl;
		exit(EXIT_INPUT_ERROR);
	}
	
	
	// Read every set and store it as an array of doubles
	for (int i = 0; i < pr.num_sets; i++) {
		if (read(ip.pipe_in, pr.data[i], sizeof(double) * NUM_RATES) == -1){
			term->failed_pipe_read();
			exit(EXIT_PIPE_READ_ERROR);
		}
	}
}

void read_pipe_int (int fd, int* address) {
	if (read(fd, address, sizeof(int)) == -1) {
		term->failed_pipe_read();
		exit(EXIT_PIPE_READ_ERROR);
	}
}

void read_pipe_set (int fd, double pars[]) {

	if (read(fd, pars, sizeof(double) * NUM_RATES) == -1){
		term->failed_pipe_read();
		exit(EXIT_PIPE_READ_ERROR);
	}
	
}

void read_file (input_data* ifd) {
	cout << term->blue << "Reading file " << term->reset << ifd->filename << " . . . ";
	// Open the file for reading
	FILE* file = fopen(ifd->filename, "r");
	if (file == NULL) {
		cout << term->red << "Couldn't open " << ifd->filename << "!" << term->reset << endl;
		exit(EXIT_FILE_READ_ERROR);
	}
	
	// Seek to the end of the file, grab its size, and then rewind
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	ifd->size = size;
	rewind(file);
	
	// Allocate enough memory to contain the whole file
	ifd->buffer = (char*)mallocate(sizeof(char) * size + 1);
	
	// Copy the file's contents into the buffer
	long result = fread(ifd->buffer, 1, size, file);
	if (result != size) {
		cout << term->red << "Couldn't read from " << ifd->filename << term->reset << endl;
		exit(EXIT_FILE_READ_ERROR);
	}
	ifd->buffer[size] = '\0';
	
	// Close the file
	if (fclose(file) != 0) {
		cout << term->red << "Couldn't close " << ifd->filename << term->reset << endl;
		exit(EXIT_FILE_READ_ERROR);
	}
	term->done();
}

/* parse_param_line reads a line in the given parameter sets buffer and stores it in the given array of doubles
	parameters:
		params: the array of doubles to store the parameters in
		buffer_line: the buffer with the line to read
		index_buffer: the index of the buffer to start from
	returns: true if a line was found, false if the end of the file was reached without finding a valid line
	notes:
		The buffer should contain one parameter set per line, each set containing comma-separated floating point parameters.
		Blank lines and lines starting with # will be ignored.
		Each line must contain the correct number of parameters or the program will exit.
		index_buffer is a reference, allowing this function to store where it finished parsing.
	todo:
*/
bool parse_param_line (double* params, char* buffer_line, int& index_buffer) {
	static const char* usage_message = "There was an error reading the given parameter sets file.";
	int index_params = 0; // Current index in params
	int index_digits = index_buffer; // Index of the start of the digits to read
	int i = index_buffer; // Current index in buffer_line
	int line_start = i; // The start of the line, used to tell whether or not a line is empty
	for (; not_EOL(buffer_line[i]); i++) {
		if (buffer_line[i] == '#') { // Skip any lines starting with #
			for(; not_EOL(buffer_line[i]); i++);
			i++;
		} else if (buffer_line[i] == ',') { // Indicates the end of the digits to read
			if (sscanf(buffer_line + index_digits, "%lf", &(params[index_params++])) < 1) { // Convert the string of digits to a double when storing it in params
				usage(usage_message);
			}
			index_digits = i + 1;
		}
	}
	index_buffer = i + 1;
	if (i - line_start > 0) { // This line has content
		if (sscanf(buffer_line + index_digits, "%lf", &(params[index_params++])) < 1) {
			usage(usage_message);
		}
		if (index_params != NUM_RATES) {
			cout << term->red << "The given parameter sets file contains sets with an incorrect number of rates! This simulation requires " << NUM_RATES << " per set but at least one line contains " << index_params << " per set." << term->reset << endl;
			exit(EXIT_INPUT_ERROR);
		}
		return true;
	} else if (buffer_line[index_buffer] != '\0') { // There are more lines to try to parse
		return parse_param_line(params, buffer_line, index_buffer);
	} else { // The end of the buffer was found
		return false;
	}
}


/* parse_ranges_file reads the given buffer and stores every range found in the given ranges array
	parameters:
		ranges: the array of pairs in which to store the lower and upper bounds of each range
		buffer: the buffer with the ranges to read
	returns: nothing
	notes:
		The buffer should contain one range per line, starting the name of the parameter followed by the bracked enclosed lower and then upper bound optionally followed by comments.
		e.g. 'msh1 [30, 65] comment'
		The name of the parameter is so humans can conveniently read the file and has no semantic value to this parser.
		Blank lines and lines starting with # will be ignored. Anything after the upper bound is ignored.
	todo:
*/
void parse_ranges_file (pair <double, double> ranges[], char* buffer) {
	int i = 0;
	int rate = 0;
	for (; buffer[i] != '\0'; i++) {
		// Ignore lines starting with #
		while (buffer[i] == '#') {
			while (buffer[i] != '\n' && buffer[i] != '\0') {i++;}
			i++;			
		}
		
		// Ignore whitespace before the opening bracket
		while (buffer[i] != '[' && buffer[i] != '\0') {i++;}
		if (buffer[i] == '\0') {break;}
		i++;
		
		// Read the bounds
		ranges[rate].first = atof(buffer + i);
		while (buffer[i] != ',') {i++;}
		i++;
		ranges[rate].second = atof(buffer + i);
		if (ranges[rate].first < 0 || ranges[rate].second < 0) { // If the ranges are invalid then set them to 0
			ranges[rate].first = 0;
			ranges[rate].second = 0;
		}
		rate++;
		// Skip any comments until the end of the line
		while (buffer[i] != '\n' && buffer[i] != '\0') {i++;}
	}
}

/* open_file opens the file with the given name and stores it in the given output file stream
	parameters:
		file_pointer: a pointer to the output file stream to open the file with
		file_name: the path and name of the file to open
		append: if true, the file will appended to, otherwise any existing data will be overwritten
	returns: nothing
	notes:
	todo:
*/
void open_file (ofstream* file_pointer, char* file_name, bool append) {
	try {
		if (append) {
			cout << term->blue << "Opening " << term->reset << file_name << " . . . ";
			file_pointer->open(file_name, fstream::app);
		} else {
			cout << term->blue << "Creating " << term->reset << file_name << " . . . ";
			file_pointer->open(file_name, fstream::out);
		}
	} catch (ofstream::failure) {
		cout << term->red << "Couldn't write to " << file_name << "!" << term->reset << endl;
		exit(EXIT_FILE_WRITE_ERROR);
	}
	term->done();
}

/* close_if_open closes the given output file stream if it is open
	parameters:
		file: a pointer to the output file stream to close
	returns: nothing
	notes:
	todo:
*/
void close_if_open (ofstream* file) {
	if (file->is_open()) {
		file->close();
	}
}

/* print_concentrations prints the concentration values of every cell at every time step of the given mutant for the given run
	parameters:
		ip: the program's input parameters
		sd: the current simulation's data
		cl: the current simulation's concentration levels
		md: mutant data
		filename_cons: the path and name of the file in which to store the concentration levels
		set_num: the index of the parameter set whose concentration levels are being printed
	returns: nothing
	notes:
		The first line of the file is the total width then a space then the height of the simulation.
		Each line after starts with the time step then a space then space-separated concentration levels for every cell ordered by their position relative to the active start of the PSM.
		If binary mode is set, the file will get the extension .bcons and print raw binary values, not ASCII text.
		The concentration printed is mutant dependent, but usually mh1.
	todo:
*/
void print_concentrations (input_params& ip, sim_data& sd, con_levels& cl, int set_num) {
	double ** data = cl.data;
	if (ip.print_cons) { // Print the concentrations only if the user specified it
		int strlen_set_num = INT_STRLEN(set_num); // How many bytes the ASCII representation of set_num takes
		char* str_set_num = (char*)mallocate(sizeof(char) * (strlen_set_num + 1));
		sprintf(str_set_num, "%d", set_num);
		char extension[7];
		if (ip.binary_cons_output) { // Binary files get the extension .bcons
			strcpy(extension, ".bcons");
		} else { // ASCII files (the default option) get the extension specified by the user
			strcpy(extension, ".cons");
		}
		char* filename_set = (char*)mallocate(sizeof(char) * ( strlen("set_") + strlen_set_num + strlen(extension) + 1));
		sprintf(filename_set, "set_%s%s", str_set_num, extension);
		
		cout << "    "; // Offset the open_file message to preserve horizontal spacing
		ofstream file_cons;
		open_file(&file_cons, filename_set, true);
	
		free(filename_set);
		free(str_set_num);
		
		if (ip.binary_cons_output) {
			for (int j = 0; j < sd.steps_total; j++) {
				file_cons.write((char*)(&j), sizeof(int));
				for (int i = 0; i < NUM_CONS; i++) {
					double con = data[i][j];
					file_cons.write((char*)(&con), sizeof(double));
				}
				file_cons << "\n";
			}
		} else {
			for (int j = 0; j < sd.steps_total; j++) {
				file_cons << j << " ";
				for (int i = 0; i < NUM_CONS ; i++) {
					file_cons << data[i][j] << " ";
					}
				file_cons << "\n";
			}
		}
	}
}



/* not_EOL returns whether or not a given character is the end of a line or file (i.e. '\n' or '\0', respectively)
	parameters:
		c: the character to check
	returns: true if c is the end of a line or file, false otherwise
	notes:
		When reading input file strings, use this instead of a straight newline check to avoid EOF (end of file) issues.
	todo:
*/
bool not_EOL (char c) {
	return c != '\n' && c != '\0';
}

void get_kf_data(con_levels& kf_cl){
	input_data kf_data((char*)KF_FILENAME);
	read_file(&kf_data);
	for (int i = 0; i < TEST_STEPS; i++){
		if (kf_data.index < kf_data.size){
			parse_kf_line(kf_cl, kf_data.buffer, i, kf_data.index);
		}
	}
}

void parse_kf_line(con_levels& kf_cl, char* buffer, int time_step_index, int& buffer_pointer){
	static const char* usage_message = "There was an error reading the Kim and Forger's results file.";
	int con_index = 0; 
	int index_digits = buffer_pointer;
	int line_start = buffer_pointer;
	int i = buffer_pointer;
	for (; not_EOL(buffer[i]); i++){
		if (buffer[i] == '#') { // Skip any lines starting with #
			for(; not_EOL(buffer[i]); i++);
			i++;
		} else if (buffer[i] == ','){ // Indicates the end of the digits to read
			if (sscanf(buffer+ index_digits, "%lf", & (kf_cl.data[con_index++][time_step_index])) < 1) { // Convert the string of digits to a double when storing it in params
				usage(usage_message);
			}
			index_digits = i + 1;
		}
	}
	buffer_pointer = i + 1;
	
	if (i - line_start > 0){
		if (sscanf(buffer+index_digits, "%lf", & (kf_cl.data[con_index ++][time_step_index])) < 1) {
			usage(usage_message);
		}
		if (con_index != NUM_CONS){
			cout << term->red << "The given KimForger results file contains sets with an incorrect number of concentrations! This simulation requires " << NUM_CONS << " per time step but line " << time_step_index << "contains " << con_index << " concentrations." << term->reset << endl;
			exit(EXIT_INPUT_ERROR);
		}
		return;
	}
	return;
}


/* write_pipe writes run scores to a pipe created by a program interacting with this one
	parameters:
		score: the array of scores to send
		ip: the program's input parameters
		sd: the current simulation's data
	returns: nothing
	notes:
		This function assumes that fd points to a valid pipe and will exit with an error if it does not.
	todo:
*/
void write_pipe (double score[], input_params& ip, sim_data& sd) {
	// Write the scores to the pipe
	
	if (write(ip.pipe_out, &score, sizeof(double) * ip.num_sets) == -1) {
		term->failed_pipe_write();
		exit(EXIT_PIPE_WRITE_ERROR);
	}
	
	// Close the pipe
	if (close(ip.pipe_out) == -1) {
		term->failed_pipe_write();
		exit(EXIT_PIPE_WRITE_ERROR);
	}
}

/* write_pipe_int writes the given integer to the given pipe
	parameters:
		fd: the file descriptor identifying the pipe
		value: the integer to write to the pipe
	returns: nothing
	notes:
		This function assumes that fd points to a valid pipe and will exit with an error if it does not.
	todo:
*/
void write_pipe_double (int fd, double value) {
	if (write(fd, &value, sizeof(double)) == -1) {
		term->failed_pipe_write();
		exit(EXIT_PIPE_WRITE_ERROR);
	}
}
