use Python;

var interp = new Interpreter();

var sp = interp.importModule("scipy");
var spi = interp.importModule("scipy.integrate");

// integrate a Python Function using scipy.integrate.quad
proc integrate(f: owned Function, a: real, b: real): real {
  return spi.call("quad", f, a, b).call(real, "__getitem__", 0);
}

{
  writeln("Integrating x**3, defined in Python");
  var f = interp.compileLambda("lambda x,: x**3");
  writeln(integrate(f, 0.0, 2.0));
  writeln(integrate(f, 0.0, 4.0));
  writeln(integrate(f, 0.0, 8.0));
}


// integrate a Chapel Function using scipy.integrate.quad
proc integrate(f:proc(_: real):real, a: real, b: real): real {
  use CTypes;
  // get a c_ptr to a Chapel proc
  var f_ptr = c_ptrTo(f):c_ptr(void);

  // create a ctypes object from the function pointer
  // another way to do this would be to support the pycapsule api in some form
  var ctypes = interp.importModule("ctypes");
  var ctypes_double = ctypes.get("c_double");
  var cfunctype = ctypes.call("CFUNCTYPE", ctypes_double, ctypes_double);
  var f_ptrPy = cfunctype(f_ptr:c_intptr);

  // using the python representation of the function pointer,
  // create a LowLevelCallable for scipy usage
  var fPy = sp.call("LowLevelCallable", f_ptrPy,
                    kwargs=["signature"=>"double (double)"]);

  return spi.call("quad", fPy, a, b).call(real, "__getitem__", 0);
}

{
  writeln("Integrating x**3, defined in Chapel");
  export proc f(x: real): real do return x;
  writeln(integrate(f, 0.0, 2.0));
  writeln(integrate(f, 0.0, 4.0));
  writeln(integrate(f, 0.0, 8.0));
}
