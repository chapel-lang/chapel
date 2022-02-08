
var x: domain(string);

x.contains(     "hi"  );
x.contains(idx=("hi",));
x.contains(  i=("hi",)); //deprecated

x.add(    "hi");
x.add(idx="hi");
x.add(  i="hi");         //deprecated

x.remove(    "hi");
x.remove(idx="hi");
x.remove(  i="hi");      //deprecated

x.requestCapacity(         5);
x.requestCapacity(capacity=5);
x.requestCapacity(       i=5);  //deprecated

compilerError("done");
