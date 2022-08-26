class Foo { }

var a = new owned Foo();
var b = new unmanaged Foo();
var c = (new owned Foo()).borrow();
var d = new shared Foo();
var e = new Foo();

var aq = new owned Foo()?;
var eq = new Foo()?;

/* Double check documentation */
var w = new owned Foo();
/* Double check documentation */
var x = new unmanaged Foo();
/* Double check documentation */
var y = (new owned Foo()).borrow();
/* Double check documentation */
var z = new shared Foo();

/* Double check documentation */
var wq = new owned Foo()?;
/* Double check documentation */
var xq = new unmanaged Foo()?;
/* Double check documentation */
var yq = (new owned Foo()?).borrow();
/* Double check documentation */
var zq = new shared Foo()?;
