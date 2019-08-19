class C {
  var x: int;
}

proc bar(c = new C()) {
  writeln(c.type:string);
  writeln(c);
}

writeln("Default bar");
bar();

writeln("Supplied unmanaged C");
bar(new unmanaged C());
