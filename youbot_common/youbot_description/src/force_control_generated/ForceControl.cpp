/**********************************************************
 * This file is generated by 20-sim C++ Code Generator
 *
 *  file:  ForceControl.cpp
 *  subm:  ForceControl
 *  model: ForceControl
 *  expmt: 
 *  date:  April 8, 2011
 *  time:  11:46:47 am
 *  user:  Campuslicentie
 *  from:  Universiteit Twente
 *  build: 4.1.2.2
 **********************************************************/

/* Standard include files */
#include <stdio.h>
#include <math.h>
#include <stdexcept>

/* 20-sim include files */
#include "ForceControl.h"

	/* this PRIVATE function sets the input variables from the input vector */
void ForceControl::CopyInputsToVariables (XXDouble *u)
{
	/* copy the input vector to the input variables */
	m_V[402] = u[0];		/* joints.f */
	m_V[403] = u[1];
	m_V[404] = u[2];
	m_V[405] = u[3];
	m_V[406] = u[4];
	m_V[407] = u[5];		/* Linkdim */
	m_V[408] = u[6];
	m_V[409] = u[7];
	m_V[410] = u[8];
	m_V[411] = u[9];
	m_V[412] = u[10];
	m_V[413] = u[11];
	m_V[414] = u[12];
	m_V[415] = u[13];
	m_V[416] = u[14];
	m_V[417] = u[15];
	m_V[418] = u[16];
	m_V[419] = u[17];
	m_V[420] = u[18];
	m_V[421] = u[19];
	m_V[422] = u[20];
	m_V[423] = u[21];
	m_V[424] = u[22];
	m_V[425] = u[23];		/* q */
	m_V[426] = u[24];
	m_V[427] = u[25];
	m_V[428] = u[26];
	m_V[429] = u[27];
	m_V[385] = u[28];		/* tip.e */
	m_V[386] = u[29];
	m_V[387] = u[30];
	m_V[388] = u[31];
	m_V[389] = u[32];
	m_V[390] = u[33];

}

/* this PRIVATE function uses the output variables to fill the output vector */
void ForceControl::CopyVariablesToOutputs (XXDouble *y)
{
	/* copy the output variables to the output vector */
	y[0] = 	m_V[430];		/* Htip0 */
	y[1] = 	m_V[431];
	y[2] = 	m_V[432];
	y[3] = 	m_V[433];
	y[4] = 	m_V[434];
	y[5] = 	m_V[435];
	y[6] = 	m_V[436];
	y[7] = 	m_V[437];
	y[8] = 	m_V[438];
	y[9] = 	m_V[439];
	y[10] = 	m_V[440];
	y[11] = 	m_V[441];
	y[12] = 	m_V[442];
	y[13] = 	m_V[443];
	y[14] = 	m_V[444];
	y[15] = 	m_V[445];
	y[16] = 	m_V[397];		/* joints.e */
	y[17] = 	m_V[398];
	y[18] = 	m_V[399];
	y[19] = 	m_V[400];
	y[20] = 	m_V[401];
	y[21] = 	m_V[391];		/* tip.f */
	y[22] = 	m_V[392];
	y[23] = 	m_V[393];
	y[24] = 	m_V[394];
	y[25] = 	m_V[395];
	y[26] = 	m_V[396];

}

ForceControl::ForceControl(void)
{
	m_start_time = 0.0;
	m_finish_time = 10.0;
	m_step_size = 1.0e-4;
	m_time = 0;
	m_major = true;

	m_number_constants = 15;
	m_number_parameters = 0;
	m_number_initialvalues = 0;
	m_number_variables = 446;
	m_number_states = 0;
	m_number_rates = 0;
	m_number_matrices = 77;
	m_number_unnamed = 327;

	/* the variable arrays */
	m_C = new XXDouble[15 + 1];		/* constants */
	m_P = new XXDouble[0 + 1];		/* parameters */
	m_I = new XXDouble[0 + 1];		/* initial values */
	m_V = new XXDouble[446 + 1];		/* variables */
	m_s = new XXDouble[0 + 1];		/* states */
	m_R = new XXDouble[0 + 1];		/* rates (or new states) */
	m_M = new XXMatrix[77 + 1];		/* matrices */
	m_U = new XXDouble[327 + 1];		/* unnamed */
	m_workarray = new XXDouble[0 + 1];

	myintegmethod.Initialize(this);

	state = initialrun;
}

ForceControl::~ForceControl(void)
{
	/* free memory */
	delete[] m_C;
	delete[] m_P;
	delete[] m_I;
	delete[] m_V;
	delete[] m_s;
	delete[] m_R;
	delete[] m_M;
	delete[] m_U;
	delete[] m_workarray;
}

