use CTypes;
extern proc printf(fmt:c_ptrConst(c_char), arg:c_ptrConst(c_char));

var s1 = "0" * 10;
var s2 = "1" * 3;

var test = s1;
test = s2;

printf("%s\n", test.c_str());
