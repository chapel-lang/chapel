module OuterModule {
  module MyTypes {
          type Foo = real;
  }

  module MyModule {
          use super.MyTypes;
          class Bar {
                  var thing : real;
                  record myR {
                          var x : int;
                  }
          }
  }

  proc main() {
          
  }
}
