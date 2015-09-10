// strMap - map string keys to string values
var D: domain(string),
  strMap: [D] string;
const a: c_string_copy = "a";

strMap[a] = "123";

for key in strMap.domain do
  writeln(key, " = ", strMap[key]);
