// This test verifies expected compilation errors for
// overrides with differing intent.

class Parent {
  proc in_in              (in arg:string) { }
  proc in_out             (in arg:string) { }
  proc in_inout           (in arg:string) { }
  proc in_const           (in arg:string) { }
  proc in_const_in        (in arg:string) { }
  proc in_ref             (in arg:string) { }
  proc in_const_ref       (in arg:string) { }
  proc in_blank           (in arg:string) { }
 
  proc out_in             (out arg:string) { }
  proc out_out            (out arg:string) { }
  proc out_inout          (out arg:string) { }
  proc out_const          (out arg:string) { }
  proc out_const_in       (out arg:string) { }
  proc out_ref            (out arg:string) { }
  proc out_const_ref      (out arg:string) { }
  proc out_blank          (out arg:string) { }
 
  proc inout_in           (inout arg:string) { }
  proc inout_out          (inout arg:string) { }
  proc inout_inout        (inout arg:string) { }
  proc inout_const        (inout arg:string) { }
  proc inout_const_in     (inout arg:string) { }
  proc inout_ref          (inout arg:string) { }
  proc inout_const_ref    (inout arg:string) { }
  proc inout_blank        (inout arg:string) { }

  proc const_in           (const arg:string) { }
  proc const_out          (const arg:string) { }
  proc const_inout        (const arg:string) { }
  proc const_const        (const arg:string) { }
  proc const_const_in     (const arg:string) { }
  proc const_ref          (const arg:string) { }
  proc const_const_ref    (const arg:string) { }
  proc const_blank        (const arg:string) { }

  proc const_in_in        (const in arg:string) { }
  proc const_in_out       (const in arg:string) { }
  proc const_in_inout     (const in arg:string) { }
  proc const_in_const     (const in arg:string) { }
  proc const_in_const_in  (const in arg:string) { }
  proc const_in_ref       (const in arg:string) { }
  proc const_in_const_ref (const in arg:string) { }
  proc const_in_blank     (const in arg:string) { }

  proc ref_in             (ref arg:string) { }
  proc ref_out            (ref arg:string) { }
  proc ref_inout          (ref arg:string) { }
  proc ref_const          (ref arg:string) { }
  proc ref_const_in       (ref arg:string) { }
  proc ref_ref            (ref arg:string) { }
  proc ref_const_ref      (ref arg:string) { }
  proc ref_blank          (ref arg:string) { }

  proc const_ref_in       (const ref arg:string) { }
  proc const_ref_out      (const ref arg:string) { }
  proc const_ref_inout    (const ref arg:string) { }
  proc const_ref_const    (const ref arg:string) { }
  proc const_ref_const_in (const ref arg:string) { }
  proc const_ref_ref      (const ref arg:string) { }
  proc const_ref_const_ref(const ref arg:string) { }
  proc const_ref_blank    (const ref arg:string) { }

  proc blank_in           (arg:string) { }
  proc blank_out          (arg:string) { }
  proc blank_inout        (arg:string) { }
  proc blank_const        (arg:string) { }
  proc blank_const_in     (arg:string) { }
  proc blank_ref          (arg:string) { }
  proc blank_const_ref    (arg:string) { }
  proc blank_blank        (arg:string) { }
}

class Child : Parent {
  override proc in_in(in arg:string) { }
  override proc in_out(out arg:string) { }                  // not overriding
  override proc in_inout(inout arg:string) { }              // not overriding
  override proc in_const(const arg:string) { }              // intent error
  override proc in_const_in(const in arg:string) { }        // intent error
  override proc in_ref(ref arg:string) { }                  // not overriding
  override proc in_const_ref(const ref arg:string) { }      // not overriding
  override proc in_blank(arg:string) { }                    // intent error
 
  override proc out_in(in arg:string) { }                   // not overriding
  override proc out_out(out arg:string) { }                 // 
  override proc out_inout(inout arg:string) { }             // intent error
  override proc out_const(const arg:string) { }             // not overriding
  override proc out_const_in(const in arg:string) { }       // not overriding
  override proc out_ref(ref arg:string) { }                 // intent error
  override proc out_const_ref(const ref arg:string) { }     // intent error
  override proc out_blank(arg:string) { }                   // not overriding

  override proc inout_in(in arg:string) { }                 // not overriding
  override proc inout_out(out arg:string) { }               // intent error
  override proc inout_inout(inout arg:string) { }           // 
  override proc inout_const(const arg:string) { }           // not overriding
  override proc inout_const_in(const in arg:string) { }     // not overriding
  override proc inout_ref(ref arg:string) { }               // intent error
  override proc inout_const_ref(const ref arg:string) { }   // intent error
  override proc inout_blank(arg:string) { }                 // not overriding

