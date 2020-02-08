//Serie definition
//u=[1:50]
aggreg=[0 0 0 0 0 0 0 0 0 0 0];
for n=1:1000000
    //resultat=input("Resultat?");
    de1=ceil(6*rand());
    de2=ceil(6*rand());
    resultat=de1+de2;
    if(resultat==2)
	aggreg(1)=aggreg(1)+1;
    elseif(resultat==3)
	aggreg(2)=aggreg(2)+1;
    elseif(resultat==4)
	aggreg(3)=aggreg(3)+1;
    elseif(resultat==5)
	aggreg(4)=aggreg(4)+1;
    elseif(resultat==6)
	aggreg(5)=aggreg(5)+1;
    elseif(resultat==7)
	aggreg(6)=aggreg(6)+1;
    elseif(resultat==8)
	aggreg(7)=aggreg(7)+1;
    elseif(resultat==9)
	aggreg(8)=aggreg(8)+1;
    elseif(resultat==10)
	aggreg(9)=aggreg(9)+1;
    elseif(resultat==11)
	aggreg(10)=aggreg(10)+1;
    elseif(resultat==12)
	aggreg(11)=aggreg(11)+1;
    end	
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
bar(aggreg)
