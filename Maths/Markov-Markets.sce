//Serie definition
markov=[0.9 0.075 0.025; 0.15 0.8 0.05;0.25 0.25 0.5]
start=[0 1 0]
identity=[1 0;0 1]
resultat=start*markov^1000

//Exemple of markov matrix from Markov.pdf
start=[0 1]
markov=[-0.2 1;-1 -0.2]
control=[2.5 2.5]
gaussian=[0 0]

for n=1:1000
//gaussian=[rand() rand()]
gaussian=grand(2,"mn",0,1)
//Without control
//resultat=start*((identity+0.02*markov)^n)

if(n==1)
previous=start;
end
//With control
resultat=previous*((identity+0.02*markov)) + (control*gaussian')
previous=resultat


plot(resultat(1),resultat(2),".r",'markersize',2)
//p = scatter(resultat(1), resultat(2))
end

