function [uk,ukp] = moving(Vector,k)
n = length(Vector);
m = n-k+1;
uk = zeros(1,m);
j = 1
//for j = 1:k
//uk = uk + Vector(j:m+j-1);
//end
disp(n);
//disp(Vector);
endfunction;
