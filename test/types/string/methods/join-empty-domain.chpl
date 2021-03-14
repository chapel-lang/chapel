/* Test passing an array over an empty domain to join */
var S: [1..0] string;
var j = ' '.join(S);
assert(j.isEmpty());
