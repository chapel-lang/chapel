proc foo(s: sync) {
  var x = s.readFE();
  writeln(x);
  s.writeEF((x+1):x.type);
}

proc bar(type t) {
  var s: sync t;
  s = 0:t;
  foo(s);
  foo(s);
}

bar(int(8));
bar(int(16));
bar(int(32));
bar(int(64));

bar(uint(8));
bar(uint(16));
bar(uint(32));
bar(uint(64));

bar(real(32));
bar(real(64));

bar(imag(32));
bar(imag(64));

//bar(complex(64));
//bar(complex(128));
