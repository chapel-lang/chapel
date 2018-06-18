class Foo { }

var a = new owned Foo();
var b = new unmanaged Foo();
var c = new borrowed Foo();
var d = new shared Foo();
/* Double check documentation */
var w = new owned Foo();
/* Double check documentation */
var x = new unmanaged Foo();
/* Double check documentation */
var y = new borrowed Foo();
/* Double check documentation */
var z = new shared Foo();
