/* Primality Test */

use Time;
proc isPrime(n:int): bool {
	if n <= 3 then return n>1;

	else if(n % 2 == 0 || n % 3 == 0) then return false;

	else {
	    for i in 5.. by 6 {
		    if i*i > n then
			    break;
		    if(n % i == 0 || n % (i + 2) == 0) then
			    return false;
	    }
	    return true;
	}
}

proc main()
{
	var test = read(int), t:int = 1;

	var T: Timer;
	//T.start();
	var totalTime: real(64);

	while(t <= test)
	{
		var m = read(int);
		var n = read(int);
		T.clear();
		T.start();
		for i in m..n
		{
			if(isPrime(i) == true) then
				writeln(i);
		}
		t+=1;
		totalTime = totalTime + T.elapsed();
		T.stop();
	}

	//writeln("Total Time elapsed ",totalTime);
	writeln("End of program reached");
}
