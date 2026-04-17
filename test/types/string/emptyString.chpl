var s = "x";
writeln("'", s, "' ", if s.buff == nil then "nil" else "not nil");
// string assignment to an empty string results in an unallocated buffer
// this is a quirk of the implementation, rather than an intentional design choice
s = "";
writeln("'", s, "' ", s.buff);
if s == "" then
  writeln("s is empty");
else
  writeln("s is not empty");
if "" == s then
  writeln("s is still empty");
else
  writeln("s is still not empty");
if s > "" then
  writeln("s is greater than empty string");
else
  writeln("s is not greater than empty string");
if "" < s then
  writeln("empty string is less than s");
else
  writeln("empty string is not less than s");
if s >= "" then
  writeln("s is greater than or equal to empty string");
else
  writeln("s is not greater than or equal to empty string");
if "" <= s then
  writeln("empty string is less than or equal to s");
else
  writeln("empty string is not less than or equal to s");

use CTypes;
extern proc printf(format: c_ptrConst(c_char), v...): c_int;
// because s.buff is unallocated, its c_str() is nil too
printf("'%s' %d\n", s.c_str(), s.c_str():c_ptr(void):c_intptr:c_int);

