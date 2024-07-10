use IO, JSON;

record myPair : serializable {
  var x: int;
  var y: string;
}

var myValue = new myPair(1, "hello");
writeln(toJson([myValue, myValue, myValue]));
