
#include "sim.hpp"

#include <string.h>
#include <iostream>
#include <fstream>
#include "debug.hpp"
#include "init.hpp"
#include "io.hpp"
#include "debug.hpp"
#include "kftest.hpp"
using namespace std;
extern terminal* term;

void simulate_all_params(input_params& ip, rates& rs, sim_data& sd, parameters& pr, ofstream* file_passed){
	con_levels cl (NUM_CONS, sd.steps_total);//cl contains simulation data
	con_levels kf_cl(NUM_CONS, TEST_STEPS);	// kf_cl contains results from Kim and Forger, for testing purposes
	get_kf_data(kf_cl);
	
	double score[ip.num_sets];
	for (int i = 0; i < ip.num_sets; i++){
		memcpy(rs.rates_base, pr.data[i], sizeof(double) * NUM_RATES);
		cout << rs.rates_base[0]<< endl;
		score[i] = simulate_param_set(i, ip, sd, rs, cl, kf_cl);
		if (score[i] > 0.5 ){
			cout << "difference: " << 1/score[i] << endl; 
			test_rates(rs);
			print_passed_set(file_passed, rs);
		}
	}
	
	close_if_open(file_passed);
	// Pipe the scores if piping specified by the user
	if (ip.piping) {
		write_pipe(score, ip, sd);
	}
}

/*	1, init_seeds
 * 	2, Set up initial conditions for cl structures
 * 	3, calculate the concentrations of different species over time. If any negative number of nan: set fails the validity test
 * 	4, 
 */
double simulate_param_set(int set_num, input_params& ip, sim_data& sd, rates& rs, con_levels& cl, con_levels & kf_cl){
	cout << term->blue << "Simulating set " << term->reset << set_num << " . . ." << endl;
	
	double score = 0;
	
	if (!ip.reset_seed) { // Reset the seed for each set if specified by the user
		init_seeds(ip, set_num, set_num > 0, true);
	}
	
	cl.reset();
	update_initial_conditions(cl);

	bool valid = calculate_concentrations(ip, cl, rs, sd);
	
	if (valid){
		double inverse_score = cost(cl, kf_cl);
		if (inverse_score == 0){
			score = 1;
		}else {
			score = 1/ inverse_score;
		}
	}
	else{
		cout << term->blue << "Set " << term->reset << set_num << term->blue << " failed because concentrations is nan ..." << endl;
	}
	return score;
}

