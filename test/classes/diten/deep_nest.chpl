class A {
  var a = "a";
  class B {
    var b = "b";
    proc fun(x: int, y: real) {
      writeln("B.fun(", x, ", ", y, ")");
    }
    class C {
      var c = "c";
      class D {
        var d = "d";
        var n = "d";
        class E {
          var e = "e";
          proc fun(x: int, y: int) {
            writeln("E.fun(", x, ", ", y, ")");
          }
          class F {
            var f = "f";
            class G {
              var g = "g";
              var n = "g";
              class H {
                var h = "h";
                class I {
                  var i = "i";
                  proc method() {
                    writeln(a, b, c, d, e, f, g, h, i);
                    writeln(n);
                    fun(3, 4);
                    fun(3, 4.5);
                  }
                }
                proc method() {
                  var x = new I(); x.method();
                } 
              }
              proc method() {
                var x = new H(); x.method();
              } 
            }
            proc method() {
              var x = new G(); x.method();
            } 
          }
          proc method() {
            var x = new F(); x.method();
          } 
        }
        proc method() {
          var x = new E(); x.method();
        } 
      }
      proc method() {
        var x = new D(); x.method();
      } 
    }
    proc method() {
      var x = new C(); x.method();
    } 
  }
  proc method() {
    var x = new B(); x.method();
  } 
}

var a = new A();
a.method();
