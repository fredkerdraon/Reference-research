//********************************************************************
// Modèle de Lotka-Volterra
// Dominique Lefebvre  Octobre 2012
// TangenteX.com
//********************************************************************

// système différentiel de Lotka-Volterra
//y1 = population des proies, y2 = population des prédateurs
function [w] = LotkaVolterra(t,y)
  w(1) = a*y(1) - b*y(1)*y(2);
  w(2) = c*y(1)*y(2) - d*y(2);
endfunction

// paramètres initiaux des populations
a = 3;   // taux de reproduction des proies isolées
b = 1;   // taux de mortalité des proies en présence de prédateurs 
c = 1;   // taux de mortalité des prédateurs isolés
d = 2;   // taux de reproduction des prédateurs en présence de proies


// paramètres de simulation
t0 = 0;
tmax = 20;
dt = 0.1;
x0 = 5;  // population initiale des proies
y0 = 2;  // population initiale des prédateurs

// initialisation des vecteurs
t  = [t0:dt:tmax];
y0 = [x0;y0];  // population initiale proies et prédateurs

// résolution du système
y  = ode(y0,t0,t,LotkaVolterra);

// tracé
subplot(2,1,1);plot2d(t,y(1,:),style = 2);xtitle('Evolution des populations','Temps','Population');
subplot(2,1,1);plot2d(t,y(2,:), style = 3);
subplot(2,1,2);plot2d(y(1,:),y(2,:), style = 5);xtitle('Portrait de phase','Proies','Prédateurs');

