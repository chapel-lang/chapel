class GenericParent { type t; }
class ConcreteParent { }

class ChildNoQ : GenericParent { }
class ChildQ : GenericParent(?) { }

class ConcreteChildNoQ : ConcreteParent { }
class ConcreteChildQ : ConcreteParent(?) { }

var a = new ChildNoQ(int);
var b = new ChildQ(int);
var c = new ConcreteChildNoQ();
var d = new ConcreteChildQ();
