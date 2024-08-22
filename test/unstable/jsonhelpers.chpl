use IO, JSON;

record myPair : serializable {
  var x: int;
  var y: string;
}

var myValue = new myPair(1, "hello");
var myValueJson = toJson(myValue);

writeln(myValueJson);
writeln(fromJson(myValueJson, myPair));
