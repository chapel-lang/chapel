enum color { red=1, green, blue };

config param exp = 3;

if (color.size != exp) then
  compilerWarning("surprise in .size");

if (color.size == exp) then
  compilerError(".size is param");
