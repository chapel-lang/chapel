use Random;

var rs = new randomStream(int, 12345);

rs.skipToNth(1);
writeln(rs.getNext());
writeln(rs.getNth(5));
