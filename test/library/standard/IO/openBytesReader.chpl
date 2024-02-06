use IO, JSON, List;

const myJsonData = b"{\"x\": 3.14159, \"y\": \"data\"}";
var r = openBytesReader(myJsonData, deserializer = new jsonDeserializer());

var thing:rec;
r.read(thing);
writeln(thing);

record rec {
    var x: real;
    var y: string;
}

var r2 = openBytesReader(b"hello world!\nI'm a bytes!");
write(r2.readLine());
writeln(r2.readLine());
