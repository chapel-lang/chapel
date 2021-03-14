class A
{
   var a = "Some value";

   proc usea()
   {
       writeln(a);
   }
}

class B : A
{
   var a = "New value";
}

var b = new unmanaged B();

b.usea(); //uses value from A, not B

delete b;

