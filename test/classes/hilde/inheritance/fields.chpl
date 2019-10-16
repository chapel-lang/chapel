// fields.chpl
//
// Show that the getter for a local field will override a parenthesesless
// function of the same name in a parent class.
//
// Show that other parenthesesless functions are also bound based on the
// declared type of the object.
//
class Base {
  proc field return "Base";
  proc sbName return "static Base";
  proc dbName() return "dyanmic Base";
}

class Derived : Base {
  var field : string;

  proc init(arg:string) {
    field = "Derived " + arg;
  }

  proc sbName return "static Derived";
  override proc dbName() return "dynamic Derived";
}

var b:borrowed Base = new borrowed Base();
writeln(b.field);		// Expect "Base"
writeln(b.sbName);		// Expect "static Base"
writeln(b.dbName());	// Expect "dynamic Base"

var d:borrowed Derived = new borrowed Derived("foo");
writeln(d.field);		// Expect "Derived foo"
writeln(d.sbName);		// Expect "static Derived"
writeln(d.dbName());	// Expect "dynamic Derived"

b = new borrowed Derived("bar");
writeln(b.field);		// Expect "Base"
writeln(b.sbName);		// Expect "static Base"
writeln(b.dbName());	// Expect "dynamic Derived"
