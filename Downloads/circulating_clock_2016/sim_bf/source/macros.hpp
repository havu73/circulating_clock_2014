#ifndef MACROS_HPP
#define MACROS_HPP

#include <math.h>
#define MNPO		10	//Per1mRNA in nu
#define MCPO		11	//Per1mRNA in cy
#define	MNPT		12	//Per2mRNA in nu
#define MCPT		13	//Per2mRNA in cy
#define	MNRO		18	//cry1mRNA in nu
#define MCRO		19	//cry1mRNA in cy
#define MNRT		14	//cry2mRNA in nu
#define MCRT		15	//cry2mRNA in cy
#define MNB			20	//Bmals mRNA in nu
#define	MCB			21	//Bmals mRNA in cy
#define MNNP		22	//Npas2 mRNA in nu
#define MCNP		23	//Npas2 mRNA in cy
#define MNREV		6	//Rev-erbs mRNA in nu
#define MCREV		17	//Rev-erbs mRNA in cy
#define B			24 	//Bmals protein in cy
#define CL 			25	//Clock/NPAS2 protein in cy
#define	BC			26	//unphosphorylated Bmals-Clock/NPAS2 
#define REVN		28	//unphosphorylated rev-erbs in nu
#define CYREV		27	//unphosphorylated rev-erbs in cy
#define	REVNG		30	//unphosphorylated rev-erbs bound with GSK3beta in nu
#define CYREVG		29	//unphosphorylated rev-erbs bound with GSK3beta in cy
#define REVNGP		32	//phosphorylated rev-erbs bound with GSK3beta in nu
#define	CYREVGP		31	//phosphorylated rev-erbs bound with GSK3beta in cy
#define REVNP		34	//phosphorylated rev-erbs in nu
#define CYREVP		33	//phosphorylated rev-erbs in cy

#define	G			1	//prob of per1, per2 and cry1 E-box being activated
#define GRB			0	//prob of per1, per2 and cry1 E-box being represssed (In literature: GR (R big))
#define GC			5	//prob of cry2 E-box activated
#define GCR			4	//prob of cry2 E-box Repressed
#define GRS			3	//prob of rev-erbs E-box activated (In literature: Gr (r small))
#define GRSRB		2 	//prob of rev-erbs E-box repressed (In literature: GrR)
#define	GB			7	//prob of npas2 and cry1 RORE activated
#define	GBR			6	//prob of npas2 and cry1 RORE repressed
#define GBB			9	//prob of Bmals RORE activated
#define GBRB		8	//prob of Bmals RORE repressed
#define GTO			35	//activity of GSK3beta
#define X00001		36	//no per, no cry, no kinases, phosphorylated Bmals--- phosphorylated CLK in cy
#define	X00011		37	//no per no cry no kinases p Bmals--pCLK in nu
#define	X00100		38	//no per no cry CKI no pBmal-pCLK in cy
#define	X00110		39
#define	X00200		40
#define	X00210		41
#define	X01000		42
#define	X01010		43
#define	X01011		44
#define X02000		45
#define	X02010		46
#define	X02011		47
#define	X10000		48
#define	X10100		49	
#define	X20000		50
#define	X20010		51
#define	X20011		52
#define	X20100		53	
#define	X20110		54	
#define	X20111		55
#define	X21000		56
#define	X21010		57
#define	X21011		58
#define	X21100		59
#define	X21110		60
#define	X21111		61
#define	X22000		62
#define	X22010		63
#define	X22011		64
#define X22100		65
#define X22110		66
#define	X22111		67
#define	X30000		68
#define	X30100		69
#define	X30200		70	
#define	X30300		71
#define	X40000		72
#define	X40010		73
#define	X40011		74	
#define	X40100		75
#define	X40110		76
#define	X40111		77
#define	X40200		78
#define	X40210		79	
#define	X40211		80
#define	X40300		81
#define	X40310		82
#define	X40311		83
#define	X41000		84
#define	X41010		85
#define	X41011		86
#define	X41100		87
#define	X41110		88
#define	X41111		89
#define	X41200		90	
#define	X41210		91
#define	X41211		92
#define	X41300		93
#define	X41310		94
#define	X41311		95
#define	X42000		96
#define	X42010		97
#define	X42011		98
#define	X42100		99
#define	X42110		100
#define	X42111		101
#define	X42200		102
#define	X42210		103
#define	X42211		104
#define	X42300		105
#define	X42310		106
#define	X42311		107
#define	X50000		108
#define	X50010		109
#define	X50011		110
#define	X50100		111
#define	X50110		112
#define	X50111		113
#define	X50200		114
#define	X50210		115
#define	X50211		116
#define	X50300		117
#define	X50310		118
#define	X50311		119
#define	X51000		120
#define	X51010		121
#define	X51011		122
#define	X51100		123
#define	X51110		124
#define	X51111		125
#define	X51200		126
#define	X51210		127
#define	X51211		128
#define	X51300		129
#define	X51310		130
#define	X51311		131
#define	X52000		132
#define	X52010		133
#define	X52011		134
#define	X52100		135
#define	X52110		136
#define	X52111		137
#define	X52200		138
#define	X52210		139
#define	X52211		140
#define	X52300		141
#define	X52310		142
#define	X52311		143
#define	X60000		144
#define	X60010		145
#define	X60011		146
#define	X60100		147
#define	X60110		148
#define	X60111		149
#define	X60200		150
#define	X60210		151
#define	X60211		152
#define	X60300		153
#define	X60310		154
#define	X60311		155
#define	X61000		156
#define	X61010		157
#define	X61011		158
#define	X61100		159
#define	X61110		160
#define	X61111		161
#define	X61200		162
#define	X61210		163
#define	X61211		164
#define	X61300		165
#define	X61310		166
#define	X61311		167
#define	X62000		168
#define	X62010		169
#define	X62011		170
#define	X62100		171
#define	X62110		172
#define	X62111		173
#define	X62200		174
#define	X62210		175
#define	X62211		176
#define	X62300		177
#define	X62310		178
#define	X62311		179