/* the initialization function for submodel */
void ForceControl::Initialize (XXDouble *u, XXDouble *y, XXDouble t)
{
	/* initialization phase (allocating memory) */
	m_initialize = true;

	/* copy the inputs */
	m_time = t;

	/* set the constants */
	m_C[0] = 0.0;		/* CalculateJ\omega1 */
	m_C[1] = 0.0;		
	m_C[2] = 1.0;		
	m_C[3] = 0.0;		/* CalculateJ\omega2 */
	m_C[4] = 1.0;		
	m_C[5] = 0.0;		
	m_C[6] = 0.0;		/* CalculateJ\omega3 */
	m_C[7] = 1.0;		
	m_C[8] = 0.0;		
	m_C[9] = 0.0;		/* CalculateJ\omega4 */
	m_C[10] = 1.0;		
	m_C[11] = 0.0;		
	m_C[12] = 0.0;		/* CalculateJ\omega5 */
	m_C[13] = 0.0;		
	m_C[14] = 1.0;		


	/* set the parameters */


	/* set the states */


	/* set the matrices */
	m_M[0].mat = &m_V[0];		/* CalculateJ\controllableJ */
	m_M[0].rows = 6;
	m_M[0].columns = 5;
	m_M[1].mat = &m_V[30];		/* CalculateJ\Htip0 */
	m_M[1].rows = 4;
	m_M[1].columns = 4;
	m_M[2].mat = &m_V[46];		/* CalculateJ\Linkdim */
	m_M[2].rows = 3;
	m_M[2].columns = 6;
	m_M[3].mat = &m_V[64];		/* CalculateJ\q */
	m_M[3].rows = 5;
	m_M[3].columns = 1;
	m_M[4].mat = &m_C[0];		/* CalculateJ\omega1 */
	m_M[4].rows = 3;
	m_M[4].columns = 1;
	m_M[5].mat = &m_C[3];		/* CalculateJ\omega2 */
	m_M[5].rows = 3;
	m_M[5].columns = 1;
	m_M[6].mat = &m_C[6];		/* CalculateJ\omega3 */
	m_M[6].rows = 3;
	m_M[6].columns = 1;
	m_M[7].mat = &m_C[9];		/* CalculateJ\omega4 */
	m_M[7].rows = 3;
	m_M[7].columns = 1;
	m_M[8].mat = &m_C[12];		/* CalculateJ\omega5 */
	m_M[8].rows = 3;
	m_M[8].columns = 1;
	m_M[9].mat = &m_V[69];		/* CalculateJ\uTr1refref */
	m_M[9].rows = 6;
	m_M[9].columns = 1;
	m_M[10].mat = &m_V[75];		/* CalculateJ\uTr211 */
	m_M[10].rows = 6;
	m_M[10].columns = 1;
	m_M[11].mat = &m_V[81];		/* CalculateJ\uTr322 */
	m_M[11].rows = 6;
	m_M[11].columns = 1;
	m_M[12].mat = &m_V[87];		/* CalculateJ\uTr433 */
	m_M[12].rows = 6;
	m_M[12].columns = 1;
	m_M[13].mat = &m_V[93];		/* CalculateJ\uTr544 */
	m_M[13].rows = 6;
	m_M[13].columns = 1;
	m_M[14].mat = &m_V[99];		/* CalculateJ\R1ref */
	m_M[14].rows = 3;
	m_M[14].columns = 3;
	m_M[15].mat = &m_V[108];		/* CalculateJ\R21 */
	m_M[15].rows = 3;
	m_M[15].columns = 3;
	m_M[16].mat = &m_V[117];		/* CalculateJ\R32 */
	m_M[16].rows = 3;
	m_M[16].columns = 3;
	m_M[17].mat = &m_V[126];		/* CalculateJ\R43 */
	m_M[17].rows = 3;
	m_M[17].columns = 3;
	m_M[18].mat = &m_V[135];		/* CalculateJ\R54 */
	m_M[18].rows = 3;
	m_M[18].columns = 3;
	m_M[19].mat = &m_V[144];		/* CalculateJ\Rtip5 */
	m_M[19].rows = 3;
	m_M[19].columns = 3;
	m_M[20].mat = &m_V[153];		/* CalculateJ\p1ref */
	m_M[20].rows = 3;
	m_M[20].columns = 1;
	m_M[21].mat = &m_V[156];		/* CalculateJ\p21 */
	m_M[21].rows = 3;
	m_M[21].columns = 1;
	m_M[22].mat = &m_V[159];		/* CalculateJ\p32 */
	m_M[22].rows = 3;
	m_M[22].columns = 1;
	m_M[23].mat = &m_V[162];		/* CalculateJ\p43 */
	m_M[23].rows = 3;
	m_M[23].columns = 1;
	m_M[24].mat = &m_V[165];		/* CalculateJ\p54 */
	m_M[24].rows = 3;
	m_M[24].columns = 1;
	m_M[25].mat = &m_V[168];		/* CalculateJ\ptip5 */
	m_M[25].rows = 3;
	m_M[25].columns = 1;
	m_M[26].mat = &m_V[171];		/* CalculateJ\Href0 */
	m_M[26].rows = 4;
	m_M[26].columns = 4;
	m_M[27].mat = &m_V[187];		/* CalculateJ\H1ref */
	m_M[27].rows = 4;
	m_M[27].columns = 4;
	m_M[28].mat = &m_V[203];		/* CalculateJ\H21 */
	m_M[28].rows = 4;
	m_M[28].columns = 4;
	m_M[29].mat = &m_V[219];		/* CalculateJ\H32 */
	m_M[29].rows = 4;
	m_M[29].columns = 4;
	m_M[30].mat = &m_V[235];		/* CalculateJ\H43 */
	m_M[30].rows = 4;
	m_M[30].columns = 4;
	m_M[31].mat = &m_V[251];		/* CalculateJ\H54 */
	m_M[31].rows = 4;
	m_M[31].columns = 4;
	m_M[32].mat = &m_V[267];		/* CalculateJ\Htip5 */
	m_M[32].rows = 4;
	m_M[32].columns = 4;
	m_M[33].mat = &m_V[283];		/* CalculateJ\H10 */
	m_M[33].rows = 4;
	m_M[33].columns = 4;
	m_M[34].mat = &m_V[299];		/* CalculateJ\H20 */
	m_M[34].rows = 4;
	m_M[34].columns = 4;
	m_M[35].mat = &m_V[315];		/* CalculateJ\H30 */
	m_M[35].rows = 4;
	m_M[35].columns = 4;
	m_M[36].mat = &m_V[331];		/* CalculateJ\H40 */
	m_M[36].rows = 4;
	m_M[36].columns = 4;
	m_M[37].mat = &m_V[347];		/* CalculateJ\H50 */
	m_M[37].rows = 4;
	m_M[37].columns = 4;
	m_M[38].mat = &m_V[363];		/* ControllableJ1\joints.e */
	m_M[38].rows = 5;
	m_M[38].columns = 1;
	m_M[39].mat = &m_V[368];		/* ControllableJ1\joints.f */
	m_M[39].rows = 5;
	m_M[39].columns = 1;
	m_M[40].mat = &m_V[373];		/* ControllableJ1\tip.e */
	m_M[40].rows = 6;
	m_M[40].columns = 1;
	m_M[41].mat = &m_V[379];		/* ControllableJ1\tip.f */
	m_M[41].rows = 6;
	m_M[41].columns = 1;
	m_M[42].mat = &m_V[385];		/* tip.e */
	m_M[42].rows = 6;
	m_M[42].columns = 1;
	m_M[43].mat = &m_V[391];		/* tip.f */
	m_M[43].rows = 6;
	m_M[43].columns = 1;
	m_M[44].mat = &m_V[397];		/* joints.e */
	m_M[44].rows = 5;
	m_M[44].columns = 1;
	m_M[45].mat = &m_V[402];		/* joints.f */
	m_M[45].rows = 5;
	m_M[45].columns = 1;
	m_M[46].mat = &m_V[407];		/* Linkdim */
	m_M[46].rows = 3;
	m_M[46].columns = 6;
	m_M[47].mat = &m_V[425];		/* q */
	m_M[47].rows = 5;
	m_M[47].columns = 1;
	m_M[48].mat = &m_V[430];		/* Htip0 */
	m_M[48].rows = 4;
	m_M[48].columns = 4;
	m_M[49].mat = &m_U[0];		/* m_U1 */
	m_M[49].rows = 3;
	m_M[49].columns = 3;
	m_M[50].mat = &m_U[9];		/* m_U2 */
	m_M[50].rows = 3;
	m_M[50].columns = 1;
	m_M[51].mat = &m_U[12];		/* m_U3 */
	m_M[51].rows = 3;
	m_M[51].columns = 1;
	m_M[52].mat = &m_U[15];		/* m_U4 */
	m_M[52].rows = 3;
	m_M[52].columns = 3;
	m_M[53].mat = &m_U[24];		/* m_U5 */
	m_M[53].rows = 3;
	m_M[53].columns = 1;
	m_M[54].mat = &m_U[27];		/* m_U6 */
	m_M[54].rows = 3;
	m_M[54].columns = 1;
	m_M[55].mat = &m_U[30];		/* m_U7 */
	m_M[55].rows = 3;
	m_M[55].columns = 3;
	m_M[56].mat = &m_U[39];		/* m_U8 */
	m_M[56].rows = 3;
	m_M[56].columns = 1;
	m_M[57].mat = &m_U[42];		/* m_U9 */
	m_M[57].rows = 3;
	m_M[57].columns = 1;
	m_M[58].mat = &m_U[45];		/* m_U10 */
	m_M[58].rows = 3;
	m_M[58].columns = 3;
	m_M[59].mat = &m_U[54];		/* m_U11 */
	m_M[59].rows = 3;
	m_M[59].columns = 1;
	m_M[60].mat = &m_U[57];		/* m_U12 */
	m_M[60].rows = 3;
	m_M[60].columns = 1;
	m_M[61].mat = &m_U[60];		/* m_U13 */
	m_M[61].rows = 3;
	m_M[61].columns = 3;
	m_M[62].mat = &m_U[69];		/* m_U14 */
	m_M[62].rows = 3;
	m_M[62].columns = 1;
	m_M[63].mat = &m_U[72];		/* m_U15 */
	m_M[63].rows = 3;
	m_M[63].columns = 1;
	m_M[64].mat = &m_U[75];		/* m_U16 */
	m_M[64].rows = 3;
	m_M[64].columns = 3;
	m_M[65].mat = &m_U[84];		/* m_U17 */
	m_M[65].rows = 3;
	m_M[65].columns = 1;
	m_M[66].mat = &m_U[87];		/* m_U18 */
	m_M[66].rows = 6;
	m_M[66].columns = 1;
	m_M[67].mat = &m_U[93];		/* m_U19 */
	m_M[67].rows = 6;
	m_M[67].columns = 6;
	m_M[68].mat = &m_U[129];		/* m_U20 */
	m_M[68].rows = 6;
	m_M[68].columns = 1;
	m_M[69].mat = &m_U[135];		/* m_U21 */
	m_M[69].rows = 6;
	m_M[69].columns = 6;
	m_M[70].mat = &m_U[171];		/* m_U22 */
	m_M[70].rows = 6;
	m_M[70].columns = 1;
	m_M[71].mat = &m_U[177];		/* m_U23 */
	m_M[71].rows = 6;
	m_M[71].columns = 6;
	m_M[72].mat = &m_U[213];		/* m_U24 */
	m_M[72].rows = 6;
	m_M[72].columns = 1;
	m_M[73].mat = &m_U[219];		/* m_U25 */
	m_M[73].rows = 6;
	m_M[73].columns = 6;
	m_M[74].mat = &m_U[255];		/* m_U26 */
	m_M[74].rows = 6;
	m_M[74].columns = 1;
	m_M[75].mat = &m_U[261];		/* m_U27 */
	m_M[75].rows = 6;
	m_M[75].columns = 6;
	m_M[76].mat = &m_U[297];		/* m_U28 */
	m_M[76].rows = 5;
	m_M[76].columns = 6;


	/* calculate initial and static equations */
	CalculateInitial ();
	CalculateStatic ();

	/* end of initialization phase */
	m_initialize = false;

	state = initialrun;
}