bool calculate_concentrations(input_params& ip, con_levels& cl, rates& rs, sim_data& sd){
	double * sim_rates = rs.rates_base;
	bool valid = true; // valid turns to false if we find a concentration that is either negative, or NaN
	for (int j = 1 ; j< sd.steps_total ; j++){
		for (int i = 0; i< NUM_CONS; i++ ){
			if (i == GRB){
				find_GRB(sim_rates, cl, j, sd.step_size);
			}
			else if (i == G){
				findG(sim_rates, cl, j, sd.step_size);
			}
			else if (i == GRSRB){
				findGRSRB(sim_rates, cl, j, sd.step_size);
			}
			else if (i == GRS){
				findGRS(sim_rates, cl, j, sd.step_size);
			}
			else if (i == GCR){
				findGCR (sim_rates, cl, j, sd.step_size);
			}
			else if (i == GC){
				findGC(sim_rates, cl, j, sd.step_size);
			}
			else if (i == GBR){
				findGBR(sim_rates, cl, j, sd.step_size);
			}
			else if (i == GB){
				findGB(sim_rates, cl, j, sd.step_size);
			}
			else if (i == GBRB){
				findGBRB(sim_rates, cl, j, sd.step_size);
			}
			else if (i == GBB){
				findGBB(sim_rates, cl, j, sd.step_size);
			}
			else if (i == MNPO){
				findMNPO(sim_rates, cl, j, ip.time_total, sd.step_size);
			}
			else if (i == MCPO){
				findMCPO(sim_rates, cl, j, sd.step_size);
			}
			else if (i == MNPT){
				findMNPT(sim_rates, cl, j, sd.step_size);
			}
			else if (i == MCPT){
				findMCPT(sim_rates, cl, j, ip.time_total, sd.step_size);
			}
			else if (i == MNRT){
				findMNRT(sim_rates, cl, j, sd.step_size);
			}
			else if (i == MCRT){
				findMCRT(sim_rates, cl, j, sd.step_size);
			}
			else if (i == MNREV){
				findMNREV(sim_rates, cl, j, sd.step_size);
			}
			else if (i == MCREV){
				findMCREV(sim_rates, cl, j, sd.step_size);
			}
			else if (i == MNRO){
				findMNRO(sim_rates, cl, j, sd.step_size);
			}
			else if (i == MCRO){
				findMCRO(sim_rates, cl, j, sd.step_size);
			}
			else if (i == MNB){
				findMNB(sim_rates, cl, j, sd.step_size);
			}
			else if (i == MCB){
				findMCB(sim_rates, cl, j, sd.step_size);
			}
			else if (i == MNNP){
				findMNNP(sim_rates, cl, j, sd.step_size);
			}
			else if (i == MCNP){
				findMCNP(sim_rates, cl, j, sd.step_size);
			} 
			else if (i == B){
				findB(sim_rates, cl, j, sd.step_size);
			}
			else if (i == CL){
				findCL(sim_rates, cl, j, sd.step_size);
			}
			else if (i == BC){
				findBC(sim_rates, cl, j, sd.step_size);
			}
			else if (i == CYREV){
				findCYREV(sim_rates, cl, j, sd.step_size);
			}
			else if (i == REVN){
				findREVN(sim_rates, cl, j, sd.step_size);
			}
			else if (i == CYREVG){
				findCYREVG(sim_rates, cl, j, sd.step_size);
			}
			else if (i == REVNG){
				findREVNG(sim_rates, cl, j, sd.step_size);
			}
			else if (i == CYREVGP){
				findCYREVGP(sim_rates, cl, j, sd.step_size);
			}
			else if (i == REVNGP){
				findREVNGP(sim_rates, cl, j, sd.step_size);
			}
			else if (i == CYREVP){
				findCYREVP(sim_rates, cl, j, sd.step_size);
			}
			else if (i == REVNP){
				findREVNP(sim_rates, cl, j, sd.step_size);
			}
			else if (i == GTO){
				findGTO (sim_rates, cl, j, sd.step_size);
			}
			else if (i == X00001){
				findX00001(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X00011){
				findX00011(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X00100){
				findX00100(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X00110){
				findX00110(sim_rates, cl, j, sd.step_size);	
			}
			else if (i == X00200){
				findX00200(sim_rates, cl, j, sd.step_size);	
			}
			else if (i == X00210){
				findX00210(sim_rates, cl, j, sd.step_size);	
			}
			else if (i == X01000){
				findX01000(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X01010){
				findX01010(sim_rates, cl, j, sd.step_size);	
			}
			else if (i == X01011){
				findX01011(sim_rates, cl, j,sd.step_size);
			}
			else if (i == X02000){
				findX02000(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X02010){
				findX02010(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X02011){
				findX02011(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X10000){
				findX10000(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X10100){
				findX10100(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X20000){
				findX20000(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X20010){
				findX20010(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X20011){
				findX20011(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X20100){
				findX20100(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X20110){
				findX20110(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X20111){
				findX20111(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X21000){
				findX21000(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X21010){
				findX21010(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X21011){
				findX21011(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X21100){
				findX21100(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X21110){
				findX21110(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X21111){
				findX21111(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X22000){
				findX22000(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X22010){
				findX22010(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X22011){
				findX22011(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X22100){
				findX22100(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X22110){
				findX22110(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X22111){
				findX22111(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X30000){
				findX30000(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X30100){
				findX30100(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X30200){
				findX30200(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X30300){
				findX30300(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X40000){
				findX40000(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X40010){
				findX40010(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X40011){
				findX40011(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X40100){
				findX40100(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X40110){
				findX40110(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X40111){
				findX40111(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X40200){
				findX40200(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X40210){
				findX40210(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X40211){
				findX40211(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X40300){
				findX40300(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X40310){
				findX40310(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X40311){
				findX40311(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X41000){
				findX41000(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X41010){
				findX41010(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X41011){
				findX41011(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X41100){
				findX41100(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X41110){
				findX41110(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X41111){
				findX41111(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X41200){
				findX41200(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X41210){
				findX41210(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X41211){
				findX41211(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X41300){
				findX41300(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X41310){
				findX41310(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X41311){
				findX41311(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X42000){
				findX42000(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X42010){
				findX42010(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X42011){
				findX42011(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X42100){
				findX42100(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X42110){
				findX42110(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X42111){
				findX42111(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X42200){
				findX42200(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X42210){
				findX42210(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X42210){
				findX42210(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X42211){
				findX42211(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X42300){
				findX42300(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X42310){
				findX42310(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X42311){
				findX42311(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X50000){
				findX50000(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X50010){
				findX50010(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X50011){
				findX50011(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X50100){
				findX50100(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X50110){
				findX50110(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X50111){
				findX50111(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X50200){
				findX50200(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X50210){
				findX50210(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X50211){
				findX50211(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X50300){
				findX50300(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X50310){
				findX50310(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X50311){
				findX50311(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X51000){
				findX51000(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X51010){
				findX51010(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X51011){
				findX51011(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X51100){
				findX51100(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X51110){
				findX51110(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X51111){
				findX51111(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X51200){
				findX51200(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X51210){
				findX51210(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X51211){
				findX51211(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X51300){
				findX51300(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X51310){
				findX51310(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X51311){
				findX51311(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X52000){
				findX52000(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X52010){
				findX52010(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X52011){
				findX52011(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X52100){
				findX52100(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X52110){
				findX52110(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X52111){
				findX52111(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X52200){
				findX52200(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X52210){
				findX52210(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X52211){
				findX52211(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X52300){
				findX52300(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X52310){
				findX52310(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X52311){
				findX52311(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X60000){
				findX60000(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X60010){
				findX60010(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X60011){
				findX60011(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X60100){
				findX60100(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X60110){
				findX50110(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X60111){
				findX60111(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X60200){
				findX60200(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X60210){
				findX60210(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X60211){
				findX60211(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X60300){
				findX60300(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X60310){
				findX60310(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X60311){
				findX60311(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X61000){
				findX61000(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X61010){
				findX61010(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X61011){
				findX61011(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X61100){
				findX61100(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X61110){
				findX61110(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X61111){
				findX61111(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X61200){
				findX61200(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X61210){
				findX61210(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X61211){
				findX61211(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X61300){
				findX61300(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X61310){
				findX61310(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X61311){
				findX61311(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X62000){
				findX62000(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X62010){
				findX62010(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X62011){
				findX62011(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X62100){
				findX62100(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X62110){
				findX62110(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X62111){
				findX62111(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X62200){
				findX62200(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X62210){
				findX62210(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X62211){
				findX62211(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X62300){
				findX62300(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X62310){
				findX62310(sim_rates, cl, j, sd.step_size);
			}
			else if (i == X62311){
				findX62311(sim_rates, cl, j, sd.step_size);
			}
			
			if (isnan(cl.data[i][j])){
				valid = false;
				break;
			}	 
		}
		if (!valid){
			break;
		}
	}
	return valid;
}

void find_GRB (double* rs, con_levels& cl, int c, double ss){
	int prev = c - 1; 
	double prev_GRB_con = cl.data[GRB][prev];
	double prev_G_con = cl.data[G][prev];
	double prev_x01011 = cl.data[X01011][prev];
	double prev_x02011 = cl.data[X02011][prev];
	cl.data[GRB][c] = ss * (-(rs[UNBIN] * prev_GRB_con) 
							+ rs[BIN] * (1 - prev_G_con - prev_GRB_con) * (prev_x01011 + prev_x02011))
							+ prev_GRB_con;
}

void findG (double * rs, con_levels& cl, int current_time, double ss){
	int p= current_time -1;
	double prev_G = cl.data[G][p];
	double prev_GRB = cl.data [GRB][p];
	cl.data[G][current_time]= ss * (- rs[UNBIN] * prev_G
							+ rs[BIN] * (1 - prev_G - prev_GRB) * cl.data[X00011][p])
							+ prev_G;
}

void findGRSRB (double* rs, con_levels& cl, int c, double ss){
	int p= c-1;
	double p_grsrb = cl.data[GRSRB][p];
	cl.data[GRSRB][c] = ss * (-(rs[UNBINR] * p_grsrb)
					+ rs[BINR]* (1 - cl.data[GRS][p] - p_grsrb) * (cl.data[X01011][p] + cl.data[X02011][p]))
					+ p_grsrb;
}

void findGRS(double * rs, con_levels& cl , int c, double ss){
	int p = c-1;
	double p_grs = cl.data[GRS][c-1];
	cl.data[GRS][c] = ss * (-(rs[UNBINR] * p_grs)
					+ rs[BINR] * (1- p_grs- cl.data[GRSRB][p])* cl.data[X00011][p])
					+ p_grs;
}

void findGCR (double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double p_gcr = cl.data[GCR][p];
	cl.data[GCR][c] = ss * (-(rs[UNBINC] * p_gcr)
					+ rs[BINC] * (1- cl.data[GC][p] - p_gcr) * (cl.data[X01011][p] + cl.data[X02011][p]))
					+ p_gcr;
}

void findGC(double* rs, con_levels& cl, int c, double ss){
	int p= c-1;
	double p_gc = cl.data[GC][p];
	cl.data[GC][c] = ss * (- (rs[UNBINC] * p_gc)
					+ rs[BINC] * (1 - p_gc - cl.data[GCR][p]) * cl.data[X00011][p])
					+ p_gc;
}

void findGBR(double* rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double p_gbr = cl.data[GBR][p];
	cl.data[GBR][c] = ss * (- (rs[UNBINREV] * p_gbr)
					+ rs[BINREV]* cl.data[GB][p]*(cl.data[REVN][p] + cl.data[REVNG][p] + cl.data[REVNGP][c] + cl.data[REVNP][c]))
					+ p_gbr;
}

void findGB(double* rs, con_levels& cl , int c, double ss){
	int p = c-1;
	double p_gb= cl.data[GB][p];
	cl.data[GB][c] = ss * (rs[UNBINREV] * cl.data[GBR][p]
					- rs[BINREV] * p_gb * (cl.data[REVN][p] + cl.data[REVNG][p] + cl.data[REVNGP][p] + cl.data[REVNP][p]))
					+ p_gb;
}

void findGBRB(double* rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double p_gbrb = cl.data[GBRB][p];
	cl.data[GBRB][c] = ss * (- (rs[UNBINREVB] * p_gbrb)
					+ rs[BINREVB] * cl.data[GBB][p] * (cl.data[REVN][p] + cl.data[REVNG][p] + cl.data[REVNGP][p] + cl.data[REVNP][p]))
					+ p_gbrb;
}

void findGBB(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double p_gbb = cl.data[GBB][p];
	cl.data[GBB][c] = ss * (rs[UNBINREVB] * cl.data[GBRB][p] 
					- rs[BINREVB] * p_gbb * (cl.data[REVN][p] + cl.data[REVNG][p] + cl.data[REVNGP][p] + cl.data[REVNP][p]))
					+ p_gbb;
}

void findMNPO(double* rs, con_levels& cl, int c, int time, double ss){
	int p = c -1;
	double p_mnpo = cl.data[MNPO][p];
	cl.data[MNPO][c] = ss * (rs[TRPO] * cl.data[G][p]
					- rs[TMC] * p_mnpo
					- rs[UMPO] * p_mnpo)
					+ p_mnpo;
}

void findMCPO(double* rs, con_levels& cl, int c, double ss){
	int p= c-1;
	double p_mcpo= cl.data[MCPO][p];
	cl.data[MCPO][c] = ss * (- (rs[UMPO] * p_mcpo)
					+ rs[TMC] * cl.data[MNPO][p])
					+ p_mcpo;
}

void findMNPT(double* rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double p_mnpt = cl.data[MNPT][p];
	cl.data[MNPT][c] = ss * (rs[TRPT] * cl.data[G][p]
					- rs[TMC] * p_mnpt
					- rs[UMPT] * p_mnpt)
					+ p_mnpt;
}

void findMCPT(double* rs, con_levels& cl, int c, int time, double ss){
	int p = c-1;
	double p_mcpt = cl.data[MCPT][p];
	cl.data[MCPT][c] = ss * (- (rs[UMPT] * p_mcpt)
					+ rs[TMC] * cl.data[MNPT][p])
					+ p_mcpt;
}

void findMNRT(double* rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double p_mnrt = cl.data[MNRT][p];
	cl.data[MNRT][c] = ss * (rs[TRRT] * cl.data[GC][p] 
					- rs[TMC] * p_mnrt 
					- rs[UMRT] * p_mnrt)
					+ p_mnrt;
}

void findMCRT(double* rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double p_mcrt = cl.data[MCRT][p];
	cl.data[MCRT][c] = ss * (- (rs[UMRT] * p_mcrt)
					+ rs[TMC] * cl.data[MNRT][p])
					+ p_mcrt;
}

void findMNREV(double* rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double p_mnrev = cl.data[MNREV][p];
	cl.data[MNREV][c] = ss * (- (rs[TMCREV] * p_mnrev)
						+ rs[UMREV] * p_mnrev
						+ rs[TRREV] * cl.data[GRS][p] * cl.data[X00011][p])
						+ p_mnrev;
}

void findMCREV(double* rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double p_mcrev = cl.data[MCREV][p];
	cl.data[MNREV][c] = ss * (- (rs[UMREV] * p_mcrev)
						+ rs[TMCREV] * cl.data[MNREV][p])
						+ p_mcrev;
}

void findMNRO(double* rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double p_mnro = cl.data[MNRO][p];
	cl.data[MNRO][c] = ss * (rs[TRRO] * cl.data[G][p] * cl.data[GB][p]
					- rs[TMC] * p_mnro
					- rs[UMRO] * p_mnro)
					+ p_mnro;
}

void findMCRO(double* rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double p_mcro = cl.data[MCRO][p];
	cl.data[MCRO][c] = ss * (- (rs[UMRO] * p_mcro)
						+ rs[TMC] * cl.data[MNRO][p])
						+ p_mcro;
}

void findMNB(double *rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double p_mnb = cl.data[MNB][p];
	cl.data[MNB][c] = ss * (rs[TRB] * cl.data[GBB][p]
					- rs[TMC] * p_mnb
					- rs[UMB] * p_mnb) 
					+ p_mnb;
}

void findMCB(double* rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double p_mcb = cl.data[MCB][p];
	cl.data[MCB][c] = ss * (- (rs[UMB] * p_mcb)
					+ rs[TMC] * cl.data[MNB][p])
					+ p_mcb;
}

void findMNNP(double* rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double p_mnnp = cl.data[MNNP][p];
	cl.data[MNNP][c] = ss * (rs[TRNP] * cl.data[GB][p]
					- rs[TMC] * p_mnnp
					- rs[UMNP] * p_mnnp)
					+ p_mnnp;
}

void findMCNP(double* rs, con_levels& cl, int c, double ss){
	int p = c-1 ;
	double p_mcnp = cl.data[MCNP][p];
	cl.data[MCNP][c] = ss * (- (rs[UMNP] * p_mcnp)
					+ rs[TMC] * cl.data[MNNP][p])
					+ p_mcnp;
}

void findB(double* rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double pb = cl.data[B][p];
	cl.data[B][c] = ss * (- (rs[UB] * pb) 
					+ rs[UNCBIN] * cl.data[BC][p] 
					- rs[CBIN] * pb * cl.data[CL][p]
					+ rs[TLB] * cl.data[MCB][p])
					+ pb;
}

void findCL (double* rs, con_levels& cl, int c,double ss){
	int p = c-1;
	double pcl = cl.data[CL][p];
	cl.data[CL][c] = ss * (rs[TLC] 
					+ rs[UNCBIN] * cl.data[BC][p]
					- rs[UC] *	pcl
					- rs[CBIN] * cl.data[B][p] * pcl
					+ rs[TLNP] * cl.data[MCNP][p])
					+ pcl;
}

void findBC(double* rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double pbc = cl.data[BC][p];
	cl.data[BC][c] = ss * (- (rs[PHOS] * pbc)
					- (rs[UBC] * pbc)
					- (rs[UNCBIN] * pbc)
					+ rs[CBIN] * cl.data[B][p] * cl.data[CL][p])
					+ pbc;
}

void findCYREV(double* rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double pcyrev = cl.data[CYREV][p];
	cl.data[CYREV][c] = ss * (- ((rs[NLREV] + rs[UREV]) * pcyrev)
						+ rs[DG] * cl.data[CYREVG][p] 
						+ rs[TLREV] * cl.data[MCREV][p]
						+ rs[NEREV] * cl.data[REVN][p] 
						- rs[AG] * pcyrev * cl.data[X00200][p])
						+ pcyrev;
}

void findREVN(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double prevn = cl.data[REVN][p];
	cl.data[REVN][c] = ss * (rs[NLREV] * cl.data[CYREV][p]
					- (rs[NEREV] + rs[UREV]) * prevn 
					+ rs[DG] * cl.data[REVNG][p] 
					- rs[AG] * rs[NF] * prevn * cl.data[X00210][p])
					+ prevn;
}

void findCYREVG(double* rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double pcyrevg = cl.data[CYREVG][p];
	cl.data[CYREVG][c] = ss * (- (pcyrevg * (rs[DG] + rs[NLREV] + rs[UREV] + cl.data[GTO][p]))
						+ rs[NEREV] * cl.data[REVNG][p] 
						+ rs[AG] * cl.data[CYREV][p] * cl.data[X00200][p])
						+ pcyrevg;
}

void findREVNG(double* rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double prevng = cl.data[REVNG][p];
	cl.data[REVNG][c] = ss * (rs[NLREV] * cl.data[CYREVG][p]
						- (rs[DG] + rs[NEREV] + rs[UREV] + cl.data[GTO][p]) * prevng 
						+ rs[AG] * rs[NF] * cl.data[REVN][p] * cl.data[X00210][p])
						+ prevng;
}

void findCYREVGP(double* rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double pcyrevgp = cl.data[CYREVGP][p];
	cl.data[CYREVGP][c] = ss * (- ((rs[DG] + rs[NLREV] + rs[UPREV]) * pcyrevgp)
						+ cl.data[CYREVG][p] * cl.data[GTO][p] 
						+ rs[NEREV] * cl.data[REVNGP][p])
						+ pcyrevgp;
}

void findREVNGP(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double prevngp = cl.data[REVNGP][p];
	cl.data[REVNGP][c] = ss * (rs[NLREV] * cl.data[CYREVGP][p] 
						+ cl.data[GTO][p] * cl.data[REVNG][p]
						- (rs[DG] + rs[NEREV] + rs[UPREV]) * prevngp)
						+ prevngp;
}

void findCYREVP(double* rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double pcyrevp = cl.data[CYREVP][p];
	cl.data[CYREVP][c] = ss * (rs[DG] * cl.data[CYREVGP][p] 
						- (rs[NLREV] + rs[UPREV]) * pcyrevp
						+ rs[NEREV] * cl.data[REVNP][p])
						+ pcyrevp;
}

void findREVNP(double* rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double prevnp = cl.data[REVNP][p];
	cl.data[REVNP][c] = ss * (rs[NLREV] * cl.data[CYREVP][p]
						+ rs[DG] * cl.data[REVNGP][p]
						- (rs[NEREV] + rs[UPREV]) * prevnp)
						+ prevnp;
}

void findGTO(double* rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double pgto = cl.data[GTO][p];
	cl.data[GTO][c] = ss * (rs[TRGTO] * cl.data[G][p] * cl.data[GB][p]
					- rs[UGTO] * pgto)
					+ pgto;
					
}

void findX00001(double* rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X00001][p];
	cl.data[X00001][c] = ss * (rs[PHOS] * cl.data[BC][p]
						- rs[NLBC] * px 
						- rs[UBC] * px)
						+ px;
}

void findX00011(double* rs, con_levels& cl, int c, double ss){
	int p =c-1;
	double px = cl.data[X00011][p];
	cl.data[X00011][c] = ss * (rs[NLBC] * cl.data[X00001][p]
						- rs[UBC] * px
						+ rs[URO] * cl.data[X01011][p]
						- rs[CBBIN] * rs[NF] * px * (cl.data[X01010][p] + cl.data[X02010][p])
						+ rs[URT] * cl.data[X02011][p] 
						+ rs[UNCBBIN] * (cl.data[X01011][p] + cl.data[X02011][p])
						+ rs[UPU] * (cl.data[X50011][p] + cl.data[X50111][p] + cl.data[X50211][p] + cl.data[X50311][p])
						+ rs[UP] * (cl.data[X20011][p] + cl.data[X20111][p] 
									+ cl.data[X40011][p] + cl.data[X40111][p] + cl.data[X40211][p] + cl.data[X40311][p] 
									+ cl.data[X60011][p] + cl.data[X60111][p] + cl.data[X60211][p] + cl.data[X60311][p])
						- rs[BBIN] * rs[NF] * px * (cl.data[X20010][p] + cl.data[X20110][p] + cl.data[X21010][p] + cl.data[X21110][p] + cl.data[X22010][p] + cl.data[X22110][p]
									+ cl.data[X40010][p] + cl.data[X40110][p] + cl.data[X40210][p] + cl.data[X40310][p]
									+ cl.data[X41010][p] + cl.data[X41110][p] + cl.data[X41210][p] + cl.data[X41310][p]
									+ cl.data[X42010][p] + cl.data[X42110][p] + cl.data[X42210][p] + cl.data[X42310][p]
									+ cl.data[X50010][p] + cl.data[X50110][p] + cl.data[X50210][p] + cl.data[X50310][p]
									+ cl.data[X51010][p] + cl.data[X51110][p] + cl.data[X51210][p] + cl.data[X51310][p]
									+ cl.data[X52010][p] + cl.data[X52110][p] + cl.data[X52210][p] + cl.data[X52310][p]
									+ cl.data[X60010][p] + cl.data[X60110][p] + cl.data[X60210][p] + cl.data[X60310][p]
									+ cl.data[X61010][p] + cl.data[X61110][p] + cl.data[X61210][p] + cl.data[X61310][p]
									+ cl.data[X62010][p] + cl.data[X62110][p] + cl.data[X62210][p] + cl.data[X62310][p])
						+ rs[UNBBIN] * (cl.data[X20011][p]+ cl.data[X20111][p]
									+ cl.data[X21011][p] + cl.data[X21111][p]
									+ cl.data[X22011][p] + cl.data[X22111][p]
									+ cl.data[X40011][p] + cl.data[X40111][p] + cl.data[X40211][p] + cl.data[X40311][p]
									+ cl.data[X41011][p] + cl.data[X41111][p] + cl.data[X41211][p] + cl.data[X41311][p]
									+ cl.data[X42011][p] + cl.data[X42111][p] + cl.data[X42211][p] + cl.data[X42311][p]
									+ cl.data[X50011][p] + cl.data[X50111][p] + cl.data[X50211][p] + cl.data[X50311][p]
									+ cl.data[X51011][p] + cl.data[X51111][p] + cl.data[X51211][p] + cl.data[X51311][p]
									+ cl.data[X52011][p] + cl.data[X52111][p] + cl.data[X52211][p] + cl.data[X52311][p]
									+ cl.data[X60011][p] + cl.data[X60111][p] + cl.data[X60211][p] + cl.data[X60311][p]
									+ cl.data[X61011][p] + cl.data[X61111][p] + cl.data[X61211][p] + cl.data[X61311][p]
									+ cl.data[X62011][p] + cl.data[X62111][p] + cl.data[X62211][p] + cl.data[X62311][p]))
						+ px;
}

void findX00100 (double* rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X00100][p];
	cl.data[X00100][p] = ss * (rs[LNE] * cl.data[X00110][p]
						+ rs[UPU] * (cl.data[X10100][p] + cl.data[X30100][p] + cl.data[X30300][p] + cl.data[X50100][p] + cl.data[X50300][p])
						+ rs[UP] * (cl.data[X20100][p] + cl.data[X40100][p] + cl.data[X40300][p] + cl.data[X60100][p] + cl.data[X60300][p])
						- rs[AC] * px * (cl.data[X10000][p] + cl.data[X30000][p]
										+ cl.data[X20000][p] + cl.data[X21000][p] + cl.data[X22000][p] 
										+ cl.data[X40000][p] + cl.data[X41000][p] + cl.data[X42000][p]
										+ cl.data[X50000][p] + cl.data[X51000][p] + cl.data[X52000][p]
										+ cl.data[X60000][p] + cl.data[X61000][p] + cl.data[X62000][p])
						+ rs[DC] * (cl.data[X10100][p] + cl.data[X30100][p]
									+ cl.data[X20100][p] + cl.data[X21100][p] + cl.data[X22100][p]
									+ cl.data[X40100][p] + cl.data[X41100][p] + cl.data[X42100][p]
									+ cl.data[X50100][p] + cl.data[X51100][p] + cl.data[X52100][p]
									+ cl.data[X60100][p] + cl.data[X61100][p] + cl.data[X62100][p])
						- rs[AC] * px * (cl.data[X30200][p]
										+ cl.data[X40200][p] + cl.data[X41200][p] + cl.data[X42200][p]
										+ cl.data[X50200][p] + cl.data[X51200][p] + cl.data[X52200][p]
										+ cl.data[X60200][p] + cl.data[X61200][p] + cl.data[X62200][p])
						+ rs[DC] * (cl.data[X30300][p]
									+ cl.data[X40300][p] + cl.data[X41300][p] + cl.data[X42300][p]
									+ cl.data[X50300][p] + cl.data[X51300][p] + cl.data[X52300][p]
									+ cl.data[X60300][p] + cl.data[X61300][p] + cl.data[X62300][p]))
						+ px;
}

void findX00110(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X00110][p];
	cl.data[X00110][c] = ss * (- rs[LNE] * px
						+ rs[UPU] * (cl.data[X50110][p] + cl.data[X50111][p] + cl.data[X50310][p] + cl.data[X50311][p])
						+ rs[UP] * (cl.data[X20110][p] + cl.data[X20111][p]
									+ cl.data[X40110][p] + cl.data[X40111][p] + cl.data[X40310][p] + cl.data[X40311][p]
									+ cl.data[X60110][p] + cl.data[X60111][p] + cl.data[X60310][p] + cl.data[X60311][p])
						- rs[AC] * rs[NF] * px * (cl.data[X20010][p] + cl.data[X21010][p] + cl.data[X22010][p]
												+ cl.data[X40010][p] + cl.data[X41010][p] + cl.data[X42010][p]
												+ cl.data[X50010][p] + cl.data[X51010][p] + cl.data[X52010][p]
												+ cl.data[X60010][p] + cl.data[X61010][p] + cl.data[X62010][p])
												
						- rs[AC] * rs[NF] * px * (cl.data[X20011][p] + cl.data[X21011][p] + cl.data[X22011][p]
												+ cl.data[X40011][p] + cl.data[X41011][p] + cl.data[X42011][p]
												+ cl.data[X50011][p] + cl.data[X51011][p] + cl.data[X52011][p]
												+ cl.data[X60011][p] + cl.data[X61011][p] + cl.data[X62011][p])
						
						+ rs[DC] * (cl.data[X20110][p] + cl.data[X21110][p] + cl.data[X22110][p]
									+ cl.data[X40110][p] + cl.data[X41110][p] + cl.data[X42110][p]
									+ cl.data[X50110][p] + cl.data[X51110][p] + cl.data[X52110][p]
									+ cl.data[X60110][p] + cl.data[X61110][p] + cl.data[X62110][p])
						
						+ rs[DC] * (cl.data[X20111][p] + cl.data[X21111][p] + cl.data[X22111][p]
									+ cl.data[X40111][p] + cl.data[X41111][p] + cl.data[X42111][p]
									+ cl.data[X50111][p] + cl.data[X51111][p] + cl.data[X52111][p]
									+ cl.data[X60111][p] + cl.data[X61111][p] + cl.data[X62111][p])
									
						- rs[AC] * rs[NF] * px * (cl.data[X40210][p] + cl.data[X41210][p] + cl.data[X42210][p]
												+ cl.data[X50210][p] + cl.data[X51210][p] + cl.data[X52210][p]
												+ cl.data[X60210][p] + cl.data[X61210][p] + cl.data[X62210][p])
												
						- rs[AC] * rs[NF] * px * (cl.data[X40211][p] + cl.data[X41211][p] + cl.data[X42211][p]
												+ cl.data[X50211][p] + cl.data[X51211][p] + cl.data[X52211][p]
												+ cl.data[X60211][p] + cl.data[X61211][p] + cl.data[X62211][p])
						
						+ rs[DC] * (cl.data[X40310][p] + cl.data[X41310][p] + cl.data[X42310][p]
									+ cl.data[X50310][p] + cl.data[X51310][p] + cl.data[X52310][p]
									+ cl.data[X60310][p] + cl.data[X61310][p] + cl.data[X62310][p])
						
						+ rs[DC] * (cl.data[X40311][p] + cl.data[X41311][p] + cl.data[X42311][p]
									+ cl.data[X50311][p] + cl.data[X51311][p] + cl.data[X52311][p]
									+ cl.data[X60311][p] + cl.data[X61311][p] + cl.data[X62311][p]))
						
						+ px;
}

void findX00200(double* rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X00200][p];
	cl.data[X00200][c] = ss * (rs[DG] * cl.data[CYREVG][p]
						+ rs[UREV] * cl.data[CYREVG][p]
						+ rs[DG] * cl.data[CYREVGP][p]
						+ rs[UPREV] * cl.data[CYREVGP][p]
						- rs[AG] * cl.data[CYREV][p] * px 
						+ rs[LNE] * cl.data[X00210][p] 
						+ rs[UPU] * (cl.data[X30200][p] + cl.data[X30300][p] + cl.data[X50200][p] + cl.data[X50300][p])
						+ rs[UP] * (cl.data[X40200][p] + cl.data[X40300][p] + cl.data[X60200][p] + cl.data[X60300][p])
						- rs[AGP] * px * (cl.data[X30000][p] + cl.data[X30100][p]
						
										+ cl.data[X40000][p] + cl.data[X40100][p] 
										+ cl.data[X41000][p] + cl.data[X41100][p]
										+ cl.data[X42000][p] + cl.data[X42100][p]
										
										+ cl.data[X50000][p] + cl.data[X50100][p] 
										+ cl.data[X51000][p] + cl.data[X51100][p]
										+ cl.data[X52000][p] + cl.data[X52100][p]
										
										+ cl.data[X60000][p] + cl.data[X60100][p] 
										+ cl.data[X61000][p] + cl.data[X61100][p]
										+ cl.data[X62000][p] + cl.data[X62100][p])
						
						+ rs[DG] * (cl.data[X30200][p] + cl.data[X30300][p]
						
									+ cl.data[X40200][p] + cl.data[X40300][p] 
									+ cl.data[X41200][p] + cl.data[X41300][p]
									+ cl.data[X42200][p] + cl.data[X42300][p]
										
									+ cl.data[X50200][p] + cl.data[X50300][p] 
									+ cl.data[X51200][p] + cl.data[X51300][p]
									+ cl.data[X52200][p] + cl.data[X52300][p]
										
									+ cl.data[X60200][p] + cl.data[X60300][p] 
									+ cl.data[X61200][p] + cl.data[X61300][p]
									+ cl.data[X62200][p] + cl.data[X62300][p]))
						
						+ px;
}

void findX00210(double* rs, con_levels& cl, int c, double ss) {
	int p = c-1;
	double px = cl.data[X00210][p];
	cl.data[X00210][c] = ss * (rs[DG] * cl.data[REVNG][p]
						+ rs[UREV] * cl.data[REVNG][p]
						+ rs[DG] * cl.data[REVNGP][p] 
						+ rs[UPREV] * cl.data[REVNGP][p]
						- rs[LNE] * cl.data[X00210][p]
						- rs[AG] * rs[NF] * cl.data[REVN][p] * px
						+ rs[UPU] * (cl.data[X50210][p] + cl.data[X50211][p] + cl.data[X50310][p] + cl.data[X50311][p])
						+ rs[UP] * (cl.data[X40210][p] + cl.data[X40211][p] + cl.data[X40310][p] + cl.data[X40311][p]
									+ cl.data[X60210][p] + cl.data[X60211][p] + cl.data[X60310][p] + cl.data[X60311][p]) 
						- rs[AGP] * rs[NF] * px * (cl.data[X40010][p] + cl.data[X40011][p] + cl.data[X40110][p] + cl.data[X40111][p]
												+ cl.data[X41010][p] + cl.data[X41011][p] + cl.data[X41110][p] + cl.data[X41111][p]
												+ cl.data[X42010][p] + cl.data[X42011][p] + cl.data[X42110][p] + cl.data[X42111][p]
												
												+ cl.data[X50010][p] + cl.data[X50011][p] + cl.data[X50110][p] + cl.data[X50111][p]
												+ cl.data[X51010][p] + cl.data[X51011][p] + cl.data[X51110][p] + cl.data[X51111][p]
												+ cl.data[X52010][p] + cl.data[X52011][p] + cl.data[X52110][p] + cl.data[X52111][p]
												
												+ cl.data[X60010][p] + cl.data[X60011][p] + cl.data[X60110][p] + cl.data[X60111][p]
												+ cl.data[X61010][p] + cl.data[X61011][p] + cl.data[X61110][p] + cl.data[X61111][p]
												+ cl.data[X62010][p] + cl.data[X62011][p] + cl.data[X62110][p] + cl.data[X62111][p])
												
						+ rs[DG] * (cl.data[X40210][p] + cl.data[X40211][p] + cl.data[X40310][p] + cl.data[X40311][p]
									+ cl.data[X41210][p] + cl.data[X41211][p] + cl.data[X41310][p] + cl.data[X41311][p]
									+ cl.data[X42210][p] + cl.data[X42211][p] + cl.data[X42310][p] + cl.data[X42311][p]
									
									+ cl.data[X50210][p] + cl.data[X50211][p] + cl.data[X50310][p] + cl.data[X50311][p]
									+ cl.data[X51210][p] + cl.data[X51211][p] + cl.data[X51310][p] + cl.data[X51311][p]
									+ cl.data[X52210][p] + cl.data[X52211][p] + cl.data[X52310][p] + cl.data[X52311][p]
									
									+ cl.data[X60210][p] + cl.data[X60211][p] + cl.data[X60310][p] + cl.data[X60311][p]
									+ cl.data[X61210][p] + cl.data[X61211][p] + cl.data[X61310][p] + cl.data[X61311][p]
									+ cl.data[X62210][p] + cl.data[X62211][p] + cl.data[X62310][p] + cl.data[X62311][p]))
						+ px;
}

void findX01000(double* rs, con_levels& cl, int c, double ss) {
	int p = c-1;
	double px = cl.data[X01000][p];
	cl.data[X01000][c] = ss * (rs[TLR] * cl.data[MCRO][p] 
					- rs[URO] * px
					- rs[AR] * px * (cl.data[X20000][p] + cl.data[X20100][p]
									+ cl.data[X40000][p] + cl.data[X40100][p] + cl.data[X40200][p] + cl.data[X40300][p]
									+ cl.data[X50000][p] + cl.data[X50100][p] + cl.data[X50200][p] + cl.data[X50300][p]
									+ cl.data[X60000][p] + cl.data[X60100][p] + cl.data[X60200][p] + cl.data[X60300][p])
									
					+ rs[DR] * (cl.data[X21000][p] + cl.data[X21100][p]
								+ cl.data[X41000][p] + cl.data[X41100][p] + cl.data[X41200][p] + cl.data[X41300][p]
								+ cl.data[X51000][p] + cl.data[X51100][p] + cl.data[X51200][p] + cl.data[X51300][p]
								+ cl.data[X61000][p] + cl.data[X61100][p] + cl.data[X61200][p] + cl.data[X61300][p]))
					
					+ px;
}

void findX01010(double* rs, con_levels& cl, int c, double ss) {
	int p = c-1;
	double px = cl.data[X01010][p];
	cl.data[X01010][c] = ss * (- rs[URO] * px
						- rs[CBBIN] * rs[NF] * cl.data[X00011][p] * px
						+ rs[UNCBBIN] * cl.data[X01011][p]
						- rs[AR] * rs[NF] * px * (cl.data[X20010][p] + cl.data[X20110][p]
											+ cl.data[X40010][p] + cl.data[X40110][p] + cl.data[X40210][p] + cl.data[X40310][p]
											+ cl.data[X50010][p] + cl.data[X50110][p] + cl.data[X50210][p] + cl.data[X50310][p]
											+ cl.data[X60010][p] + cl.data[X60110][p] + cl.data[X60210][p] + cl.data[X60310][p])
						- rs[AR] * rs[NF] * px * (cl.data[X20011][p] + cl.data[X20111][p]
											+ cl.data[X40011][p] + cl.data[X40111][p] + cl.data[X40211][p] + cl.data[X40311][p]
											+ cl.data[X50011][p] + cl.data[X50111][p] + cl.data[X50211][p] + cl.data[X50311][p]
											+ cl.data[X60011][p] + cl.data[X60111][p] + cl.data[X60211][p] + cl.data[X60311][p])
						
						+ rs[DR] * (cl.data[X21010][p] + cl.data[X21110][p]
									+ cl.data[X41010][p] + cl.data[X41110][p] + cl.data[X41210][p] + cl.data[X41310][p]
									+ cl.data[X51010][p] + cl.data[X51110][p] + cl.data[X51210][p] + cl.data[X51310][p]
									+ cl.data[X61010][p] + cl.data[X61110][p] + cl.data[X61210][p] + cl.data[X61310][p])
									
						+ rs[DR] * (cl.data[X21011][p] + cl.data[X21111][p]
									+ cl.data[X41011][p] + cl.data[X41111][p] + cl.data[X41211][p] + cl.data[X41311][p]
									+ cl.data[X51011][p] + cl.data[X51111][p] + cl.data[X51211][p] + cl.data[X51311][p]
									+ cl.data[X61011][p] + cl.data[X61111][p] + cl.data[X61211][p] + cl.data[X61311][p]))
						+ px;
}

void findX01011(double* rs, con_levels& cl, int c, double ss) {
	int p = c-1;
	double px = cl.data[X01011][p];
	cl.data[X01011][c] = ss * (rs[CBBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X01010][p]
						- rs[UNCBBIN] * px
						- rs[URO] * px
						- rs[AR] * rs[NF] * px * (cl.data[X20010][p] + cl.data[X20110][p]
											+ cl.data[X40010][p] + cl.data[X40110][p] + cl.data[X40210][p] + cl.data[X40310][p]
											+ cl.data[X50010][p] + cl.data[X50110][p] + cl.data[X50210][p] + cl.data[X50310][p]
											+ cl.data[X60010][p] + cl.data[X60110][p] + cl.data[X60210][p] + cl.data[X60310][p])
						
						+ rs[DR] * (cl.data[X21011][p] + cl.data[X21111][p]
									+ cl.data[X41011][p] + cl.data[X41111][p] + cl.data[X41211][p] + cl.data[X41311][p]
									+ cl.data[X51011][p] + cl.data[X51111][p] + cl.data[X51211][p] + cl.data[X51311][p]
									+ cl.data[X61011][p] + cl.data[X61111][p] + cl.data[X61211][p] + cl.data[X61311][p]))
						
						+ px;
}

void findX02000(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X02000][p];
	cl.data[X02000][c] = ss * (rs[TLR] * cl.data[MCRT][p]
						- rs[URT] * px
						- rs[AR] * px * (cl.data[X20000][p] + cl.data[X20100][p]
									+ cl.data[X40000][p] + cl.data[X40100][p] + cl.data[X40200][p] + cl.data[X40300][p]
									+ cl.data[X50000][p] + cl.data[X50100][p] + cl.data[X50200][p] + cl.data[X50300][p]
									+ cl.data[X60000][p] + cl.data[X60100][p] + cl.data[X60200][p] + cl.data[X60300][p])
						+ rs[DR] * (cl.data[X22000][p] + cl.data[X22100][p]
									+ cl.data[X42000][p] + cl.data[X42100][p] + cl.data[X42200][p] + cl.data[X42300][p]
									+ cl.data[X52000][p] + cl.data[X52100][p] + cl.data[X52200][p] + cl.data[X52300][p]
									+ cl.data[X62000][p] + cl.data[X62100][p] + cl.data[X62200][p] + cl.data[X62300][p]))
						+ px;
}

void findX02010(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X02010][p];
	cl.data[X02010][c] = ss * (- rs[URT] * px
						- rs[CBBIN] * rs[NF] * cl.data[X00011][p] * px
						+ rs[UNCBBIN] * cl.data[X02011][p] 
						- rs[AR] * rs[NF] * px * (cl.data[X20010][p] + cl.data[X20110][p]
											+ cl.data[X40010][p] + cl.data[X40110][p] + cl.data[X40210][p] + cl.data[X40310][p]
											+ cl.data[X50010][p] + cl.data[X50110][p] + cl.data[X50210][p] + cl.data[X50310][p]
											+ cl.data[X60010][p] + cl.data[X60110][p] + cl.data[X60210][p] + cl.data[X60310][p])
						
						- rs[AR] * rs[NF] * px * (cl.data[X20011][p] + cl.data[X20111][p]
											+ cl.data[X40011][p] + cl.data[X40111][p] + cl.data[X40211][p] + cl.data[X40311][p]
											+ cl.data[X50011][p] + cl.data[X50111][p] + cl.data[X50211][p] + cl.data[X50311][p]
											+ cl.data[X60011][p] + cl.data[X60111][p] + cl.data[X60211][p] + cl.data[X60311][p])
						+ rs[DR] * (cl.data[X22010][p] + cl.data[X22110][p]
									+ cl.data[X42010][p] + cl.data[X42110][p] + cl.data[X42210][p] + cl.data[X42310][p]
									+ cl.data[X52010][p] + cl.data[X52110][p] + cl.data[X52210][p] + cl.data[X52310][p]
									+ cl.data[X62010][p] + cl.data[X62110][p] + cl.data[X62210][p] + cl.data[X62310][p])
						+ rs[DR] * (cl.data[X22011][p] + cl.data[X22111][p]
									+ cl.data[X42011][p] + cl.data[X42111][p] + cl.data[X42211][p] + cl.data[X42311][p]
									+ cl.data[X52011][p] + cl.data[X52111][p] + cl.data[X52211][p] + cl.data[X52311][p]
									+ cl.data[X62011][p] + cl.data[X62111][p] + cl.data[X62211][p] + cl.data[X62311][p]))
						+ px;
}

void findX02011(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X02011][p];
	cl.data[X02011][c] = ss * (rs[CBBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X02010][p]
						- rs[UNCBBIN] * px
						- rs[URT] * px
						- rs[AR] * rs[NF] * px *  (cl.data[X20010][p] + cl.data[X20110][p]
											+ cl.data[X40010][p] + cl.data[X40110][p] + cl.data[X40210][p] + cl.data[X40310][p]
											+ cl.data[X50010][p] + cl.data[X50110][p] + cl.data[X50210][p] + cl.data[X50310][p]
											+ cl.data[X60010][p] + cl.data[X60110][p] + cl.data[X60210][p] + cl.data[X60310][p])
						+ rs[DR] * (cl.data[X22011][p] + cl.data[X22111][p]
									+ cl.data[X42011][p] + cl.data[X42111][p] + cl.data[X42211][p] + cl.data[X42311][p]
									+ cl.data[X52011][p] + cl.data[X52111][p] + cl.data[X52211][p] + cl.data[X52311][p]
									+ cl.data[X62011][p] + cl.data[X62111][p] + cl.data[X62211][p] + cl.data[X62311][p]))
						+ px;
}

void findX10000(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X10000][p];
	cl.data[X10000][c] = ss * (rs[TLP] * cl.data[MCPO][p]
						- rs[UPU] * px
						- rs[AC] * cl.data[X00100][p] * px
						+ rs[DC] * cl.data[X10100][p])
						+ px;
}

void findX10100(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X10100][p];
	cl.data[X10100][c] = ss * (rs[AC] * cl.data[X00100][p] * cl.data[X10000][p]
						- rs[DC] * px
						- rs[HOO] * px
						- rs[UPU] * px)
						+ px;
}

void findX20000(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X20000][p];
	cl.data[X20000][c] = ss * (- rs[NL] * px 
						- rs[UP] * px
						- rs[AC] * cl.data[X00100][p] * px
						- rs[AR] * (cl.data[X01000][p] + cl.data[X02000][p]) * px
						+ rs[NE] * cl.data[X20010][p] 
						+ rs[DC] * cl.data[X20100][p]
						+ rs[DR] * (cl.data[X21000][p] + cl.data[X22000][p]))
						+ px ;
}

void findX20010(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X20010][p];
	cl.data[X20010][c] = ss * (rs[NL] * cl.data[X20000][p] 
						- rs[NE] * px
						- rs[UP] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						- rs[AR] * rs[NF] * (cl.data[X01011][p] + cl.data[X02011][p]) * px
						+ rs[UBC] * cl.data[X20011][p]
						+ rs[UNBBIN] * cl.data[X20011][p]
						+ rs[DC] * cl.data[X20110][p]
						+ rs[DR] * (cl.data[X21010][p] + cl.data[X22010][p])
						+ rs[DR] * (cl.data[X21011][p] + cl.data[X22011][p]))
						+ px;
}

void findX20011(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X20011][p];
	cl.data[X20011][c] = ss * (rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X20010][p]
						- rs[UBC] * px
						- rs[UNBBIN] * px
						- rs[UP] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						+ rs[DC] * cl.data[X20111][p]
						+ rs[DR] * (cl.data[X21011][p] + cl.data[X22011][p]))
						+ px;
}

void findX20100(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X20100][p];
	cl.data[X20100][c] = ss * (rs[HOO] * cl.data[X10100][p] 
						+ rs[AC] * cl.data[X00100][p] * cl.data[X20000][p]
						- rs[DC] * px
						- rs[NL] * px
						- rs[UP] * px
						- rs[AR] * (cl.data[X01000][p] + cl.data[X02000][p]) * px
						+ rs[NE] * cl.data[X20110][p]
						+ rs[DR] * (cl.data[X21100][p] + cl.data[X22100][p]))
						+ px;
}

void findX20110(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X20110][p];
	cl.data[X20110][c] = ss * (rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X20010][p] 
						+ rs[NL] * cl.data[X20100][p] 
						- rs[DC] * px
						- rs[NE] * px
						- rs[UP] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						- rs[AR] * rs[NF] * (cl.data[X01011][p] + cl.data[X02011][p]) * px
						+ rs[UBC] * cl.data[X20111][p]
						+ rs[UNBBIN] * cl.data[X20111][p]
						+ rs[DR] * (cl.data[X21110][p] + cl.data[X22110][p])
						+ rs[DR] * (cl.data[X21111][p] + cl.data[X22111][p])) 
						+ px;
}

void findX20111(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X20111][p];
	cl.data[X20111][c] = ss * (rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X20011][p] 
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X20110][p]
						- rs[DC] * px
						- rs[UBC] * px
						- rs[UNBBIN] * px
						- rs[UP] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						+ rs[DR] * (cl.data[X21111][p] + cl.data[X22111][p]))
						+ px;
}

void findX21000(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X21000][p];
	cl.data[X21000][c] = ss * (rs[AR] * cl.data[X01000][p] * cl.data[X20000][p] 
						- rs[DR] * px
						- rs[NL] * px
						- rs[AC] * cl.data[X00100][p] * px
						+ rs[NE] * cl.data[X21010][p]
						+ rs[DC] * cl.data[X21100][p])
						+ px;
}

void findX21010(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X21010][p];
	cl.data[X21010][c] = ss * (rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X20010][p]
						+ rs[NL] * cl.data[X21000][p]
						- rs[DR] * px
						- rs[NE] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						+ rs[UNBBIN] * cl.data[X21011][p]
						+ rs[DC] * cl.data[X21110][p])
						+ px;
}

void findX21011(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X21011][p];
	cl.data[X21011][c] = ss * (rs[AR] * rs[NF] * cl.data[X01011][p] * cl.data[X20010][p]
						+ rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X20011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X21010][p]
						- rs[DR] * px * 2
						- rs[UNBBIN] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						+ rs[DC] * cl.data[X21111][p])
						+ px;
}

void findX21100(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X21100][p];
	cl.data[X21100][c] = ss * (rs[AR] * cl.data[X01000][p] * cl.data[X20100][p]
						+ rs[AC] * cl.data[X00100][p] * cl.data[X21000][p]
						- rs[DC] * px
						- rs[DR] * px
						- rs[NL] * px
						+ rs[NE] * cl.data[X21110][p])
						+ px;
}

void findX21110(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X21110][p];
	cl.data[X21110][c] = ss * (rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X20110][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X21010][p]
						+ rs[NL] * cl.data[X21100][p]
						- rs[DC] * px
						- rs[DR] * px
						- rs[NE] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						+ rs[UNBBIN] * cl.data[X21111][p])
						+ px;
}

void findX21111(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X21111][p];
	cl.data[X21111][c] = ss * (rs[AR] * rs[NF] * cl.data[X01011][p] * cl.data[X20110][p]
						+ rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X20111][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X21011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X21110][p]
						- rs[DC] * px
						- rs[DR] * px * 2
						- rs[UNBBIN] * px)
						+ px;
}

void findX22000(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X22000][p];
	cl.data[X22000][c] = ss * (rs[AR] * cl.data[X02000][p] * cl.data[X20000][p]
						- rs[DR] * px
						- rs[NL] * px
						- rs[AC] * cl.data[X00100][p] * px
						+ rs[NE] * cl.data[X22010][p]
						+ rs[DC] * cl.data[X22100][p])
						+ px;
}

void findX22010(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X22010][p];
	cl.data[X22010][c] = ss * (rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X20010][p]
						+ rs[NL] * cl.data[X22000][p]
						- rs[DR] * px
						- rs[NE] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						+ rs[UNBBIN] * cl.data[X22011][p]
						+ rs[DC] * cl.data[X22110][p])
						+ px;
}

void findX22011(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X22011][p];
	cl.data[X22011][c] = ss * (rs[AR] * rs[NF] * cl.data[X02011][p] * cl.data[X20010][p]
						+ rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X20011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X22010][p]
						- rs[DR] * px * 2
						- rs[UNBBIN] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						+ rs[DC] * cl.data[X22111][p])
						+ px;
}

void findX22100(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X22100][p];
	cl.data[X22100][c] = ss * (rs[AR] * cl.data[X02000][p] * cl.data[X20100][p]
						+ rs[AC] * cl.data[X00100][p] * cl.data[X22000][p]
						- rs[DC] * px
						- rs[DR] * px
						- rs[NL] * px
						+ rs[NE] * cl.data[X22110][p])
						+ px;
}

void findX22110(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X22110][p];
	cl.data[X22110][c] = ss * (rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X20110][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X22010][p]
						+ rs[NL] * cl.data[X22100][p]
						- rs[DC] * px
						- rs[DR] * px
						- rs[NE] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						+ rs[UNBBIN] * cl.data[X22111][p]) 
						+ px;
}

void findX22111(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X22111][p];
	cl.data[X22111][c] = ss * (rs[AR] * rs[NF] * cl.data[X02011][p] * cl.data[X20110][p]
						+ rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X20111][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X22011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X22110][p]
						- rs[DC] * px
						- rs[DR] * px * 2 
						- rs[UNBBIN] * px)
						+ px;
}

void findX30000(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X30000][p];
	cl.data[X30000][c] = ss * (rs[TLP] * cl.data[MCPT][p]
						- rs[UPU] * px
						- rs[AC] * cl.data[X00100][p] * px
						- rs[AGP] * cl.data[X00200][p] * px
						+ rs[DC] * cl.data[X30100][p]
						+ rs[DG] * cl.data[X30200][p])
						+ px;
}

void findX30100(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X30100][p];
	cl.data[X30100][c] = ss * (rs[AC] * cl.data[X00100][p] * cl.data[X30000][p]
						- rs[DC] * px
						- rs[HTO] * px
						- rs[UPU] * px
						- rs[AGP] * cl.data[X00200][p] * px
						+ rs[DG] * cl.data[X30300][p])
						+ px;
}

void findX30200(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X30200][p];
	cl.data[X30200][c] = ss * (rs[AGP] * cl.data[X00200][p] * cl.data[X30000][p]
						- rs[DG] * px
						- rs[UPU] * px
						- cl.data[GTO][p] * px
						- rs[AC] * cl.data[X00100][p] * px
						+ rs[DC] * cl.data[X30300][p])
						+ px;
}

void findX30300(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X30300][p];
	cl.data[X30300][c] = ss * (rs[AGP] * cl.data[X00200][p] * cl.data[X30100][p]
						+ rs[AC] * cl.data[X00100][p] * cl.data[X30200][p]
						- rs[DC] * px
						- rs[DG] * px
						- rs[HTO] * px
						- rs[UPU] * px
						- cl.data[GTO][p] * px)
						+ px;
}

void findX40000(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X40000][p];
	cl.data[X40000][c] = ss * (- rs[NL] * px
						- rs[UP] * px
						- rs[AC] * cl.data[X00100][p] * px
						- rs[AGP] * cl.data[X00200][p] * px
						- rs[AR] * (cl.data[X01000][p] + cl.data[X02000][p]) * px
						+ rs[NE] * cl.data[X40010][p]
						+ rs[DC] * cl.data[X40100][p]
						+ rs[DG] * cl.data[X40200][p]
						+ rs[DR] * (cl.data[X41000][p] + cl.data[X42000][p])) 
						+ px;
}

void findX40010(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X40010][p];
	cl.data[X40010][c] = ss * (rs[NL] * cl.data[X40000][p]
						- rs[NE] * px
						- rs[UP] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						- rs[AR] * rs[NF] * (cl.data[X01011][p] + cl.data[X02011][p]) * px
						+ rs[UBC] * cl.data[X40011][p]
						+ rs[UNBBIN] * cl.data[X40011][p]
						+ rs[DC] * cl.data[X40110][p]
						+ rs[DG] * cl.data[X40210][p]
						+ rs[DR] * (cl.data[X41010][p] + cl.data[X42010][p])
						+ rs[DR] * (cl.data[X41011][p] + cl.data[X42011][p]))
						+ px;
}

void findX40011(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X40011][p];
	cl.data[X40011][c] = ss * (rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X40010][p]
						- rs[UBC] * px
						- rs[UNBBIN] * px
						- rs[UP] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						+ rs[DC] * cl.data[X40111][p]
						+ rs[DG] * cl.data[X40211][p]
						+ rs[DR] * (cl.data[X41011][p] + cl.data[X42011][p]))
						+ px; 
}

void findX40100(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X40100][p];
	cl.data[X40100][c] = ss * (rs[HTO] * cl.data[X30100][p]
						+ rs[AC] * cl.data[X00100][p] * cl.data[X40000][p]
						- rs[DC] * px
						- rs[NL] * px
						- rs[UP] * px
						- rs[AGP] *  cl.data[X00200][p] * px
						- rs[AR] * (cl.data[X01000][p] + cl.data[X02000][p]) * px
						+ rs[NE] * cl.data[X40110][p]
						+ rs[DG] * cl.data[X40300][p]
						+ rs[DR] * (cl.data[X41100][p] + cl.data[X42100][p]))
						+ px;
}

void findX40110(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X40110][p];
	cl.data[X40110][c] = ss * (rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X40010][p]
						+ rs[NL] * cl.data[X40100][p]
						- rs[DC] * px
						- rs[NE] * px
						- rs[UP] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						- rs[AR] * rs[NF] * (cl.data[X01011][p] + cl.data[X02011][p]) * px
						+ rs[UBC] * cl.data[X40111][p]
						+ rs[UNBBIN] * cl.data[X40111][p]
						+ rs[DG] * cl.data[X40310][p]
						+ rs[DR] * (cl.data[X41110][p] + cl.data[X42110][p])
						+ rs[DR] * (cl.data[X41111][p] + cl.data[X42111][p]))
						+ px;
						
}

void findX40111(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X40111][p];
	cl.data[X40111][c] = ss * (rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X40011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X40110][p]
						- rs[DC] * px
						- rs[UBC] * px
						- rs[UNBBIN] * px
						- rs[UP] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						+ rs[DG] * cl.data[X40311][p]
						+ rs[DR] * (cl.data[X41111][p] + cl.data[X42111][p]))
						+ px;
}

void findX40200(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X40200][p];
	cl.data[X40200][c] = ss * (rs[AGP] *  cl.data[X00200][p] * cl.data[X40000][p]
						- rs[DG] * px
						- rs[NL] * px
						- rs[UP] * px
						- cl.data[GTO][p] * px
						- rs[AC] * cl.data[X00100][p] * px
						- rs[AR] * (cl.data[X01000][p] + cl.data[X02000][p]) * px
						+ rs[NE] * cl.data[X40210][p]
						+ rs[DC] * cl.data[X40300][p]
						+ rs[DR] * (cl.data[X41200][p] + cl.data[X42200][p]))
						+ px;
}

void findX40210(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X40210][p];
	cl.data[X40210][c] = ss * (rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X40010][p]
						+ rs[NL] * cl.data[X40200][p]
						- rs[DG] * px
						- rs[NE] * px
						- rs[UP] * px
						- cl.data[GTO][p] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						- rs[AR] * rs[NF] * (cl.data[X01011][p] + cl.data[X02011][p]) * px
						+ rs[UBC] * cl.data[X40211][p]
						+ rs[UNBBIN] * cl.data[X40211][p]
						+ rs[DC] * cl.data[X40310][p]
						+ rs[DR] * (cl.data[X41210][p] + cl.data[X42210][p])
						+ rs[DR] * (cl.data[X41211][p] + cl.data[X42211][p]))
						+ px;
}

void findX40211(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X40211][p];
	cl.data[X40211][c] = ss * (rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X40011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X40210][p]
						- rs[DG] * px
						- rs[UBC] * px
						- rs[UNBBIN] * px
						- rs[UP] * px
						- cl.data[GTO][p] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						+ rs[DC] * cl.data[X40311][p]
						+ rs[DR] * (cl.data[X41211][p] + cl.data[X42211][p]))
						+ px;
}

void findX40300(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X40300][p];
	cl.data[X40300][c] = ss * (rs[HTO] * cl.data[X30300][p] 
						+ rs[AGP] *  cl.data[X00200][p] * cl.data[X40100][p]
						+ rs[AC] * cl.data[X00100][p] * cl.data[X40200][p]
						- rs[DC] * px
						- rs[DG] * px
						- rs[NL] * px
						- rs[UP] * px
						- cl.data[GTO][p] * px
						- rs[AR] * (cl.data[X01000][p] + cl.data[X02000][p]) * px
						+ rs[NE] * cl.data[X40310][p]
						+ rs[DR] * (cl.data[X41300][p] + cl.data[X42300][p]))
						+ px;
}	

void findX40310(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X40310][p];
	cl.data[X40310][c] = ss * (rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X40110][p]		
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X40210][p]
						+ rs[NL] * cl.data[X40300][p]
						- rs[DC] * px
						- rs[DG] * px
						- rs[NE] * px
						- rs[UP] * px
						- cl.data[GTO][p] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						- rs[AR] * rs[NF] * (cl.data[X01011][p] + cl.data[X02011][p]) * px
						+ rs[UBC] * cl.data[X40311][p]
						+ rs[UNBBIN] * cl.data[X40311][p]
						+ rs[DR] * (cl.data[X41310][p] + cl.data[X42310][p])
						+ rs[DR] * (cl.data[X41311][p] + cl.data[X42311][p]))
						+ px;
}		

void findX40311(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X40311][p];
	cl.data[X40311][c] = ss * (rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X40111][p]		
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X40211][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X40310][p]
						- rs[DC] * px
						- rs[DG] * px
						- rs[UBC] * px
						- rs[UNBBIN] * px
						- rs[UP] * px
						- cl.data[GTO][p] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						+ rs[DR] * (cl.data[X41311][p] + cl.data[X42311][p]))
						+ px;
}	

void findX41000(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X41000][p];
	cl.data[X41000][c] = ss * (rs[AR] * cl.data[X01000][p] * cl.data[X40000][p]
						- rs[DR] * px
						- rs[NL] * px
						- rs[AC] * cl.data[X00100][p] * px
						- rs[AGP] * cl.data[X00200][p] * px
						+ rs[NE] * cl.data[X41010][p] 
						+ rs[DC] * cl.data[X41100][p] 
						+ rs[DG] * cl.data[X41200][p] )
						+ px;
}

void findX41010(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X41010][p];
	cl.data[X41010][c] = ss * (rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X40010][p]	
						+ rs[NL] * cl.data[X41000][p] 
						- rs[DR] * px
						- rs[NE] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px	
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						+ rs[UNBBIN] * cl.data[X41011][p]
						+ rs[DC] * cl.data[X41110][p] 
						+ rs[DG] * cl.data[X41210][p]) 
						+ px;
}

void findX41011(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X41011][p];
	cl.data[X41011][c] = ss * (rs[AR] * rs[NF] * cl.data[X01011][p] * cl.data[X40010][p]
						+ rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X40011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X41010][p]
						- rs[DR] * px * 2
						- rs[UNBBIN] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						+ rs[DC] * cl.data[X41111][p] 
						+ rs[DG] * cl.data[X41211][p]) 
						+ px;
}

void findX41100(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X41100][p];
	cl.data[X41100][c] = ss * (rs[AR] * cl.data[X01000][p] * cl.data[X40100][p]
						+ rs[AC] * cl.data[X00100][p] * cl.data[X41000][p]
						- rs[DC] * px
						- rs[DR] * px
						- rs[NL] * px
						- rs[AGP] * cl.data[X00200][p] * px
						+ rs[NE] * cl.data[X41110][p] 
						+ rs[DG] * cl.data[X41300][p])  
						+ px;
}

void findX41110(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X41110][p];
	cl.data[X41110][c] = ss * (rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X40110][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X41010][p]
						+ rs[NL] * cl.data[X41100][p] 
						- rs[DC] * px
						- rs[DR] * px
						- rs[NE] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X41110][p]
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						+ rs[UNBBIN] * cl.data[X41111][p]
						+ rs[DG] * cl.data[X41310][p]) 
						+ px;
}

void findX41111(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X41111][p];
	cl.data[X41111][c] = ss * (rs[AR] * rs[NF] * cl.data[X01011][p] * cl.data[X40110][p]
						+ rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X40111][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X41011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X41110][p]
						- rs[DC] * px
						- rs[DR] * px *2
						- rs[UNBBIN] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						+ rs[DG] * cl.data[X41311][p]) 
						+ px;
}

void findX41200(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X41200][p];
	cl.data[X41200][c] = ss * (rs[AR] * cl.data[X01000][p] * cl.data[X40200][p]
						+ rs[AGP] * cl.data[X00200][p] * cl.data[X41000][p]
						- rs[DG] * px
						- rs[DR] * px
						- rs[NL] * px
						- cl.data[GTO][p] * px
						- rs[AC] * cl.data[X00100][p] * px
						+ rs[NE] * cl.data[X41210][p] 
						+ rs[DC] * cl.data[X41300][p])
						+ px; 
}

void findX41210(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X41210][p];
	cl.data[X41210][c] = ss * (rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X40210][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X41010][p]
						+ rs[NL] * cl.data[X41200][p] 
						- rs[DG] * px
						- rs[DR] * px
						- rs[NE] * px
						- cl.data[GTO][p] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X41210][p]
						- rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X41210][p]
						+ rs[UNBBIN] * cl.data[X41211][p]
						+ rs[DC] * cl.data[X41310][p] )
						+ px;
}

void findX41211(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X41211][p];
	cl.data[X41211][c] = ss * (rs[AR] * rs[NF] * cl.data[X01011][p] * cl.data[X40210][p]
						+ rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X40211][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X41011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X41210][p]
						- rs[DG] * px
						- rs[DR] * px * 2
						- rs[UNBBIN] * px
						- cl.data[GTO][p] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X41211][p]
						+ rs[DC] * cl.data[X41311][p]) 
						+ px;
}

void findX41300(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X41300][p];
	cl.data[X41300][c] = ss * (rs[AR] * cl.data[X01000][p] * cl.data[X40300][p]
						+ rs[AGP] * cl.data[X00200][p] * cl.data[X41100][p]
						+ rs[AC] * cl.data[X00100][p] * cl.data[X41200][p]
						- rs[DG] * px
						- rs[DR] * px 
						- rs[DC] * px 
						- rs[NL] * px
						- cl.data[GTO][p] * px
						+ rs[NE] * cl.data[X41310][p]) 
						+ px;
}

void findX41310(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X41310][p];
	cl.data[X41310][c] = ss * (rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X40310][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X41110][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X41210][p]
						+ rs[NL] * cl.data[X41300][p]
						- rs[DG] * px
						- rs[DR] * px 
						- rs[DC] * px 
						- rs[NE] * px
						- cl.data[GTO][p] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X41310][p]
						+ rs[UNBBIN] * cl.data[X41311][p])
						+ px;
}

void findX41311(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X41311][p];
	cl.data[X41311][c] = ss * (rs[AR] * rs[NF] * cl.data[X01011][p] * cl.data[X40310][p]
						+ rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X40311][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X41111][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X41211][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X41310][p]
						- rs[DG] * px
						- rs[DR] * px * 2
						- rs[DC] * px 
						- rs[UNBBIN] * px
						- cl.data[GTO][p] * px)
						+ px;
}

void findX42000(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X42000][p];
	cl.data[X42000][c] = ss * (rs[AR] * cl.data[X02000][p] * cl.data[X40000][p]
						- rs[DR] * px
						- rs[NL] * px
						- rs[AC] * cl.data[X00100][p] * cl.data[X42000][p]
						- rs[AGP] * cl.data[X00200][p] * cl.data[X42000][p]
						+ rs[NE] * cl.data[X42010][p]
						+ rs[DC] * cl.data[X42100][p]
						+ rs[DG] * cl.data[X42200][p])
						+ px;
}

void findX42010(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X42010][p];
	cl.data[X42010][c] = ss * (rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X40010][p]
						+ rs[NL] * cl.data[X42000][p]
						- rs[DR] * px 
						- rs[NE] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						+ rs[UNBBIN] * cl.data[X42011][p]
						+ rs[DC] * cl.data[X42110][p]
						+ rs[DG] * cl.data[X42210][p])
						+ px;
}

void findX42011(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X42011][p];
	cl.data[X42011][c] = ss * (rs[AR] * rs[NF] * cl.data[X02011][p] * cl.data[X40010][p]
						+ rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X40011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X42010][p]
						- rs[DR] * px * 2
						- rs[UNBBIN] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						+ rs[DC] * cl.data[X42111][p]
						+ rs[DG] * cl.data[X42211][p])
						+ px;
}

void findX42100(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X42100][p];
	cl.data[X42100][c] = ss * (rs[AR] * cl.data[X02000][p] * cl.data[X40100][p]
						+ rs[AC] * cl.data[X00100][p] * cl.data[X42000][p]
						- rs[DR] * px 
						- rs[DC] * px 
						- rs[NL] * px
						- rs[AGP] * cl.data[X00200][p] * px
						+ rs[NE] * cl.data[X42110][p]
						+ rs[DG] * cl.data[X42300][p])
						+ px;
} 

void findX42110(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X42110][p];
	cl.data[X42110][c] = ss * (rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X40110][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X42010][p]
						+ rs[NL] * cl.data[X42100][p]
						- rs[DR] * px 
						- rs[DC] * px 
						- rs[NE] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						+ rs[UNBBIN] * cl.data[X42111][p]
						+ rs[DG] * cl.data[X42310][p])
						+ px;
}

void findX42111(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X42111][p];
	cl.data[X42111][c] = ss * (rs[AR] * rs[NF] * cl.data[X02011][p] * cl.data[X40110][p]
						+ rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X40111][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X42011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X42110][p]
						- rs[DR] * px * 2
						- rs[DC] * px 
						- rs[UNBBIN] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						+ rs[DG] * cl.data[X42311][p])
						+ px;
}

void findX42200(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X42200][p];
	cl.data[X42200][c] = ss * (rs[AR] * cl.data[X02000][p] * cl.data[X40200][p]
						+ rs[AGP] * cl.data[X00200][p] * cl.data[X42000][p]
						- rs[DR] * px 
						- rs[DG] * px 
						- rs[NL] * px
						- cl.data[GTO][p] * px 
						- rs[AC] * cl.data[X00100][p] * px
						+ rs[NE] * cl.data[X42210][p]
						+ rs[DC] * cl.data[X42300][p])
						+ px;
}

void findX42210(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X42210][p];
	cl.data[X42210][c] = ss * (rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X40210][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X42010][p]
						+ rs[NL] * cl.data[X42200][p]
						- rs[DR] * px 
						- rs[DG] * px
						- rs[NE] * px
						- cl.data[GTO][p] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						+ rs[UNBBIN] * cl.data[X42211][p]
						+ rs[DC] * cl.data[X42310][p])
						+ px;
}

void findX42211(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X42211][p];
	cl.data[X42211][c] = ss * (rs[AR] * rs[NF] * cl.data[X02011][p] * cl.data[X40210][p]
						+ rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X40211][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X42011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X42210][p]
						- rs[DR] * px * 2 
						- rs[DG] * px
						- rs[UNBBIN] * px
						- cl.data[GTO][p] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						+ rs[DC] * cl.data[X42311][p]) 
						+ px;
}

void findX42300(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X42300][p];
	cl.data[X42300][c] = ss * (rs[AR] * cl.data[X02000][p] * cl.data[X40300][p]
						+ rs[AGP] * cl.data[X00200][p] * cl.data[X42100][p]
						+ rs[AC] * cl.data[X00100][p] * cl.data[X42200][p]
						- rs[DC] * px
						- rs[DR] * px 
						- rs[DG] * px
						- rs[NL] * px
						- cl.data[GTO][p] * px
						+ rs[NE] * cl.data[X42310][p])
						+ px;
}

void findX42310(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X42310][p];
	cl.data[X42310][c] = ss * (rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X40310][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X42110][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X42210][p]
						+ rs[NL] * cl.data[X42300][p]
						- rs[DC] * px
						- rs[DR] * px 
						- rs[DG] * px
						- rs[NE] * px
						- cl.data[GTO][p] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X42310][p]
						+ rs[UNBBIN] * cl.data[X42311][p])
						+ px;
}

void findX42311(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X42311][p];
	cl.data[X42311][c] = ss * (rs[AR] * rs[NF] * cl.data[X02011][p] * cl.data[X40310][p]
						+ rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X40311][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X42111][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X42211][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X42310][p]
						- rs[DC] * px
						- rs[DR] * px * 2
						- rs[DG] * px
						- rs[UNBBIN] * px
						- cl.data[GTO][p] * px)
						+ px;
}

void findX50000(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X50000][p];
	cl.data[X50000][c] = ss * (- rs[NL] * px
						- rs[UPU] * px
						- rs[AC] * cl.data[X00100][p] * px
						- rs[AGP] * cl.data[X00200][p] * px
						- rs[AR] * (cl.data[X01000][p] + cl.data[X02000][p]) * px
						+ rs[NE] * cl.data[X50010][p]
						+ rs[DC] * cl.data[X50100][p]
						+ rs[DG] * cl.data[X50200][p]
						+ rs[DR] * (cl.data[X51000][p] + cl.data[X52000][p]))
						+ px;
}

void findX50010(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X50010][p];
	cl.data[X50010][c] = ss * (rs[NL] * cl.data[X50000][p]
						- rs[NE] * px
						- rs[UPU] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						- rs[AR] * rs[NF] * (cl.data[X01011][p] + cl.data[X02011][p]) * px
						+ rs[UBC] * cl.data[X50011][p]
						+ rs[UNBBIN] * cl.data[X50011][p]
						+ rs[DC] * cl.data[X50110][p]
						+ rs[DG] * cl.data[X50210][p]
						+ rs[DR] * (cl.data[X51010][p] + cl.data[X52010][p])
						+ rs[DR] * (cl.data[X51011][p] + cl.data[X52011][p]))
						+ px;
}

void findX50011(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X50011][p];
	cl.data[X50011][c] = ss * (rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X50010][p]
						- rs[UBC] * px
						- rs[UNBBIN] * px
						- rs[UPU] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						+ rs[DC] * cl.data[X50111][p]
						+ rs[DG] * cl.data[X50211][p]
						+ rs[DR] * (cl.data[X51011][p] + cl.data[X52011][p]))
						+ px;
}

void findX50100(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X50100][p];
	cl.data[X50100][c] = ss * (rs[AC] * cl.data[X00100][p] * cl.data[X50000][p]
						- rs[DC] * px
						- rs[HTO] * px
						- rs[NL] * px
						- rs[UPU] * px
						- rs[AGP] * cl.data[X00200][p] * px
						- rs[AR] * (cl.data[X01000][p] + cl.data[X02000][p]) * px
						+ rs[NE] * cl.data[X50110][p]
						+ rs[DG] * cl.data[X50300][p]
						+ rs[DR] * (cl.data[X51100][p] + cl.data[X52100][p]))
						+ px;
}

void findX50110(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X50110][p];
	cl.data[X50110][c] =  ss * (rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X50010][p]
						+ rs[NL] * cl.data[X50100][p]
						- rs[DC] * px
						- rs[HTO] * px
						- rs[NE] * px
						- rs[UPU] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						- rs[AR] * rs[NF] * (cl.data[X01011][p] + cl.data[X02011][p]) * px
						+ rs[UBC] * cl.data[X50111][p]
						+ rs[UNBBIN] * cl.data[X50111][p]
						+ rs[DG] * cl.data[X50310][p]
						+ rs[DR] * (cl.data[X51110][p] + cl.data[X52110][p])
						+ rs[DR] * (cl.data[X51111][p] + cl.data[X52111][p]))
						+ px;
}

void findX50111(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X50111][p];
	cl.data[X50111][c] =  ss * (rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X50011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X50110][p]
						- rs[DC] * px
						- rs[HTO] * px
						- rs[UBC] * px
						- rs[UNBBIN] * px
						- rs[UPU] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						+ rs[DG] * cl.data[X50311][p]
						+ rs[DR] * (cl.data[X51111][p] + cl.data[X52111][p]))
						+ px;
}

void findX50200(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X50200][p];
	cl.data[X50200][c] = ss * (cl.data[GTO][p] * cl.data[X30200][p]
						+ rs[AGP] * cl.data[X00200][p] * cl.data[X50000][p]
						- rs[DC] * px
						- rs[NL] * px
						- rs[UPU] * px
						- rs[AC] * cl.data[X00100][p] * px
						- rs[AR] * (cl.data[X01000][p] + cl.data[X02000][p]) * px
						+ rs[NE] * cl.data[X50210][p]
						+ rs[DC] * cl.data[X50300][p]
						+ rs[DR] * (cl.data[X51200][p] + cl.data[X52200][p]))
						+ px;
}

void findX50210(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X50210][p];
	cl.data[X50210][c] = ss * (rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X50010][p]
						+ rs[NL] * cl.data[X50200][p]
						- rs[DG] * px
						- rs[NE] * px
						- rs[UPU] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						- rs[AR] * rs[NF] * (cl.data[X01011][p] + cl.data[X02011][p]) * px
						+ rs[UBC] * cl.data[X50211][p]
						+ rs[UNBBIN] * cl.data[X50211][p]
						+ rs[DC] * cl.data[X50310][p]
						+ rs[DR] * (cl.data[X51210][p] + cl.data[X52210][p])
						+ rs[DR] * (cl.data[X51211][p] + cl.data[X52211][p]))
						+ px;
}

void findX50211(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X50211][p];
	cl.data[X50211][c] = ss * (rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X50011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X50210][p]
						- rs[DG] * px
						- rs[UBC] * px
						- rs[UNBBIN] * px
						- rs[UPU] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X50211][p]
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						+ rs[DC] * cl.data[X50311][p]
						+ rs[DR] * (cl.data[X51211][p] + cl.data[X52211][p]))
						+ px;
} 

void findX50300(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X50300][p];
	cl.data[X50300][c] = ss * (cl.data[GTO][p] * cl.data[X30300][p]
						+ rs[AGP] * cl.data[X00200][p] * cl.data[X50100][p]
						+ rs[AC] * cl.data[X00100][p] * cl.data[X50200][p]
						- rs[DG] * px
						- rs[DC] * px
						- rs[HTO] * px
						- rs[NL] * px
						- rs[UPU] * px
						- rs[AR] * (cl.data[X01000][p] + cl.data[X02000][p]) * px
						+ rs[NE] * cl.data[X50310][p]
						+ rs[DR] * (cl.data[X51300][p] + cl.data[X52300][p]))
						+ px;
}

void findX50310(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X50310][p];
	cl.data[X50310][c] = ss * (rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X50110][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X50210][p]
						+ rs[NL] * cl.data[X50300][p]
						- rs[DG] * px
						- rs[DC] * px
						- rs[HTO] * px
						- rs[NE] * px
						- rs[UPU] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						- rs[AR] * rs[NF] * (cl.data[X01011][p] + cl.data[X02011][p]) * px
						+ rs[UBC] * cl.data[X50311][p]
						+ rs[UNBBIN] * cl.data[X50311][p]
						+ rs[DR] * (cl.data[X51310][p] + cl.data[X52310][p])
						+ rs[DR] * (cl.data[X51311][p] + cl.data[X52311][p]))
						+ px;
}

void findX50311(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X50311][p];
	cl.data[X50311][c] = ss * (rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X50111][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X50211][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X50310][p]
						- rs[DG] * px
						- rs[DC] * px
						- rs[HTO] * px
						- rs[UBC] * px
						- rs[UNBBIN] * px
						- rs[UPU] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						+ rs[DR] * (cl.data[X51311][p] + cl.data[X52311][p]))
						+ px;
}

void findX51000(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X51000][p];
	cl.data[X51000][c] = ss * (rs[AC] * cl.data[X01000][p] * cl.data[X50000][p]
						- rs[DR] * px
						- rs[NL] * px
						- rs[AC] * cl.data[X00100][p] * px
						- rs[AGP] * cl.data[X00200][p] * px
						+ rs[NE] * cl.data[X51010][p]
						+ rs[DC] * cl.data[X51100][p]
						+ rs[DG] * cl.data[X51200][p])
						+ px;
}

void findX51010(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X51010][p];
	cl.data[X51010][c] = ss * (rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X50010][p]
						+ rs[NL] * cl.data[X51000][p]
						- rs[DR] * px
						- rs[NE] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X51010][p]
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						+ rs[UNBBIN] * cl.data[X51011][p]
						+ rs[DC] * cl.data[X51110][p]
						+ rs[DG] * cl.data[X51210][p])
						+ px;
}

void findX51011(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X51011][p];
	cl.data[X51011][c] = ss * (rs[AR] * rs[NF] * cl.data[X01011][p] * cl.data[X50010][p]
						+ rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X50011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X51010][p]
						- rs[DR] * px * 2
						- rs[UNBBIN] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X51011][p]
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						+ rs[DC] * cl.data[X51111][p]
						+ rs[DG] * cl.data[X51211][p])
						+ px;
}

void findX51100(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X51100][p];
	cl.data[X51100][c] = ss * (rs[AR] * cl.data[X01000][p] * cl.data[X50100][p]
						+ rs[AC] * cl.data[X00100][p] * cl.data[X51000][p]
						- rs[DC] * px
						- rs[DR] * px
						- rs[NL] * px
						- rs[AGP] * cl.data[X00200][p] * px
						+ rs[NE] * cl.data[X51110][p]
						+ rs[DG] * cl.data[X51300][p])
						+ px;
}

