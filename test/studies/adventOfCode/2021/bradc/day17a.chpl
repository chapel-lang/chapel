config const practice = true, debug = false;

const Target = if practice then {20..30, -10..-5} else {155..182, -117..-67};

param X = 0, Y = 1;

// the following defaults assume 'Target.x is positive and Target.y is
// negative; I also don't feel confident that they'd work for all
// targets.

writeln(Target);
const searchSpace = {findXVelRange(Target.dim(X)), findYVelRange(Target.dim(Y))};
writeln(searchSpace);

var numHits: atomic int;

forall vel in searchSpace {
  if shoot(vel) then {
    if debug then writeln(vel);
    numHits.add(1);
  }
}

writeln(numHits.read());

proc shoot(in vel) {
  var pos = (0,0);
  var maxheight = pos(Y);
  while (pos[X] <= Target.dim(X).high && pos[Y] >= Target.dim(Y).low) {
    if Target.contains(pos) {
      if debug then
        writeln("Hit at ", pos);
      return true;
    }
    pos += vel;
    if pos(Y) > maxheight then maxheight = pos(Y);
    if vel[X] > 0 then
      vel[X] -= 1;
    else if vel[X] < 0 then
      vel[X] += 1;
    vel[Y] -= 1;
  }
  return false;
}

proc findXVelRange(xtarg) {
  return findMinXVel(xtarg)..findMaxXVel(xtarg);
}

proc findYVelRange(ytarg) {
  return findMinYVel(ytarg)..findMaxYVel(ytarg);
}

proc findMinXVel(xtarg) {
  var xpos = 0;
  for i in (1..) {
    xpos += i;
    if xpos >= xtarg.low then
      return i;
  }
  return 1;
}

proc findMaxXVel(xtarg) {
  return xtarg.high;
}

proc findMinYVel(ytarg) {
  return ytarg.low-1;
}

proc findMaxYVel(ytarg) {
  return -(ytarg.low+1);
}
