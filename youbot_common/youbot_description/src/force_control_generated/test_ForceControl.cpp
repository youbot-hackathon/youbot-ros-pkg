/**********************************************************
 * This file is generated by 20-sim C++ Code Generator
 *
 *  file:  test_ForceControl.cpp
 *  subm:  ForceControl
 *  model: ForceControl
 *  expmt: 
 *  date:  April 8, 2011
 *  time:  11:46:47 am
 *  user:  Campuslicentie
 *  from:  Universiteit Twente
 *  build: 4.1.2.2
 **********************************************************/

/* This file is a demo application of how the submodel function can
 * be used. It uses the global time variables that are used by both
 * the submodel and the integration method.
 *
 * PLEASE NOTE: THIS IS AN EXAMPLE WHERE ALL INPUTS ARE ZERO !
 * USE YOUR OWN INPUTS INSTEAD!! ALSO THE SUBMODEL MIGHT SIMPLY
 * NOT WORK CORRECTLY WITH INPUTS THAT ARE ZERO.
 */

#include <stdio.h>

/* 20-sim submodel class include file */
#include "ForceControl.h"

/* the main function */
int main()
{
	XXDouble u [34 + 1];
	XXDouble y [27 + 1];

	/* initialize the inputs and outputs with correct initial values */
	u[0] = 0.0;		/* joints.f */
	u[1] = 0.0;
	u[2] = 0.0;
	u[3] = 0.0;
	u[4] = 0.0;
	u[5] = 0.0;		/* Linkdim */
	u[6] = 0.0;
	u[7] = 0.0;
	u[8] = 0.0;
	u[9] = 0.0;
	u[10] = 0.0;
	u[11] = 0.0;
	u[12] = 0.0;
	u[13] = 0.0;
	u[14] = 0.0;
	u[15] = 0.0;
	u[16] = 0.0;
	u[17] = 0.0;
	u[18] = 0.0;
	u[19] = 0.0;
	u[20] = 0.0;
	u[21] = 0.0;
	u[22] = 0.0;
	u[23] = 0.0;		/* q */
	u[24] = 0.0;
	u[25] = 0.0;
	u[26] = 0.0;
	u[27] = 0.0;
	u[28] = 0.0;		/* tip.e */
	u[29] = 0.0;
	u[30] = 0.0;
	u[31] = 0.0;
	u[32] = 0.0;
	u[33] = 0.0;

	y[0] = 0.0;		/* Htip0 */
	y[1] = 0.0;
	y[2] = 0.0;
	y[3] = 0.0;
	y[4] = 0.0;
	y[5] = 0.0;
	y[6] = 0.0;
	y[7] = 0.0;
	y[8] = 0.0;
	y[9] = 0.0;
	y[10] = 0.0;
	y[11] = 0.0;
	y[12] = 0.0;
	y[13] = 0.0;
	y[14] = 0.0;
	y[15] = 0.0;
	y[16] = 0.0;		/* joints.e */
	y[17] = 0.0;
	y[18] = 0.0;
	y[19] = 0.0;
	y[20] = 0.0;
	y[21] = 0.0;		/* tip.f */
	y[22] = 0.0;
	y[23] = 0.0;
	y[24] = 0.0;
	y[25] = 0.0;
	y[26] = 0.0;


	ForceControl my20simSubmodel;

	/* initialize the submodel itself */
	my20simSubmodel.Initialize(u, y, 0.0);

	/* simple loop, the time is incremented by the integration method */
	while (my20simSubmodel.state != ForceControl::finished)
	{
		/* call the submodel to calculate the output */
		my20simSubmodel.Calculate (u, y);
		printf("Time: %f\n", my20simSubmodel.GetTime() );
	}

	/* perform the final calculations */
	my20simSubmodel.Terminate (u, y);

	/* and we are done */
	return 0;
}

