// regression test for https://github.com/chapel-lang/chapel/issues/26017
class C { }

forall 1..10 with (const c = new C()) { writeln(c); }
forall 1..10 with (const c = new shared C()) { writeln(c); }
