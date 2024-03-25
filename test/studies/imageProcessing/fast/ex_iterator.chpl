
/*****
      ex_iterator.chpl -
      An example of an iterator function.  See text for details.

      c 2015-2018 Primordial Machine Vision Systems
*****/

iter whilerange(lo : int, hi : int, stride = 1) {
  var cnt = lo;

  if (((hi < lo) && (0 < stride)) || ((lo < hi) && (stride < 0)) ||
      (0 == stride)) then return;

  if (0 < stride) {
    while (cnt <= hi) {
      yield cnt;
      cnt += stride;
    }
  } else {
    while (hi <= cnt) {
      yield cnt;
      cnt += stride;
    }
  }
}

writeln("whilerange 1..5");
for i in whilerange(1, 5) do writeln("  ", i);

writeln("whilerange -1..-5 by -1");
for i in whilerange(-1, -5, -1) do writeln("  ", i);

writeln("whilerange 1..10 by 2");
for i in whilerange(1, 10, 2) do writeln("  ", i);

writeln("whilerange 1..5 by -1");
for i in whilerange(1, 5, -1) do writeln("  ", i);
writeln("whilerange -1..-5 by 1");
for i in whilerange(-1, -5, 1) do writeln("  ", i);
writeln("whilerange -1..-5 by 0");
for i in whilerange(-1, -5, 0) do writeln("  ", i);

writeln(whilerange(1,10,2));
