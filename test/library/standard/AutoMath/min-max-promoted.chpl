// ensures that min/max can be promoted over iterable expressions.

var A = for i in -10..10 do i;

writeln(max(0, A+1));
writeln(min(0, A+1));
writeln(max(0, for i in -10..10 do i+1));
writeln(min(0, for i in -10..10 do i+1));
