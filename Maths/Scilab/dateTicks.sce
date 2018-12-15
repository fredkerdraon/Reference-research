all = [datenum(2016, 01, 01); datenum(2016, 6, 01); datenum(2016, 12, 01)];
all2 = [all, [2;1;3]];
plot(all2(:,1), [all2(:,2)]);
h = gca();                                                                                          
v1 = h.x_ticks;                                                                                
v_dat = datevec(v1(2));                                                                  
v_dat = [string(v_dat(:,2)) string(v_dat(:,1))];                                  

v_neu=[];                                                                                          
s=size(v_dat);                                                                                  
for k=1:s(1)  v_neu=[v_neu  v_dat(k,1) + "/" + v_dat(k,2)]; end     
h.x_ticks.labels = v_neu;                                                                

//gg = gce();
h.title.text = "$f(\mathbf{x}) = x_1^2+x_2^2$";
xs2png(0,'Dates.png');
exit
