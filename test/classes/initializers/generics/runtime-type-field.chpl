
// From #10085

class Map {
  type domain_type;
  var parent_domain : domain_type;
  proc init( in_domain : ?T )
    where isRectangularDom( in_domain ) || isSparseDom( in_domain )
    {
      writeln( "Creating Map(", T : string, ")" );
      this.domain_type = T;
      this.parent_domain = in_domain; // Fault happens in this statement
      writeln( this.parent_domain );
    }
}

proc main() {
  var D : domain(2) = {1..5, 1..5 };
  var sD : sparse subdomain( D );

  // add some indexes into sD
  for idx in D {
    if sD.size >= D.size / 2 then break;
    sD += idx;
  }

  var M1 = new owned Map( D );

  writeln();

  // In #10085, this call to Map's initializer would result in a segfault
  // because the compiler used the static type of 'domain_type' instead of its
  // runtime type. This led to use of uninitialized memory when creating the
  // 'parent_domain' field.
  var M2 = new owned Map( sD );

  writeln("after");
}
