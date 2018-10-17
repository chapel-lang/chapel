type twod_t = (int, int);
var d: domain( twod_t);

d.add( (0,0));
d.add( (-1,0));
d.add( (0,-1));
d.add( (1,0));
d.add( (0,1));

proc check( t: twod_t) {
  writeln( t, " contains = ", d.contains( t));
}

check( (0,0));
check( (0,-1));
check( (1,1));
check( (2,9));
check( (-3,4));
check( (1,0));
