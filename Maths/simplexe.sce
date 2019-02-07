function [xsol,z] = AlgebricSimplexe(A,b,c)

[m,n] = size(A);
Ac = [A,eye(m,m)];
B = [n+1:n+m];
N = [1:m];
z = 0;
l = 0;
while(1)
	l = l + 1;
	disp ('Iteration' + string(l) + ':..');
	disp (B, "Les indices de la variable de base");
	disp (N, "Les indices des variables hors base");
	
	xbase = SolutionDeBase(Ac,b,B);

	if(min(xbase)<0)
		disp("Solution de base non realisable");
		break;
	end
	disp(xbase, "Solution de base");
	if(max(c)<=0)
		disp("Stop tous les couts sont négatifs");
		break;
	else 
		[maxcout,e] = max(c);
		disp('Indice de la variable entrante' + string(N(e)));
	end;

	s=indiceVariableSortante(Ac,B,N,e);

	disp('Indice de la variable sortante' + string(B(s)));

	[Ac,b]=Pivoter(Ac,b,N,e,s);
	
	disp("Effectuons un pivot...");	
	disp(Ac,"...A = ",b,"...b");
	z=z+b(s)*c(e);
	for j=1:n
		if(j<>N(e))
			tab(j) = Ac(s,N(j))*c(e);
		else
			tab(j) = Ac(s,B(s))*c(e);
		end	
	end
	tab=-tab;
	for j=1:n
		if(j<>e)
			c(j)=c(j)+tab(j);
		else
			c(j)=tab(j);
		end
	end
	tmp=B(s);
	B(s)=N(e);
	N(e)=tmp;
end
	x=[zeros(n,1),xbase];
	t=[N,B];
	disp(t);
	for k=1:n+m
		xsol(k)=x(t(k));
	end
	disp(xsol);
	disp(z);
	
endfunction;

function [Xbase] = SolutionDeBase(A,b,B)
	[m,n] = size(A);
	for i=1:m
		Xbase(i) = b(i)/A(i,B(i));
	end
endfunction

function i=indiceVariableSortante(Ac,B,N,e)
	m=length(b);
	for k=1:m
		if(A(k,N(e))>0)
			Y(k)=b(k)/A(k,N(e));
		else
			Y(k)=%inf;
		end
	end
	[x,i] = min(Y);
endfunction

function [A,b]= Pivoter(A,b,N,e,s)
	m=length(b);
	for i=1:m
		if(A(s,N(e))<>0)
			if(i<>s)
				b(i)=b(i)-b(s)*A(i,N(e))/A(s,N(e));
				A(i,:)=A(i,:)-A(s,:)*A(i,N(e))/A(s,N(e));
			else
				b(i)=b(i)/A(s,N(e));
				A(i,:)=A(i,:)/A(s,N(e));
			end
		end
	end
endfunction
