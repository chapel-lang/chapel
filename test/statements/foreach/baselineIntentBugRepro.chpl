{
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
}

{
record Outer {
  var outerField : 2*int;

  proc init() {
    outerField = (1,2);
  }

  iter const myIter() {
    foreach idx in 0..10 /*with (ref this)*/ do  // Uncomment with clause and it'll work
      if outerField[0] == 1 then yield outerField[1];
  }
}

var x = new Outer();

for i in x.myIter() do writeln(i);
}
