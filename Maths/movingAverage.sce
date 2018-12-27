function c = movingAverage(Vector, width)
	// moving average of the data.
	  N = length(Vector); 
	  width = min(floor(width),N);
	  width = width-1+modulo(width,2);// force it to be odd
	  c = filter(ones(width,1)/width,1,y);
	  cbegin = cumsum(Vector(1:width-2));
	  cbegin = cbegin(1:2:$)./(1:2:(width-2))';
	  cend = cumsum(y($:-1:$-width+3));
	  cend = cend($:-2:1)./(width-2:-2:1)';
	  pause
	  c = [cbegin;c(width:$);cend];
endfunction
