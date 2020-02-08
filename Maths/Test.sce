//Serie definition
u=[1:50]
for n=1:20
    w(n)=1/n^2
    //disp([n,u(n)])
   
end
a=gca();
a.tight_limits = "on";
//a.x_ticks=string([1 10 20 30 40 50]);
//x_label=a.x_label;
//x_label.text=" Weight"
//x_label.font_style= 5;
//a.y_label.foreground = 12;
//a.auto_ticks="on";
tmp=a.x_ticks;
tmp.locations=[1:5:20];
tmp.labels=string(tmp.locations);
a.x_ticks=tmp;
//a.auto_scale="on";
//a.sub_ticks=[.5,1]
bar(w)
hl=legend(['1/n^2'],pos=4,boxed=%f);