/* the function that calculates the submodel */
void ForceControl::Calculate (XXDouble *u, XXDouble *y /*, XXDouble t*/)
{
	/* another precessor submodel could determine the parameters of this submodel
	   and therefore the static parameter calculations need to be performed. */
	CalculateStatic ();

	switch (state)
	{
		case initialrun:	/* calculate the model for the first time */
			CopyInputsToVariables (u);
			CalculateInput ();
			CalculateDynamic();
			CalculateOutput ();
			CopyVariablesToOutputs (y);
			state = mainrun;
			break;
		case mainrun:	/* calculate the model */

			
				CopyInputsToVariables (u);
				CalculateInput ();
				myintegmethod.Step();
				CalculateOutput ();
				CopyVariablesToOutputs (y);
			break;
		case finished:
			throw std::logic_error("The simulation was finished"); 
			break;
		default:
			throw std::logic_error("The simulation state machine is inconsistent"); 
			break;
	}
}

/* the termination function for submodel */
void ForceControl::Terminate (XXDouble *u, XXDouble *y /*, XXDouble t */)
{
	/* copy the inputs */
	CopyInputsToVariables (u);

	/* calculate the final model equations */
	CalculateFinal ();

	/* set the outputs */
	CopyVariablesToOutputs (y);
}


/* This function calculates the initial equations of the model.
 * These equations are calculated before anything else
 */