void findX51110(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X51110][p];
	cl.data[X51110][c] = ss * (rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X50110][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X51010][p]
						+ rs[NL] * cl.data[X51100][p]
						- rs[DC] * px
						- rs[DR] * px
						- rs[NE] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X51110][p]
						+ rs[UNBBIN] * cl.data[X51111][p]
						+ rs[DG] * cl.data[X51310][p])
						+ px;
}

void findX51111(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X51111][p];
	cl.data[X51111][c] = ss * (rs[AR] * rs[NF] * cl.data[X01011][p] * cl.data[X50110][p]
						+ rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X50111][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X51011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X51110][p]
						- rs[DC] * px
						- rs[DR] * px * 2
						- rs[UNBBIN] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						+ rs[DG] * cl.data[X51311][p])
						+ px;
}

void findX51200(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X51200][p];
	cl.data[X51200][c] = ss * (rs[AR] * cl.data[X01000][p] * cl.data[X50200][p]
						+ rs[AGP] * cl.data[X00200][p] * cl.data[X51000][p]
						- rs[DG] * px
						- rs[DR] * px 
						- rs[NL] * px
						- rs[AC] * cl.data[X00100][p] * px
						+ rs[NE] * cl.data[X51210][p]
						+ rs[DC] * cl.data[X51300][p])
						+ px;
}

