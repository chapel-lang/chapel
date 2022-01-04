use BlockDist;

config const n = 10;

on here.getChild(1) {
  var space = {1..n};
  var dom = space dmapped Block(space, targetLocales=[here,]);
  var arr: [dom] int;

  forall i in dom do
    arr[i] = 1;

  /* The following does not work yet:

    forall a in arr do
      a = 1;

  */

  writeln(arr);
}
