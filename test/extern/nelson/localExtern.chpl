
extern proc puts(s : c_string) : int;

local {
  puts("inside local");
}
puts("outside local");
