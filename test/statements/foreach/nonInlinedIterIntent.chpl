record Inner {
  var innerField = 42;
}

record Outer {
  var outerField : Inner;

  proc init() {
    outerField = new Inner();
  }

  iter const myIter() {
    var D1 = {1..3, 1..3};
    var D2 = {1..3, 1..3};
    // this will end up being non-inlined that could trigger a bug we fixed
    foreach idx in zip(D1, D2) /* with (ref this)*/ do  // Uncomment with clause and it'll work
      if outerField.innerField == 42 then yield outerField.innerField+idx[0][0];
  }
}

var x = new Outer();

for i in x.myIter() do writeln(i);
