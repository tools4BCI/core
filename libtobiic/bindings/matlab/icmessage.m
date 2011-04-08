% 2010-03-17  Michele Tavella <michele.tavella@epfl.ch>
clear all;

message = tobiic_new();

tobiic_addclassifier(message, ...
	'cnbi_mi', 'MI Classifier', ...
	tobiic_getvaluetype('prob'), tobiic_getlabeltype('biosig'));
tobiic_addclass(message, 'cnbi_mi', '0x300', 0.60);
tobiic_addclass(message, 'cnbi_mi', '0x301', 0.10);
tobiic_addclass(message, 'cnbi_mi', '0x302', 0.30);

tobiic_addclassifier(message, ...
	'fsl_erp', 'ERP Classifier', ...
	tobiic_getvaluetype('rcoe'), tobiic_getlabeltype('custom'));
tobiic_addclass(message, 'fsl_erp', 'A', 1.00);
tobiic_addclass(message, 'fsl_erp', 'B', 2.00);
tobiic_addclass(message, 'fsl_erp', 'C', 3.00);


value = tobiic_getvalue(message, 'cnbi_mi', '0x300');
disp(['[icmessage] Value: ' num2str(value)]);
tobiic_setvalue(message, 'cnbi_mi', '0x300', 1000);
value = tobiic_getvalue(message, 'cnbi_mi', '0x300');
disp(['[icmessage] Value: ' num2str(value)]);

buffer = tobiic_serialize(message);
disp(['[icmessage] Sender has: ' buffer]);

message2 = tobiic_new();
tobiic_deserialize(message2, buffer);

buffer2 = tobiic_serialize(message2);
disp(['[icmessage] Receiver got: ' buffer2]);

disp('[icmessage] Stress test with 20s loop at 512Hz (check CPU usage)');
tic;
it = 0;
while(toc < 20)
    buffer = tobiic_serialize(message);
    tobiic_deserialize(message2, buffer);
    it = it + 1;
    pause(1/512);
end


tobiic_delete(message);
tobiic_delete(message2);
