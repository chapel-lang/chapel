use Python;
use CTypes;

// integrate a function using scipy.integrate.quad
proc integrate(spi: borrowed Value, f: borrowed Value, a: real, b: real): real {
  return spi.call("quad", f, a, b).call(real, "__getitem__", 0);
}

// converts a c_ptr to a Chapel proc to a scipy LowLevelCallable,
// which is a wrapper around a ctypes function pointer
proc procPtrToPython(interp: borrowed, sp: borrowed, f): owned Value {
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

proc main() {

  var interp = new Interpreter();

  var sp = interp.importModule("scipy");
  var spi = interp.importModule("scipy.integrate");

  {
    writeln("Integrating x**3, defined in Python");
    var f = interp.compileLambda("lambda x,: x**3");
    writeln(integrate(spi, f, 0.0, 2.0));
    writeln(integrate(spi, f, 0.0, 4.0));
    writeln(integrate(spi, f, 0.0, 8.0));
  }

  {
    writeln("Integrating x**3, defined in Chapel");
    export proc chplFunc(x: c_int): c_int do return x**3;
    var p = c_ptrTo(chplFunc);
    var f = procPtrToPython(interp, sp, p);
    writeln(integrate(spi, f, 0.0, 2.0));
    writeln(integrate(spi, f, 0.0, 4.0));
    writeln(integrate(spi, f, 0.0, 8.0));
  }

}
