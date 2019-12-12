module OuterModule {
  module MyTypes {
          type Foo = real;
  }

  module MyModule {
          use MyTypes;
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
