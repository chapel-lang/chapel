class A {
 const loc : locale;

 proc init() {
   loc = here.locale;
 }
}

record B {
 var y : unmanaged A?;
 var yloc: locale;

 proc init() {
   this.y = nil;
   this.yloc = here;
 }
 proc init(y : unmanaged A) {
   this.y = y;
   this.yloc = y.locale;
 }
}


proc main() {

  var r = new B(new unmanaged A());
  var buff = [new B(new unmanaged A())];

  on Locales[numLocales-1] {
   // uncommenting the following gets it to work
   /*var r1 = r;
   writeln("r1 = ", r1, " has locale ", r1.locale);
   writeln("r1.y = ", r1.y, " has locale ", r1.y.locale);*/
   // this fails
   var r2 = buff[0];
   writeln("r2 = ", r2);
   writeln("r2 has locale ", r2.locale);
   writeln("r2.y = ", r2.y);
   writeln("r2.y has locale ", r2.y.locale);
  }
}