  override proc const_in(in arg:string) { }                 // intent error
  override proc const_out(out arg:string) { }               // not overriding
  override proc const_inout(inout arg:string) { }           // not overriding
  override proc const_const(const arg:string) { }           //
  override proc const_const_in(const in arg:string) { }     // intent error
  override proc const_ref(ref arg:string) { }               // not overriding
  override proc const_const_ref(const ref arg:string) { }   // not overriding
  override proc const_blank(arg:string) { }                 // intent error

  override proc const_in_in(in arg:string) { }              // intent error
  override proc const_in_out(out arg:string) { }            // not overriding
  override proc const_in_inout(inout arg:string) { }        // not overriding
  override proc const_in_const(const arg:string) { }        // intent error
  override proc const_in_const_in(const in arg:string) { }  //
  override proc const_in_ref(ref arg:string) { }            // not overriding
  override proc const_in_const_ref(const ref arg:string) {} // not overriding
  override proc const_in_blank(arg:string) { }              // intent error

  override proc ref_in(in arg:string) { }                   // not overriding
  override proc ref_out(out arg:string) { }                 // intent error
  override proc ref_inout(inout arg:string) { }             // intent error
  override proc ref_const(const arg:string) { }             // not overriding
  override proc ref_const_in(const in arg:string) { }       // not overriding
  override proc ref_ref(ref arg:string) { }                 //
  override proc ref_const_ref(const ref arg:string) {}      // intent error
  override proc ref_blank(arg:string) { }                   // not overriding

  override proc const_ref_in(in arg:string) { }             // not overriding
  override proc const_ref_out(out arg:string) { }           // intent error
  override proc const_ref_inout(inout arg:string) { }       // intent error
  override proc const_ref_const(const arg:string) { }       // not overriding
  override proc const_ref_const_in(const in arg:string) { } // not overriding
  override proc const_ref_ref(ref arg:string) { }           // intent error
  override proc const_ref_const_ref(const ref arg:string) {}//
  override proc const_ref_blank(arg:string) { }             // not overriding

  override proc blank_in(in arg:string) { }                 // intent error
  override proc blank_out(out arg:string) { }               // not overriding
  override proc blank_inout(inout arg:string) { }           // not overriding
  override proc blank_const(const arg:string) { }           // intent error
  override proc blank_const_in(const in arg:string) { }     // intent error
  override proc blank_ref(ref arg:string) { }               // not overriding
  override proc blank_const_ref(const ref arg:string) {}    // not overriding
  override proc blank_blank(arg:string) { }                 //
}

proc main() {
  var x:borrowed Parent = new borrowed Child();

  var s = "hi"*2;

  x.in_in(s);
  x.in_out(s);
  x.in_inout(s);
  x.in_const(s);
  x.in_const_in(s);
  x.in_ref(s);
  x.in_const_ref(s);
  x.in_blank(s);
 
  x.out_in(s);
  x.out_out(s);
  x.out_inout(s);
  x.out_const(s);
  x.out_const_in(s);
  x.out_ref(s);
  x.out_const_ref(s);
  x.out_blank(s);
 
  x.inout_in(s);
  x.inout_out(s);
  x.inout_inout(s);
  x.inout_const(s);
  x.inout_const_in(s);
  x.inout_ref(s);
  x.inout_const_ref(s);
  x.inout_blank(s);

  x.const_in(s);
  x.const_out(s);
  x.const_inout(s);
  x.const_const(s);
  x.const_const_in(s);
  x.const_ref(s);
  x.const_const_ref(s);
  x.const_blank(s);

  x.const_in_in(s);
  x.const_in_out(s);
  x.const_in_inout(s);
  x.const_in_const(s);
  x.const_in_const_in(s);
  x.const_in_ref(s);
  x.const_in_const_ref(s);
  x.const_in_blank(s);

  x.ref_in(s);
  x.ref_out(s);
  x.ref_inout(s);
  x.ref_const(s);
  x.ref_const_in(s);
  x.ref_ref(s);
  x.ref_const_ref(s);
  x.ref_blank(s);

  x.const_ref_in(s);
  x.const_ref_out(s);
  x.const_ref_inout(s);
  x.const_ref_const(s);
  x.const_ref_const_in(s);
  x.const_ref_ref(s);
  x.const_ref_const_ref(s);
  x.const_ref_blank(s);

  x.blank_in(s);
  x.blank_out(s);
  x.blank_inout(s);
  x.blank_const(s);
  x.blank_const_in(s);
  x.blank_ref(s);
  x.blank_const_ref(s);
  x.blank_blank(s);

}
