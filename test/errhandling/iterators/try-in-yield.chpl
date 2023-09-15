config const tryBang = true;
{
  writeln("Case 1 - try!");
  iter foo() {
    yield try! 1;
  }

  for i in foo() {
    writeln(i);
  }
}

{
  writeln("Case 1 - try");
  iter foo() {
    yield try 1;
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

  writeln("Case 2 - try!");
  var sum = 0;
  forall i in foo() with (+ reduce sum){
    sum += i;
  }
  writeln(sum);
  sum = 0;

  writeln("Case 3 - try!");
  forall (i,j) in zip(foo(), 1..) with (+ reduce sum) {
    sum += i + j;
  }
  writeln(sum);
}

{
  iter foo() {
    for i in 1..10 do yield try i;
  }

  iter foo(param tag: iterKind) where tag==iterKind.leader {
    const r = 1..10;
    for followThis in r.these(tag) do yield try followThis;
  }

  iter foo(param tag: iterKind, followThis) where tag==iterKind.follower {
    const r = 1..10;
    for i in r.these(tag, followThis) do yield try i;
  }

  writeln("Case 2 - try");
  var sum = 0;
  forall i in foo() with (+ reduce sum){
    sum += i;
  }
  writeln(sum);
  sum = 0;

  writeln("Case 3 - try");
  forall (i,j) in zip(foo(), 1..) with (+ reduce sum) {
    sum += i + j;
  }
  writeln(sum);
}

if tryBang {
  writeln("Case 4 - try!");

  proc bar(): int throws {
    throw new Error("test");

  }

  iter foo() {
    yield try! bar();
  }

  for i in foo() {
    writeln(i);
  }
}
else {
  writeln("Case 4 - try");

  proc bar(): int throws {
    throw new Error("test");

  }

  iter foo() throws { // can throw because can be inlined
     yield try bar();
  }

  for i in foo() {
    writeln(i);
  }
}
