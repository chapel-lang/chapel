param str1 = "hello";
param str2 = "world";
param str3 = "ell";
param len = (str1 + " " + str2).size;
param val = str2.byte(0);
param contains = _string_contains(str1, str3);
writeln(len);
writeln(val);
writeln(contains);
