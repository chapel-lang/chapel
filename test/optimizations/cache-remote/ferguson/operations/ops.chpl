use Operations;

config param nocache = true;

proc makeString( size:int, buf:c_void_ptr ) {
  return new string(buf:c_ptr(uint(8)), size, size, true, true);
}

class MyOperationsHandler : OperationsHandler {
  proc processOperations( size:int, buf:c_void_ptr ) {
    writeln(makeString(size, buf),
            " in MyOperationsHandler.processOperations");
  }
}
class OtherOperationsHandler : OperationsHandler {
  proc processOperations( size:int, buf:c_void_ptr ) {
    writeln(makeString(size, buf),
            " in OtherOperationsHandler.processOperations");
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

proc test_operation(obj:OperationsHandler, i:int)
{
  var str = "hello" + i;
  var len = str.length;
  var buf = str.c_str():c_void_ptr;
  do_operation(obj, 0, 0, len, buf);
}

if numLocales == 1 {
  var a = new MyOperationsHandler();

  writeln("a local: starting operations a a a");

  test_operation(a, 1);
  test_operation(a, 2);
  test_operation(a, 3);

  writeln("b local: done operations");
}


if numLocales == 2 {
  var a = new MyOperationsHandler();

  on Locales[1] {
    var b = new OtherOperationsHandler();

    writeln("c 2 locales: starting operations a a b b a");

    test_operation(a, 1);
    test_operation(a, 2);
    test_operation(b, 3);
    test_operation(b, 4);
    test_operation(a, 5);

    writeln("d 2 locales: done operations");
  }
}

if numLocales == 3 {
  var a:MyOperationsHandler;
  var b:OtherOperationsHandler;
  
  on Locales[1] {
    a = new MyOperationsHandler();
  }

  on Locales[2] {
    b = new OtherOperationsHandler();
  }

  writeln("e 3 locales: starting operations a a b b a");

  test_operation(a, 1);
  test_operation(a, 2);
  test_operation(b, 3);
  test_operation(b, 4);
  test_operation(a, 5);

  writeln("f 3 locales: done operations");
}
