
function [] = PlotMotionProfAccTimeDiff(dt_tilde, dx, v_0, v_f, v_min, v_max)

    output = OctaveMotionProfAccTimeDiff(dt_tilde, dx, v_0, v_f, v_min, v_max);

    if output.status != 0 
        return
    end

    figure_handle = figure(1, "name", "MotionProfAccTimeDiff");
    % set(figure_handle, "menubar", "none");
    figure_position = get(figure_handle, "position");
    set(figure_handle, "position", [figure_position(1:2), 700, 500]);
    font_size = 12;
    cla;
    ylim([max(0.0, v_min - 0.1), v_max + 0.1]);

    plot(output.dec_acc.t_(1:output.dec_acc.n), 
        output.dec_acc.v_(1:output.dec_acc.n), 
        'g');

    hold on;
    plot(output.acc_dec.t_(1:output.acc_dec.n), 
        output.acc_dec.v_(1:output.acc_dec.n), 
        'r');

    plot([output.acc_dec.t_(output.acc_dec.n); ...
        output.dec_acc.t_(output.dec_acc.n)],
        [output.acc_dec.v_(output.acc_dec.n); ...
        output.dec_acc.v_(output.dec_acc.n)], 
        'k--');

    xlabel('t', 'fontsize', font_size);
    ylabel('v', 'fontsize', font_size);

end