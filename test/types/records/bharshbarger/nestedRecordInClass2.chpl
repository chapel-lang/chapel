module OuterModule {
  module MyTypes {
          type Foo = real;
  }

  module MyModule {
          use super.MyTypes;
          class Bar {
                  var thing : Foo;
                  record myR {
                          var x : int;
                  }
          }
  }

  proc main() {
          
  }
}
