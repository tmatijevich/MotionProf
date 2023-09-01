
TYPE
	MotionProfMoveEnum : 
		( (*Movement types*)
		MOTIONPROF_MOVE_NONE := 0,
		MOTIONPROF_MOVE_DECACC := 1,
		MOTIONPROF_MOVE_DECACC_SATURATED := 2,
		MOTIONPROF_MOVE_ACCDEC := 3,
		MOTIONPROF_MOVE_ACCDEC_SATURATED := 4,
		MOTIONPROF_MOVE_DECDEC := 10,
		MOTIONPROF_MOVE_ACCACC := 11
		);
	MotionProfBaseOutputType : 	STRUCT  (*Base output structure*)
		t_ : ARRAY[0..3]OF LREAL; (*[s] Time points*)
		dx : LREAL; (*[units] Distance*)
		v_ : ARRAY[0..3]OF LREAL; (*[units/s] Velocity points*)
		a : LREAL; (*[units/s/s] Acceleration magnitude*)
		move : MotionProfMoveEnum; (*Movement type*)
	END_STRUCT;
END_TYPE
