funcprot(0);
function [chi2,cmark,fcount]=histnorm(x, xclass)
//This function calculates the frequency distribution
//for the data in (row) vector x according to the
//class boundaries contained in the (row) vector
//xclass. It also produces a histogram of the
//data and the normal curve that best fit the data.
//
//Typical call: [chi2,cm,f] = freqdist(x,xclass)
//where cm = class marks, f = frequency count,
//chi2 = chi-square parameter for the fitting
[m n] = size(x);
[m nB] = size(xclass);
k = nB - 1;
//Sample size
//Number of class boundaries
//Number of classes
//Calculate class marks
cmark = zeros(1,k);
for ii = 1:k
cmark(ii) = 0.5*(xclass(ii)+xclass(ii+1));
end
//Initialize frequency counts to zero
fcount=zeros(1,k);
fbelow=0; fabove=0;
//Accumulate frequency counts
for ii = 1:n
if x(ii) < xclass(1)
fbelow = fbelow + 1;
elseif x(ii) > xclass(nB)
fabove = fabove + 1;
else
for jj = 1:k
if x(ii)>= xclass(jj) & x(ii)< xclass(jj+1)
fcount(jj) = fcount(jj) +1;
end
end
end
end
//define normal CDF, calculate xbar, sx, chi-square parameter
nn = sum(fcount);
xbar = mean(x);
sx = stdev(x);
xmin = min(xclass); xmax = max(xclass);
pk = [];
for j = 1:k+1
pk = [pk cdfnor("PQ",xclass(j),xbar,sx)];
end;
p_in_classes = pk(k+1)-pk(1);
pxclass = pk(2:k+1) - pk(1:k);
fc = pxclass*nn*p_in_classes;
//Chi square parameter
chi2=0;
for j = 1:length(fc)
chi2 = chi2 + (fcount(j)-fc(j))^2/fc(j);
//chi2 = chi2 + (Count(j)-fc(j))^2/fc(j);
end;
//Produce normal distribution for data
Dx = (xmax-xmin)/100;
xx = [xmin:Dx:xmax];
xxx = xx(1:100) + Dx/2;
pkk = [];
for j = 1:101
pkk = [pkk cdfnor("PQ",xx(j),xbar,sx)];
end;
pp = pkk(2:101) - pkk(1:100);
fcc = pp*p_in_classes*nn*100/k;
//Determine plot rectangle
ymin = 0;
ymaxf = max(fcount); ymaxy = max(fcc);
ymax = max(ymaxf,ymaxy);
ymax = int(1.1*ymax);
plotrectangle = [xmin ymin xmax ymax];
//plot the histogram and normal curve
xp = xclass(1:k);
xset('window',1);xbasc(1);
plot2d2('onn',xclass',[fcount fcount(k)]',[1],'011','y',[xmin ymin xmax ymax]);
plot2d3('onn',xp',fcount',[1],'000');
plot2d(xxx',fcc',[2],'000');
xtitle('Histogram with normal curve','x','frequency');
endfunction histnorm

x=[1.05 1.07 1.09 1.11 1.13 1.15 1.17 1.19 1.21 1.23 1.25 1.27 1.29 1.31 1.33 1.35];
y=[17 15 27 47 63 85 117 129 126 112 88 69 42 31 16 16];
//bar(x,y);
val=y*x';
moy=val/sum(y);
disp(moy);
square=(x-moy).*(x-moy)
//disp(square);
//variance=(x-moy)^2*y';
variance=square*y'/sum(y);
disp(variance);
ecartType=sqrt(variance);
disp(ecartType);
random = moy + ecartType.*rand(1,20000);
//random = 150 + 50.*rand(1,200);
randomMin = min(random);
randomMax = max(random);
disp(randomMin);
disp(randomMax);
//disp(random);
//xclass = [1:1:20000];
//histnorm(random,xclass);
//plot(xclass,random,'ro-.');
histplot(100,random);
