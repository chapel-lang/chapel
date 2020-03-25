class Foo { }

var f = new Foo();  //owned

var b = f.borrow(); //borrowed
writeln(b.type:string);

var bb = b.borrow(); //can I borrow from borrowed?
writeln(bb.type:string);

var u = b:unmanaged;
writeln(u.type:string);

var bu = u.borrow(); //can I borrow from unmanaged?
writeln(bu.type:string);


var bq: borrowed Foo? = b;
writeln(bq.type:string);

var bbq = bq.borrow();
writeln(bbq.type:string);

var uq = bq:unmanaged class?;
writeln(uq.type:string);

var buq = uq.borrow();
writeln(buq.type:string);


