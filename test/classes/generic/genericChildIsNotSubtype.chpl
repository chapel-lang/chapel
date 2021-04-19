class GenericParentClass {
  type t;

  proc init(type t) {
    this.t = t;
  }
}
class ChildClass: GenericParentClass {}

class ParentClass {}
class ChildClass2: ParentClass {}

writeln(isSubtype(ChildClass, GenericParentClass(int)));
writeln(isSubtype(ChildClass2, ParentClass));
