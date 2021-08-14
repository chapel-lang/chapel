// this module name is hard-coded in the compiler
module GPUOutlineTest {
  use CPtr;

  extern proc chpl_gpu_init(): void;
  extern proc chpl_gpu_is_device_ptr(ptr): bool;

  proc testMain() {
    on here.getChild(1) {
      chpl_gpu_init();
      var a: [0..10] int;
      var value = 20;

      /*writeln("A._instance: ", chpl_gpu_is_device_ptr(a._instance));*/
      /*writeln("A._instance.data: ", chpl_gpu_is_device_ptr(a._instance.data));*/
      /*writeln("A._instance.shiftedData: ", chpl_gpu_is_device_ptr(a._instance.shiftedData));*/

      /*writeln("The fields");*/
      /*writeln("A.pid: ", (a.pid));*/
      /*writeln("A._instance: ", (a._instance):c_void_ptr);*/
      /*writeln("A._unowned: ", (a._unowned));*/
      /*writeln("A._instance.data: ", (a._instance.data):c_void_ptr);*/
      /*writeln("PRIM A._instance.data: ", __primitive("_wide_get_addr", a._instance.data));*/
      /*writeln("A._instance.shiftedData: ", (a._instance.shiftedData):c_void_ptr);*/
      /*writeln("PRIM A._instance.shiftedData: ", __primitive("_wide_get_addr", a._instance.shiftedData));*/
      /*writef("A's address: %xu\n", __primitive("_wide_get_addr", a)));*/

      /*writeln("A's address: ", __primitive("addr of", a));*/
      /*writeln("value's address: ", __primitive("addr of", value));*/


      forall i in 0..0 {
        a[i] += i+10;
      }

      writeln(a);

      forall i in 0..0 {
        a[i] += i+value;
      }

      writeln(a);

      foreach i in 0..0 {
        a[i] += i+10;
      }

      writeln(a);

      foreach i in 0..0 {
        a[i] += i+value;
      }

      writeln(a);

    }
  }

  testMain();
}
