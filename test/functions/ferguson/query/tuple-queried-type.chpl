proc f(tuple: 2*?t) {
  writeln(t:string);
  writeln(tuple(0));
  writeln(tuple(1));
}

proc g(tuple: 2*integral) {
  writeln(tuple(0).type:string);
  writeln(tuple(0));
  writeln(tuple(1));
}
f( (1,2) );
g( (1,2) );
