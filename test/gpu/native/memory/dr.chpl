extern proc chpl_gpu_init(): void;

/*class MyClass {*/
  /*var x: int;*/

  /*[>pragma "codegen for GPU"<]*/
  /*proc init() {*/

  /*}*/

/*}*/

/*on here.getChild(1) {*/
  /*chpl_gpu_init();*/

  /*var c = new unmanaged MyClass();*/

  /*[>var arr: [1..10] int;<]*/

/*}*/

pragma "always resolve function"
pragma "codegen for GPU"
export proc bar() {

}

pragma "always resolve function"
pragma "codegen for GPU"
export proc foo() {
  bar();
}

chpl_gpu_init();
__primitive("gpu kernel launch flat", c"foo", 1, 1, 0);
