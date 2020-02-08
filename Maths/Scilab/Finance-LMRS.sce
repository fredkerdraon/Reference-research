function V0=crr_put_europ(r,sigma,T,K,S0,M)
// version naÃ¯ve avec tableau !
  dt=T/M
  beta=.5*(exp(-r*dt)+exp((r+sigma^2)*dt))
  u=beta+sqrt((beta^2-1))
  d=1/u
  p=(exp(r*dt)-d)/(u-d)
  MS=zeros(M+1,M)
  MV=zeros(M+1,M)
  for i=1:M
    for j=0:i
      MS(j+1,i)=S0*u^j*d^(i-j)
    end
  end
  for j=0:M
    MV(j+1,M)=max(K-MS(j+1,M),0)
  end
  for i=M-1:-1:1
    for j=0:i
      MV(j+1,i)=exp(-r*dt)*(p*MV(j+2,i+1)+(1-p)*MV(j+1,i+1))
    end 
  end
  V0=exp(-r*dt)*(p*MV(2,1)+(1-p)*MV(1,1))
endfunction

function V0=crr_put_europ2(r,sigma,T,K,S0,M)
// version sans tableau
// encore optimisable en utilisant produit matriciel
// de Scilab
  dt=T/M
  beta=.5*(exp(-r*dt)+exp((r+sigma^2)*dt))
  u=beta+sqrt((beta^2-1))
  d=1/u
  p=(exp(r*dt)-d)/(u-d)
  MV=zeros(M+1,1)
  a=exp(-r*dt)
  for j=0:M
    MV(j+1,1)=max(K-S0*u^j*d^(M-j),0)
  end
  for i=M-1:-1:1 // boucles imbriquÃ©es !!
    for j=0:i
      MV(j+1,1)=a*(p*MV(j+2,1)+(1-p)*MV(j+1,1))
    end 
  end
  V0=a*(p*MV(2,1)+(1-p)*MV(1,1))
endfunction


function V0=crr_put_europ3(r,sigma,T,K,S0,M)
// version sans tableau
// 2nde optimisation
  dt=T/M
  beta=.5*(exp(-r*dt)+exp((r+sigma^2)*dt))
  u=beta+sqrt((beta^2-1))
  d=1/u
  p=(exp(r*dt)-d)/(u-d)
  MV=zeros(M+1,1)
  a=exp(-r*dt)
  for j=0:M
    MV(j+1,1)=max(K-S0*u^j*d^(M-j),0)
  end
  for i=M-1:-1:1 // 1 seule boucle
    MV(1:i+1)=a*(p*MV(2:i+2,1)+(1-p)*MV(1:i+1,1))
 // vectorisation Scilab : le top !
  end
  V0=a*(p*MV(2,1)+(1-p)*MV(1,1))
endfunction


function V0=crr_calldiv_europ3(r,sigma,dd,T,K,S0,M)
// version sans tableau
// 2nde optimisation
 
  dt=T/M
 beta=.5*(exp(-(r-dd)*dt)+exp((r-dd+sigma^2)*dt))

  u=beta+sqrt((beta^2-1))
  d=1/u
  p=(exp((r-dd)*dt)-d)/(u-d)
  MV=zeros(M+1,1)
  a=exp(-(r)*dt)
  for j=0:M
    MV(j+1,1)=max(S0*u^j*d^(M-j)-K,0)
  end
  for i=M-1:-1:1 // 1 seule boucle
    MV(1:i+1)=a*(p*MV(2:i+2,1)+(1-p)*MV(1:i+1,1))
 // vectorisation Scilab : le top !
  end
  V0=a*(p*MV(2,1)+(1-p)*MV(1,1))
endfunction

