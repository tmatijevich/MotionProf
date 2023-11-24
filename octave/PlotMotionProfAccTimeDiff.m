
function [] = PlotMotionProfAccTimeDiff(dt_tilde, dx, v_0, v_f, v_min, v_max)

    output = OctaveMotionProfAccTimeDiff(dt_tilde, dx, v_0, v_f, v_min, v_max);

    if output.status != 0 
        return
    end

    t_plot = 0.0:0.001:output.dec_acc.t_(end);
    t_plot = transpose(t_plot);
    v_plot = zeros(size(t_plot));

    for i = 1:length(t_plot)
        point_output = OctaveMotionProfPoint(0.0, 
                                            output.dec_acc.t_, 
                                            output.dec_acc.v_, 
                                            length(output.dec_acc.t_), 
                                            t_plot(i), 
                                            1.0);
        if point_output.status == 0
            v_plot(i) = point_output.v;
        end
    end

    figure_handle = figure(1, "name", "MotionProfAccTimeDiff");
    % set(figure_handle, "menubar", "none");
    figure_position = get(figure_handle, "position");
    set(figure_handle, "position", [figure_position(1:2), 700, 500]);
    font_size = 12;

    plot(t_plot, v_plot, 'g');

    t_plot = 0.0:0.001:output.acc_dec.t_(end);
    t_plot = transpose(t_plot);
    v_plot = zeros(size(t_plot));

    for i = 1:length(t_plot)
        point_output = OctaveMotionProfPoint(0.0, 
                                            output.acc_dec.t_, 
                                            output.acc_dec.v_, 
                                            length(output.acc_dec.t_), 
                                            t_plot(i), 
                                            1.0);
        if point_output.status == 0
            v_plot(i) = point_output.v;
        end
    end

    figure(1);
    hold on;
    plot(t_plot, v_plot, 'r');

    plot([output.acc_dec.t_(end); output.dec_acc.t_(end)],
        [output.acc_dec.v_(end); output.dec_acc.v_(end)], 'k-')

end