#ifndef IO_HPP
#define IO_HPP

#include "structs.hpp"
void store_filename (char**,const char*);
void read_pipe (parameters& , input_params&);
void read_pipe_int (int, int* );
void read_pipe_set (int, double[]);
void read_file (input_data* ifd);
bool parse_param_line (double*, char*, int& ); 
void parse_ranges_file (pair <double, double> [], char* );
void open_file (ofstream*, char* , bool);
void close_if_open (ofstream*);
void print_concentrations (input_params& , sim_data& , con_levels& , int );
bool not_EOL (char);
void get_kf_data(con_levels& cl);
void parse_kf_line(con_levels& , char*, int, int&);
void write_pipe (double[], input_params&, sim_data&);
void write_pipe_double (int, double);
#endif
