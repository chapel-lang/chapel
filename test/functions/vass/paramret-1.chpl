proc a() param      do return 5;
proc b() param :int do return 6;
proc c   param      do return 7;
proc d   param :int do return 8;

writeln(a());
writeln(b());
writeln(c);
writeln(d);
