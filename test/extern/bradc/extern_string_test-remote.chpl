extern proc return_string_test():c_string;
extern proc return_string_arg_test(ref c_string);

writeln("returned string ",return_string_test()); stdout.flush();
var s:string;
on Locales(1) do
  return_string_arg_test(s);
writeln("returned string arg ",s);

