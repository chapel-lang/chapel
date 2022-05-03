var s = "çççççççççççççççççç";

var a: [0..<s.size] string;

forall (elem, char) in zip(a, s) {
  elem = char;
}

writeln(a);