void findX51210(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X51210][p];
	cl.data[X51210][c] = ss * (rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X50210][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X51010][p]
						+ rs[NL] * cl.data[X51200][p]
						- rs[DG] * px
						- rs[DR] * px 
						- rs[NE] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						+ rs[UNBBIN] * cl.data[X51211][p]
						+ rs[DC] * cl.data[X51310][p])
						+ px;
}

void findX51211(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X51211][p];
	cl.data[X51211][c] = ss * (rs[AR] * rs[NF] * cl.data[X01011][p] * cl.data[X50210][p]
						+ rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X50211][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X51011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X51210][p]
						- rs[DG] * px
						- rs[DR] * px * 2
						- rs[UNBBIN] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						+ rs[DC] * cl.data[X51311][p])
						+ px;
}

void findX51300(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X51300][p];
	cl.data[X51300][c] = ss * (rs[AR] * cl.data[X01000][p] * cl.data[X50300][p]
						+ rs[AGP] * cl.data[X00200][p] * cl.data[X51100][p]
						+ rs[AC] * cl.data[X00100][p] * cl.data[X51200][p]
						- rs[DC] * px
						- rs[DG] * px
						- rs[DR] * px 
						- rs[NL] * px
						+ rs[NE] * cl.data[X51310][p])
						+ px;
}

