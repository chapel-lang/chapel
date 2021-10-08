use Regex;

/* Support testing on local and with ideally 3 locales to exhibit all cases */
var locales1 = Locales[max(numLocales - 2, 0)];
var locales2 = Locales[numLocales - 1];

/* init= */
var r1 = compile("a+");
assert(r1:string == "a+");
var r2 = compile("b+");
assert(r1.match("aaa").matched);

/* operator.= */
/* here(0) == ret: r1.home(0) == x: r2.home(0) */
r1 = r2;
assert(r1.match("bbb").matched);

var pattern = "a+";
on locales1 {
  var r2 = compile(pattern);
}

r1 = compile("a+");
on locales1 {
  /* operator.= */
  /* here(1) != ret: (r1.home(0) == x: r2.home(0)) */
  r1 = r2;
}
assert(r1.match("bbb").matched);

r1 = compile("a+");
on locales1 {
  /* init= */
  /* here(1) != x: r1.home(0) */
  var r3 = r1;
  assert(r3.match("aaa").matched);
}

r1 = compile("a+");
on locales1 {
  var r3 = compile("c+");
  /* operator.= */
  /* here(1) == ret: r3.home(1) != x: r1.home(0) */
  r3 = r1;
  assert(r3.match("aaa").matched);
}

/* long regex that doesn't fit in a small string */
r1 = compile("abcdefghijklmnopqrstuvwxyxabcdefghijklmnopqrstuvwxyx");
on locales1 {
  var r3 = compile("c+");
  /* operator.= */
  /* here(1) == ret: r3.home(1) != x: r1.home(0) */
  r3 = r1;
  assert(r3.getPattern() == "abcdefghijklmnopqrstuvwxyxabcdefghijklmnopqrstuvwxyx");
}

r1 = compile("a+");
on locales1 {
  var r3 = compile("c+");
  /* operator.= */
  /* here(1) != ret: r1.home(0) != (x: r3.home(1) == here(1)) */
  r1 = r3;
}
assert(r1.match("ccc").matched);

var r4 = compile("a+");

on locales1 {
  var r5 = compile("b+");
  on locales2 {
    /* operator.= */
    /* here(2) != ret: r4.home(0) != x: r5.home(1) */
    assert(r4.match("aaa").matched);
    r4 = r5;
    assert(r4.match("bbb").matched);
  }
  assert(r4.match("bbb").matched);
}
assert(r4.match("bbb").matched);

/* And the reverse */
/* reset r4.home(0) */
r4 = compile("a+");
on locales1 {
  var r5 = compile("b+");
  on locales2 {
    /* operator.= */
    /* here(2) != ret: r5.home(1) != x: r4.home(0) */
    r5 = r4;
  }
}

r4 = compile("a+");
on locales1 {
  var (s, n) = r4.subn("hello", "aaaa world aa");
  assert(n == 2);
  assert(s == "hello world hello");
}

var r6 = compile("a+");
var strings : [1..10] string = [
  "a",
  "aa",
  "aaa",
  "aaaa",
  "aaaaa",
  "aaaaaa",
  "aaaaaaa",
  "aaaaaaaa",
  "aaaaaaaaa",
  "aaaaaaaaaa",
];
/* motivated by https://github.com/Bears-R-Us/arkouda/pull/912/files#r701869909 */
forall s in strings do
  assert(r6.match(s).matched);

coforall taskId in 1..#strings.size do
  assert(r6.match(strings[taskId]).matched);

use CyclicDist;
var d = {1..10} dmapped Cyclic(startIdx=1);
var strings2: [d] string;
/* Today, this explicit with brings the regex local to each locale */
forall s in strings2 with (var r = r6) do
  assert(here == r.home);

/* Today, rvf either isn't kicking in but the regex is not copied here */
/* But either way its not incorrect for it not to be local, but not ideal perf wise */
/* forall s in strings2 do */
/*   writeln(here, r6.home); */

coforall taskId in 1..#strings.size do
  assert(r6.match(strings[taskId]).matched);