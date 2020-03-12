module MyLibrary {
  public import Unimportant.Important1;
  public import Unimportant.Important2;
}
module Unimportant {
  module Important1 { proc foo() { writeln("Important1.foo"); } }
  module Important2 { proc foo() { writeln("Important1.foo"); } }
}

module MyNestedLibrary {
  module Unimportant {
    module Important1 { proc foo() { writeln("Important1.foo"); } }
    module Important2 { proc foo() { writeln("Important1.foo"); } }
  }
  public import MyNestedLibrary.Unimportant.Important1;
  public import MyNestedLibrary.Unimportant.Important2;
}
