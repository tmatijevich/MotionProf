
TYPE
	PathPlanRootsOutputType : 	STRUCT  (*Roots output structure*)
		r_1 : LREAL; (*First root*)
		r_2 : LREAL; (*Second root*)
	END_STRUCT;
	PathPlanMoveEnum : 
		( (*Movement types*)
		PATHPLAN_MOVE_NONE := 0,
		PATHPLAN_MOVE_DEC_ACC := 1,
		PATHPLAN_MOVE_DEC_ACC_SATURATED := 2,
		PATHPLAN_MOVE_ACC_DEC := 3,
		PATHPLAN_MOVE_ACC_DEC_SATURATED := 4,
		PATHPLAN_MOVE_DEC_DEC := 10,
		PATHPLAN_MOVE_ACC_ACC := 11
		);
	PathPlanBaseOutputType : 	STRUCT  (*Base output structure*)
		t_ : ARRAY[0..3]OF LREAL; (*[s] Time points*)
		dx : LREAL; (*[units] Distance*)
		v_ : ARRAY[0..3]OF LREAL; (*[units/s] Velocity points*)
		a : LREAL; (*[units/s/s] Acceleration magnitude*)
		move : PathPlanMoveEnum; (*Movement type*)
	END_STRUCT;
END_TYPE
