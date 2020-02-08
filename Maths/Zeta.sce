//Serie definition
u=[1:50]
for n=1:50
    w(n)=1/n^2
    //disp([n,u(n)])
    
end
a=gca();
//a.tight_limits = "on"; 
//a.x_ticks=string([1 10 20 30 40 50]);
//x_label=a.x_label;
//x_label.text=" Weight"
//x_label.font_style= 5;
//a.y_label.foreground = 12;
//a.auto_ticks="on";
bar(w)
tmp=a.x_ticks;
tmp.locations=[0:10:50];
tmp.labels=string(tmp.locations);
//tmp.labels = =["0";"10";"20";"30";"40";"50"]
//tmp.labels =["0";"10";"20";"30";"40";"50"]
//tmp.locations = [0;10;20;30;40;50]
a.x_ticks=tmp; 
//a.auto_scale="on";
//a.sub_ticks=[.5,1]



