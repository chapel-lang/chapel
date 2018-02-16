class Instance {
  var x: int;
}

/*proc Instance.init(x:int) {
  this.x = x;
  super.init();
}
*/

record R {
  var x: int = 0;
  var ptr: Instance = nil;
}

proc R.R(x:int) {
  this.x = x;
  this.ptr = new Instance(x);
}

/*
proc R.init() {
  this.x = 0;
  this.ptr = nil;
  super.init();
}

proc R.init(x:int) {
  this.x = x;
  this.ptr = new Instance(x);
  super.init();
}

proc R.init(from: R) {
  this.x = from.x + 1;
  this.ptr = new Instance(this.x);
  super.init();
  writeln("    initCopy"); // ie copy-init
}
*/

proc R.deinit() {
  delete this.ptr;
  this.ptr = nil;
}

// I'd like this to be ref, but that breaks
//    var outerX: R; begin { var x = outerX; }
pragma "init copy fn"
proc chpl__initCopy(arg: R) {
  // TODO - is no auto destroy necessary here?
  pragma "no auto destroy"
  var ret: R;

  ret.x = arg.x + 1;
  ret.ptr = new Instance(ret.x);

  writeln("    initCopy");

  return ret;
}

proc =(ref lhs: R, rhs: R) {
  writeln("    assign");
  lhs.x = rhs.x;
  delete lhs.ptr;
  lhs.ptr = new Instance(rhs.x);
}


proc check(r:R)
{
  assert(r.x > 0);
  assert(r.ptr != nil);
  assert(r.ptr.x == r.x);
}

var global = new R(1);
ref refGlobal = global;

proc returnR() {
  return new R(1);
}

proc returnRConst() const {
  return new R(1);
}

proc returnRRef() ref {
  return global;
}

proc returnRConstRef() const ref {
  return global;
}

proc varInitValueCall()
{
  writeln(" variable initialization / value call");
  writeln("   blank return intent, untyped variable");
  var v1 = returnR();
  check(v1);
  writeln("   const return intent, untyped variable");
  var v2 = returnRConst();
  check(v2);
  writeln("   blank return intent, typed variable");
  var v3:R = returnR();
  check(v3);
  writeln("   const return intent, typed variable");
  var v4:R = returnRConst();
  check(v4);
}

proc callToInArg_inner(in arg:R)
{
  check(arg);
}
proc callToInArg()
{
  writeln(" call in arg / call");
  callToInArg_inner(returnR());
}


proc varInitLocal()
{
  writeln(" variable initialization / local var");
  var loc = new R(1);
  writeln("   untyped variable");
  var v1 = loc;
  check(v1);
  writeln("   typed variable");
  var v2:R = loc;
  check(v2);
}

proc varInitGlobal()
{
  writeln(" variable initialization / global var");
  writeln("   untyped variable");
  var v1 = global;
  check(v1);
  writeln("   typed variable");
  var v2:R = global;
  check(v2);
}

proc varInitOuter()
{
  writeln(" variable initialization / outer var");
  var outer = new R(1);
  proc inner() {
    writeln("   untyped variable");
    var v1 = outer;
    check(v1);
    writeln("   typed variable");
    var v2:R = outer;
    check(v2);
  }
  inner();
}

proc varInitRefGlobal()
{
  writeln(" variable initialization / global ref");
  writeln("   untyped variable");
  var v1 = refGlobal;
  check(v1);
  writeln("   typed variable");
  var v2:R = refGlobal;
  check(v2);
}

proc varInitLocalRef()
{
  writeln(" variable initialization / local ref");
  var loc = new R(1);
  ref locRef = loc;
  writeln("   untyped variable");
  var v1 = locRef;
  check(v1);
  writeln("   typed variable");
  var v2:R = locRef;
  check(v2);
}

proc varInitArgBlank(arg)
{
  writeln(" variable initialization / blank-intent arg");
  writeln("   untyped variable");
  var v1 = arg;
  check(v1);
  writeln("   typed variable");
  var v2:R = arg;
  check(v2);
}

