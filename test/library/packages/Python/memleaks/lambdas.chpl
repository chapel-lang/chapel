use Python, List;

proc main() {

  var interp = new Interpreter();

  var l = new Function(interp, "lambda x,: [x] + [x]");
  writeln(l(list(int), 1));
  writeln(l(list(string), "hi"));

  // this has an extra list
  writeln(l(list(list(int)), [1, 2, 3]));

  // chapelList([4,5,6])
  var l2 = new Function(interp, "lambda x,y,z,: [x,y,z]");
  // printing this out will cause a memleak, the __str__ method for list leaks
  // e.g., 'print([[1,2,3])' in python leaks memory
  l2(list(owned Value?), 1, "hi", [4, 5, 6]);
  writeln("skipping potentially leaking print");


  var l3 = new Function(interp, "lambda x,: x");
  writeln(l3(list(int), [1,]));
  writeln(l3(NoneType, [1,]));
}
