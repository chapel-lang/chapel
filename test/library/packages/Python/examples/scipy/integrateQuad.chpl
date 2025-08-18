use Python;
use CTypes;

var interp = new Interpreter();

var sp = interp.importModule("scipy");
var spi = interp.importModule("scipy.integrate");

// integrate a function using scipy.integrate.quad
proc integrate(f: borrowed Value, a: real, b: real): real {
  return spi.call("quad", f, a, b).call(real, "__getitem__", 0);
}

{
  writeln("Integrating x**3, defined in Python");
  var f = interp.compileLambda("lambda x,: x**3");
  writeln(integrate(f, 0.0, 2.0));
  writeln(integrate(f, 0.0, 4.0));
  writeln(integrate(f, 0.0, 8.0));
}


// converts a c_ptr to a Chapel proc to a scipy LowLevelCallable,
// which is a wrapper around a ctypes function pointer
proc procPtrToPython(f): owned Value {
  // wrap the pointer as a ctypes function pointer
  var ctypes = interp.importModule("ctypes");
  var ctypes_double = ctypes.get("c_double");
  var cfunctype = ctypes.call("CFUNCTYPE", ctypes_double, ctypes_double);
  var f_ptrPy = cfunctype(f:c_ptr(void):c_intptr);

  // create a LowLevelCallable from the function pointer
  var fPy = sp.call("LowLevelCallable", f_ptrPy,
                    kwargs=["signature"=>"double (double)"]);
  return fPy;
}

{
  writeln("Integrating x**3, defined in Chapel");
  proc chplFunc(x: real): real do return x**3;
  var f = procPtrToPython(c_ptrTo(chplFunc));
  writeln(integrate(f, 0.0, 2.0));
  writeln(integrate(f, 0.0, 4.0));
  writeln(integrate(f, 0.0, 8.0));
}
