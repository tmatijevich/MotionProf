
TYPE
	KinGetAccSoln_typ : 	STRUCT 
		a : REAL;
		v12 : REAL;
		t1 : REAL;
		t2 : REAL;
		cs : USINT;
		sMessage : STRING[KIN_LENGTH_MESSAGE];
	END_STRUCT;
	KinGetAccInTimespanSoln_typ : 	STRUCT 
		a : REAL;
		cs : USINT;
		sMessage : STRING[KIN_LENGTH_MESSAGE];
	END_STRUCT;
	KinGetTimespanSoln_typ : 	STRUCT 
		tspan : REAL;
		tVmax1 : REAL;
		tVmax2 : REAL;
		tVmax : REAL;
		v1max : REAL;
		tVmin1 : REAL;
		tVmin2 : REAL;
		tVmin : REAL;
		v1min : REAL;
		cs : USINT;
		sMessage : STRING[KIN_LENGTH_MESSAGE];
	END_STRUCT;
	KinGetVelProfPointSoln_typ : 	STRUCT 
		x : REAL;
		v : REAL;
		a : REAL;
		segment : USINT;
	END_STRUCT;
END_TYPE
