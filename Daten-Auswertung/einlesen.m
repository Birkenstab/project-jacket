data = readtable('data.txt');
data(:,22) = [];
data.Properties.VariableNames = {
    'mitte1AccX', 'mitte1AccY', 'mitte1AccZ', 'mitte1Temp', 'mitte1GyroX', 'mitte1GyroY', 'mitte1GyroZ', ...
    'mitte2AccX', 'mitte2AccY', 'mitte2AccZ', 'mitte2Temp', 'mitte2GyroX', 'mitte2GyroY', 'mitte2GyroZ', ...
    'linksAccX', 'linksAccY', 'linksAccZ', 'linksTemp', 'linksGyroX', 'linksGyroY', 'linksGyroZ'
    };

plot(data.mitte1GyroZ,'b')
hold on;
plot(data.mitte2GyroZ, 'r')
plot(data.linksGyroZ, 'g')

xline(376, 'm', 'Schacht');

xline(429, 'k', 'Arm ausfahren');
xline(462, 'k', 'Arm einfahren');

xline(541, 'k', 'Arm ausfahren');
xline(574, 'k', 'Arm einfahren');

xline(580, 'm', 'Vollbremsung');

%plot(content{5}, 'y')
%plot(content{6}, 'k')
%plot(content{7}, 'c')

hold off;
