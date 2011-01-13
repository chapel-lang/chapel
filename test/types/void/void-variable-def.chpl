var x:void;

class y {
  var z: void;
}

config param enabled = false;

def vartype() type {
  if enabled then
    return int;
  else
    return void;
}

class c {
  var v: vartype();
  def c() { if enabled then v = 5; }
}

writeln(new c());
