/*

This test is to make sure that links for nested types are handled correctly. All of these links should resolve correctly

- :type:`nested.A`
- :proc:`nested.A.foo`
- :type:`nested.A.B`
- :proc:`nested.A.B.bar`
- :proc:`nested.A.baz`
- :type:`nested.C`
- :proc:`nested.C.foo`

*/
module nested {
  record A {
    proc foo() {}
    record B {
      proc bar() {}
    }
    proc baz() {}
  }
  record C {
    proc foo() {}
  }
}
