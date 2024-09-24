use IO, JSON;

record myPair : serializable {
  var x: int;
  var y: string;
}

writeln(fromJson("[{\"x\":1, \"y\":\"hello\"}, {\"x\":1, \"y\":\"hello\"}, {\"x\":1, \"y\":\"hello\"}]", [0..2] myPair));
