
var parentName = "Parent";
var childName = "Child";

class Parent {
  // ref + const ref
  proc name const ref return parentName;
  proc name2 ref return parentName;

  // const ref + value
  proc name3 const ref return parentName;
  proc name4 return parentName;

  // ref + value
  proc name5 return parentName;
  proc name6 ref return parentName;
}


class Child : Parent {
  // ref + const ref
  proc name ref return childName;
  proc name2 const ref return childName;

  // const ref + value
  proc name3 return childName;
  proc name4 const ref return childName;

  // ref + value
  proc name5 ref return childName;
  proc name6 return childName;
}


writeln("Calling child name functions");
var c = new borrowed Child();
writeln(c.name);
writeln(c.name2);
writeln(c.name3);
writeln(c.name4);
writeln(c.name5);
writeln(c.name6);

writeln("Calling parent name functions");
var d = c:borrowed Parent;
writeln(d.name);
writeln(d.name2);
writeln(d.name3);
writeln(d.name4);
writeln(d.name5);
writeln(d.name6);

