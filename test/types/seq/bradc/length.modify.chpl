var s: seq(integer) = (/ 1, 2, 3 /);

writeln("s is: ", s);
writeln("s' length is: ", s.length);

-- this should result in an error;  it would be nice if it
-- was clearer than it currently is (06/2005)
s.length = 4;

writeln("s' length is: ", s.length);
