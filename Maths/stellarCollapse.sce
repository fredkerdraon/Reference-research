function [  ] = Stellar_Collapse_1(  )
//%Written by Elmer G. Wiens December 2018
//%Stellar Collapse
//%Eddington-Finkelstein Coordinates
M = 1; 
M2 = 2;
rv = 4.5;
up = 22; 
low = 7.5; 
steps = 100;
//figure(1)
//hold on
vr = linspace(0, rv, steps);
vones = ones(1,steps);
tvones = [10.86, 11.37, 12.21, 15.42, 15.91, 17.03, 21] ;
for k = 1:length(tvones)
plot(vr, tvones(k) * vones, 'b','LineWidth',.1);
end
//axis([0 rv low up])
//line([0 rv], [0 0], 'Color','black','LineWidth',2)
//line([0 0], [low up], 'Color','black','LineWidth',2)
//line([M2 M2], [low up], 'Color','red','LineWidth',2)
//line([0, rv], [up, up], 'Color','black','LineWidth',1) 
//line([rv, rv], [low, up], 'Color','black','LineWidth',1) 
xlabel('r/M', 'FontSize',4)
ylabel('v', 'FontSize',4)
//title({'Eddington-Finkelstein Coordinates','Null Geodesics: blue; Radial Geodesics: red'}, 'FontSize',2)
title('Eddington-Finkelstein Coordinates','FontSize',2)
c = 10;
ct = [4,  0,  -4, -8, -12, -16, -20];
rtu = 2.01;
r2 = linspace(rtu, rv, steps);
for j = 1:length(r2)
rs2(j) = r2(j) + M2 * log(r2(j) / M2 -1)  + c;
end
for i =1:length(ct)
plot(r2, 2*rs2 + ct(i), 'b','LineWidth',.1)
end
//%return
c = 13.5;
stu = .01;
r1 = linspace(0, M2 -stu, steps);
for j = 1:length(r1)
rs1(j) = r1(j) + M2 * log(abs( r1(j) / M2  -1)) + c;
end    
for i =1:length(ct)
plot(r1, 2*rs1 + ct(i), 'b', 'LineWidth',.1)
end
r = [.5, 1.5, 2.5,  3.0000,    3.5000, 4];
for i = 1:length(r)
line([r(i) r(i)], [low up], 'Color','red','LineWidth',1)
end
line([M2 M2], [low up], 'Color','red','LineWidth',2)
%taustar = 5.2704;
taustar = 8.83;
tau = linspace(0, taustar, steps*3);
for j = 1:length(r)
	tstar = -M2 * ( (-2/3) * (r(j)/M2)^(3/2) -2 * (r(j)/M2)^(1/2) + log(abs(( (r(j)/M2)^(1/2) + 1) / ((r(j)/M2)^(1/2) -1 ))));
	for i = 1:length(tau)
	rtau(i) = (3/2)^(2/3) * M2^(1/3)* (taustar -tau(i))^(2/3);
	ttau(i) = tstar + M2 * ( (-2/3) * (rtau(i)/M2)^(3/2) -2 * (rtau(i)/M2)^(1/2) + log(abs(( (rtau(i)/M2)^(1/2) + 1) / ((rtau(i)/M2)^(1/2) -1 ))));
	end
	for i = 1:length(rtau)
	rstau(i) = rtau(i) + M2 * log(abs( rtau(i) / M2  -1)) + c;
	vtau(i) = ttau(i) +rstau(i);
	end
	plot(rtau, vtau, 'r', 'LineWidth', .5);
end
text(2.42, 13.6, 'r = 2.5M', 'FontSize',1)
plot(2.5, 13.23, 'k*', 'LineWidth', .5)
text(0.42, 12.8, 'r = 0.5M', 'FontSize',1)
plot(0.5, 12.51, 'k*', 'LineWidth', .5)
text(3.42, 16.9, 'r = 3.5M', 'FontSize',1)
plot(3.5, 16.42, 'k*', 'LineWidth', .5)
set(gca,'FontSize',2)
endfunction

