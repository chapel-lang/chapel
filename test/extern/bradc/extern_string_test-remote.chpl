extern proc return_string_test():c_string;
extern proc return_string_arg_test(ref str:c_string);

writeln("returned string ",return_string_test()); stdout.flush();
var s:string;
on Locales(1) {
  var tmp:c_string = s.localize().c_str();
  return_string_arg_test(tmp);
  s = tmp:string; // TODO -- this cast should not be necessary.
}
writeln("returned string arg ",s);

