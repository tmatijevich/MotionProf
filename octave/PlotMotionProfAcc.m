function [] = PlotMotionProfAcc(dt, dx, v_0, v_f, v_min, v_max, k)

    acc_solution = OctaveMotionProfAcc(dt, dx, v_0, v_f, v_min, v_max);

    if acc_solution.status != 0
        return;
    end

    t_plot = 0.0:0.001:acc_solution.t_(4);
    t_plot = transpose(t_plot);
    x_plot = zeros(size(t_plot));
    v_plot = zeros(size(t_plot));
    a_plot = zeros(size(t_plot));
    j_plot = zeros(size(t_plot));

    for i = 1:length(t_plot)
        point_solution = OctaveMotionProfPoint(0.0, acc_solution.t_, acc_solution.v_, 4, t_plot(i), k);
        if point_solution.status == 0
            x_plot(i) = point_solution.x;
            v_plot(i) = point_solution.v;
            a_plot(i) = point_solution.a;
            j_plot(i) = point_solution.j;
        end
    end

    figure(1);
    cla;

    subplot(4, 1, 1);
    plot(t_plot, x_plot, 'k.');

    subplot(4, 1, 2);
    plot(t_plot, v_plot, 'r.');

    subplot(4, 1, 3);
    plot(t_plot, a_plot, 'g.');

    subplot(4, 1, 4);
    plot(t_plot, j_plot, 'b.');

end