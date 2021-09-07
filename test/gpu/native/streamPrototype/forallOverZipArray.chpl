config const start = 1;
config const end = 10;

on here.getChild(1) {
  var a, b: [start..end] int;

  forall  (x,i) in zip(b, b.domain) do x = i+12;

  forall  (aElem, bElem) in zip(a, b) { aElem += bElem + 10;    } writeln(a);

  foreach bElem in b do bElem += 100;

  foreach (aElem, bElem) in zip(a, b) { aElem += bElem + 10;    } writeln(a);
}
