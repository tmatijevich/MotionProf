%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File: PlotMotionProfTimeDiff.m
% Created: 2023-10-06
%
% Contributors:
% - Tyler Matijevich
%
% License:
%  This file PlotMotionProfTimeDiff.m is part of the MotionProf project 
%  released under the GNU General Public License v3.0 agreement.  For more 
%  information, please visit 
%  https://github.com/tmatijevich/MotionProf/blob/main/LICENSE.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [] = PlotMotionProfTimeDiff(dx, v_0, v_f, v_min, v_max, a, k)
    
    profile = OctaveMotionProfTimeDiff(dx, v_0, v_f, v_min, v_max, a);
    
    if profile.status != 0
        return
    end

    t_plot = 0.0:0.001:profile.acc_dec.t_(end);
    t_plot = transpose(t_plot);
    x_plot = zeros(size(t_plot));
    v_plot = zeros(size(t_plot));
    a_plot = zeros(size(t_plot));
    j_plot = zeros(size(t_plot));

    for i = 1:length(t_plot)
        point_output = OctaveMotionProfPoint(0.0, profile.acc_dec.t_, 
                                            profile.acc_dec.v_, 
                                            length(profile.acc_dec.t_), 
                                            t_plot(i), k);
        if point_output.status == 0
            x_plot(i) = point_output.x;
            v_plot(i) = point_output.v;
            a_plot(i) = point_output.a;
            j_plot(i) = point_output.j;
        end
    end

    figure_handle = figure(1, "name", "MotionProfTimeDiff");
    % set(figure_handle, "menubar", "none");
    figure_position = get(figure_handle, "position");
    set(figure_handle, "position", [figure_position(1:2), 700, 700]);
    font_size = 12;

    subplot(3, 1, 1);
    cla;
    plot(t_plot, x_plot, 'g.');
    ylabel("x", "fontsize", font_size);

    subplot(3, 1, 2);
    cla;
    plot(t_plot, v_plot, 'g.');
    ylabel("v", "fontsize", font_size);

    subplot(3, 1, 3);
    cla;
    plot(t_plot, a_plot, 'g.');
    ylabel("a", "fontsize", font_size);

    t_plot = 0.0:0.001:profile.dec_acc.t_(end);
    t_plot = transpose(t_plot);
    x_plot = zeros(size(t_plot));
    v_plot = zeros(size(t_plot));
    a_plot = zeros(size(t_plot));
    j_plot = zeros(size(t_plot));

    for i = 1:length(t_plot)
        point_output = OctaveMotionProfPoint(0.0, profile.dec_acc.t_, 
                                            profile.dec_acc.v_, 
                                            length(profile.dec_acc.t_), 
                                            t_plot(i), k);
        if point_output.status == 0
            x_plot(i) = point_output.x;
            v_plot(i) = point_output.v;
            a_plot(i) = point_output.a;
            j_plot(i) = point_output.j;
        end
    end

    subplot(3, 1, 1);
    hold on;
    plot(t_plot, x_plot, 'r.');
    ylabel("x", "fontsize", font_size);

    subplot(3, 1, 2);
    hold on;
    plot(t_plot, v_plot, 'r.');
    ylabel("v", "fontsize", font_size);

    subplot(3, 1, 3);
    hold on;
    plot(t_plot, a_plot, 'r.');
    ylabel("a", "fontsize", font_size);
    
end