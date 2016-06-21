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
io.cpp contains functions for input and output of files and pipes. All I/O related functions should be placed in this file.
*/

#include <sys/wait.h> // Needed for waitpid
#include <unistd.h> // Needed for pipe, read, write, close, fork, execv
#include <stdio.h>
#include "io.hpp" // Function declarations

#include "init.hpp"
#include "macros.hpp"
extern input_params ip; // Declared in main.cpp
extern terminal* term; // Declared in init.cpp

/* store_filename stores the given value in the given field
	parameters:
		field: a pointer to the filename's field
		value: the filename to store
	returns: nothing
	notes:
		The previous field value is freed before assigning the new one.
	todo:
*/
void store_filename (char** field, const char* value) {
	mfree(*field);
	*field = copy_str(value);
}


/* simulate_set performs the required piping to setup and run a simulation with the given parameters
	parameters:
		parameters: the parameters to pass as a parameter set to the simulation
	returns: the score the simulation received
	notes:
	todo:
*/
void simulate_set (parameters& pr) {
	ostream& v = term->verbose();

	// Create a pipe
	int sim_in[2];
	int sim_out[2]; 
	v << "  ";
	v << term->blue << "Creating a pipe " << term->reset << ". . . ";
	if (pipe(sim_in) == -1 || pipe(sim_out) == -1) {
		term->failed_pipe_create();
		exit(EXIT_PIPE_CREATE_ERROR);
	}
	v << term->blue << "Done: " << term->reset << "parent_read " << sim_out[0] << " parent_write " << sim_in[1] << " child_write " << sim_out[1] << " child_read "<< sim_in[0] << endl;
	
	int parent_read = sim_out[0];
	int parent_write = sim_in[1];
	int child_write = sim_out[1];
	int child_read = sim_in[0];
	
	// Copy the user-specified simulation arguments and fill the copy with the pipe's file descriptors
	char** sim_args = copy_args(ip.sim_args, ip.num_sim_args);
	store_pipe(sim_args, ip.num_sim_args - 4, child_read);
	store_pipe(sim_args, ip.num_sim_args - 2, child_write);
	
	// Fork the process so the child can run the simulation
	v << "  ";
	v << term->blue << "Forking the process " << term->reset << ". . . ";
	pid_t pid = fork();
	if (pid == -1) {
		term->failed_fork();
		exit(EXIT_FORK_ERROR);
	}
	if (pid == 0) { // The child runs the simulation
		v << "  ";
		v << term->blue << "Checking that the simulation file exists and can be executed " << term->reset << ". . . ";
		if (access(ip.sim_file, X_OK) == -1) {
			term->failed_exec();
			exit(EXIT_EXEC_ERROR);
		}
		term->done(v);
		if (execv(ip.sim_file, sim_args) == -1) {
			term->failed_exec();
			exit(EXIT_EXEC_ERROR);
		}
	}
	else { // The parent pipes in the parameter set to run
		v << term->blue << "Done: " << term->reset << "the child process's PID is " << pid << endl;
		v << "  ";
		v << term->blue << "Writing to the pipe " << term->reset << "(file descriptor " << parent_write << ") . . . ";
		write_pipe(parent_write, pr);
		term->done(v);
	}
	
	// Wait for the child to finish simulating
	int status = 0;
	waitpid(pid, &status, WUNTRACED);
	if (WIFEXITED(status) == 0) {
		term->failed_child();
		exit(EXIT_CHILD_ERROR);
	}
	
	
	// Pipe in the simulation's score
	double* score;
	v << "  ";
	v << term->blue << "Reading the pipe " << term->reset << "(file descriptor " << parent_read << ") . . . ";
	read_pipe(parent_read, &score);
	v << term->blue << "Done: " << term->reset << "(raw score " << score << " / " << 1 << ")" << endl;

	// CLOSE PIPES
	v << "  ";
	v << term->blue << "Closing the the pipes " << term->reset << "(file descriptor " << parent_read << ", "<< parent_write << ", " << child_read << ", " << child_write<< ") . . . ";
	if (close(parent_read) == -1) {
		term->failed_pipe_read();
		exit(EXIT_PIPE_WRITE_ERROR);
	}
	
	if (close(parent_write) == -1) {
		term->failed_pipe_write();
		exit(EXIT_PIPE_WRITE_ERROR);
	}

	if (close(child_read) == -1) {
		term->failed_pipe_write();
		exit(EXIT_PIPE_WRITE_ERROR);
	}
	
	if (close(child_write) == -1) {
		term->failed_pipe_write();
		exit(EXIT_PIPE_WRITE_ERROR);
	}
	term->done(v);
	
	print_good_set(pr, &score);
	// Free the simulation arguments
	for (int i = 0; sim_args[i] != NULL; i++) {
		mfree(sim_args[i]);
	}
	mfree(sim_args);
}

void print_good_set (parameters& pr, double** score) {
	ofstream passed_file ;
	open_file(&passed_file, ip.good_sets_file, false);
	for (int i = 0; i < ip.num_sets; i ++){
		if (*score[i] != 0){
			for (int j = 0; j < DIM; j ++){
				passed_file.write((char *)(&(pr.data[i][j])), sizeof(double));
			}
			passed_file << "\n";
		}
	}
	close_if_open(passed_file);
}


/* write_pipe writes the given parameter set to the given pipe
	parameters:
		fd: the file descriptor of the pipe to write to
		parameters: the parameter set to pipe
	returns: nothing
	notes:
	todo:
*/
void write_pipe (int fd, parameters& pr) {
	write_pipe_int(fd, DIM); // Write the number of dimensions, i.e. parameters per set, being sent
	for (int i = 0; i < ip.num_sets; i ++){
		if (write(fd, pr.data[i], sizeof(double) * DIM) == -1) {
			term->failed_pipe_write();
			exit(EXIT_PIPE_WRITE_ERROR);
		}
	}
}

/* write_pipe_int writes the given integer to the given pipe
	parameters:
		fd: the file descriptor of the pipe to write to
		value: the integer to pipe
	returns: nothing
	notes:
	todo:
*/
void write_pipe_int (int fd, int value) {
	if (write(fd, &value, sizeof(int)) == -1) {
		term->failed_pipe_write();
		exit(EXIT_PIPE_WRITE_ERROR);
	}
}

/* read_pipe reads the maximum score and the received score from the given pipe
	parameters:
		fd: the file descriptor of the pipe to write to
		max_score: a pointer to store the maximum score the simulation could have received
		score: a pointer to store the score the simulation actually received
	returns: nothing
	notes:
	todo:
*/
void read_pipe (int fd, double** score) {
	for (int i = 0; i < ip.num_sets; i ++){
		read_pipe_int(fd, score[i]);
	}
}

/* read_pipe_int writes an integer from the given pipe
	parameters:
		fd: the file descriptor of the pipe to write to
		address: a pointer to store the received integer
	returns: nothing
	notes:
	todo:
*/
void read_pipe_int (int fd, double* address) {
	if (read(fd, address, sizeof(double)) == -1) {
		term->failed_pipe_read();
		exit(EXIT_PIPE_READ_ERROR);
	}
}

/* close_if_open closes the given output file stream if it is open
	parameters:
		file: a pointer to the output file stream to close
	returns: nothing
	notes:
	todo:
		TODO: check if this function is still being used
*/
void close_if_open (ofstream& file) {
	if (file.is_open()) {
		file.close();
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
