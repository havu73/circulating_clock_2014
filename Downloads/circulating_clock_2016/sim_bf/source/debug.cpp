#include "debug.hpp"
#include "macros.hpp"
using namespace std;
void test_concentrations(con_levels& cl, int t){
	for (int i = 0; i< NUM_CONS; i++){
		if (i == GRB){
				cout << "GRB at time step " << t << " is : "<< cl.data[i][t]<< endl; 
			}
			else if (i == G){
				cout << "G at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == GRSRB){
				cout << "GRSRB at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == GRS){
				cout << "GRS at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == GCR){
				cout << "GCR at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == GC){
				cout << "GC at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == GBR){
				cout << "GBR at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == GB){
				cout << "GB at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == GBRB){
				cout << "GBRB at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == GBB){
				cout << "GBB at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == MNPO){
				cout << "MNPO at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == MCPO){
				cout << "MCPO at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == MNPT){
				cout << "MNPT at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == MCPT){
				cout << "MCPT at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == MNRT){
				cout << "MNRT at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == MCRT){
				cout << "MCRT at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == MNREV){
				cout << "MNREV at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == MCREV){
				cout << "MCREV at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == MNRO){
				cout << "MNRO at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == MCRO){
				cout << "MCRO at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == MNB){
				cout << "MNB at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == MCB){
				cout << "MCB at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == MNNP){
				cout << "MNNP at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == MCNP){
				cout << "MCNP at time step " << t << " is : "<< cl.data[i][t]<< endl;
			} 
			else if (i == B){
				cout << "B at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == CL){
				cout << "CL at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == BC){
				cout << "BC at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == CYREV){
				cout << "CYREV at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == REVN){
				cout << "REVN at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == CYREVG){
				cout << "CYREVG at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == REVNG){
				cout << "REVNG at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == CYREVGP){
				cout << "CYREVGP at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == REVNGP){
				cout << "REVNGP at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == CYREVP){
				cout << "CYREVP at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == REVNP){
				cout << "REVNP at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == GTO){
				cout << "GTO at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X00001){
				cout << "X00001 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X00011){
				cout << "X00011 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X00100){
				cout << "X00100 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X00110){
				cout << "X00110 at time step " << t << " is : "<< cl.data[i][t]<< endl;	
			}
			else if (i == X00200){
				cout << "X00200 at time step " << t << " is : "<< cl.data[i][t]<< endl;	
			}
			else if (i == X00210){
				cout << "X00210 at time step " << t << " is : "<< cl.data[i][t]<< endl;	
			}
			else if (i == X01000){
				cout << "X01000 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X01010){
				cout << "X01010 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X01011){
				cout << "X01011 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X02000){
				cout << "X02000 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X02010){
				cout << "X02010 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X02011){
				cout << "X02011 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X10000){
				cout << "X10000 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X10100){
				cout << "X10100 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X20000){
				cout << "X20000 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X20010){
				cout << "X20010 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X20011){
				cout << "X20011 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X20100){
				cout << "X20100 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X20110){
				cout << "X20110 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X20111){
				cout << "X20111 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X21000){
				cout << "X21000 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X21010){
				cout << "X21010 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X21011){
				cout << "X21011 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X21100){
				cout << "X21100 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X21110){
				cout << "X21110 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X21111){
				cout << "X21111 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X22000){
				cout << "X22000 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X22010){
				cout << "X22010 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X22011){
				cout << "X22011 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X22100){
				cout << "X22100 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X22110){
				cout << "X22110 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X22111){
				cout << "X22111 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X30000){
				cout << "X30000 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X30100){
				cout << "X30100 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X30200){
				cout << "X30200 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X30300){
				cout << "X30300 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X40000){
				cout << "X40000 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X40010){
				cout << "X40010 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X40011){
				cout << "X40011 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X40100){
				cout << "X40100 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X40110){
				cout << "X40110 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X40111){
				cout << "X40111 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X40200){
				cout << "X40200 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X40210){
				cout << "X40210 at time step " << t << " is : "<< cl.data[i][t]<< endl;;
			}
			else if (i == X40211){
				cout << "X40211 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X40300){
				cout << "X40300 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X40310){
				cout << "X40310 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X40311){
				cout << "X40311 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X41000){
				cout << "X41000 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X41010){
				cout << "X41010 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X41011){
				cout << "X41011 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X41100){
				cout << "X41100 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X41110){
				cout << "X41110 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X41111){
				cout << "X41111 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X41200){
				cout << "X41200 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X41210){
				cout << "X41210 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X41211){
				cout << "X41211 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X41300){
				cout << "X41300 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X41310){
				cout << "X41310 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X41311){
				cout << "X41311 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X42000){
				cout << "X42000 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X42010){
				cout << "X42010 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X42011){
				cout << "X42011 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X42100){
				cout << "X42100 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X42110){
				cout << "X42110 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X42111){
				cout << "X42111 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X42200){
				cout << "X42200 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X42210){
				cout << "X42210 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X42210){
				cout << "X42210 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X42211){
				cout << "X42211 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X42300){
				cout << "X42300 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X42310){
				cout << "X42310 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X42311){
				cout << "X42311 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X50000){
				cout << "X50000 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X50010){
				cout << "X50010 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X50011){
				cout << "X50011 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X50100){
				cout << "X50100 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X50110){
				cout << "X50110 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X50111){
				cout << "X50111 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X50200){
				cout << "X50200 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X50210){
				cout << "X50210 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X50300){
				cout << "X50300 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X50310){
				cout << "X50310 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X50311){
				cout << "X50311 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X51000){
				cout << "X51000 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X51010){
				cout << "X51010 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X51011){
				cout << "X51011 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X51100){
				cout << "X51100 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X51110){
				cout << "X51110 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X51111){
				cout << "X51111 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X51200){
				cout << "X51200 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X51210){
				cout << "X51210 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X51211){
				cout << "X51211 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X51300){
				cout << "X51300 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X51310){
				cout << "X51310 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X51311){
				cout << "X51311 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X52000){
				cout << "X52000 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X52010){
				cout << "X52010 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X52011){
				cout << "X52011 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X52100){
				cout << "X52100 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X52110){
				cout << "X52110 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X52111){
				cout << "X52111 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X52200){
				cout << "X52200 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X52210){
				cout << "X52210 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X52211){
				cout << "X52211 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X52300){
				cout << "X52300 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X52310){
				cout << "X52310 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X52311){
				cout << "X52311 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X60000){
				cout << "X60000 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X60010){
				cout << "X60010 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X60011){
				cout << "X60011 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X60100){
				cout << "X60100 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X60110){
				cout << "X60110 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X60111){
				cout << "X60111 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X60200){
				cout << "X60200 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X60210){
				cout << "X60210 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X60300){
				cout << "X60300 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X60310){
				cout << "X60310 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X60311){
				cout << "X60311 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X61000){
				cout << "X61000 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X61010){
				cout << "X61010 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X61011){
				cout << "X61011 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X61100){
				cout << "X61100 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X61110){
				cout << "X61110 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X61111){
				cout << "X61111 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X61200){
				cout << "X61200 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X61210){
				cout << "X61210 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X61211){
				cout << "X61211 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X61300){
				cout << "X61300 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X61310){
				cout << "X61310 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X61311){
				cout << "X61311 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X62000){
				cout << "X62000 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X62010){
				cout << "X62010 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X62011){
				cout << "X62011 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X62100){
				cout << "X62100 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X62110){
				cout << "X62110 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X62111){
				cout << "X62111 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X62200){
				cout << "X62200 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X62210){
				cout << "X62210 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X62211){
				cout << "X62211 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X62300){
				cout << "X62300 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X62310){
				cout << "X62310 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
			else if (i == X62311){
				cout << "X62311 at time step " << t << " is : "<< cl.data[i][t]<< endl;
			}
	}
}


void test_rates(rates& rs){
	double* r = rs.rates_base;
	for (int i = 0 ; i < NUM_RATES ; i++){
		if (i == TRPO){
			cout << "TRPO: " << r[i]<< endl;
		}
		else if (i == TRPT){
			cout << "TRPT: " << r[i]<< endl;
		}
		else if (i == TRRO){
			cout << "TRRO: " << r[i]<< endl;
		}
		else if (i == TRRT){
			cout << "TRRT: " << r[i]<< endl;
		}
		else if (i == TRB){
			cout << "TRB: " << r[i]<< endl;
		}
		else if (i == TRREV){
			cout << "TRREV: " << r[i]<< endl;
		}
		else if (i == TRNP){
			cout << "TRNP: " << r[i]<< endl;
		}
		else if (i == TLP){
			cout << "TLP: " << r[i]<< endl;
		}
		else if (i == TLR){
			cout << "TLR: " << r[i]<< endl;
		}
		else if (i == TLB){
			cout << "TLB: " << r[i]<< endl;
		}
		else if (i == TLREV){
			cout << "TLREV: " << r[i]<< endl;
		}
		else if (i == TLC){
			cout << "TLC: " << r[i]<< endl;
		}
		else if (i == TLNP){
			cout << "TLNP: " << r[i]<< endl;
		}
		else if (i == TRPT){
			cout << "TRPT: " << r[i]<< endl;
		}
		else if (i == AGP){
			cout << "AGP: " << r[i]<< endl;
		}
		else if (i == DG){
			cout << "DG: " << r[i]<< endl;
		}
		else if (i == AC){
			cout << "AC: " << r[i]<< endl;
		}
		else if (i == DC){
			cout << "DC: " << r[i]<< endl;
		}
		else if (i == AR){
			cout << "AR: " << r[i]<< endl;
		}
		else if (i == DR){
			cout << "DR: " << r[i]<< endl;
		}
		else if (i == CBIN){
			cout << "CBIN: " << r[i]<< endl;
		}
		else if (i == UNCBIN){
			cout << "UNCBIN: " << r[i]<< endl;
		}
		else if (i == BBIN){
			cout << "BBIN: " << r[i]<< endl;
		}
		else if (i == UNBBIN){
			cout << "UNBBIN: " << r[i]<< endl;
		}
		else if (i == CBBIN){
			cout << "CBBIN: " << r[i]<< endl;
		}
		else if (i == UNCBBIN){
			cout << "UNCBBIN: " << r[i]<< endl;
		}
		else if (i == AG){
			cout << "AG: " << r[i]<< endl;
		}
		else if (i == BIN){
			cout << "BIN: " << r[i]<< endl;
		}
		else if (i == UNBIN){
			cout << "UNBIN: " << r[i]<< endl;
		}
		else if (i == BINREV){
			cout << "BINREV: " << r[i]<< endl;
		}
		else if (i == UNBINREV){
			cout << "UNBINREV: " << r[i]<< endl;
		}
		else if (i == BINR){
			cout << "BINR: " << r[i]<< endl;
		}
		else if (i == UNBINR){
			cout << "UNBINR: " << r[i]<< endl;
		}
		else if (i == BINC){
			cout << "BINC: " << r[i]<< endl;
		}
		else if (i == UNBINC){
			cout << "UNBINC: " << r[i]<< endl;
		}
		else if (i == BINREVB){
			cout << "BINREVB: " << r[i]<< endl;
		}
		else if (i == UNBINREVB){
			cout << "UNBINREVB: " << r[i]<< endl;
		}
		else if (i == TMC){
			cout << "TMC: " << r[i]<< endl;
		}
		else if (i == TMCREV){
			cout << "TMCREV: " << r[i]<< endl;
		}
		else if (i == NL){
			cout << "NL: " << r[i]<< endl;
		}
		else if (i == NE){
			cout << "NE: " << r[i]<< endl;
		}
		else if (i == NLREV){
			cout << "NLREV: " << r[i]<< endl;
		}
		else if (i == NEREV){
			cout << "NEREV: " << r[i]<< endl;
		}
		else if (i == LNE){
			cout << "LNE: " << r[i]<< endl;
		}
		else if (i == NLBC){
			cout << "NLBC: " << r[i]<< endl;
		}
		else if (i == HOO){
			cout << "HOO: " << r[i]<< endl;
		}
		else if (i == HTO){
			cout << "HTO: " << r[i]<< endl;
		}
		else if (i == PHOS){
			cout << "PHOS: " << r[i]<< endl;
		}
		else if (i == LONO){
			cout << "LONO: " << r[i]<< endl;
		}
		else if (i == LONT){
			cout << "LONT: " << r[i]<< endl;
		}
		else if (i == LTA){
			cout << "LTA: " << r[i]<< endl;
		}
		else if (i == LTB){
			cout << "LTB: " << r[i]<< endl;
		}
		else if (i == TRGTO){
			cout << "TRGTO: " << r[i]<< endl;
		}
		else if (i == UGTO){
			cout << "UGTO: " << r[i]<< endl;
		}
		else if (i == NF){
			cout << "NF: " << r[i]<< endl;
		}
		else if (i == UP){
			cout << "UP: " << r[i]<< endl;
		}
		else if (i == URO){
			cout << "URO: " << r[i]<< endl;
		}
		else if (i == URT){
			cout << "URT: " << r[i]<< endl;
		}
		else if (i == UMNP){
			cout << "UMNP: " << r[i]<< endl;
		}
		else if (i == UMPO){
			cout << "UMPO: " << r[i]<< endl;
		}
		else if (i == UMPT){
			cout << "UMPT: " << r[i]<< endl;
		}
		else if (i == UMRO){
			cout << "UMRO: " << r[i]<< endl;
		}
		else if (i == UMRT){
			cout << "UMRT: " << r[i]<< endl;
		}
		else if (i == UB){
			cout << "UB: " << r[i]<< endl;
		}
		else if (i == UC){
			cout << "UC: " << r[i]<< endl;
		}
		else if (i == UBC){
			cout << "UBC: " << r[i]<< endl;
		}
		else if (i == UPU){
			cout << "UPU: " << r[i]<< endl;
		}
		else if (i == UREV){
			cout << "UREV: " << r[i]<< endl;
		}
		else if (i == UPREV){
			cout << "UPREV: " << r[i]<< endl;
		}
		else if (i == UMB){
			cout << "UMB: " << r[i]<< endl;
		}
		else if (i == UMREV){
			cout << "UMREV: " << r[i]<< endl;
		}
	}
}
