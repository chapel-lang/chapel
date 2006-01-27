record history_float {
  var h : seq of float;
}

function =(x : history_float, y : float) {
  x.h = x.h # y;
  return x;
}

function >(x : history_float, y : float) {
  return x.h(1) > y;
}

function +(x : history_float, y : float) {
  return x.h(1) + y;
}

function fwrite(f : file, x : history_float) {
  fwrite(f, x.h(1), " (");
//  for h in x.h do
//    fwrite(f, h, ", ");
}

var h : history_float;

h = 2;

writeln(h);
