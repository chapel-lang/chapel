use Containers;

// test zero based offset
var v = new Vector(int);
writeln("Fill: ");
writeln("------------------------------------------------");
for i in 1..10 do {
    v.push(i);
    writeln(v);
}
writeln("Empty: ");
writeln("------------------------------------------------");
while !v.empty {
    v.pop();
    writeln(v);
}
writeln("------------------------------------------------");
writeln("------------------------------------------------");
// test one based offset
var v2 = new Vector(int, offset=1);
writeln("Fill: ");
writeln("------------------------------------------------");
for i in 1..10 do {
    v2.push(i);
    writeln(v2);
}
writeln("Empty: ");
writeln("------------------------------------------------");
while !v2.empty {
    v2.pop();
    writeln(v2);
}

