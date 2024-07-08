use CTypes;
extern proc puts(s : c_ptrConst(c_char)) : int;

local {
  puts("inside local");
}
puts("outside local");
