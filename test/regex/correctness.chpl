use Regex;
use CyclicDist;

config type t = string;

/* Support testing on local and with ideally 3 locales to exhibit all cases */
var locales1 = Locales[max(numLocales - 2, 0)];
var locales2 = Locales[numLocales - 1];

/* ---- GLOBAL ---- */
/* First copy of this test case where top-level definitions are subject to
 * global broadcast
 */

/* init= */
var r1 = compile("a+":t);
assert(r1:t == "a+":t);
var r2 = compile("b+":t);
assert(r1.match("aaa":t).matched);

/* operator.= */
/* here(0) == ret: r1.home(0) == x: r2.home(0) */
r1 = r2;
assert(r1.match("bbb":t).matched);

var pattern = "a+":t;
on locales1 {
  var r2 = compile(pattern);
}

r1 = compile("a+":t);
on locales1 {
  /* operator.= */
  /* here(1) != ret: (r1.home(0) == x: r2.home(0)) */
  r1 = r2;
}
assert(r1.match("bbb":t).matched);

r1 = compile("a+":t);
on locales1 {
  /* init= */
  /* here(1) != x: r1.home(0) */
  var r3 = r1;
  assert(r3.match("aaa":t).matched);
}

r1 = compile("a+":t);
on locales1 {
  var r3 = compile("c+":t);
  /* operator.= */
  /* here(1) == ret: r3.home(1) != x: r1.home(0) */
  r3 = r1;
  assert(r3.match("aaa":t).matched);
}

/* long regex that doesn't fit in a small string */
r1 = compile("abcdefghijklmnopqrstuvwxyxabcdefghijklmnopqrstuvwxyx":t);
on locales1 {
  var r3 = compile("c+":t);
  /* operator.= */
  /* here(1) == ret: r3.home(1) != x: r1.home(0) */
  r3 = r1;
  assert(r3:t == "abcdefghijklmnopqrstuvwxyxabcdefghijklmnopqrstuvwxyx":t);
}

r1 = compile("a+":t);
on locales1 {
  var r3 = compile("c+":t);
  /* operator.= */
  /* here(1) != ret: r1.home(0) != (x: r3.home(1) == here(1)) */
  r1 = r3;
}
assert(r1.match("ccc":t).matched);

var r4 = compile("a+":t);

on locales1 {
  var r5 = compile("b+":t);
  on locales2 {
    /* operator.= */
    /* here(2) != ret: r4.home(0) != x: r5.home(1) */
    assert(r4.match("aaa":t).matched);
    r4 = r5;
    assert(r4.match("bbb":t).matched);
  }
  assert(r4.match("bbb":t).matched);
}
assert(r4.match("bbb":t).matched);

/* And the reverse */
/* reset r4.home(0) */
r4 = compile("a+":t);
on locales1 {
  var r5 = compile("b+":t);
  on locales2 {
    /* operator.= */
    /* here(2) != ret: r5.home(1) != x: r4.home(0) */
    r5 = r4;
  }
}

r4 = compile("a+":t);
on locales1 {
  var (s, n) = r4.subn("hello":t, "aaaa world aa":t);
  assert(n == 2);
  assert(s == "hello world hello":t);
}

var r6 = compile("a+":t);
var xs : [1..10] t = [
  "a":t,
  "aa":t,
  "aaa":t,
  "aaaa":t,
  "aaaaa":t,
  "aaaaaa":t,
  "aaaaaaa":t,
  "aaaaaaaa":t,
  "aaaaaaaaa":t,
  "aaaaaaaaaa":t,
];
/* motivated by https://github.com/Bears-R-Us/arkouda/pull/912/files#r701869909 */
forall s in xs do
  assert(r6.match(s).matched);

coforall taskId in 1..#xs.size do
  assert(r6.match(xs[taskId]).matched);

var d = {1..10} dmapped Cyclic(startIdx=1);
var ys: [d] t = xs;
/* Today, this explicit with brings the regex local to each locale */
forall s in ys with (var r = r6) do {
  assert(here == r.home);
  assert(r.match(s).matched);
 }

forall s in ys do
  assert(r6.match(s).matched);

coforall taskId in 1..#ys.size do
  assert(r6.match(ys[taskId]).matched);

