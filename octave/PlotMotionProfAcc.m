function [] = PlotMotionProfAcc(dt, dx, v_0, v_f, v_min, v_max, k)

    output = OctaveMotionProfAcc(dt, dx, v_0, v_f, v_min, v_max);

    if output.status != 0
        return;
    end

    PlotMotionProfile(output, k, "MotionProfAcc");

end