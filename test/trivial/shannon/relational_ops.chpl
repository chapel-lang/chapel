var m = 5;
var n = 2;
var o = 30;
var p = 5;
var answer = 0;

writeln("m = ", m);
writeln("n = ", n);
writeln("o = ", o);
writeln("p = ", p);
writeln();

writeln("test: m > n"); 
writeln("expected answer = 1");
answer = m > n;
writeln("actual answer = ", answer);
writeln();

writeln("test: m > p");  
writeln("expected answer = 0");
answer = m > p; 
writeln("actual answer = ", answer);
writeln();

writeln("test: o >= m");
writeln("expected answer = 1");
answer = o >= m;
writeln("actual answer = ", answer);
writeln();

writeln("test: m >= p");
writeln("expected answer = 1");
answer = m >= p;
writeln("actual answer = ", answer);
writeln();

writeln("test: m < n"); 
writeln("expected answer = 0");
answer = m < n;
writeln("actual answer = ", answer);
writeln();

writeln("test: m < p");    
writeln("expected answer = 0");
answer = m < p;
writeln("actual answer = ", answer);
writeln();

writeln("test: m <= n"); 
writeln("expected answer = 0");
answer = m <= n;
writeln("actual answer = ", answer);
writeln();

writeln("test: m <= p");  
writeln("expected answer = 1");
answer = m <= n;
writeln("actual answer = ", answer);
