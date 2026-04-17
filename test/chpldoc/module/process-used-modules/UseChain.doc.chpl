module UseChain {
  use Used1;

  module PublicSubmodule {
  }
  import UseChain.PublicSubmodule;

  @chpldoc.nodoc
  module NoDocSubmodule {
  }
  public import UseChain.NoDocSubmodule;

  private module PrivateSubmodule {
    var y: int;
  }
  public use PrivateSubmodule;

}
