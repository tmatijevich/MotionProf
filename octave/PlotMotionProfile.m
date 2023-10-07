%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File: PlotMotionProfile.m
% Created: 2021-10-08
%
% Contributors:
% - Tyler Matijevich
%
% License:
%  This file PlotMotionProfile.m is part of the MotionProf project released 
%  under the GNU General Public License v3.0 agreement.  For more information, %  please visit https://github.com/tmatijevich/MotionProf/blob/main/LICENSE.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [] = PlotMotionProfile(profile, k, title)

    t_ = profile.t_(1:profile.n);
    v_ = profile.v_(1:profile.n);
    t_plot = 0.0:0.001:t_(end);
    t_plot = transpose(t_plot);
    x_plot = zeros(size(t_plot));
    v_plot = zeros(size(t_plot));
    a_plot = zeros(size(t_plot));
    j_plot = zeros(size(t_plot));

    for i = 1:length(t_plot)
        point_output = OctaveMotionProfPoint(0.0, t_, v_, profile.n, 
                                            t_plot(i), k);
        if point_output.status == 0
            x_plot(i) = point_output.x;
            v_plot(i) = point_output.v;
            a_plot(i) = point_output.a;
            j_plot(i) = point_output.j;
        end
    end

    figure_handle = figure(1, "name", title);
    % set(figure_handle, "menubar", "none");
    figure_position = get(figure_handle, "position");
    set(figure_handle, "position", [figure_position(1:2), 700, 700]);
    cla;
    font_size = 12;

    subplot(4, 1, 1);
    plot(t_plot, x_plot, 'k.');
    ylabel("x", "fontsize", font_size);

    subplot(4, 1, 2);
    plot(t_plot, v_plot, 'r.');
    ylabel("v", "fontsize", font_size);

    subplot(4, 1, 3);
    plot(t_plot, a_plot, 'g.');
    ylabel("a", "fontsize", font_size);

    subplot(4, 1, 4);
    plot(t_plot, j_plot, 'b.');
    xlabel("Time [s]", "fontsize", font_size);
    ylabel("j", "fontsize", font_size);
    
end