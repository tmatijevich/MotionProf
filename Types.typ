
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
END_TYPE
