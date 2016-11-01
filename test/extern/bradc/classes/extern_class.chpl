extern class myclass {
  var a:int(64);
}

extern proc myfunc(c:myclass):int(64);

var c = new myclass(5);
writeln( c );
writeln( myfunc(c) );
delete c;
