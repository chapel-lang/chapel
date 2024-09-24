use IO, JSON;

record myPair : serializable {
  var x: int;
  var y: string;
}

writeln(fromJson("{\"x\": 1, \"y\": \"hello\"}", myPair));
