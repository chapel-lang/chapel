proc printtype(param name, type t) {
  writeln(name, " = ", t:string);
}

config const option = true;

proc main() {
  // check runtime types
  var A:[1..10] int;
  var B:[1..9] int;
  type rtt1;
  type rtt2;
  if option {
    rtt1 = A.type;
  } else {
    rtt1 = B.type;
  }
  if !option {
    rtt2 = A.type;
  } else {
    rtt2 = B.type;
  }

  var rtt1arr:rtt1;
  var rtt2arr:rtt2;
  writeln("rtt1 ", rtt1arr.domain);
  writeln("rtt2 ", rtt2arr.domain);
}
