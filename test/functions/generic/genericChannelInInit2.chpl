module m {
  use IO;

  class C {

   var nl: int;
   var C_chnl: fileWriter;

   proc init() {
     nl = 0;
     //nl = 0;
   }
 };

 proc main() {
   var b = new owned C();
// writeln("HERE");
 }
}
