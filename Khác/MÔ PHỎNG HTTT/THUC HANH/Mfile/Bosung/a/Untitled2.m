M = 16; % Number of points in constellation
h=modem.qammod(M); % Modulator object
mapping=h.SymbolMapping; % Symbol mapping vector
pt = h.Constellation; % Vector of all points in constellation
% Plot the constellation.
scatterplot(pt);
% Include text annotations that number the points.
text(real(pt)+0.1,imag(pt),dec2bin(mapping));
axis([-4 4 -4 4]); % Change axis so all labels fit in plot.
%% Modified Plot, With Gray Coding
M = 16; % Number of points in constellation
h = modem.qammod('M',M,'SymbolOrder','Gray'); % Modulator object
mapping = h.SymbolMapping; % Symbol mapping vector
pt = h.Constellation; % Vector of all points in constellation
scatterplot(pt); % Plot the constellation.
% Include text annotations that number the points.
text(real(pt)+0.1,imag(pt),dec2bin(mapping));
axis([-4 4 -4 4]); % Change axis so all labels fit in plot.