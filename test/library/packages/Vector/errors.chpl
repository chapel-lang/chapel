
use Vector;

config const mode = 0;

var inc : int = 0;

proc step() : bool {
  const ret = inc;
  inc += 1;
  
  if ret == mode {
    writeln("MODE ", mode);
    return true;
  } else {
    return false;
  }
}

proc main() {
  var empty : vector(int);

  var ten : vector(int);
  for i in 1..10 do ten.push_back(i);

  if step() {
    empty.front();
  } else if step() {
    empty.back();
  } else if step() {
    empty.pop_back();
  } else if step() {
    empty.pop_front();
  } else if step() {
    ten.insert(500, 11);
  } else if step() {
    ten.remove(ten.size);
  } else if step() {
    ten.remove(5..100);
  } else if step() {
    ten.remove(..100);
  } else if step() {
    ten.remove(-100..);
  } else if step() {
    empty[5] = 100;
  } else {
    halt("invalid mode");
  }
}
