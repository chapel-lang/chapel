var data : [1..10] int;

proc getter() ref {
  writeln("access write");
  return data;
}

proc getter() {
  writeln("access read");
  return data;
}

// Currently enters the 'write' case, but I would expect a read
write("Read array: ");
var x = getter()[1];
writeln(x);
