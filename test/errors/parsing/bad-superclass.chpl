class Parent {

}

class Wrapper {
  proc type wrapsType type do return Parent;
}

proc doNothing(type input) type do return input;

class C : doNothing(Parent) {}
class D : doNothing(Wrapper).wrapsType {}
class E : (doNothing(Parent))(?) {}
class F : (doNothing(Wrapper).wrapsType)(?) {}
