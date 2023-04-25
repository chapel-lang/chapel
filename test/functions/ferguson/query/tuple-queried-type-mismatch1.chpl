proc f(tuple: 2*?t) {
  writeln(t:string);
  writeln(tuple(0));
  writeln(tuple(1));
}

f( ("hi",2) );
