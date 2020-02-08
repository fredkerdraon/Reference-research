//5% Interest saving with a Kapital of 50 at the begining
w(1)=50
for n=1:50
    w(n+1)=w(n)+w(n)*0.05
end
a=gca();
bar(w)
tmp=a.x_ticks;
tmp.locations=[0:10:50];
tmp.labels=string(tmp.locations);
a.x_ticks=tmp; 