proc varInitArgConstRef(const ref arg)
{
  writeln(" variable initialization / const-ref arg");
  writeln("   untyped variable");
  var v1 = arg;
  check(v1);
  writeln("   typed variable");
  var v2:R = arg;
  check(v2);
}

proc varInitArgRef(ref arg)
{
  writeln(" variable initialization / ref arg");
  writeln("   untyped variable");
  var v1 = arg;
  check(v1);
  writeln("   typed variable");
  var v2:R = arg;
  check(v2);
}

proc varInitArgIn(in arg)
{
  writeln(" variable initialization / in-intent arg");
  writeln("   untyped variable");
  var v1 = arg;
  check(v1);
  writeln("   typed variable");
  var v2:R = arg;
  check(v2);
}

proc varInitArgConstIn(const in arg)
{
  writeln(" variable initialization / const-in-intent arg");
  writeln("   untyped variable");
  var v1 = arg;
  check(v1);
  writeln("   typed variable");
  var v2:R = arg;
  check(v2);
}

proc do_varInitArgIn(in arg)
{
  writeln(" passing in intent arg to in intent arg");
  varInitArgIn(arg);
}

proc do_varInitArgConstIn(const in arg)
{
  writeln(" passing const in intent arg to const in intent arg");
  varInitArgConstIn(arg);
}

proc varInitArgInout(inout arg)
{
  writeln(" variable initialization / inout-intent arg");
  writeln("   untyped variable");
  var v1 = arg;
  check(v1);
  writeln("   typed variable");
  var v2:R = arg;
  check(v2);
}







record Container {
  var field: R;
}

// initializing a field
proc fieldInitCall()
{
  writeln(" field initialization / value call");
  writeln("   default constructor");
  var cont = new Container(returnR());
  check(cont.field);
}

proc fieldInitLocal()
{
  writeln(" field initialization / local var");
  var loc = new R(1);
  writeln("   default constructor");
  var cont = new Container(loc);
  check(cont.field);
}

proc fieldInitGlobal()
{
  writeln(" field initialization / global var");
  writeln("   default constructor");
  var cont = new Container(global);
  check(cont.field);
}

proc fieldInitRefGlobal()
{
  writeln(" field initialization / global ref");
  writeln("   default constructor");
  var cont = new Container(refGlobal);
  check(cont.field);
}

proc fieldInitLocalRef()
{
  writeln(" field initialization / local ref");
  var loc = new R(1);
  ref locRef = loc;
  writeln("   default constructor");
  var cont = new Container(locRef);
  check(cont.field);
}

proc fieldInitArgBlank(arg)
{
  writeln(" field initialization / blank-intent arg");
  writeln("   default constructor");
  var cont = new Container(arg);
  check(cont.field);
}

proc fieldInitArgConstRef(const ref arg)
{
  writeln(" field initialization / const-ref arg");
  writeln("   default constructor");
  var cont = new Container(arg);
  check(cont.field);
}

proc fieldInitArgRef(ref arg)
{
  writeln(" field initialization / ref arg");
  writeln("   default constructor");
  var cont = new Container(arg);
  check(cont.field);
}

proc fieldInitArgIn(in arg)
{
  writeln(" field initialization / in arg");
  writeln("   default constructor");
  var cont = new Container(arg);
  check(cont.field);
}

proc fieldInitArgInout(inout arg)
{
  writeln(" field initialization / inout arg");
  writeln("   default constructor");
  var cont = new Container(arg);
  check(cont.field);
}

proc returnValueCall() {
  writeln(" return / value call");
  return returnR();
}
proc returnValueCall2_inner() {
  return returnR();
}
proc returnValueCall2() {
  return returnValueCall2_inner();
}

proc returnValueCallConst() {
  writeln(" return / value call const");
  return returnRConst();
}

proc returnLocal() {
  writeln(" return / local variable");
  var x = new R(1);
  return x;
}

proc returnOuter() {
  var outer = new R(1);
  proc inner() {
    writeln(" return / outer variable");
    return outer;
  }
  check(inner());
}

proc returnInArg(in arg) {
  writeln(" return / in arg");
  return arg;
}
proc returnConstInArg(const in arg) {
  writeln(" return / const in arg");
  return arg;
}

