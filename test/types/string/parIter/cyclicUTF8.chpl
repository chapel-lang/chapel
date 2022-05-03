use CyclicDist;

config const n = 10;

var s= "çş"*n;

var arr = newCyclicArr(0..<2*n, string);

forall (elem, char) in zip(arr, s) {
  elem = char;
}

writeln(arr);

