// This test checks error messages from extern/export function
// intents.

// See issue #16329.

// If more cases are allowed, they should be removed from
// here and tested in other tests that check their behavior.

extern record my_extern_record { }
 
extern proc extern_blank_intent(arg: my_extern_record);
extern proc extern_const_intent(const arg: my_extern_record);
extern proc extern_out_intent(out arg: my_extern_record);
extern proc extern_inout_intent(inout arg: my_extern_record);

export proc export_blank_intent(arg: my_extern_record) { }
export proc export_const_intent(const arg: my_extern_record) { }
export proc export_out_intent(out arg: my_extern_record) { }
export proc export_inout_intent(inout arg: my_extern_record) { }


proc main() {
  var x: my_extern_record;

  extern_blank_intent(x);
  extern_const_intent(x);
  extern_out_intent(x);
  extern_inout_intent(x);

  export_blank_intent(x);
  export_const_intent(x);
  export_out_intent(x);
  export_inout_intent(x);
}
