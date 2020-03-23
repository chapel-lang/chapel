proc f(msg: string) {
  var b: bool;
  if msg == "true" then b = true;
  else if msg == "false" then b = false;
  else return;

  writeln(b);
}

var s = "true";

f(s);
