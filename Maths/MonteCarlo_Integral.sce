function y=f(x)
   y=log(x+1)
endfunction
x=linspace(0,1,100)
plot(x,f)
k=0
n=10000
//n=input("Number of points =")
for i=1:n
   x=rand()
   y=rand()
   if y<f(x) then k=k+1
   end
   plot(x,y,"r--.",'markersiz',1)
   //plot(x,y,-10)
end
//a=gca();//axes courants
//p=a.children(3);//première courbe (celle en noir)
//p.foreground=5;//maintenant en rouge 
//p.thickness=2
k/n
title(string(k/n))
