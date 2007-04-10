class C {
  var x : int;
  var y : real;
}

var c1 : C = C(1, 2.3), c2 : C = C(2, 3.4);

var s : seq of C = _seq( c1, c2 );

writeln(s);

s = _seqcat(s, _seq( c1, c2 ));

writeln(s);
