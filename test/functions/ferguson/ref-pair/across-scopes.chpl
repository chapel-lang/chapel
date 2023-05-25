
var parentName = "Parent";
var childName = "Child";

class Parent {
  // ref + const ref
  proc name const ref do return parentName;
  proc name2 ref do return parentName;

  // const ref + value
  proc name3 const ref do return parentName;
  proc name4 do return parentName;

  // ref + value
  proc name5 do return parentName;
  proc name6 ref do return parentName;
}


class Child : Parent {
  // ref + const ref
  override proc name ref do return childName;
  override proc name2 const ref do return childName;

  // const ref + value
  override proc name3 do return childName;
  override proc name4 const ref do return childName;

  // ref + value
  override proc name5 ref do return childName;
  override proc name6 do return childName;
}


writeln("Calling child name functions");
var c = (new owned Child()).borrow();
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

