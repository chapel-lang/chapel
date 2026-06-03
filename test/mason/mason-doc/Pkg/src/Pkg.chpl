/* Documentation for Pkg

The project version is |version|

The project name is |project_name|

The project description is |project_description|

*/
module Pkg {
  use SubPkg;
  use FileSystem;
  include module Included;
  import Included;
  writeln("New library: Pkg");
}
