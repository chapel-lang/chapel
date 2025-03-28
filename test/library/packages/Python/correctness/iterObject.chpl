use Python;


proc main() {
  var interp = new Interpreter();

  var iterObject = interp.importModule("iterObject");


  // iteration over an iterator
  writeln("yielding Values from myCustomIterator");
  for i in iterObject.get("myCustomIterator")() {
    writeln(i);
  }
  writeln("yielding ints from myCustomIterator");
  for i in iterObject.get("myCustomIterator")().these(int) {
    writeln(i);
  }

  // iteration over an object
  var obj = iterObject.get("MyCustomObject")();
  writeln("yielding Values from MyCustomObject");
  for i in obj {
    writeln(i);
  }
  writeln("yielding ints from MyCustomObject");
  for i in obj.these(int) {
    writeln(i);
  }

  // iteration over a list
  var lst = interp.get("list")(owned PyList, [1, 2, 3, 4, 5]);
  writeln("yielding Values from list");
  for i in lst {
    writeln(i);
  }
  writeln("yielding ints from list");
  for i in lst.these(int) {
    writeln(i);
  }

  // iteration over a tuple
  var tup = interp.get("tuple")(owned PyTuple, [1, 2, 3, 4, 5]);
  writeln("yielding Values from tuple");
  for i in tup {
    writeln(i);
  }
  writeln("yielding ints from tuple");
  for i in tup.these(int) {
    writeln(i);
  }

  // iteration over a set
  var s = interp.get("set")(owned PySet, [1, 2, 3, 4, 5]);
  // sets are unordered, print the sum instead
  writeln("sum of Values from set");
  var sum = 0;
  for i in s {
    sum += i:int;
  }
  writeln(sum);
  writeln("sum of ints from set");
  sum = 0;
  for i in s.these(int) {
    sum += i;
  }
  writeln(sum);



  try {
    var x = new Value(interp, 1);
    for i in x.these() {
      writeln(i);
    }
  } catch e {
    writeln("Caught Exception: ", e);
  }
  try {
    for i in iterObject.get("IAmNotIterable")() {
      writeln(i);
    }
  } catch e {
    writeln("Caught Exception: ", e);
  }
  // error cases: unknown type
  try {
    record R { var x: int; }
    for i in lst.these(R) {
      writeln(i);
    }
  } catch e {
    writeln("Caught Exception: ", e);
  }


}