void findX51310(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X51310][p];
	cl.data[X51310][c] = ss * (rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X50310][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X51110][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X51210][p]
						+ rs[NL] * cl.data[X51300][p]
						- rs[DC] * px
						- rs[DG] * px
						- rs[DR] * px 
						- rs[NE] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						+ rs[UNBBIN] * cl.data[X51311][p])
						+ px;
}

void findX51311(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X51311][p];
	cl.data[X51311][c] = ss * (rs[AR] * rs[NF] * cl.data[X01011][p] * cl.data[X50310][p]
						+ rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X50311][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X51111][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X51211][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X51310][p]
						- rs[DC] * px
						- rs[DG] * px
						- rs[DR] * px * 2
						- rs[UNBBIN] * px)
						+ px;
}

void findX52000(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X52000][p];
	cl.data[X52000][c] = ss * (rs[AR] * cl.data[X02000][p] * cl.data[X50000][p]
						- rs[DR] * px
						- rs[NL] * px
						- rs[AC] * cl.data[X00100][p] * px
						- rs[AGP] * cl.data[X00200][p] * cl.data[X52000][p]
						+ rs[NE] * cl.data[X52010][p]
						+ rs[DC] * cl.data[X52100][p]
						+ rs[DG] * cl.data[X52200][p])
						+ px;
}

