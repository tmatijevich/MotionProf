function [] = PlotMotionProfVel(dt, dx, v_0, v_f, v_min, v_max, a, k)

    output = OctaveMotionProfVel(dt, dx, v_0, v_f, v_min, v_max, a);

    if output.status != 0
        return;
    end

    PlotMotionProfile(output, k, "MotionProfVel");

end