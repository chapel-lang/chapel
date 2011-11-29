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

var b = new B();
b.usea(); //uses value from A, not B
