// motivated by functions/deitz/test_setter_method4.chpl
// I called out the cases where the current behavior is incorrect IMO.
// -Vass August'2014

proc f_blank(arg) {}
proc f_in(in arg) {}
proc f_inout(inout arg) {}
proc f_out(out arg) {}
proc f_ref(ref arg) {}
proc f_const_in(const in arg) {}
proc f_const_ref(const ref arg) {}

var globalInt: int;
proc accessorInt() ref {
  writeln(setter);
  return globalInt;
}

var globalArr: [1..2] int;
proc accessorArr() ref {
  writeln(setter);
  return globalArr;
}

write("int drop result  "); accessorInt();
write("int assign to    "); accessorInt() = 5;
write("int by blank     "); f_blank(accessorInt());
write("int by in        "); f_in(accessorInt());
write("int by inout     "); f_inout(accessorInt());
write("int by out       "); f_out(accessorInt());
write("int by ref       "); f_ref(accessorInt());
write("int by const in  "); f_const_in(accessorInt());
write("int by const ref "); f_const_ref(accessorInt()); // currentltly: false

writeln();

write("arr drop result  "); accessorArr();
write("arr assign to    "); accessorArr() = 5;
write("arr by blank     "); f_blank(accessorArr());     // currentltly: false
write("arr by in        "); f_in(accessorArr());
write("arr by inout     "); f_inout(accessorArr());
write("arr by out       "); f_out(accessorArr());
write("arr by ref       "); f_ref(accessorArr());
write("arr by const in  "); f_const_in(accessorArr());
write("arr by const ref "); f_const_ref(accessorArr());  // currentltly: false