void findX52010(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X52010][p];
	cl.data[X52010][c] = ss * (rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X50010][p]
						+ rs[NL] * cl.data[X52000][p]
						- rs[DR] * px
						- rs[NE] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						+ rs[UNBBIN] * cl.data[X52011][p]
						+ rs[DC] * cl.data[X52110][p]
						+ rs[DG] * cl.data[X52210][p])
						+ px;
}

void findX52011(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X52011][p];
	cl.data[X52011][c] = ss * (rs[AR] * rs[NF] * cl.data[X02011][p] * cl.data[X50010][p]
						+ rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X50011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X52010][p]
						- rs[DR] * px * 2
						- rs[UNBBIN] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						+ rs[DC] * cl.data[X52111][p]
						+ rs[DG] * cl.data[X52211][p])
						+ px;
}

void findX52100(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X52100][p];
	cl.data[X52100][c] = ss * (rs[AR] * cl.data[X02000][p] * cl.data[X50100][p]
						+ rs[AC] * cl.data[X00100][p] * cl.data[X52000][p]
						- rs[DR] * px
						- rs[DC] * px
						- rs[NL] * px
						- rs[AGP] * cl.data[X00200][p] * cl.data[X52100][p]
						+ rs[NE] * cl.data[X52110][p]
						+ rs[DG] * cl.data[X52300][p])
						+ px;
}

