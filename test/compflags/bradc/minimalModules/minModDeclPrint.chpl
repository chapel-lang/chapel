inline proc _defaultOf(type t) param where t == int(8) return 0:t;

var xyz = 1: int(8);
extern proc printf(f:c_string, x...);
printf(c"%d\n", xyz);
