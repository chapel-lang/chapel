{
  writeln("Case 1");
  iter foo() {
    yield try! 1;
  }

  for i in foo() {
    writeln(i);
  }
}

{
  iter foo() {
    for i in 1..10 do yield try! i;
  }

  iter foo(param tag: iterKind) where tag==iterKind.leader {
    const r = 1..10;
    for followThis in r.these(tag) do yield try! followThis;
  }

  iter foo(param tag: iterKind, followThis) where tag==iterKind.follower {
    const r = 1..10;
    for i in r.these(tag, followThis) do yield try! i;
  }

  writeln("Case 2");
  var sum = 0;
  forall i in foo() with (+ reduce sum){
    sum += i;
  }
  writeln(sum);
  sum = 0;

  writeln("Case 3");
  forall (i,j) in zip(foo(), 1..) with (+ reduce sum) {
    sum += i + j;
  }
  writeln(sum);
}

{
  writeln("Case 4");

  proc bar() throws {
    throw new Error("test");
    return 0;
  }

  iter foo() {
    yield try! bar();
  }
  
  for i in foo() {
    writeln(i);
  }
}
