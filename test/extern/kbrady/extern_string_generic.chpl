use CTypes;
extern proc printf(format: c_ptrConst(c_char), args ...);
var s:string = "test\n";
printf("%s", s);
printf("%s", s);
printf("%s", s);
printf("%s", s);