proc returnGlobal() {
  writeln(" return / global variable");
  return global;
}
proc returnRefGlobal() {
  writeln(" return / ref global variable");
  return refGlobal;
}
proc returnBlankArg(arg:R) {
  writeln(" return / blank intent arg");
  return arg;
}
proc returnRefArg(ref arg:R) {
  writeln(" return / ref intent arg");
  return arg;
}
proc returnConstRefArg(const ref arg:R) {
  writeln(" return / const ref intent arg");
  return arg;
}
proc returnCallReturnsRef() {
  writeln(" return / call returning ref");
  return returnRRef();
}
proc returnCallReturnsConstRef() {
  writeln(" return / call returning const ref");
  return returnRConstRef();
}


proc returnGlobalRef() ref {
  writeln(" return ref / global variable");
  return global;
}
proc returnGlobalConstRef() const ref {
  writeln(" return const ref / global variable");
  return global;
}

proc returnRefArgRef(ref arg:R) ref {
  writeln(" return ref / ref arg");
  return arg;
}


proc initFromExpiringValue()
{
  writeln(" init / expiring value");
  var a = new R(1);
  var b = a;
  check(b);
}

proc assignFromExpiringValue()
{
  writeln(" assign / expiring value");
  var a = new R(1);
  var b:R;
  b = a;
  check(b);
}



proc main() {

  writeln();
  writeln("VARIABLE INITIALIZATION -- VALUE CALL");
  varInitValueCall();
  callToInArg();
  fieldInitCall();

  writeln();
  writeln("VARIABLE INITIALIZATION -- LOCAL VAR");
  varInitLocal();
  fieldInitLocal();

  writeln();
  writeln("VARIABLE INITIALIZATION -- IN ARG (GLOBAL)");
  varInitArgIn(global);
  writeln();
  varInitArgConstIn(global);

  writeln();
  writeln("VARIABLE INITIALIZATION -- IN ARG (IN ARG (CALL-EXPR)))");
  do_varInitArgIn(returnR());
  do_varInitArgConstIn(returnR());

  writeln();
  writeln("VARIABLE INITIALIZATION -- OUTER VAR");
  varInitOuter();

  writeln();
  writeln("VARIABLE INITIALIZATION -- GLOBAL / REF");
  varInitGlobal();
  varInitRefGlobal();
  varInitLocalRef();
  varInitArgBlank(global);
  varInitArgConstRef(global);
  varInitArgRef(global);
  varInitArgInout(global);

  fieldInitGlobal();
  fieldInitRefGlobal();
  fieldInitLocalRef();
  fieldInitArgBlank(global);
  fieldInitArgConstRef(global);
  fieldInitArgRef(global);
  fieldInitArgIn(global);
  fieldInitArgInout(global);

  writeln();
  writeln("VALUE RETURN -- VALUE CALL");
  check(returnValueCall());
  check(returnValueCall2());
  check(returnValueCallConst());

  writeln();
  writeln("VALUE RETURN -- LOCAL VAR");
  check(returnLocal());

  writeln();
  writeln("VALUE RETURN -- OUTER VAR");
  returnOuter();

  writeln();
  writeln("VALUE RETURN -- IN ARG (GLOBAL)");
  check(returnInArg(global));
  writeln();
  check(returnConstInArg(global));

  writeln();
  writeln("VALUE RETURN -- IN ARG (CALL-EXPR)");
  check(returnInArg(returnR()));
  check(returnConstInArg(returnR()));

  writeln();
  writeln("VALUE RETURN -- GLOBAL/REF");
  check(returnGlobal());
  check(returnRefGlobal());
  check(returnBlankArg(global));
  check(returnRefArg(global));
  check(returnConstRefArg(global));
  check(returnCallReturnsRef());
  check(returnCallReturnsConstRef());


  writeln();
  writeln("REF USE -- REF VALUE");
  // No copy expected for these ref variants
  {
    check(returnGlobalRef());
    ref v2 = returnGlobalRef();
    check(v2);
    check(returnGlobalConstRef());
    check(returnRefArgRef(global));
  }
}

