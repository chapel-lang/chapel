var s = (/1, 2, 3, 5, 6, 7/);
writeln(s);
s = s(1..3) # 4 # s(4..6);
writeln(s);
