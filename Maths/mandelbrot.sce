function R = computeMandelbrotVect(xsize,ysize,nmax,xmin,xmax,ymin,ymax)
    xvect = linspace( xmin, xmax, xsize );
    yvect = linspace( ymin, ymax, ysize );
    [X,Y]=meshgrid(xvect,yvect);

    Z = zeros(xsize,ysize);
    R = -ones(xsize,ysize);
    W = zeros(xsize,ysize);
    C=X+%i*Y;
    J = 1:xsize*ysize;
    for k=0:nmax
        L = J(J>0);
        Z(L) = Z(L).^2+C(L);
        W(L) = abs(Z(L));
        M = find(W(L)>2);
        R(L(M)) = k;
        J(L(M)) = 0;
    end
    R = R';
    // The maximum number of colors
    CMAX = 1000;
    f=gcf();
    f.color_map = jetcolormap(CMAX);
    D = R;
    k = find(R<>-1);
    D(k) = floor(R(k)/max(R(k))*CMAX);
    k = find(R==-1);
    D(k) = CMAX;
    Matplot(D);
    f.children.isoview="on";
    f.children.axes_visible=["off" "off" "off"];
endfunction

//stacksize("max");
xsize = 200;
ysize = 200;
nmax = 10000;
xmin = 0.2675;
xmax = 0.2685;
ymin = 0.591;
ymax = 0.592;

tic();
R = computeMandelbrotVect(xsize,ysize,nmax,xmin,xmax,ymin,ymax);
t = toc();
mprintf("Time = %f (s)\n",t);
PPS = floor(xsize*ysize/t);
mprintf("PPS = %d\n",PPS);
plotFractal(R,1000);
