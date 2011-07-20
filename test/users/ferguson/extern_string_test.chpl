_extern proc return_string_test():string;
_extern proc return_string_arg_test(inout arg:string);

_extern class mys {
  var x:int(32);
}

_extern proc return_mys(inout x:mys);

var x:mys;
return_mys(x);
/*

writeln("returned string ",return_string_test());
var s:string;
return_string_arg_test(s);
writeln("returned string arg ",s);
*/
