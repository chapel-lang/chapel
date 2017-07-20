proc test() {
  var e1 = new Error("e1");
  var e2 = new Error("e2");
  var group = new ErrorGroup();
  group.append(e1);
  group.append(e2);
  for e in group {
    writeln(e.msg);
  }
  delete group;
}

test();
