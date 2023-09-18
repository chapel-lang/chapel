// A test of explicit intents on `this`.
// In which case fields of `this` do not get their own shadow variables.

const DOM = {1..128};
const dom2 = {1..28};

record RR {
  var aa: [DOM] int;
  var xx: int;
}

proc ref RR.asdf() {
  forall idx in DOM with (ref this) {
    this.aa[idx] = idx*10;
    if idx == DOM.high / 2 then
      this.xx += 1;
  }
  writeln("forall w/ ref intent: ", this);

  forall idx in DOM with (const ref this) {
    // can't update 'this'
  }
  writeln("forall w/ const ref intent: ", this);

  // implicit intent, using shadow variables for the fields of `this`
  forall idx in DOM {
    this.aa[idx] /= 10;
    // can't update this.xx
  }
  writeln("forall w/ implicit intent: ", this);

  coforall idx in dom2 with (ref this) {
    this.aa[idx] += 2;
    if idx == dom2.high / 2 + 1 then
      this.xx += 1;
  }
  writeln("coforall w/ ref intent: ", this);

  coforall idx in dom2 with (const ref this) {
    // can't update 'this'
  }
  writeln("coforall w/ const ref intent: ", this);

  // implicit intent, using shadow variables for the fields of `this`
  coforall idx in dom2 with (ref this) {
    this.aa[idx] += 3;
    // can't update this.xx
  }
  writeln("coforall w/ implicit intent: ", this);

}

var rr: RR;
rr.asdf();
