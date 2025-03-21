use Map;

var m: map(int, int);

for i in 1..100 do m[i] = i;

{
  var s = 0;
  forall k in m.keys() with (+ reduce s) do s += k;
  writeln(s);
}

{
  var s = 0;
  forall v in m.values() with (+ reduce s) do s += v;
  writeln(s);
}
