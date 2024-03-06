use IO;

config const debug = false;

var X = 1;
var cycle = 1;
var stall = 0;
var val = 0;
var line: string;
var pixel = 0;
var s: string;

iter draw() {
for cycle in (1..) {
  if debug then writeln("cycle: ", cycle);
  var op, space, num: string;

  line += if (X-1..X+1).contains(pixel%40) then "#" else ".";
  if debug then writeln(line);
  pixel += 1;

  if cycle % 40 == 0 {
//    yield X*cycle;
    yield line;
    line = "";
  }
  if !stall {
    var s: string;
    if !readLine(s, stripNewline=true) {
      break;
    }
    (op, space, num) = s.partition(" ");
    select op {
      when "addx" {
        stall = 1;
        val = num: int;
        if debug then writeln("> stalling");
      }
      when "noop" {
        if debug then writeln("> noop-ing");
      }
    }
  } else {
    X += val;
    if debug then writeln("> adding ", val, " to get ", X);
    val = 0;
    stall = 0;
  }
  if debug then writeln("X is: ", X);
}
}

var total = 0;
for i in draw() {
  writeln(i);
}

if debug then writeln(total);
