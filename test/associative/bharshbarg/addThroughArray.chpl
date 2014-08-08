
var dom : domain(string);
var data : [dom] int;

data["alice"] = 24;
data["bob"] = 19;
data["carl"] = 35;

for (k, v) in zip(dom, data) do writeln(k, " => ", v);

/*
var illegal : [dom] string;

data["foo"] = 42;
illegal["badKey"] = "you can't do this with multiple domains";
*/
