extern proc return_string_test():string;
extern proc return_string_arg_test(ref string);

writeln("returned string ",return_string_test()); stdout.flush();
var s:string;
on Locales(1) do
  return_string_arg_test(s);
writeln("returned string arg ",s);

