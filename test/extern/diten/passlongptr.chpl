require "passlongptr.h", "passlongptr.c";

extern proc f_long(ref l: c_long);

var l: c_long;

f_long(l);