function V0=crr_put_amer(r,sigma,T,K,S0,M)
// version sans tableau
// plus difficilement optimisable 
// en utilisant produit matriciel
  dt=T/M
  beta=.5*(exp(-r*dt)+exp((r+sigma^2)*dt))
  u=beta+sqrt((beta^2-1))
  d=1/u
  p=(exp(r*dt)-d)/(u-d)
  MV=zeros(M+1,1)
  a=exp(-r*dt)
  for j=0:M
    MV(j+1,1)=max(K-S0*u^j*d^(M-j),0)
  end
  for i=M-1:-1:1 // boucles imbriquÃ©es !!
    for j=0:i
      MV(j+1,1)=max(max(K-S0*u^j*d^(i-j),0),a*(p*MV(j+2,1)+(1-p)*MV(j+1,1)))
    end 
  end
  V0=max(max(K-S0),a*(p*MV(2,1)+(1-p)*MV(1,1)))
endfunction


function V0=crr_call_amer(r,sigma,T,K,S0,M)
// version sans tableau
// plus difficilement optimisable 
// en utilisant produit matriciel
  dt=T/M
  beta=.5*(exp(-r*dt)+exp((r+sigma^2)*dt))
  u=beta+sqrt((beta^2-1))
  d=1/u
  p=(exp(r*dt)-d)/(u-d)
  MV=zeros(M+1,1)
  a=exp(-r*dt)
  for j=0:M
    MV(j+1,1)=max(S0*u^j*d^(M-j)-K,0)
  end
  for i=M-1:-1:1 // boucles imbriquÃ©es !!
    for j=0:i
      MV(j+1,1)=max(max(S0*u^j*d^(i-j)-K,0),a*(p*MV(j+2,1)+(1-p)*MV(j+1,1)))
    end 
  end
  V0=max(max(S0-K),a*(p*MV(2,1)+(1-p)*MV(1,1)))
endfunction

function V0=crr_calldiv_amer(r,sigma,dd,T,K,S0,M)
// version sans tableau
// plus difficilement optimisable 
// en utilisant produit matriciel
  
  dt=T/M
 beta=.5*(exp(-(r-dd)*dt)+exp((r-dd+sigma^2)*dt))

  u=beta+sqrt((beta^2-1))
  d=1/u
  p=(exp((r-dd)*dt)-d)/(u-d)
  MV=zeros(M+1,1)
  a=exp(-r*dt)
  for j=0:M
    MV(j+1,1)=max(S0*u^j*d^(M-j)-K,0)
  end
  for i=M-1:-1:1 // boucles imbriquÃ©es !!
    for j=0:i
      MV(j+1,1)=max(max(S0*u^j*d^(i-j)-K,0),a*(p*MV(j+2,1)+(1-p)*MV(j+1,1)))
    end 
  end
  V0=max(max(S0-K),a*(p*MV(2,1)+(1-p)*MV(1,1)))
endfunction

function V=callbs_formule(S,K,T,r,sig,dd)
  d1=(log(S/K)+(r-dd+sig^2/2)*T)/(sig*T^.5)
  d2=d1-sig*T^.5
  D1=cdfnor("PQ",d1,0,1)
  D2=cdfnor("PQ",d2,0,1)
  V=S*exp(-dd*T)*D1-K*exp(-T*r)*D2
endfunction

function V=putbs_formule(S,K,T,r,sig)
  d1=(log(S/K)+(r+sig^2/2)*T)/(sig*T^.5)
  d2=d1-sig*T^.5
  D1=cdfnor("PQ",-d1,0,1)
  D2=cdfnor("PQ",-d2,0,1)
  V=-S*D2+K*exp(-T*r)*D1
endfunction


xpt=linspace(1,30,20);
K=10
r=.25
sigma=.6
T=1
dd=.2
M=1024
VAM=[]
VEU=[]
for i=1:length(xpt)
  VAM=[VAM, callbs_formule(xpt(i),K,T,r,sigma,dd)];
  VEU=[VEU, crr_calldiv_amer(r,sigma,dd,T,K,xpt(i),M)];
end


