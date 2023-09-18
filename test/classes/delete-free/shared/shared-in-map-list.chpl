// modified from https://github.com/chapel-lang/chapel/issues/14713
use Map;
use List;

class Node { }

var children: map(string, shared Node?);
var n = new shared Node();
children.add('key', n);
writeln(children);

var childrenList: list(shared Node?);
childrenList.pushBack(n);
writeln(childrenList);
