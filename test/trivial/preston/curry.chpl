-- let's try some Curry

function sum(a, b) {
  return a + b;
}



var succ => sum(1);

writeln(succ(10));
