config const n: int = 4096;

extern proc printf(x...);

proc foo(i: int) {
  var x: sync int;
  var y: single int;
  if i < n {
    sync {
      begin { printf("%s\n", here.id + " pre " + x.readFE()); y = x; }
      begin {x = i; foo(i+1); x = i;}
    }
    printf("%s\n", here.id + " post " + y.readFF());
  }
}

foo(1);