void findX52110(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X52110][p];
	cl.data[X52110][c] = ss * (rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X50110][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X52010][p]
						+ rs[NL] * cl.data[X52100][p]
						- rs[DR] * px
						- rs[DC] * px
						- rs[NE] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						+ rs[UNBBIN] * cl.data[X52111][p]
						+ rs[DG] * cl.data[X52310][p])
						+ px;
}

void findX52111(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X52111][p];
	cl.data[X52111][c] = ss * (rs[AR] * rs[NF] * cl.data[X02011][p] * cl.data[X50110][p]
						+ rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X50111][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X52011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X52110][p]
						- rs[DR] * px * 2
						- rs[DC] * px
						- rs[UNBBIN] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						+ rs[DG] * cl.data[X52311][p])
						+ px;
}

void findX52200(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X52200][p];
	cl.data[X52200][c] = ss * (rs[AR] * cl.data[X02000][p] * cl.data[X50200][p]
						+ rs[AGP] * cl.data[X00200][p] * cl.data[X52000][p]
						- rs[DR] * px
						- rs[DG] * px
						- rs[NL] * px
						- rs[AC] * cl.data[X00100][p] * px
						+ rs[NE] * cl.data[X52210][p]
						+ rs[DC] * cl.data[X52300][p])
						+ px;
}

void findX52210(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X52210][p];
	cl.data[X52210][c] = ss * (rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X50210][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X52010][p]
						+ rs[NL] * cl.data[X52200][p]
						- rs[DR] * px
						- rs[DG] * px
						- rs[NE] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						+ rs[UNBBIN] * cl.data[X52211][p]
						+ rs[DC] * cl.data[X52310][p])
						+ px;
}

void findX52211(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X52211][p];
	cl.data[X52211][c] = ss * (rs[AR] * rs[NF] * cl.data[X02011][p] * cl.data[X50210][p]
						+ rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X50211][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X52011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X52210][p]
						- rs[DR] * px * 2
						- rs[DG] * px
						- rs[UNBBIN] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						+ rs[DC] * cl.data[X52311][p])
						+ px;
}

void findX52300(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X52300][p];
	cl.data[X52300][c] = ss * (rs[AR] * cl.data[X02000][p] * cl.data[X50300][p]
						+ rs[AGP] * cl.data[X00200][p] * cl.data[X52100][p]
						+ rs[AC] * cl.data[X00100][p] * cl.data[X52200][p]
						- rs[DC] * px
						- rs[DR] * px
						- rs[DG] * px
						- rs[NL] * px
						+ rs[NE] * cl.data[X52310][p])
						+ px;
}

void findX52310(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X52310][p];
	cl.data[X52310][c] = ss * (rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X50310][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X52110][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X52210][p]
						+ rs[NL] * cl.data[X52300][p]
						- rs[DC] * px
						- rs[DR] * px
						- rs[DG] * px
						- rs[NE] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						+ rs[UNBBIN] * cl.data[X52311][p])
						+ px;
}

void findX52311(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X52311][p];
	cl.data[X52311][c] = ss * (rs[AR] * rs[NF] * cl.data[X02011][p] * cl.data[X50310][p]
						+ rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X50311][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X52111][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X52211][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X52310][p]
						- rs[DC] * px
						- rs[DR] * px * 2
						- rs[DG] * px
						- rs[UNBBIN] * px)
						+ px;
}

void findX60000(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X60000][p];
	cl.data[X60000][c] = ss * (- rs[NL] * px
						- rs[UP] * px
						- rs[AC] * cl.data[X00100][p] * px
						- rs[AGP] * cl.data[X00200][p] * px
						- rs[AR] * (cl.data[X01000][p] + cl.data[X02000][p]) * px
						+ rs[NE] * cl.data[X60010][p]
						+ rs[DC] * cl.data[X60100][p]
						+ rs[DG] * cl.data[X60200][p]
						+ rs[DR] * (cl.data[X61000][p] + cl.data[X62000][p]))
						+ px;
}

void findX60010(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X60010][p];
	cl.data[X60010][c] = ss * (rs[NL] * cl.data[X60000][p]
						- rs[NE] * px
						- rs[UP] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						- rs[AR] * rs[NF] * (cl.data[X01011][p] + cl.data[X02011][p]) * px
						+ rs[UBC] * cl.data[X60011][p]
						+ rs[UNBBIN] * cl.data[X60011][p]
						+ rs[DC] * cl.data[X60110][p]
						+ rs[DG] * cl.data[X60210][p]
						+ rs[DR] * (cl.data[X61010][p] + cl.data[X62010][p])
						+ rs[DR] * (cl.data[X61011][p] + cl.data[X62011][p]))
						+ px;
}

void findX60011(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X60011][p];
	cl.data[X60011][c] = ss * (rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X60010][p]
						- rs[UBC] * px
						- rs[UNBBIN] * px
						- rs[UP] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						+ rs[DC] * cl.data[X60111][p]
						+ rs[DG] * cl.data[X60211][p]
						+ rs[DR] * (cl.data[X61011][p] + cl.data[X62011][p]))
						+ px;
}

void findX60100(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X60100][p];
	cl.data[X60100][c] = ss * (rs[HTO] * cl.data[X50100][p]
						+ rs[AC] * cl.data[X00100][p] * cl.data[X60000][p]
						- rs[DC] * px
						- rs[NL] * px
						- rs[UP] * px
						- rs[AGP] * cl.data[X00200][p] * px
						- rs[AR] * (cl.data[X01000][p] + cl.data[X02000][p]) * px
						+ rs[NE] * cl.data[X60110][p]
						+ rs[DG] * cl.data[X60300][p]
						+ rs[DR] * (cl.data[X61100][p] + cl.data[X62100][p]))
						+ px;
}

void findX60110(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X60110][p];
	cl.data[X60110][c] = ss * (rs[HTO] * cl.data[X50110][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X60010][p]
						+ rs[NL] * cl.data[X60100][p]
						- rs[DC] * px
						- rs[NE] * px
						- rs[UP] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						- rs[AR] * rs[NF] * (cl.data[X01011][p] + cl.data[X02011][p]) * px
						+ rs[UBC] * cl.data[X60111][p]
						+ rs[UNBBIN] * cl.data[X60111][p]
						+ rs[DG] * cl.data[X60310][p]
						+ rs[DR] * (cl.data[X61110][p] + cl.data[X62110][p])
						+ rs[DR] * (cl.data[X61111][p] + cl.data[X62111][p]))
						+ px;
}

void findX60111(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X60111][p];
	cl.data[X60111][c] = ss * (rs[HTO] * cl.data[X50111][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X60011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X60110][p]
						- rs[DC] * px
						- rs[UBC] * px
						- rs[UNBBIN] * px
						- rs[UP] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] + cl.data[X60111][p]
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						+ rs[DG] * cl.data[X60311][p]
						+ rs[DR] * (cl.data[X61111][p] + cl.data[X62111][p]))
						+ px;
}	

void findX60200(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X60200][p];
	cl.data[X60200][c] = ss * (cl.data[GTO][p] * cl.data[X40200][p]
						+ rs[AGP] * cl.data[X00200][p] * cl.data[X60000][p]
						- rs[DG] * px
						- rs[NL] * px
						- rs[UP] * px
						- rs[AC] * cl.data[X00100][p] * px
						- rs[AR] * (cl.data[X01000][p] + cl.data[X02000][p]) * px
						+ rs[NE] * cl.data[X60210][p]
						+ rs[DC] * cl.data[X60300][p]
						+ rs[DR] * (cl.data[X61200][p] + cl.data[X62200][p]))
						+ px;
}

void findX60210(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X60210][p];
	cl.data[X60210][c] = ss * (cl.data[GTO][p] * cl.data[X40210][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X60010][p]
						+ rs[NL] * cl.data[X60200][p]
						- rs[DG] * px
						- rs[NE] * px
						- rs[UP] * px	
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						- rs[AR] * rs[NF] * (cl.data[X01011][p] + cl.data[X02011][p]) * px
						+ rs[UBC] * cl.data[X60211][p]
						+ rs[UNBBIN] * cl.data[X60211][p]
						+ rs[DC] * cl.data[X60310][p]
						+ rs[DR] * (cl.data[X61210][p] + cl.data[X62210][p])
						+ rs[DR] * (cl.data[X61211][p] + cl.data[X62211][p]))
						+ px;
}

void findX60211(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X60211][p];
	cl.data[X60211][c] = ss * (cl.data[GTO][p] * cl.data[X40211][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X60011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X60210][p]
						- rs[DG] * px
						- rs[UBC] * px
						- rs[UNBBIN] * px
						- rs[UP] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						+ rs[DC] * cl.data[X60311][p]
						+ rs[DR] * (cl.data[X61211][p] + cl.data[X62211][p]))
						+ px;
}

