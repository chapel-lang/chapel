record R {
  param flag = true;
}

// Note: need two functions (instead of adding a second type formal) because
// regular function calls have type actuals instantiated-if-needed. Thus,
// foo(R) will be instantiate as foo(R(true)). As such, it wouldn't catch
// the case in which the call `==` fails to instantiate R (the arg would already
// be instantiated).

proc foo(type t) {
  if t == R then
    writeln("Got an R");
  else
    writeln("It seems I didn't get an R, but rather a ", t:string);
}

proc bar(type t) {
  if t == R(?) then
    writeln("Got an R(?)");
  else
    writeln("It seems I didn't get an R(?), but rather a ", t:string);
}

foo(R);
foo(R(?));
bar(R);
bar(R(?));
