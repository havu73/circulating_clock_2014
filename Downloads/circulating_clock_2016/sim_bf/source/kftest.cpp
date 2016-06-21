#include <cmath>

#include "kftest.hpp"
#include "main.hpp"
#include "macros.hpp"
extern terminal* term;
double cost (con_levels& cl, con_levels& kf_cl){
	double score = 0;
	for (int i =0; i < TEST_STEPS; i ++){
		for (int j = 0 ; j < NUM_CONS; j ++) {
			score += abs(cl.data[j][i] - kf_cl.data[j][i]);
		}
	}
	
	score = score / (TEST_STEPS * NUM_CONS);
	return score;
}
