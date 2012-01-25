
extern proc puts(s : string) : int;

local {
  puts("inside local");
}
puts("outside local");