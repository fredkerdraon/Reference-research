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

function avg=sma(vec, windw)
    avg=[]
    for i=1:length(vec)-windw+1
            av=sum(vec(i:i+(windw-1)))/windw
            avg=[avg;av]
    end
endfunction
