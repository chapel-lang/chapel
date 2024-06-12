use IO;

var X = 1;
var cycle = 1;
var stall = 0;
var val = 0;

iter draw() {
for cycle in (1..) {
  writeln("cycle: ", cycle);
  var s: string;
  var op, space, num: string;
  if cycle % 40 == 20 then
    yield X*cycle;
  if !stall {
    if !readLine(s, stripNewline=true) {
      break;
    }
    (op, space, num) = s.partition(" ");
    select op {
      when "addx" {
        stall = 1;
        val = num: int;
        writeln("> stalling");
      }
      when "noop" {
        writeln("> noop-ing");
      }
    }
  } else {
    X += val;
    writeln("> adding ", val, " to get ", X);
    val = 0;
    stall = 0;
  }
  writeln("X is: ", X);
}
}

var total = 0;
for i in draw() {
  writeln("***> ", i);
  total += i;
}

writeln(total);
