use IO, JSON;

const myJsonData = "{\"x\": 3.14159, \"y\": \"data\"}";
var r = openStringReader(myJsonData, deserializer = new jsonDeserializer());

var thing:rec;
r.read(thing);
writeln(thing);

record rec {
    var x: real;
    var y: string;
}

var r2 = openStringReader("hello world!\nI'm a string!");
write(r2.readLine());
writeln(r2.readLine());
