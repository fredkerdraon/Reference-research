//// Plot trajectory of Lorenz system
 clear; clf;
 
 function udot=lorenz_ode(t,u)
// odefile for lorenz system
  udot(1)=a*(u(2)-u(1));
  udot(2)=u(1)*(b-u(3))-u(2);
  udot(3)=u(1)*u(2)-c*u(3);
  udot=udot';
endfunction

 a=10; b=28; c=8/3;
 u0 = zeros(3,1)+0.5; t=(0:0.02:100)';
 u=ode(u0,0,t,lorenz_ode);
 plot(u(1,:),u(3,:))

