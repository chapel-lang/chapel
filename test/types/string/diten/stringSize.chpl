var str1 = "hello";
var str2 = "world";
var str3 = "ell";
var str4 = "йцы";
var sz = (str1 + " " + str2).size;
var sz2 = str4.size;
writeln("Size: ", sz);
writeln("Size: ", sz2);
assert(str4.size == sz2);
