% mean, adaptive_mean, adaptive_var, win_mean, win_median, win_min, win_max, win_var
%  1        2               3           4           5         6        7        8

% /home/breidi/playground/libTiD-build-desktop/tests/libtid_recv_client_desc_len_5nr_reps_1000000.csv

tests = {'tobiid_deserialize', 'tobiid_serialize', ...
             'libtid_send_client', 'libtid_recv_client', };
subsitutors = {'_desc_len_', 'nr_reps_'};
file_type = '.csv';

addpath('export_fig');

for k = 1:length(tests)

  files = dir([tests{k} subsitutors{1} '*' file_type]);
  means = [];
  desc_lens = [];

  for l = 1:length(files)  
    disp([ 'Processing file: ' files(l).name ] );

    p1 = strfind(files(l).name, subsitutors{1} );
    p2 = strfind(files(l).name, subsitutors{2} );
    p3 = strfind(files(l).name, file_type );

    desc_len = str2num( files(l).name( p1 + length(subsitutors{1}) :  p2-1 ) );
    nr_reps  = str2num( files(l).name( p2 + length(subsitutors{2}) :  p3-1 ) );

    vals = reshape( csvread(files(l).name) ,8,[]);

    fig = figure;
    hold on; grid on;
    ylim([0,15]);
    plot(vals(1,:), 'r', 'LineWidth', 2);    % mean
    plot(vals(4,:), 'b', 'LineWidth', 0.5);    % win mean
    plot(vals(5,:), 'k--', 'LineWidth', 0.5);   % win median

    plot(vals(6,:), 'c');  % win min
    plot(vals(7,:), 'Color', [0.85 0.85 0.85], 'LineWidth', 0.5);  % win max

    plot(vals(8,:), 'g');    % win var

    xlabel(['TiD message nr']);
    ylabel(['Time (\mus)']);
    legend('mean','win mean','win median','win min','win max','win var');
    [p n e] = fileparts(files(l).name);
    eval( [' export_fig ' n ' -eps -native -q95 -a1 -cmyk -painters '] );

    close(fig);

    means = [means; vals(1,:)];
    desc_lens = [desc_lens desc_len];
  end

  fig = figure;
  hold on;
  grid on;
  ylim([2.5,3.5]);

  plot(means(1,:), 'r');
  plot(means(2,:), 'b');
  plot(means(3,:), 'k');
  plot(means(4,:), 'c');
  plot(means(5,:), 'm');


  xlabel(['TiD message nr']);
  ylabel(['Time (\mus)']);

  legend( [ num2str(desc_lens(1)) ' characters' ], ...
          [ num2str(desc_lens(2)) ' characters' ], ...
          [ num2str(desc_lens(3)) ' characters' ], ...
          [ num2str(desc_lens(4)) ' characters' ], ...
          [ num2str(desc_lens(5)) ' characters' ] );

  eval( [' export_fig ' tests{k} ' -eps -native -q95 -a1 -cmyk -painters '] );
  close(fig);

end




  