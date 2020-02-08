//Already defined functions (type help function)
disp(factorial(4));
//Serie definition
u(1)=1;
for n=1:10
    u(n+1)=2*u(n)-3
    disp([n,u(n)])
end
//Function definition
function y=f(x);
    y=exp(x);
endfunction
f(3)
//Vector definition
v=[3;-2;1]
//Display comment + vector
disp("Bob won: "+string(f(3)))
//Definition of vector in a range with increment (default increment = 1)
1:2:50
1:f(3):50
//While loop
h=1.2;
y=2005;
while h<7
    h=h+0.3;
    y=y+1;
end
disp("I will cut the..tree in "+string(y))
//Vector comparison by axis
X=[1,2,5]; Y=[5,3,5];
X==Y
//Vector comparison
isequal(X,Y)
//The inverse doesn't work anymore)
-isequal(X,Y)
//Vector of 3 random numbers beetwen 1 and 6
D=grand(1,3,"uin",1,6);
//Graph of the vector (a bit funny as it's 2d)
plot(D)
//Plot one point in red (lol! It's a big point :-))
plot(1,2,".r")
//Plot of 2d function - Try with a linspace(-2,5,3), it's funny
function y=f(x)
y=(x^2+2*x)*exp(-x)
endfunction
x=linspace(-2,5,50);
plot(x,f)
function y=g(x)
y=sin(x/2)
endfunction
x=linspace(-2,5,50);
clf
//Plot of two 2d functions with defined colors
plot(x,f,"r",x,g,"g")
//Plot of a serie (Careful that the serie is not defined before)
for n=1:50
u(n)=(-0.8)^n;
end
clf; plot(u,"*r")
//Bivariate statistics
X=[1,3,3,7,7,9,10];
Y=[8,7,5,5,4,2,2];
clf; plot(X,Y,"<")
//Surface plot 
function z=f(x,y)
z=2*x^2+y^2;
endfunction
x=linspace(-1,1,100);
y=linspace(-2,2,200);
z=feval(x,y,f)';
clf
surf(x,y,z)
//Parametric 3d
t=linspace(0,4*%pi,100);
param3d(cos(t),sin(t),t)
//Random vectors
//Integers
grand(1,8,"uin",-1,1)
//Real
grand(1,8,"unf",-1,1)
//Bar plot (Becareful here I tried dividing n by 2)
x=[1:10];
n=[8,6,13,10,6,4,16,7,8,5];
clf; bar(x,n/2)
//Bar plot with two series
X=[1,2,5];n1=[5,10,5];n2=[6,8,7];
bar(X,[n1',n2'])
//Matrix definition
m=[1 2 3;4 5 6]
//Element 2,3 of the previous matrix
m(2,3)
m(2,:)
//Matrix multiplication
A=[1,2,3;4,5,6]
B=[1;1;2]
A*B
//Multiplication of all the elements by themselves
2*(A+2)
//Result of X.A=A
A/A
A./A
A.*A
//
C=1:4
C.*C
C.^2
//Result of C*X=1
1/C
//System resolution
A=[1 2 3;4 5 6];
Y=[1;1];
X=A/Y
//Sorting a vector scalars
v=[2,6,9,6,-4,0,2]
gsort(v) //In increasing order
gsort(v,"g","i")
//Lenght of a vector
U=[1:10]
length(U)
//Number of line/columns
m=[1 2 3;4 5 6];
size(m)
//Sum of the scalars
sum(U)
//Product of the scalars
prod(U)
//Keep the distinct scalars from the vector
v=[2,6,9,6,-4,0,2]
unique(v)
//Search for some scalars in the vector
w=[1,5,3,8,14,7,3,2,12,6]; 
find(w<5)
find(w==3)
//Precision of scilab
%eps
//Example
a=sqrt(3)
b=1
c=2
b^2+a^2==c^2
//The result is false because of the approximations
//The correct test should not be:
abs(a^2+b^2-c^2)<%eps
abs(a^2+b^2-c^2)/c^2<%eps
//Nice test
a^2
format(20)
a^2
//Solving differential equations
function yprim=f(t,y)
yprim(1)=y(2);
yprim(2)=-4*y(1) ;
endfunction
  
t0=0; tmax=5;
t=t0:0.05:tmax;
y0=3; yprim0=0;
y=ode([y0;yprim0],t0,t,f);
clf; plot(t,y(1,:))
