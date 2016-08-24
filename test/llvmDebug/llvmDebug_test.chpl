/*  This program is conceptually very similar to hello.chpl, but it
 *  uses a more structured programming style, explicitly defining a
 *  module, a configuration constant, and a main() procedure.
 */

module My_foo {
  config const My_message = "Hello, world!";
  var My_gv_t: (int, real) = (1, 2.3);
  var My_gv_b: bool = true;

  // Define an Actor class
  class My_Actor {
    var My_name: string;
    var My_age: uint;
  }

  var My_globalActor: My_Actor;

  proc My_sayhello(My_level: int){
    writeln("Hello, Hui",My_level);
  }
    
  proc My_factorial(My_x: int) : int {
    var My_anything=(My_x+5)**My_x : int;
    My_sayhello(My_anything);
    if My_x < 0 then
         halt("factorial -- Sorry, this is not the gamma procedure!");
    return if My_x == 0 then 1 else My_x * My_factorial(My_x-1);
  }

  proc main() {
    var My_localActor = new My_Actor();
    My_localActor.My_name = "Bruce Wayne";
    My_localActor.My_age = 6;
    var My_Number = My_localActor.My_age: int;
    
    writeln(My_message);
    writeln(My_localActor.My_name);
    writeln("6! is ", My_factorial(My_Number));
  }
}
