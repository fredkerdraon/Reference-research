function [y] = ranf(x)
//Random Fourier series
y=[];
for j = 1:length(x)
y = [y ff(x(j))];
end;
endfunction;

function [y] = ff(x)
n=100*rand();
aa = int(10*(rand(1,n)-0.5));
bb = int(10*(rand(1,n)-0.5));
y = 0.0;
for j = 1:n
y = y + aa(j)*sin(2*%pi*x)+bb(j)*cos(2*%pi*x);
end;
endfunction;
