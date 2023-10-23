function [output] = PlotMotionProfTime(dx, v_0, v_f, v_min, v_max, a, k)

    output = OctaveMotionProfTime(dx, v_0, v_f, v_min, v_max, a);

    if output.status != 0
        return;
    end

    PlotMotionProfile(output, k, "MotionProfTime");

end