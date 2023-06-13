class Foo { }

var f = new Foo();  //owned

var b = f.borrow(); //borrowed
writeln(b.type:string);

var bb = b.borrow(); //can I borrow from borrowed?
writeln(bb.type:string);

var u = new unmanaged Foo();

var bu = u.borrow(); //can I borrow from unmanaged?
writeln(bu.type:string);


var bq: borrowed Foo? = b;
writeln(bq.type:string);

var bbq = bq.borrow();
writeln(bbq.type:string);

var uq = u:unmanaged class?;

var buq = uq.borrow();
writeln(buq.type:string);

delete u;
