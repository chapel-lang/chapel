use Python;
import Reflection;

proc test_no_args(mod: borrowed Module) {
  const funcName = "no_args";
  var func = new Function(mod, funcName);

  func(NoneType);

  // error: wrong return type
  try { func(int); }
  catch e: PythonException { writeln("Caught PythonException: ", e.message()); }
  catch                    { writeln("Caught unknown exception"); }

  // error: too many args
  try { func(NoneType, 2); }
  catch e: PythonException { writeln("Caught PythonException: ", e.message()); }
  catch                    { writeln("Caught unknown exception"); }
}
proc test_one_arg(mod: borrowed Module) {
  const funcName = "one_arg";
  var func = new Function(mod, funcName);

  func(NoneType, 1);

  // error: too many args
  try { func(NoneType, 2, 3); }
  catch e: PythonException { writeln("Caught PythonException: ", e.message()); }
  catch                    { writeln("Caught unknown exception"); }
}
proc test_two_args(mod: borrowed Module) {
  const funcName = "two_args";
  var func = new Function(mod, funcName);

  func(NoneType, 1, 2);
  func(NoneType, "hello", "world");
  func(NoneType, None, None);
}
proc test_three_args(mod: borrowed Module) {
  const funcName = "three_args";
  var func = new Function(mod, funcName);

  func(NoneType, 1, 2, 3);
}
proc test_varargs(mod: borrowed Module) {
  const funcName = "varargs";
  var func = new Function(mod, funcName);

  func(NoneType);
  func(NoneType, 1);
  func(NoneType, 1, 2);
  func(NoneType, 1, 2, 3);
  func(NoneType, 1, 2, 3, [1,2,3,]);
  func(NoneType, 1, 2, 3, [1,2,3,], 4);
  func(NoneType, 1, 2, 3, [1,2,3,], 4, ["key" => "value", "key2" => "value2"]);
}
proc test_one_arg_with_default(mod: borrowed Module) {
  const funcName = "one_arg_with_default";
  var func = new Function(mod, funcName);

  func(NoneType);
  func(NoneType, 7);
}
proc test_three_args_with_default(mod: borrowed Module) {
  const funcName = "three_args_with_default";
  var func = new Function(mod, funcName);

  func(NoneType, 8);
  func(NoneType, 8, 9);
  func(NoneType, 8, 9, 10);
  func(NoneType, 8, kwargs=["c" => 10]);
}
proc test_three_args_with_default_and_kwargs(mod: borrowed Module) {
  const funcName = "three_args_with_default_and_kwargs";
  var func = new Function(mod, funcName);

  func(NoneType, 8);
  func(NoneType, 8, 9);
  func(NoneType, 8, 9, 10);
  func(NoneType, 8, kwargs=["b" => 10]);
  func(NoneType, 8, kwargs=["c" => 11, "abc" => 12]);
}
proc test_varargs_and_kwargs(mod: borrowed Module) {
  const funcName = "varargs_and_kwargs";
  var func = new Function(mod, funcName);

  func(NoneType);
  func(NoneType, 1);
  func(NoneType, 1, ["key" => "value"]);
  func(NoneType, kwargs=["a" => 19]);
  func(NoneType, 1, kwargs=["a" => 20]);
  func(NoneType, 1, 2, ["key" => "value"], kwargs=["a" => 7, "b" => 8]);
}


proc main() {
  var interp = new Interpreter();

  var modName = Reflection.getModuleName();
  var m = new Module(interp, modName);

  test_no_args(m);
  test_one_arg(m);
  test_two_args(m);
  test_three_args(m);
  test_varargs(m);
  test_one_arg_with_default(m);
  test_three_args_with_default(m);
  test_three_args_with_default_and_kwargs(m);
  test_varargs_and_kwargs(m);

}

