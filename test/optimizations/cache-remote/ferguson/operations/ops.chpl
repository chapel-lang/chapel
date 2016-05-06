use Operations;

config param nocache = true;

proc makeString( size:int, buf:c_void_ptr ) {
  return new string(buf:c_ptr(uint(8)), size, size, true, true);
}

class MyOperationsHandler : OperationsHandler {
  proc processOperations( size:int, buf:c_void_ptr ) {
    writeln("in MyOperationsHandler.processOperations ",
            makeString(size, buf));
  }
}
class OtherOperationsHandler : OperationsHandler {
  proc processOperations( size:int, buf:c_void_ptr ) {
    writeln("in OtherOperationsHandler.processOperations ",
            makeString(size, buf));
  }
}

proc do_operation(obj:OperationsHandler, mode:int(32), idx:int, size:int, buf:c_void_ptr) {

  extern proc chpl_gen_comm_start_op(node:int(32), subloc:int(32), fid:int(32),
                                     obj:c_void_ptr, args:c_void_ptr,
                                     size:size_t, mode:int(32), idx:int);

  var loc = __primitive("_wide_get_locale", obj);
  var node = chpl_nodeFromLocaleID(loc);
  var subloc = chpl_sublocFromLocaleID(loc);
  var raddr = __primitive("_wide_get_addr", obj);
  chpl_gen_comm_start_op(node, subloc, -1, raddr, buf, size:size_t,
                         mode, idx);
}

{
  var a = new MyOperationsHandler();

  var test = "hello";
  var len = test.length;

  var buf = test.c_str():c_void_ptr;

  writeln("local: starting operations a a a");

  do_operation(a, 0, 0, len, buf);
  do_operation(a, 0, 0, len, buf);
  do_operation(a, 0, 0, len, buf);

  writeln("local: done operations");
}

if numLocales > 1 {
  var a = new MyOperationsHandler();

  on Locales[1] {
    var b = new OtherOperationsHandler();

    var test = "hello";
    var len = test.length;

    var buf = test.c_str():c_void_ptr;

    writeln("2 locales: starting operations a a b b a");

    do_operation(a, 0, 0, len, buf);
    do_operation(a, 0, 0, len, buf);
    do_operation(b, 0, 0, len, buf);
    do_operation(b, 0, 0, len, buf);
    do_operation(a, 0, 0, len, buf);

    writeln("2 locales: done operations");
  }
}

if numLocales > 2 {
  var a:MyOperationsHandler;
  var b:OtherOperationsHandler;
  
  on Locales[1] {
    a = new MyOperationsHandler();
  }

  on Locales[2] {
    b = new OtherOperationsHandler();
  }

  var test = "hello";
  var len = test.length;

  var buf = test.c_str():c_void_ptr;

  writeln("3 locales: starting operations a a b b a");

  do_operation(a, 0, 0, len, buf);
  do_operation(a, 0, 0, len, buf);
  do_operation(b, 0, 0, len, buf);
  do_operation(b, 0, 0, len, buf);
  do_operation(a, 0, 0, len, buf);

  writeln("3 locales: done operations");
}
