use CTypes;

extern {
  #define CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))
}

var below = CLAMP((-1):c_int, 0:c_int, 10:c_int);
var within = CLAMP(7:c_int, 0:c_int, 10:c_int);
var above = CLAMP(11:c_int, 0:c_int, 10:c_int);
writeln(below.type == c_int);
writeln(below, " ", within, " ", above);
