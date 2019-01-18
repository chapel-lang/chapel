// strMap - map string keys to string values
var D: domain(string),
  strMap: [D] string;
const a: c_string = "a";

strMap[a:string] = "123";

for key in strMap.domain do
  writeln(key, " = ", strMap[key]);
