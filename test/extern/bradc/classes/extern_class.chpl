_extern class myclass {
  var a:int(64);
}

_extern def myfunc(c:myclass):int(64);

var c = new myclass(5);
writeln( c );
writeln( myfunc(c) );
