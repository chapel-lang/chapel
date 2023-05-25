// A test of const-checking in the presence of explicit intent on `this`.
// In which case fields of `this` do not get their own shadow variables.

const DOM = {1..128};
const dom2 = {1..28};

record RR {
  var aa: [DOM] int;
  var xx: int;
}

proc RR.asdf() {
  forall idx in DOM with (ref this) {
    this.aa[idx] = idx*10;             // ok
    if idx == DOM.high / 2 then
      this.xx += 1;                    // ok
  }
  writeln("forall w/ ref intent: ", this);

  forall idx in DOM with (const ref this) {
    this.aa[idx] = idx*10;             // error
    if idx == DOM.high / 2 then
      this.xx += 1;                    // error
  }
  writeln("forall w/ const ref intent: ", this);

  // implicit intent, using shadow variables for the fields of `this`
  forall idx in DOM {
    this.aa[idx] = idx*10;             // ok
    if idx == DOM.high / 2 then
      this.xx += 1;                    // error
  }
  writeln("forall w/ implicit intent: ", this);

  coforall idx in dom2 with (ref this) {
    this.aa[idx] = idx*10;             // ok
    if idx == DOM.high / 2 then
      this.xx += 1;                    // ok
  }
  writeln("coforall w/ ref intent: ", this);

  coforall idx in dom2 with (const ref this) {
    this.aa[idx] = idx*10;             // error
    if idx == DOM.high / 2 then
      this.xx += 1;                    // error
  }
  writeln("coforall w/ const ref intent: ", this);

  // implicit intent, using shadow variables for the fields of `this`
  coforall idx in dom2 {
    this.aa[idx] = idx*10;             // ok
    if idx == DOM.high / 2 then
      this.xx += 1;                    // error
  }
  writeln("coforall w/ implicit intent: ", this);

}

var rr: RR;
rr.asdf();
