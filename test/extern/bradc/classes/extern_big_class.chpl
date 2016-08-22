extern class myclass {
  var a:int(64);
  var b:int(64);
  var c:int(64);
  var d:int(64);
  var e:int(64);
}

extern proc myfunc(c:myclass):int(64);

var c = new myclass(e=5);

writeln( c );
writeln( myfunc(c) );

delete c;
