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
io.hpp contains function declarations for io.cpp.
*/

#ifndef IO_HPP
#define IO_HPP

#include "structs.hpp"

void store_filename(char**, const char*);
void simulate_set(parameters&);
//void print_good_set (double parameters[], double score, input_params&);
void write_pipe (int fd, parameters& pr, int);
void write_pipe_int(int, int);
void read_pipe(int, double**);
void read_pipe_int(int, double*);
void close_if_open(ofstream&);
void open_file (ofstream*, char* , bool);
void print_good_set (parameters&, double**, int);
#endif

