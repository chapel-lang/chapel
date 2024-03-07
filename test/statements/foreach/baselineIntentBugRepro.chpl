record Inner {
  var innerField = 42;
}

record Outer {
  var outerField : Inner;

  proc init() {
    outerField = new Inner();
  }

  iter const myIter() {
    foreach idx in 0..10 /* with (ref this)*/ do  // Uncomment with clause and it'll work
      if outerField.innerField == 42 then yield outerField.innerField;
  }
}

var x = new Outer();

for i in x.myIter() do writeln(i);
