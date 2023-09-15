function [] = PlotMotionProfDist(dt, v_0, v_f, v_min, v_max, a, k)

    output = OctaveMotionProfDist(dt, v_0, v_f, v_min, v_max, a);

    if output.status != 0
        return;
    end

    PlotMotionProfile(output, k, "MotionProfDist");

end