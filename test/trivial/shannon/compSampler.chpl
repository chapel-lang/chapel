var m = 2;
var n = 3;
var o = 5;
var answer = 0;

answer = m + n - o;
writeln(answer);
if (answer != 0) writeln("**ERROR** =>  m + n - o");

answer = 0;
answer = m - n + o;
writeln(answer);
if (answer != 4) writeln("**ERROR** =>  m - n + o");

answer = 0;
answer = m + n / o;
writeln(answer);
if (answer != 2) writeln("**ERROR** =>  m + n / o");

answer = 0;
answer = m / n + o;
writeln(answer);
if (answer != 5) writeln("**ERROR** =>  m / n + o");

answer = 0;
answer = m + n % o;
writeln(answer);
if (answer != 5) writeln("**ERROR** =>  m + n % o");

answer = 0;
answer = m % n + o;
writeln(answer);
if (answer != 7) writeln("**ERROR** =>  m % n + o");

