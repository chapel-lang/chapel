var m = 2;
var n = 3;
var o = 5;
var answer = 0;

answer = m + n - o;
writeln(answer);
if (answer != 0) then writeln("**ERROR** =>  m + n - o");

answer = 0;
answer = m - n + o;
writeln(answer);
if (answer != 4) then writeln("**ERROR** =>  m - n + o");

answer = 0;
answer = m + n / o;
writeln(answer);
if (answer != 2) then writeln("**ERROR** =>  m + n / o");

answer = 0;
answer = m / n + o;
writeln(answer);
if (answer != 5) then writeln("**ERROR** =>  m / n + o");

answer = 0;
answer = m + n mod o;
writeln(answer);
if (answer != 5) then writeln("**ERROR** =>  m + n % o");

answer = 0;
answer = m mod n + o;
writeln(answer);
if (answer != 7) then writeln("**ERROR** =>  m % n + o");

