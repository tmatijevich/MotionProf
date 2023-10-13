
TYPE
	MotionProfMoveEnum : 
		( (*Movement types*)
		MOTIONPROF_MOVE_NONE := 0, (*Undefined profile*)
		MOTIONPROF_MOVE_DEC := 1, (*Decelerate then accelerate profile*)
		MOTIONPROF_MOVE_ACC := 2, (*Decelerate then accelerate profile*)
		MOTIONPROF_MOVE_DECACC := 10, (*Decelerate then accelerate profile*)
		MOTIONPROF_MOVE_DECACC_SATURATED := 11, (*Decelerate, saturate at min velocity, accelerate profile*)
		MOTIONPROF_MOVE_ACCDEC := 12, (*Accelerate then decelerate profile*)
		MOTIONPROF_MOVE_ACCDEC_SATURATED := 13, (*Accelerate, saturate at max velocity, decelerate profile*)
		MOTIONPROF_MOVE_DECDEC := 20, (*Decelerate, hold at intermediate velocity, decelerate profile*)
		MOTIONPROF_MOVE_ACCACC := 21 (*Accelerate, hold at intermediate velocity, accelerate profile*)
		);
	MotionProfProfileType : 	STRUCT  (*Profile structure*)
		NumberOfPoints : USINT; (*Number of points in profile*)
		TimePoints : ARRAY[0..4]OF LREAL; (*[s] Time points*)
		Distance : LREAL; (*[units] Distance*)
		VelocityPoints : ARRAY[0..4]OF LREAL; (*[units/s] Velocity points*)
		Acceleration : LREAL; (*[units/s/s] Acceleration magnitude*)
		MoveType : MotionProfMoveEnum; (*Movement type*)
	END_STRUCT;
	MotionProfBaseOutputType : 	STRUCT  (*Base output structure*)
		t_ : ARRAY[0..3]OF LREAL; (*[s] Time points*)
		dx : LREAL; (*[units] Distance*)
		v_ : ARRAY[0..3]OF LREAL; (*[units/s] Velocity points*)
		a : LREAL; (*[units/s/s] Acceleration magnitude*)
		move : MotionProfMoveEnum; (*Movement type*)
	END_STRUCT;
	MotionProfPointOutputType : 	STRUCT  (*Time series point output structure*)
		x : LREAL; (*[units] Position*)
		v : LREAL; (*[units/s] Velocity*)
		a : LREAL; (*[units/s/s] Acceleration*)
		j : LREAL; (*[units/s/s/s] Jerk*)
	END_STRUCT;
	MotionProfTimeDiffOutputType : 	STRUCT  (*Time duration difference output structure*)
		dt_tilde : LREAL; (*Difference in time durations*)
		AccDec : MotionProfBaseOutputType; (*AccDec profile*)
		DecAcc : MotionProfBaseOutputType; (*DecAcc profile*)
	END_STRUCT;
END_TYPE