#define NUM_CONS		180	//total number of things (mRNA, protein, complexes) in the system

#define NUM_FEATURE_CONS	180
		
#define	TRPO		0	//transcription rate fro Per1
#define	TRPT		0	//Transcription rate for Per2
#define	TRRO		0	//Transcription rate for Cry1
#define	TRRT		0	//Transcription rate for Cry2
#define	TRB			0	//Transcription rate for Bmal
#define	TRREV		0	//Transcription rate for Rev-erbs
#define TRNP		0	//Transcription rate for Npas2
#define	TLP			1	//Translation rate for Per1 and Per2
#define	TLR			1	//Translation rate for Cry1 and Cry2
#define	TLB			1	//Translation rate for Bmal
#define	TLREV		1	//Translation rate for Rev-erbs
#define	TLC			1	//Translation rate for Clock
#define	TLNP		1	//Translation rate for NPAS2	
#define	AGP			2	//Binding rate for Per2-GSK3Beta
#define	DG			3	//Binding rate for Per2/Rev-erbs to GSK3Beta
#define	AC			2	//Binding rate for Per1/2-CKIepsilon/delta
#define	DC			3	//Unbinding rate for Per1/2-CKIepsilon/delta
#define	AR			2	//Binding rate for Per1/2-Cry1/2
#define	DR			3	//Unbinding rate for Per1/2-Cry1/2
#define	CBIN		2	//Binding rate for Bmal to clock/Npas2
#define	UNCBIN		3	//unbinidng rate for Bmal to Clock/Npas2
#define	BBIN		2	//Binding rate Per1/2to Bmal-Clock/Npas2
#define	UNBBIN		3	//Unbinding rate for  Per1/2to Bmal-Clock/Npas2
#define	CBBIN		2	//Binding rate for Cry1/2 to Bmal-Clock/Npas2
#define	UNCBBIN		3	//Unbinding rate for Cry1/2 to Bmal-Clock/Npas2
#define	AG			2	//Binding rate for Rev-erbs to GSK3Beta
#define	BIN			2	//Normalized binding rate for Bmal-Clock/Npas2 to Per1/2/Cry1 Ebox
#define	UNBIN		3	//Normalized unbinding rate for Bmal-Clock/Npas2 to Per1/2/Cry1 Ebox
#define	BINREV		2	//Normalized binding rate Rev-erbs to Cry1/Npas2 RORE
#define	UNBINREV 	3	//Normalized unbinding rate ------
#define	BINR		2	//Normalized binding rate Bmal-Clock/Npas2 to Rev-erbs Ebox
#define	UNBINR		3	//Normalized Unbinding rate Bmal-Clock/Npas2 to Rev-erbs Ebox
#define	BINC		2	//Normalized binding rate for Bmal-Clock/Npas2 to Cry2 ebox
#define	UNBINC		3	//Normalized unbinding rate for Bmal-Clock/Npas2 to Cry2 ebox
#define	BINREVB		2	//Normalized binding rate Rev-erbs to Bmal RORE
#define	UNBINREVB	3	//Normalized unbinding rate Rev-erbs to Bmal RORE
#define	TMC			4	//Rate for folding and nuclear export of Per1/2, Cry1/2, Bnak, Npas2 and mRNA
#define	TMCREV		4	//Folding and nu export of Rev-erbs mRNA	
#define	NL			5	//Nuclear localization rate for proteins bound to Per
#define	NE			6	//nucleat export rate for Protein boudn to Per
#define	NLREV		5	//Nuclear localization rate Rev-erbs and GSK3beta if bound
#define	NEREV		6	//Nu export rate Rev-erbs as well as GSK3beat if bound
#define	LNE			6	//nu export rate for unbound kinases GSK3beta and CKI
#define	NLBC		5	//Nu localization rate Bmal-clock/Npas2
#define	HOO			8	//CKE epsilon/delta phosphorylation rate Per1
#define	HTO			8	//---------------------------------------Per2
#define	PHOS		8	//Phos--- rate Bmal-Clock/Npas2
#define	LONO		12	//Additional Per1 transcription rate in light
#define	LONT		12	//-----------Per2----------------------------
#define	LTA			12	//Rate of activation of pho	
#define	LTB			12	//Light effect decreate rate
#define	TRGTO		13	//increate rate GSK3beta activity
#define	UGTO		13	//decrease-----------------------
#define	NF			11	//ratio of cy and nu compartment volume
#define	UP			10	//-----------------CKI phosphorylated Per
#define	URO			10	//-----------------CRY1
#define	URT			10	//-----------------CRY2
#define	UMNP		9	//-----------------Npas2
#define	UMPO		9	//Degradation rate Per1
#define	UMPT		9	//-----------------Per2
#define	UMRO		9	//-----------------Cry1
#define	UMRT		9	//-----------------Cry2
#define	UB			10	//-----------------BMAL
#define	UC			10	//-----------------CLOCK/NPAS2
#define	UBC			10	//-----------------BMAL-CLOCK/NPAS2
#define	UPU			10	//-----------------unphosphorylated Per
#define	UREV		10	//-----------------unphosphorylated REV_ERBS
#define	UPREV		10	//-----------------GSK3beta phosphorylated REV_ERBS
#define	UMB			9	//-----------------Bmal
#define	UMREV		9	//-----------------Rev-erbs

#define NUM_RATES	14

#define NUM_MUTANTS	//to be determined

#define TEST_STEPS 	12	// How many steps do we take data from kfResults.txt (file containing simulation data from Kim and Forger's model) to compare with our own simulation data
#define KF_FILENAME	"../sim_bf/kfResults.txt"

// Macros for commonly used functions small enough to inject directly into the code
#define INT_STRLEN(x) (log10((x) > 0 ? (x) : 1) + 1)

// Exit statuses
#define EXIT_SUCCESS			0
#define EXIT_MEMORY_ERROR		1
#define EXIT_FILE_READ_ERROR	2
#define EXIT_FILE_WRITE_ERROR	3
#define EXIT_PIPE_READ_ERROR	4
#define EXIT_PIPE_WRITE_ERROR	5
#define EXIT_INPUT_ERROR		6

#endif
