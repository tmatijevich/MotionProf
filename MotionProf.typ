
TYPE
	MotionProfMoveEnum : 
		( (*Movement types*)
		MOTIONPROF_MOVE_NONE := 0, (*Undefined profile*)
		MOTIONPROF_MOVE_ZERO := 1, (*Zero acceleration profile*)
		MOTIONPROF_MOVE_DEC := 2, (*Decelerate then accelerate profile*)
		MOTIONPROF_MOVE_ACC := 3, (*Decelerate then accelerate profile*)
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
	MotionProfPointType : 	STRUCT  (*Time series point output structure*)
		Position : LREAL; (*[units] Position*)
		Velocity : LREAL; (*[units/s] Velocity*)
		Acceleration : LREAL; (*[units/s/s] Acceleration*)
		Jerk : LREAL; (*[units/s/s/s] Jerk*)
	END_STRUCT;
	MotionProfTimeDiffType : 	STRUCT  (*Time duration difference output structure*)
		TimeDifference : LREAL; (*[s] Difference in time durations*)
		AccDec : MotionProfProfileType; (*AccDec profile*)
		DecAcc : MotionProfProfileType; (*DecAcc profile*)
	END_STRUCT;
END_TYPE