void ForceControl::CalculateInitial (void)
{

}

/* This function calculates the static equations of the model.
 * These equations are only dependent from parameters and constants
 */
void ForceControl::CalculateStatic (void)
{
	/* CalculateJ\Href0 = homogeneous (eye (3), [0; 0; 0]); */
	XXMatrixEye (&m_M[49]);
	m_M[50].mat[0] = 0.0;
	m_M[50].mat[1] = 0.0;
	m_M[50].mat[2] = 0.0;
	XXMatrixHomogeneous (&m_M[26], &m_M[49], &m_M[50]);

	/* CalculateJ\Rtip5 = eye (3); */
	XXMatrixEye (&m_M[19]);

	/* CalculateJ\uTr1refref[1:3] = CalculateJ\omega1; */
	m_M[9].mat[0] = m_M[4].mat[0];
	m_M[9].mat[1] = m_M[4].mat[1];
	m_M[9].mat[2] = m_M[4].mat[2];

	/* CalculateJ\uTr211[1:3] = CalculateJ\omega2; */
	m_M[10].mat[0] = m_M[5].mat[0];
	m_M[10].mat[1] = m_M[5].mat[1];
	m_M[10].mat[2] = m_M[5].mat[2];

	/* CalculateJ\uTr322[1:3] = CalculateJ\omega3; */
	m_M[11].mat[0] = m_M[6].mat[0];
	m_M[11].mat[1] = m_M[6].mat[1];
	m_M[11].mat[2] = m_M[6].mat[2];

	/* CalculateJ\uTr433[1:3] = CalculateJ\omega4; */
	m_M[12].mat[0] = m_M[7].mat[0];
	m_M[12].mat[1] = m_M[7].mat[1];
	m_M[12].mat[2] = m_M[7].mat[2];

	/* CalculateJ\uTr544[1:3] = CalculateJ\omega5; */
	m_M[13].mat[0] = m_M[8].mat[0];
	m_M[13].mat[1] = m_M[8].mat[1];
	m_M[13].mat[2] = m_M[8].mat[2];

}

