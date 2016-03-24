proc fib(n:int):int{
	if(n == 0){
		return 0;
	}
	else if(n==1){
		return 1;
	}
	else if(n > 1){
		var f: [0..n] int;
		var i: int;
		f(0) = 0;   
		f(1) = 1;
		for i in (2..n){
			f(i) = f(i-1) + f(i-2);
		}
		return f(n);
	}
	else{
		writeln("Pls pass a positive integer");
		return -1;	
	}
}
 
config var n: int = 10;
writeln("The ", n, "th fibonacci number is: ", fib(n));