void findX60300(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X60300][p];
	cl.data[X60300][c] = ss * (cl.data[GTO][p] * cl.data[X40300][p]
						+ rs[HTO] * cl.data[X50300][p]
						+ rs[AGP] * cl.data[X00200][p] * cl.data[X60100][p]
						+ rs[AC] * cl.data[X00100][p] * cl.data[X60200][p]
						- rs[DC] * px
						- rs[DG] * px
						- rs[NL] * px
						- rs[UP] * px
						- rs[AR] * (cl.data[X01000][p] + cl.data[X02000][p]) * px
						+ rs[NE] * cl.data[X60310][p]
						+ rs[DR] * (cl.data[X61300][p] + cl.data[X62300][p]))
						+ px;
}

void findX60310(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X60310][p];
	cl.data[X60310][c] = ss * (cl.data[GTO][p] * cl.data[X40310][p]
						+ rs[HTO] * cl.data[X50310][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X60110][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X60210][p]
						+ rs[NL] * cl.data[X60300][p]
						- rs[DC] * px
						- rs[DG] * px
						- rs[NE] * px
						- rs[UP] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X60310][p]
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						- rs[AR] * rs[NF] * (cl.data[X01011][p] + cl.data[X02011][p]) * px
						+ rs[UBC] * cl.data[X60311][p]
						+ rs[UNBBIN] * cl.data[X60311][p]
						+ rs[DR] * (cl.data[X61310][p] + cl.data[X62310][p])
						+ rs[DR] * (cl.data[X61311][p] + cl.data[X62311][p]))
						+ px;
}

void findX60311(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X60311][p];
	cl.data[X60311][c] = ss * (cl.data[GTO][p] * cl.data[X40311][p]
						+ rs[HTO] * cl.data[X50311][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X60111][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X60211][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X60310][p]
						- rs[DC] * px
						- rs[DG] * px
						- rs[UBC] * px
						- rs[UNBBIN] * px
						- rs[UP] * px
						- rs[AR] * rs[NF] * (cl.data[X01010][p] + cl.data[X02010][p]) * px
						+ rs[DR] * (cl.data[X61311][p] + cl.data[X62311][p]))
						+ px;
}

void findX61000(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X61000][p];
	cl.data[X61000][c] = ss * (rs[AR] * cl.data[X01000][p] * cl.data[X60000][p]
						- rs[DR] * px
						- rs[NL] * px
						- rs[AC] * cl.data[X00100][p] * px
						- rs[AGP] * cl.data[X00200][p] * px
						+ rs[NE] * cl.data[X61010][p]
						+ rs[DC] * cl.data[X61100][p]
						+ rs[DG] * cl.data[X61200][p])
						+ px;
}

void findX61010(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X61010][p];
	cl.data[X61010][c] = ss * (rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X60010][p]
						+ rs[NL] * cl.data[X61000][p]
						- rs[DR] * px
						- rs[NE] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						+ rs[UNBBIN] * cl.data[X61011][p]
						+ rs[DC] * cl.data[X61110][p]
						+ rs[DG] * cl.data[X61210][p])
						+ px;
}

void findX61011(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X61011][p];
	cl.data[X61011][c] = ss * (rs[AR] * rs[NF] * cl.data[X01011][p] * cl.data[X60010][p]
						+ rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X60011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X61010][p]
						- rs[DR] * px * 2
						- rs[UNBBIN] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X61011][p]
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X61011][p]
						+ rs[DC] * cl.data[X61111][p]
						+ rs[DG] * cl.data[X61211][p])
						+ px;
}

void findX61100(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X61100][p];
	cl.data[X61100][c] = ss * (rs[AR] * cl.data[X01000][p] * cl.data[X60100][p]
						+ rs[AC] * cl.data[X00100][p] * cl.data[X61000][p]
						- rs[DC] * px
						- rs[DR] * px
						- rs[NL] * px
						- rs[AGP] * cl.data[X00200][p] * px
						+ rs[NE] * cl.data[X61110][p]
						+ rs[DG] * cl.data[X61300][p])
						+ px;
}

void findX61110(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X61110][p];
	cl.data[X61110][c] = ss * (rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X60110][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X61010][p]
						+ rs[NL] * cl.data[X61100][p]
						- rs[DC] * px
						- rs[DR] * px
						- rs[NE] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						+ rs[UNBBIN] * cl.data[X61111][p]
						+ rs[DG] * cl.data[X61310][p])
						+ px;
}

void findX61111(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X61111][p];
	cl.data[X61111][c] = ss * (rs[AR] * rs[NF] * cl.data[X01011][p] * cl.data[X60110][p]
						+ rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X60111][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X61011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X61110][p]
						- rs[DC] * px
						- rs[DR] * px * 2
						- rs[UNBBIN] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						+ rs[DG] * cl.data[X61311][p])
						+ px;
}

void findX61200(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X61200][p];
	cl.data[X61200][c] = ss * (cl.data[GTO][p] * cl.data[X41200][p]
						+ rs[AR] * cl.data[X01000][p] * cl.data[X60200][p]
						+ rs[AGP] * cl.data[X00200][p] * cl.data[X61000][p]
						- rs[DG] * px
						- rs[DR] * px
						- rs[NL] * px
						- rs[AC] * cl.data[X00100][p] * px
						+ rs[NE] * cl.data[X61210][p]
						+ rs[DC] * cl.data[X61300][p])
						+ px;
}

void findX61210(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X61210][p];
	cl.data[X61210][c] = ss * (cl.data[GTO][p] * cl.data[X41210][p]
						+ rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X60210][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X61010][p]
						+ rs[NL] * cl.data[X61200][p]
						- rs[DG] * px
						- rs[DR] * px
						- rs[NE] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						+ rs[UNBBIN] * cl.data[X61211][p]
						+ rs[DC] * cl.data[X61310][p])
						+ px;
}

void findX61211(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X61211][p];
	cl.data[X61211][c] = ss * (cl.data[GTO][p] * cl.data[X41211][p]
						+ rs[AR] * rs[NF] * cl.data[X01011][p] * cl.data[X60210][p]
						+ rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X60211][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X61011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X61210][p]
						- rs[DG] * px
						- rs[DR] * px * 2
						- rs[UNBBIN] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * px
						+ rs[DC] * cl.data[X61311][p])
						+ px;
}

void findX61300(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X61300][p];
	cl.data[X61300][c] = ss * (cl.data[GTO][p] * cl.data[X41300][p]
						+ rs[AR] * cl.data[X01000][p] * cl.data[X60300][p]
						+ rs[AGP] * cl.data[X00200][p] * cl.data[X61100][p]
						+ rs[AC] * cl.data[X00100][p] * cl.data[X61200][p]
						- rs[DC] * px
						- rs[DG] * px
						- rs[DR] * px
						- rs[NL] * px
						+ rs[NE] * cl.data[X61310][p])
						+ px;
}

void findX61310(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X61310][p];
	cl.data[X61310][c] = ss * (cl.data[GTO][p] * cl.data[X41310][p]
						+ rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X60310][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X61110][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X61210][p]
						+ rs[NL] * cl.data[X61300][p]
						- rs[DC] * px
						- rs[DG] * px
						- rs[DR] * px
						- rs[NE] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						+ rs[UNBBIN] * cl.data[X61311][p])
						+ px;
}

void findX61311(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X61311][p];
	cl.data[X61311][c] = ss * (cl.data[GTO][p] * cl.data[X41311][p]
						+ rs[AR] * rs[NF] * cl.data[X01011][p] * cl.data[X60310][p]
						+ rs[AR] * rs[NF] * cl.data[X01010][p] * cl.data[X60311][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X61111][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X61211][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X61310][p]
						- rs[DC] * px
						- rs[DG] * px
						- rs[DR] * px * 2
						- rs[UNBBIN] * px)
						+ px;
}

void findX62000(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X62000][p];
	cl.data[X62000][c] = ss * (rs[AR] * cl.data[X02000][p] * cl.data[X60000][p]
						- rs[DR] * px
						- rs[NL] * px
						- rs[AC] * cl.data[X00100][p] * px
						- rs[AGP] * cl.data[X00200][p] * px
						+ rs[NE] * cl.data[X62010][p]
						+ rs[DC] * cl.data[X62100][p]
						+ rs[DG] * cl.data[X62200][p])
						+ px;
}

void findX62010(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X62010][p];
	cl.data[X62010][c] = ss * (rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X60010][p]
						+ rs[NL] * cl.data[X62000][p]
						- rs[DR] * px
						- rs[NE] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X62010][p]
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X62010][p]
						+ rs[UNBBIN] * cl.data[X62011][p]
						+ rs[DC] * cl.data[X62110][p]
						+ rs[DG] * cl.data[X62210][p])
						+ px;
}

void findX62011(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X62011][p];
	cl.data[X62011][c] = ss * (rs[AR] * rs[NF] * cl.data[X02011][p] * cl.data[X60010][p]
						+ rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X60011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X62010][p]
						- rs[DR] * px * 2
						- rs[UNBBIN] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X62011][p]
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X62011][p]
						+ rs[DC] * cl.data[X62111][p]
						+ rs[DG] * cl.data[X62211][p])
						+ px;
}

void findX62100(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X62100][p];
	cl.data[X62100][c] = ss * (rs[AR] * cl.data[X02000][p] * cl.data[X60100][p]
						+ rs[AC] * cl.data[X00100][p] * cl.data[X62000][p]
						- rs[DC] * px
						- rs[DR] * px
						- rs[NL] * px
						- rs[AGP] * cl.data[X00200][p] * px
						+ rs[NE] * cl.data[X62110][p]
						+ rs[DG] * cl.data[X62300][p])
						+ px;
}

void findX62110(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X62110][p];
	cl.data[X62110][c] = ss * (rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X60110][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X62010][p]
						+ rs[NL] * cl.data[X62100][p]
						- rs[DC] * px
						- rs[DR] * px
						- rs[NE] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						+ rs[UNBBIN] * cl.data[X62111][p]
						+ rs[DG] * cl.data[X62310][p])
						+ px;
}

void findX62111(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X62111][p];
	cl.data[X62111][c] = ss * (rs[AR] * rs[NF] * cl.data[X02011][p] * cl.data[X60110][p]
						+ rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X60111][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X62011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X62110][p]
						- rs[DC] * px
						- rs[DR] * px * 2
						- rs[UNBBIN] * px
						- rs[AGP] * rs[NF] * cl.data[X00210][p] * px
						+ rs[DG] * cl.data[X62311][p])
						+ px;
}

void findX62200(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X62200][p];
	cl.data[X62200][c] = ss * (cl.data[GTO][p] * cl.data[X42200][p]
						+ rs[AR] * cl.data[X02000][p] * cl.data[X60200][p]
						+ rs[AGP] * cl.data[X00200][p] * cl.data[X62000][p]
						- rs[DG] * px
						- rs[DR] * px
						- rs[NL] * px
						- rs[AC] * cl.data[X00100][p] * px
						+ rs[NE] * cl.data[X62210][p]
						+ rs[DC] * cl.data[X62300][p])
						+ px;
}

void findX62210(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X62210][p];
	cl.data[X62210][c] = ss * (cl.data[GTO][p] * cl.data[X42210][p]
						+ rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X60210][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X62010][p]
						+ rs[NL] * cl.data[X62200][p]
						- rs[DG] * px
						- rs[DR] * px
						- rs[NE] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X62210][p]
						- rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X62210][p]
						+ rs[UNBBIN] * cl.data[X62211][p]
						+ rs[DC] * cl.data[X62310][p])
						+ px;
}

void findX62211(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X62211][p];
	cl.data[X62211][c] = ss * (cl.data[GTO][p] * cl.data[X42211][p]	
						+ rs[AR] * rs[NF] * cl.data[X02011][p] * cl.data[X60210][p]
						+ rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X60211][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X62011][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X62210][p]
						- rs[DG] * px
						- rs[DR] * px * 2
						- rs[UNBBIN] * px
						- rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X62211][p]
						+ rs[DC] * cl.data[X62311][p])
						+ px;
}

void findX62300(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X62300][p];
	cl.data[X62300][c] = ss * (cl.data[GTO][p] * cl.data[X42300][p]
						+ rs[AR] * cl.data[X02000][p] * cl.data[X60300][p]
						+ rs[AGP] * cl.data[X00200][p] * cl.data[X62100][p]
						+ rs[AC] * cl.data[X00100][p] * cl.data[X62200][p]
						- rs[DC] * px
						- rs[DG] * px
						- rs[DR] * px
						- rs[NL] * px
						+ rs[NE] * cl.data[X62310][p])
						+ px;
}

void findX62310(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X62310][p];
	cl.data[X62310][c] = ss * (cl.data[GTO][p] * cl.data[X42310][p]
						+ rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X60310][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X62110][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X62210][p]
						+ rs[NL] * cl.data[X62300][p]
						- rs[DC] * px
						- rs[DG] * px
						- rs[DR] * px
						- rs[NE] * px
						- rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X62310][p]
						+ rs[UNBBIN] * cl.data[X62311][p])
						+ px;
}

void findX62311(double * rs, con_levels& cl, int c, double ss){
	int p = c-1;
	double px = cl.data[X62311][p];
	cl.data[X62311][c] = ss * (cl.data[GTO][p] * cl.data[X42311][p]
						+ rs[AR] * rs[NF] * cl.data[X02011][p] * cl.data[X60310][p]
						+ rs[AR] * rs[NF] * cl.data[X02010][p] * cl.data[X60311][p]
						+ rs[AGP] * rs[NF] * cl.data[X00210][p] * cl.data[X62111][p]
						+ rs[AC] * rs[NF] * cl.data[X00110][p] * cl.data[X62211][p]
						+ rs[BBIN] * rs[NF] * cl.data[X00011][p] * cl.data[X62310][p]
						- rs[DC] * px
						- rs[DG] * px
						- rs[DR] * px * 2
						- rs[UNBBIN] * px)
						+ px;
}