/* This function calculates the input equations of the model.
 * These equations are dynamic equations that must not change
 * in calls from the integration method (like random and delay).
 */
void ForceControl::CalculateInput (void)
{

}

/* This function calculates the dynamic equations of the model.
 * These equations are called from the integration method
 * to calculate the new model rates (that are then integrated).
 */
void ForceControl::CalculateDynamic (void)
{
	/* ControllableJ1\tip.e = tip.e; */
	XXMatrixMov (&m_M[40], &m_M[42]);

	/* ControllableJ1\joints.f = joints.f; */
	XXMatrixMov (&m_M[39], &m_M[45]);

	/* CalculateJ\Linkdim = Linkdim; */
	XXMatrixMov (&m_M[2], &m_M[46]);

	/* CalculateJ\q = q; */
	XXMatrixMov (&m_M[3], &m_M[47]);

	/* CalculateJ\R1ref = [cos (CalculateJ\q[1]), -sin (CalculateJ\q[1]), 0; sin (CalculateJ\q[1]), cos (CalculateJ\q[1]), 0; 0, 0, 1]; */
	m_M[14].mat[0] = cos (m_M[3].mat[0]);
	m_M[14].mat[1] = -(sin (m_M[3].mat[0]));
	m_M[14].mat[2] = 0.0;
	m_M[14].mat[3] = sin (m_M[3].mat[0]);
	m_M[14].mat[4] = cos (m_M[3].mat[0]);
	m_M[14].mat[5] = 0.0;
	m_M[14].mat[6] = 0.0;
	m_M[14].mat[7] = 0.0;
	m_M[14].mat[8] = 1.0;

	/* CalculateJ\p1ref = CalculateJ\Linkdim[1:3,1]; */
	m_M[20].mat[0] = m_M[2].mat[0];
	m_M[20].mat[1] = m_M[2].mat[6];
	m_M[20].mat[2] = m_M[2].mat[12];

	/* CalculateJ\H1ref = homogeneous (CalculateJ\R1ref, CalculateJ\p1ref); */
	XXMatrixHomogeneous (&m_M[27], &m_M[14], &m_M[20]);

	/* CalculateJ\R21 = [cos (CalculateJ\q[2]), 0, sin (CalculateJ\q[2]); 0, 1, 0; -sin (CalculateJ\q[2]), 0, cos (CalculateJ\q[2])]; */
	m_M[15].mat[0] = cos (m_M[3].mat[1]);
	m_M[15].mat[1] = 0.0;
	m_M[15].mat[2] = sin (m_M[3].mat[1]);
	m_M[15].mat[3] = 0.0;
	m_M[15].mat[4] = 1.0;
	m_M[15].mat[5] = 0.0;
	m_M[15].mat[6] = -(sin (m_M[3].mat[1]));
	m_M[15].mat[7] = 0.0;
	m_M[15].mat[8] = cos (m_M[3].mat[1]);

	/* CalculateJ\p21 = CalculateJ\Linkdim[1:3,2]; */
	m_M[21].mat[0] = m_M[2].mat[1];
	m_M[21].mat[1] = m_M[2].mat[7];
	m_M[21].mat[2] = m_M[2].mat[13];

	/* CalculateJ\H21 = homogeneous (CalculateJ\R21, CalculateJ\p21); */
	XXMatrixHomogeneous (&m_M[28], &m_M[15], &m_M[21]);

	/* CalculateJ\R32 = [cos (CalculateJ\q[3]), 0, sin (CalculateJ\q[3]); 0, 1, 0; -sin (CalculateJ\q[3]), 0, cos (CalculateJ\q[3])]; */
	m_M[16].mat[0] = cos (m_M[3].mat[2]);
	m_M[16].mat[1] = 0.0;
	m_M[16].mat[2] = sin (m_M[3].mat[2]);
	m_M[16].mat[3] = 0.0;
	m_M[16].mat[4] = 1.0;
	m_M[16].mat[5] = 0.0;
	m_M[16].mat[6] = -(sin (m_M[3].mat[2]));
	m_M[16].mat[7] = 0.0;
	m_M[16].mat[8] = cos (m_M[3].mat[2]);

	/* CalculateJ\p32 = CalculateJ\Linkdim[1:3,3]; */
	m_M[22].mat[0] = m_M[2].mat[2];
	m_M[22].mat[1] = m_M[2].mat[8];
	m_M[22].mat[2] = m_M[2].mat[14];

	/* CalculateJ\H32 = homogeneous (CalculateJ\R32, CalculateJ\p32); */
	XXMatrixHomogeneous (&m_M[29], &m_M[16], &m_M[22]);

	/* CalculateJ\R43 = [cos (CalculateJ\q[4]), 0, sin (CalculateJ\q[4]); 0, 1, 0; -sin (CalculateJ\q[4]), 0, cos (CalculateJ\q[4])]; */
	m_M[17].mat[0] = cos (m_M[3].mat[3]);
	m_M[17].mat[1] = 0.0;
	m_M[17].mat[2] = sin (m_M[3].mat[3]);
	m_M[17].mat[3] = 0.0;
	m_M[17].mat[4] = 1.0;
	m_M[17].mat[5] = 0.0;
	m_M[17].mat[6] = -(sin (m_M[3].mat[3]));
	m_M[17].mat[7] = 0.0;
	m_M[17].mat[8] = cos (m_M[3].mat[3]);

	/* CalculateJ\p43 = CalculateJ\Linkdim[1:3,4]; */
	m_M[23].mat[0] = m_M[2].mat[3];
	m_M[23].mat[1] = m_M[2].mat[9];
	m_M[23].mat[2] = m_M[2].mat[15];

	/* CalculateJ\H43 = homogeneous (CalculateJ\R43, CalculateJ\p43); */
	XXMatrixHomogeneous (&m_M[30], &m_M[17], &m_M[23]);

	/* CalculateJ\R54 = [cos (CalculateJ\q[5]), -sin (CalculateJ\q[5]), 0; sin (CalculateJ\q[5]), cos (CalculateJ\q[5]), 0; 0, 0, 1]; */
	m_M[18].mat[0] = cos (m_M[3].mat[4]);
	m_M[18].mat[1] = -(sin (m_M[3].mat[4]));
	m_M[18].mat[2] = 0.0;
	m_M[18].mat[3] = sin (m_M[3].mat[4]);
	m_M[18].mat[4] = cos (m_M[3].mat[4]);
	m_M[18].mat[5] = 0.0;
	m_M[18].mat[6] = 0.0;
	m_M[18].mat[7] = 0.0;
	m_M[18].mat[8] = 1.0;

	/* CalculateJ\p54 = CalculateJ\Linkdim[1:3,5]; */
	m_M[24].mat[0] = m_M[2].mat[4];
	m_M[24].mat[1] = m_M[2].mat[10];
	m_M[24].mat[2] = m_M[2].mat[16];

	/* CalculateJ\H54 = homogeneous (CalculateJ\R54, CalculateJ\p54); */
	XXMatrixHomogeneous (&m_M[31], &m_M[18], &m_M[24]);

	/* CalculateJ\ptip5 = CalculateJ\Linkdim[1:3,6]; */
	m_M[25].mat[0] = m_M[2].mat[5];
	m_M[25].mat[1] = m_M[2].mat[11];
	m_M[25].mat[2] = m_M[2].mat[17];

	/* CalculateJ\Htip5 = homogeneous (CalculateJ\Rtip5, CalculateJ\ptip5); */
	XXMatrixHomogeneous (&m_M[32], &m_M[19], &m_M[25]);

	/* CalculateJ\H10 = CalculateJ\Href0 * CalculateJ\H1ref; */
	XXMatrixMul (&m_M[33], &m_M[26], &m_M[27]);

	/* CalculateJ\H20 = CalculateJ\H10 * CalculateJ\H21; */
	XXMatrixMul (&m_M[34], &m_M[33], &m_M[28]);

	/* CalculateJ\H30 = CalculateJ\H20 * CalculateJ\H32; */
	XXMatrixMul (&m_M[35], &m_M[34], &m_M[29]);

	/* CalculateJ\H40 = CalculateJ\H30 * CalculateJ\H43; */
	XXMatrixMul (&m_M[36], &m_M[35], &m_M[30]);

	/* CalculateJ\H50 = CalculateJ\H40 * CalculateJ\H54; */
	XXMatrixMul (&m_M[37], &m_M[36], &m_M[31]);

	/* CalculateJ\Htip0 = CalculateJ\H50 * CalculateJ\Htip5; */
	XXMatrixMul (&m_M[1], &m_M[37], &m_M[32]);

	/* CalculateJ\uTr1refref[4:6] = skew (CalculateJ\Linkdim[1:3,1]) * CalculateJ\omega1; */
	m_M[53].mat[0] = m_M[2].mat[0];
	m_M[53].mat[1] = m_M[2].mat[6];
	m_M[53].mat[2] = m_M[2].mat[12];
	XXMatrixSkew (&m_M[52], &m_M[53]);
	XXMatrixMul (&m_M[51], &m_M[52], &m_M[4]);
	m_M[9].mat[3] = m_M[51].mat[0];
	m_M[9].mat[4] = m_M[51].mat[1];
	m_M[9].mat[5] = m_M[51].mat[2];

	/* CalculateJ\uTr211[4:6] = skew (CalculateJ\Linkdim[1:3,2]) * CalculateJ\omega2; */
	m_M[56].mat[0] = m_M[2].mat[1];
	m_M[56].mat[1] = m_M[2].mat[7];
	m_M[56].mat[2] = m_M[2].mat[13];
	XXMatrixSkew (&m_M[55], &m_M[56]);
	XXMatrixMul (&m_M[54], &m_M[55], &m_M[5]);
	m_M[10].mat[3] = m_M[54].mat[0];
	m_M[10].mat[4] = m_M[54].mat[1];
	m_M[10].mat[5] = m_M[54].mat[2];

	/* CalculateJ\uTr322[4:6] = skew (CalculateJ\Linkdim[1:3,3]) * CalculateJ\omega3; */
	m_M[59].mat[0] = m_M[2].mat[2];
	m_M[59].mat[1] = m_M[2].mat[8];
	m_M[59].mat[2] = m_M[2].mat[14];
	XXMatrixSkew (&m_M[58], &m_M[59]);
	XXMatrixMul (&m_M[57], &m_M[58], &m_M[6]);
	m_M[11].mat[3] = m_M[57].mat[0];
	m_M[11].mat[4] = m_M[57].mat[1];
	m_M[11].mat[5] = m_M[57].mat[2];

	/* CalculateJ\uTr433[4:6] = skew (CalculateJ\Linkdim[1:3,4]) * CalculateJ\omega4; */
	m_M[62].mat[0] = m_M[2].mat[3];
	m_M[62].mat[1] = m_M[2].mat[9];
	m_M[62].mat[2] = m_M[2].mat[15];
	XXMatrixSkew (&m_M[61], &m_M[62]);
	XXMatrixMul (&m_M[60], &m_M[61], &m_M[7]);
	m_M[12].mat[3] = m_M[60].mat[0];
	m_M[12].mat[4] = m_M[60].mat[1];
	m_M[12].mat[5] = m_M[60].mat[2];

	/* CalculateJ\uTr544[4:6] = skew (CalculateJ\Linkdim[1:3,5]) * CalculateJ\omega5; */
	m_M[65].mat[0] = m_M[2].mat[4];
	m_M[65].mat[1] = m_M[2].mat[10];
	m_M[65].mat[2] = m_M[2].mat[16];
	XXMatrixSkew (&m_M[64], &m_M[65]);
	XXMatrixMul (&m_M[63], &m_M[64], &m_M[8]);
	m_M[13].mat[3] = m_M[63].mat[0];
	m_M[13].mat[4] = m_M[63].mat[1];
	m_M[13].mat[5] = m_M[63].mat[2];

	/* CalculateJ\controllableJ[1:6,1] = Adjoint (CalculateJ\Href0) * CalculateJ\uTr1refref; */
	XXMatrixAdjoint (&m_M[67], &m_M[26]);
	XXMatrixMul (&m_M[66], &m_M[67], &m_M[9]);
	m_M[0].mat[0] = m_M[66].mat[0];
	m_M[0].mat[5] = m_M[66].mat[1];
	m_M[0].mat[10] = m_M[66].mat[2];
	m_M[0].mat[15] = m_M[66].mat[3];
	m_M[0].mat[20] = m_M[66].mat[4];
	m_M[0].mat[25] = m_M[66].mat[5];

	/* CalculateJ\controllableJ[1:6,2] = Adjoint (CalculateJ\H10) * CalculateJ\uTr211; */
	XXMatrixAdjoint (&m_M[69], &m_M[33]);
	XXMatrixMul (&m_M[68], &m_M[69], &m_M[10]);
	m_M[0].mat[1] = m_M[68].mat[0];
	m_M[0].mat[6] = m_M[68].mat[1];
	m_M[0].mat[11] = m_M[68].mat[2];
	m_M[0].mat[16] = m_M[68].mat[3];
	m_M[0].mat[21] = m_M[68].mat[4];
	m_M[0].mat[26] = m_M[68].mat[5];

	/* CalculateJ\controllableJ[1:6,3] = Adjoint (CalculateJ\H20) * CalculateJ\uTr322; */
	XXMatrixAdjoint (&m_M[71], &m_M[34]);
	XXMatrixMul (&m_M[70], &m_M[71], &m_M[11]);
	m_M[0].mat[2] = m_M[70].mat[0];
	m_M[0].mat[7] = m_M[70].mat[1];
	m_M[0].mat[12] = m_M[70].mat[2];
	m_M[0].mat[17] = m_M[70].mat[3];
	m_M[0].mat[22] = m_M[70].mat[4];
	m_M[0].mat[27] = m_M[70].mat[5];

	/* CalculateJ\controllableJ[1:6,4] = Adjoint (CalculateJ\H30) * CalculateJ\uTr433; */
	XXMatrixAdjoint (&m_M[73], &m_M[35]);
	XXMatrixMul (&m_M[72], &m_M[73], &m_M[12]);
	m_M[0].mat[3] = m_M[72].mat[0];
	m_M[0].mat[8] = m_M[72].mat[1];
	m_M[0].mat[13] = m_M[72].mat[2];
	m_M[0].mat[18] = m_M[72].mat[3];
	m_M[0].mat[23] = m_M[72].mat[4];
	m_M[0].mat[28] = m_M[72].mat[5];

	/* CalculateJ\controllableJ[1:6,5] = Adjoint (CalculateJ\H40) * CalculateJ\uTr544; */
	XXMatrixAdjoint (&m_M[75], &m_M[36]);
	XXMatrixMul (&m_M[74], &m_M[75], &m_M[13]);
	m_M[0].mat[4] = m_M[74].mat[0];
	m_M[0].mat[9] = m_M[74].mat[1];
	m_M[0].mat[14] = m_M[74].mat[2];
	m_M[0].mat[19] = m_M[74].mat[3];
	m_M[0].mat[24] = m_M[74].mat[4];
	m_M[0].mat[29] = m_M[74].mat[5];

	/* Htip0 = CalculateJ\Htip0; */
	XXMatrixMov (&m_M[48], &m_M[1]);

	/* ControllableJ1\joints.e = transpose (CalculateJ\controllableJ) * ControllableJ1\tip.e; */
	XXMatrixTranspose (&m_M[76], &m_M[0]);
	XXMatrixMul (&m_M[38], &m_M[76], &m_M[40]);

	/* ControllableJ1\tip.f = CalculateJ\controllableJ * ControllableJ1\joints.f; */
	XXMatrixMul (&m_M[41], &m_M[0], &m_M[39]);

	/* tip.f = ControllableJ1\tip.f; */
	XXMatrixMov (&m_M[43], &m_M[41]);

	/* joints.e = ControllableJ1\joints.e; */
	XXMatrixMov (&m_M[44], &m_M[38]);

}

/* This function calculates the output equations of the model.
 * These equations are not needed for calculation of the rates
 * and are kept separate to make the dynamic set of equations smaller.
 * These dynamic equations are called often more than one time for each
 * integration step that is taken. This makes model computation much faster.
 */
void ForceControl::CalculateOutput (void)
{

}

/* This function calculates the final equations of the model.
 * These equations are calculated after all the calculations
 * are performed
 */
void ForceControl::CalculateFinal (void)
{

}


