handle = matlab_tid_client('connect','127.0.0.1', 9001);

disp(['Connected: '  num2str( matlab_tid_client('isConnected', handle)) ]);
disp(['Receiving: '  num2str( matlab_tid_client('isReceiving', handle)) ]);


disp('Blocking message receive:');
for n = 1:5
  msg = matlab_tid_client('Wait4NewTiDMessage', handle);
  disp(msg);
end

disp('Start receiving messages in the background:');
matlab_tid_client('startReceiving', handle);
disp(['Receiving: '  num2str( matlab_tid_client('isReceiving', handle)) ]);

disp('Waiting for 1 second...');
pause(3);

disp('Getting messages:');
msgs = matlab_tid_client('GetLastMessagesContexts', handle);
disp(msgs)

disp('Stopping receiving and disconnect:');
matlab_tid_client('stopReceiving', handle);
disp(['Receiving: '  num2str( matlab_tid_client('isReceiving', handle)) ]);

matlab_tid_client('disconnect',handle);
disp(['Connected: '  num2str( matlab_tid_client('isConnected', handle)) ]);