/* ---- LOCAL ---- */
/* Second copy of this test case where things are in a function
 * We've seen different results on RVF vs global broadcast and
 * want to test both of them
 */

proc inafunction() {
  /* init= */
  var r1 = compile("a+":t);
  assert(r1:t == "a+":t);
  var r2 = compile("b+":t);
  assert(r1.match("aaa":t).matched);

  /* operator.= */
  /* here(0) == ret: r1.home(0) == x: r2.home(0) */
  r1 = r2;
  assert(r1.match("bbb":t).matched);

  var pattern = "a+":t;
  on locales1 {
    var r2 = compile(pattern);
  }

  r1 = compile("a+":t);
  on locales1 {
    /* operator.= */
    /* here(1) != ret: (r1.home(0) == x: r2.home(0)) */
    r1 = r2;
  }
  assert(r1.match("bbb":t).matched);

  r1 = compile("a+":t);
  on locales1 {
    /* init= */
    /* here(1) != x: r1.home(0) */
    var r3 = r1;
    assert(r3.match("aaa":t).matched);
  }

  r1 = compile("a+":t);
  on locales1 {
    var r3 = compile("c+":t);
    /* operator.= */
    /* here(1) == ret: r3.home(1) != x: r1.home(0) */
    r3 = r1;
    assert(r3.match("aaa":t).matched);
  }

  /* long regex that doesn't fit in a small string */
  r1 = compile("abcdefghijklmnopqrstuvwxyxabcdefghijklmnopqrstuvwxyx":t);
  on locales1 {
    var r3 = compile("c+":t);
    /* operator.= */
    /* here(1) == ret: r3.home(1) != x: r1.home(0) */
    r3 = r1;
    assert(r3:t == "abcdefghijklmnopqrstuvwxyxabcdefghijklmnopqrstuvwxyx":t);
  }

  r1 = compile("a+":t);
  on locales1 {
    var r3 = compile("c+":t);
    /* operator.= */
    /* here(1) != ret: r1.home(0) != (x: r3.home(1) == here(1)) */
    r1 = r3;
  }
  assert(r1.match("ccc":t).matched);

  var r4 = compile("a+":t);

  on locales1 {
    var r5 = compile("b+":t);
    on locales2 {
      /* operator.= */
      /* here(2) != ret: r4.home(0) != x: r5.home(1) */
      assert(r4.match("aaa":t).matched);
      r4 = r5;
      assert(r4.match("bbb":t).matched);
    }
    assert(r4.match("bbb":t).matched);
  }
  assert(r4.match("bbb":t).matched);

  /* And the reverse */
  /* reset r4.home(0) */
  r4 = compile("a+":t);
  on locales1 {
    var r5 = compile("b+":t);
    on locales2 {
      /* operator.= */
      /* here(2) != ret: r5.home(1) != x: r4.home(0) */
      r5 = r4;
    }
  }

  r4 = compile("a+":t);
  on locales1 {
    var (s, n) = r4.subn("hello":t, "aaaa world aa":t);
    assert(n == 2);
    assert(s == "hello world hello":t);
  }

  var r6 = compile("a+":t);
  var xs : [1..10] t = [
                        "a":t,
                        "aa":t,
                        "aaa":t,
                        "aaaa":t,
                        "aaaaa":t,
                        "aaaaaa":t,
                        "aaaaaaa":t,
                        "aaaaaaaa":t,
                        "aaaaaaaaa":t,
                        "aaaaaaaaaa":t,
                        ];
  /* motivated by https://github.com/Bears-R-Us/arkouda/pull/912/files#r701869909 */
  forall s in xs do
    assert(r6.match(s).matched);

  coforall taskId in 1..#xs.size do
    assert(r6.match(xs[taskId]).matched);

  var d = {1..10} dmapped Cyclic(startIdx=1);
  var ys: [d] t = xs;
  /* Today, this explicit with brings the regex local to each locale */
  forall s in ys with (var r = r6) do {
    assert(here == r.home);
    assert(r.match(s).matched);
  }

  forall s in ys do
    assert(r6.match(s).matched);

  coforall taskId in 1..#ys.size do
    assert(r6.match(ys[taskId]).matched);
}

inafunction();