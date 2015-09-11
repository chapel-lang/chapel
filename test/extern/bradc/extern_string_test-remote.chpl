extern proc return_string_test():c_string;
extern proc return_string_arg_test(ref str:c_string);

writeln("returned string ",return_string_test()); stdout.flush();
var s:string;
on Locales(1) {
  var temp_cs: c_string;
  return_string_arg_test(temp_cs);
  var temp_s:string = temp_cs;
  s = temp_s;
}
writeln("returned string arg ",s);
