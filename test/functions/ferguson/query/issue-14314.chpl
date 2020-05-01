class Sender {
  type sendType;
}

class Receiver {
  type recvType1;
  type recvType2;
}

addEdge(new Sender(int), new Receiver(int, real));
addEdge(new Sender(int), new Receiver(real, int));

proc addEdge(s: Sender(?st), r: Receiver(?rt1, ?rt2)) {
  if (st == rt1 || st == rt2) {
    writeln("addEdge matches ", s.type:string, " and ", r.type:string);
  } else {
    compilerWarning(rt1:string);
    compilerError("bad addEdge match: ", s.type:string, " vs. ", r.type:string);
  }
}
