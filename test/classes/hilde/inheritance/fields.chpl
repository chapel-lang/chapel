// fields.chpl
//
// Show that the getter for a local field will override a parenthesesless
// function of the same name in a parent class.
//
// Show that other parenthesesless functions are also bound based on the
// declared type of the object.
//
class Base {
  proc field do return "Base";
  proc sbName do return "static Base";
  proc dbName() do return "dyanmic Base";
}

class Derived : Base {
  var field : string;

  proc init(arg:string) {
    field = "Derived " + arg;
  }

  proc sbName do return "static Derived";
  override proc dbName() do return "dynamic Derived";
}

var b:borrowed Base = (new owned Base()).borrow();
writeln(b.field);		// Expect "Base"
writeln(b.sbName);		// Expect "static Base"
writeln(b.dbName());	// Expect "dynamic Base"

var d:borrowed Derived = (new owned Derived("foo")).borrow();
writeln(d.field);		// Expect "Derived foo"
writeln(d.sbName);		// Expect "static Derived"
writeln(d.dbName());	// Expect "dynamic Derived"

b = (new owned Derived("bar")).borrow();
writeln(b.field);		// Expect "Base"
writeln(b.sbName);		// Expect "static Base"
writeln(b.dbName());	// Expect "dynamic Derived"
