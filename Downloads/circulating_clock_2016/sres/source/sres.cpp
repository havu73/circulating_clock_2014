/*
Stochastically ranked evolutionary strategy sampler for zebrafish segmentation
Copyright (C) 2013 Ahmet Ay, Jack Holland, Adriana Sperlea, Sebastian Sangervasi

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.	If not, see <http://www.gnu.org/licenses/>.
*/

/*
sres.cpp contains function to interact with libSRES.
Avoid placing I/O functions here and add them to io.cpp instead.
*/

#include <ctime> // Needed for time_t in libSRES (they don't include time.h for some reason)

// Include MPI if compiled with it
#if defined(MPI)
	#undef MPI // MPI uses this macro as well, so temporarily undefine it
	#include <mpi.h> // Needed for MPI_Comm_rank, MPI_COMM_WORLD
	#define MPI // The MPI macro should be checked only for definition, not value
#endif

// libSRES has different files for MPI and non-MPI versions
#if defined(MPI)
	#include "../libsres-mpi/sharefunc.hpp"
	#include "../libsres-mpi/ESSRSort.hpp"
	#include "../libsres-mpi/ESES.hpp"
#else
	#include "../libsres/sharefunc.hpp"
	#include "../libsres/ESSRSort.hpp"
	#include "../libsres/ESES.hpp"
#endif

#include "sres.hpp" // Function declarations

#include "io.hpp"

extern terminal* term; // Declared in init.cpp

/* get_rank gets the MPI rank of the process or returns 0 if MPI is not active
	parameters:
	returns: the rank
	notes:
	todo:
*/
int get_rank () {
	int rank = 0;
	#if defined(MPI)
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	#endif
	return rank;
}

/* init_sres initializes libSRES functionality, including population data, generations, ranges, etc.
	parameters:
		ip: the program's input parameters
		sp: parameters required by libSRES
	returns: nothing
	notes:
		Excuse the awful variable names. They are named according to libSRES conventions for the sake of consistency.
		Many of the parameters required by libSRES are not configurable via the command-line because they haven't needed to be changed but this does not mean they aren't significant.
	todo:
*/
void init_sres (input_params& ip, sres_params& sp) {
	// Initialize parameters required by libSRES
	int es = esDefESSlash;
	int constraint = 0;
	int dim = ip.num_dims;
	int miu = ip.pop_parents;
	int lambda = ip.pop_total;
	int gen = ip.generations;
	double gamma = esDefGamma;
	double alpha = esDefAlpha;
	double varphi = esDefVarphi;
	int retry = 0;
	sp.pf = essrDefPf;
	
	// Transform is a dummy function f(x)->x but is still required to fit libSRES's code structure
	sp.trsfm = (ESfcnTrsfm*)mallocate(sizeof(ESfcnTrsfm) * dim);
	for (int i = 0; i < dim; i++) {
		sp.trsfm[i] = transform;
	}
	
	// Call libSRES's initialize function
	int rank = get_rank();
	ostream& v = term->verbose();
	if (rank == 0) {
		cout << term->blue << "Running libSRES initialization simulations " << term->reset << ". . . ";
		cout.flush();
		v << endl;
	}
	ESInitial(ip.seed, &(sp.param), sp.trsfm, fitness, es, constraint, dim, sp.ub, sp.lb, miu, lambda, gen, gamma, alpha, varphi, retry, &(sp.population), &(sp.stats));
	if (rank == 0) {
		cout << term->blue << "Done";
		v << " with libSRES initialization simulations";
		cout << term->reset << endl;
	}
}

/* run_sres iterates through every specified generation of libSRES
	parameters:
		sp: parameters required by libSRES
	returns: nothing
	notes:
	todo:
*/
void run_sres (sres_params& sp) {
	int rank = get_rank();
	while (sp.stats->curgen < sp.param->gen) {
		int cur_gen = sp.stats->curgen;
		if (rank == 0) {
			cout << term->blue << "Starting generation " << term->reset << cur_gen << " . . ." << endl;
		}
		ESStep(sp.population, sp.param, sp.stats, sp.pf);
		if (rank == 0) {
			cout << term->blue << "Done with generation " << term->reset << cur_gen << endl;
		}
	}
}

/* free_sres frees parameters required by libSRES and calls libSRES's deinitialization function
	parameters:
		sp: parameters required by libSRES
	returns: nothing
	notes:
	todo:
*/
void free_sres (sres_params& sp) {
	mfree(sp.trsfm);
	mfree(sp.lb);
	mfree(sp.ub);
	ESDeInitial(sp.param, sp.population, sp.stats);
}

/* fitness runs a simulation and stores its resulting score in a variable libSRES then accesses
	parameters:
		parameters: the parameters provided by libSRES
		score: a pointer to store the score the simulation received
		constraints: parameter constraints (not used but required by libSRES's code structure)
	returns: nothing
	notes:
		This function is called by libSRES for every population member every generation.
	todo:
*/
void fitness (double* parameters, double* score, double* constraints) {
	cout << sizeof(parameters)<< endl;
	*score = simulate_set(parameters);
}

/* transform is a dummy function required by libSRES's code structure
	parameters:
		x: a parameter to potentially transform
	returns: the given parameter
	notes:
	todo:
*/
double transform (double x) {
	return x;
}

