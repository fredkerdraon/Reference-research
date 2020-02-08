//FK - Discounting a 100 cashflow in ten year at 6%
CT=100;
y=.06;
T=10;
PV=CT/(1+y)^T;
disp(PV);

//FK - Future value of 100 in 10 years
FV=CT*(1+y)^10;
disp(FV);

//FK - Semi annual rate conversion
// (1+y) = (1+y^S/2)^2)
yS=((1+y)^(1/2)-1)*2;
disp(yS)

//FK - PV of a 6% bond 
