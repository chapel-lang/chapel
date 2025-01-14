extern proc printf(f, x);
extern {
  const char* formatStr = "%d\n";
}

var xyz = 1: int(8);
printf(formatStr, xyz);
