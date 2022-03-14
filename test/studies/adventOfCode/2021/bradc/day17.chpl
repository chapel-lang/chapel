const Target = {155..182, -117..-67};

param X = 0, Y = 1;

// the following defaults assume 'Target.x is positive and Target.y is
// negative; I also don't feel confident that they'd work for all
// targets.

config const vx = findStableXVel(Target.dim(X)), vy = -(Target.dim(Y).low+1);

var pos = (0,0),
    vel = (vx, vy);

writeln(shoot(vel));

proc shoot(in vel) {
  var maxheight = pos(Y);
  while (pos[X] <= Target.dim(X).high && pos[Y] >= Target.dim(Y).low) {
    if Target.contains(pos) {
      writeln("Hit at ", pos);
      return maxheight;
    }
    pos += vel;
    if pos(Y) > maxheight then maxheight = pos(Y);
    if vel[X] > 0 then
      vel[X] -= 1;
    else if vel[X] < 0 then
      vel[X] += 1;
    vel[Y] -= 1;
    writeln(pos);
  }
  return min(int);
}

// for what value k does 1 + 2 + 3 + ... + k land us in the target's x
// range?  TODO: switch to a non-iterative way to compute this... I was
// just being lazy about math... :)
proc findStableXVel(xtarg) {
  var xvel = 0;
  for i in (1..) {
    xvel += i;
    if xtarg.contains(xvel) then
      return i;
    if xvel > xtarg.high then
      break;
  }
  halt("Can't find an x velocity that goes to 0 above the target");
}
