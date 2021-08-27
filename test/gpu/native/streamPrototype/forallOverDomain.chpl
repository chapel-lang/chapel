config const start = 1;
config const end = 10;

on here.getChild(1) {
  var a, b: [start..end] int;
  var value = 20;

  // one array
  forall  i in a.domain { a[i] += i+10;       } writeln(a);
  forall  i in a.domain { a[i] += i+value;    } writeln(a);
  foreach i in a.domain { a[i] += i+10;       } writeln(a);
  foreach i in a.domain { a[i] += i+value;    } writeln(a);

  // two arrays
  forall  i in a.domain { b[i] += a[i]*10;    } writeln(b);
  forall  i in a.domain { b[i] += a[i]*value; } writeln(b);
  foreach i in a.domain { b[i] += a[i]*10;    } writeln(b);
  foreach i in a.domain { b[i] += a[i]*value; } writeln(b);
}
