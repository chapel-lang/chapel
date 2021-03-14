var stringDom:  domain(string);
var stringArr: [stringDom] int;

stringDom += "foo";
stringArr["foo"] = 1;
writeln(stringArr);

var bytesDom:  domain(bytes);
var bytesArr: [bytesDom] int;

bytesDom += b"foo";
bytesArr[b"foo"] = 1;
writeln(bytesArr);

