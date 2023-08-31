
class MyClass {

}
type t = owned MyClass;
type t2 = t.borrow();
type t3 = t:borrowed;
writeln(t2 == t3);
