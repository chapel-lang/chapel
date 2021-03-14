require "passlongptr.h", "passlongptr.c";

extern proc f_long(ref l: c_long);
extern proc f_longlong(ref l: c_longlong);

var mylong: c_long;
var mylonglong: c_longlong;

f_long(mylong);
f_longlong(mylonglong);
