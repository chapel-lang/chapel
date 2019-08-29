config param enable_type_i32 = true;
config param enable_type_i64 = true;
config param enable_type_ui32 = true;
config param enable_type_ui64 = true;
config param enable_type_r32 = true;
config param enable_type_r64 = true;

config const default_do_type_flag = true;

config const do_type_i32 = default_do_type_flag;
config const do_type_i64 = default_do_type_flag;
config const do_type_ui32 = default_do_type_flag;
config const do_type_ui64 = default_do_type_flag;
config const do_type_r32 = default_do_type_flag;
config const do_type_r64 = default_do_type_flag;

config const default_do_op_flag = true;

config const do_op_read = default_do_op_flag;
config const do_op_write = default_do_op_flag;
config const do_op_xchg = default_do_op_flag;
config const do_op_cmpxchg = default_do_op_flag;
config const do_op_add = default_do_op_flag;
config const do_op_sub = default_do_op_flag;
config const do_op_and = default_do_op_flag;
config const do_op_or = default_do_op_flag;
config const do_op_xor = default_do_op_flag;
config const do_op_fadd = default_do_op_flag;
config const do_op_fsub = default_do_op_flag;
config const do_op_fand = default_do_op_flag;
config const do_op_for = default_do_op_flag;
config const do_op_fxor = default_do_op_flag;


config const verbose_failures = false;
config const verbose = verbose_failures;

var pass = true;

if enable_type_i32 && do_type_i32 {
  var i32_glb: [0..2] atomic int(32);
  pass &&= test_i32(i32_glb);
}

if enable_type_i64 && do_type_i64 {
  var i64_glb: [0..2] atomic int(64);
  pass &&= test_i64(i64_glb);
}

if enable_type_ui32 && do_type_ui32 {
  var ui32_glb: [0..2] atomic uint(32);
  pass &&= test_ui32(ui32_glb);
}

if enable_type_ui64 && do_type_ui64 {
  var ui64_glb: [0..2] atomic uint(64);
  pass &&= test_ui64(ui64_glb);
}

if enable_type_r32 && do_type_r32 then {
  var r32_glb: [0..2] atomic real(32);
  pass &&= test_r32(r32_glb);
}

if enable_type_r64 && do_type_r64 then {
  var r64_glb: [0..2] atomic real(64);
  pass &&= test_r64(r64_glb);
}


writeln(if pass then "pass" else "fail");


proc test_i32(ref nonstack_var) {
  var pass = true;

  pass = test_i32_var(nonstack_var, 'global int(32)');

  on Locales[numLocales - 1] {
    var stack_var: [0..2] atomic int(32);
    on Locales[0] do pass &&= test_i32_var(stack_var, 'stack int(32)');
  }

  return pass;
}


proc test_i32_var(ref x, what) {
  for a in x do a.write(0);
  return (   test_read(x, what)
          && test_write(x, 0x0fffffff: int(32), what)
          && test_xchg(x, 0x00ffff00: int(32), what)
          && test_cmpxchg(x, 0xff0000ff: int(32), 2: int(32), what)
          && test_cmpxchg(x, 0x00ffff00: int(32), 0: int(32), what)
          && test_add(x, 0x00ff00ff: int(32), what)
          && test_sub(x, 0x00f000f0: int(32), what)
          && test_and(x, 0x0f0f0f0f: int(32), what)
          && test_or(x, 0x00ff00ff: int(32), what)
          && test_xor(x, 0x0f0f0f0f: int(32), what)
          && test_fadd(x, ~0x00ff00ff: int(32), what)
          && test_fsub(x, ~0x00f000f0: int(32), what)
          && test_fand(x, ~0x0f0f0f0f: int(32), what)
          && test_for(x, ~0x00ff00ff: int(32), what)
          && test_fxor(x, ~0x0f0f0f0f: int(32), what));
}


proc test_i64(ref nonstack_var) {
  var pass = true;

  pass = test_i64_var(nonstack_var, 'global int(64)');

  on Locales[numLocales - 1] {
    var stack_var: [0..2] atomic int(64);
    on Locales[0] do pass &&= test_i64_var(stack_var, 'stack int(64)');
  }

  return pass;
}


proc test_i64_var(ref x, what) {
  for a in x do a.write(0);
  return (   test_read(x, what)
          && test_write(x, 0x0fffffff0fffffff: int(64), what)
          && test_xchg(x, 0x00ffff0000ffff00: int(64), what)
          && test_cmpxchg(x, 0xff0000ffff0000ff: int(64), 2: int(64), what)
          && test_cmpxchg(x, 0x00ffff0000ffff00: int(64), 0: int(64), what)
          && test_add(x, 0x00ff00ff00ff00ff: int(64), what)
          && test_sub(x, 0x00f000f000f000f0: int(64), what)
          && test_and(x, 0x0f0f0f0f0f0f0f0f: int(64), what)
          && test_or(x, 0x00ff00ff00ff00ff: int(64), what)
          && test_xor(x, 0x0f0f0f0f0f0f0f0f: int(64), what)
          && test_fadd(x, ~0x00ff00ff00ff00ff: int(64), what)
          && test_fsub(x, ~0x00f000f000f000f0: int(64), what)
          && test_fand(x, ~0x0f0f0f0f0f0f0f0f: int(64), what)
          && test_for(x, ~0x00ff00ff00ff00ff: int(64), what)
          && test_fxor(x, ~0x0f0f0f0f0f0f0f0f: int(64), what));
}


proc test_ui32(ref nonstack_var) {
  var pass = true;

  pass = test_ui32_var(nonstack_var, 'global uint(32)');

  on Locales[numLocales - 1] {
    var stack_var: [0..2] atomic uint(32);
    on Locales[0] do pass &&= test_ui32_var(stack_var, 'stack uint(32)');
  }

  return pass;
}


proc test_ui32_var(ref x, what) {
  for a in x do a.write(0);
  return (   test_read(x, what)
          && test_write(x, 0x0fffffff: uint(32), what)
          && test_xchg(x, 0x00ffff00: uint(32), what)
          && test_cmpxchg(x, 0xff0000ff: uint(32), 2: uint(32), what)
          && test_cmpxchg(x, 0x00ffff00: uint(32), 0: uint(32), what)
          && test_add(x, 0x00ff00ff: uint(32), what)
          && test_sub(x, 0x00f000f0: uint(32), what)
          && test_and(x, 0x0f0f0f0f: uint(32), what)
          && test_or(x, 0x00ff00ff: uint(32), what)
          && test_xor(x, 0x0f0f0f0f: uint(32), what)
          && test_fadd(x, ~0x00ff00ff: uint(32), what)
          && test_fsub(x, ~0x00f000f0: uint(32), what)
          && test_fand(x, ~0x0f0f0f0f: uint(32), what)
          && test_for(x, ~0x00ff00ff: uint(32), what)
          && test_fxor(x, ~0x0f0f0f0f: uint(32), what));
}


proc test_ui64(ref nonstack_var) {
  var pass = true;

  pass = test_ui64_var(nonstack_var, 'global uint(64)');

  on Locales[numLocales - 1] {
    var stack_var: [0..2] atomic uint(64);
    on Locales[0] do pass &&= test_ui64_var(stack_var, 'stack uint(64)');
  }

  return pass;
}


proc test_ui64_var(ref x, what) {
  for a in x do a.write(0);
  return (   test_read(x, what)
          && test_write(x, 0x0fffffff0fffffff: uint(64), what)
          && test_xchg(x, 0x00ffff0000ffff00: uint(64), what)
          && test_cmpxchg(x, 0xff0000ffff0000ff: uint(64), 2: uint(64), what)
          && test_cmpxchg(x, 0x00ffff0000ffff00: uint(64), 0: uint(64), what)
          && test_add(x, 0x00ff00ff00ff00ff: uint(64), what)
          && test_sub(x, 0x00f000f000f000f0: uint(64), what)
          && test_and(x, 0x0f0f0f0f0f0f0f0f: uint(64), what)
          && test_or(x, 0x00ff00ff00ff00ff: uint(64), what)
          && test_xor(x, 0x0f0f0f0f0f0f0f0f: uint(64), what)
          && test_fadd(x, ~0x00ff00ff00ff00ff: uint(64), what)
          && test_fsub(x, ~0x00f000f000f000f0: uint(64), what)
          && test_fand(x, ~0x0f0f0f0f0f0f0f0f: uint(64), what)
          && test_for(x, ~0x00ff00ff00ff00ff: uint(64), what)
          && test_fxor(x, ~0x0f0f0f0f0f0f0f0f: uint(64), what));
}


proc test_r32(ref nonstack_var) {
  var pass = true;

  pass = test_r32_var(nonstack_var, 'global real(32)');

  on Locales[numLocales - 1] {
    var stack_var: [0..2] atomic real(32);
    on Locales[0] do pass &&= test_r32_var(stack_var, 'stack real(32)');
  }

  return pass;
}


proc test_r32_var(ref x, what) {
  for a in x do a.write(0);
  return (   test_read(x, what)
          && test_write(x, 1.10101: real(32), what)
          && test_xchg(x, 2.0202: real(32), what)
          && test_cmpxchg(x, 1.10101: real(32), 3.33333: real(32), what)
          && test_cmpxchg(x, 2.0202: real(32), 4.44444: real(32), what)
          && test_add(x, 3.14159: real(32), what)
          && test_sub(x, 2.71828: real(32), what)
          && test_fadd(x, -3.14159: real(32), what)
          && test_fsub(x, -2.71828: real(32), what));
}


proc test_r64(ref nonstack_var) {
  var pass = true;

  pass = test_r64_var(nonstack_var, 'global real(64)');

  on Locales[numLocales - 1] {
    var stack_var: [0..2] atomic real(64);
    on Locales[0] do pass &&= test_r64_var(stack_var, 'stack real(64)');
  }

  return pass;
}


proc test_r64_var(ref x, what) {
  for a in x do a.write(0);
  return (   test_read(x, what)
          && test_write(x, 1.10101: real(64), what)
          && test_xchg(x, 2.0202: real(64), what)
          && test_cmpxchg(x, 1.10101: real(64), 3.33333: real(64), what)
          && test_cmpxchg(x, 2.0202: real(64), 4.44444: real(64), what)
          && test_add(x, 3.14159: real(64), what)
          && test_sub(x, 2.71828: real(64), what)
          && test_fadd(x, -3.14159: real(64), what)
          && test_fsub(x, -2.71828: real(64), what));
}


proc test_read(ref x: ?t, in what) {
  if !do_op_read then return true;
  var x0: t;
  for i in x.domain do x0[i].write(x[i].read());
  if verbose then writeln('test atomic Read for ', what);
  var pass = check(x, x0, x0[1].read());
  if !pass && verbose then writeln('atomic Read failed for ', what);
  return pass;
}


proc test_write(ref x: ?t, in val, in what) {
  if !do_op_write then return true;
  var x0: t;
  for i in x.domain do x0[i].write(x[i].read());
  if verbose then writeln('test atomic Write for ', what);
  x[1].write(val);
  var pass = check(x, x0, val);
  if !pass && verbose then writeln('atomic Write failed for ', what);
  return pass;
}


proc test_xchg(ref x: ?t, in val, in what) {
  if !do_op_xchg then return true;
  var x0: t;
  for i in x.domain do x0[i].write(x[i].read());
  if verbose then writeln('test atomic Xchg for ', what);
  var val0 = x[1].exchange(val);
  var pass = check(x, x0, val) && (val0 == x0[1].read());
  if !pass && verbose then writeln('atomic Xchg failed for ', what);
  return pass;
}


proc test_cmpxchg(ref x: ?t, in cmp, in val, in what) {
  if !do_op_cmpxchg then return true;
  var x0: t;
  for i in x.domain do x0[i].write(x[i].read());
  if verbose then writeln('test atomic CmpXchg for ', what);
  var did = x[1].compareAndSwap(cmp, val);
  var pass = (   check(x, x0,
                       if cmp == x0[1].read() then val else x0[1].read())
              && if cmp == x0[1].read() then did else !did);
  if !pass && verbose then writeln('atomic CmpXchg failed for ', what);
  return pass;
}


proc test_add(ref x: ?t, in val, in what) {
  if !do_op_add then return true;
  var x0: t;
  for i in x.domain do x0[i].write(x[i].read());
  if verbose then writeln('test atomic Add for ', what);
  x[1].add(val);
  var pass = check(x, x0, x0[1].read() + val);
  if !pass && verbose then writeln('atomic Add failed for ', what);
  return pass;
}


proc test_sub(ref x: ?t, in val, in what) {
  if !do_op_sub then return true;
  var x0: t;
  for i in x.domain do x0[i].write(x[i].read());
  if verbose then writeln('test atomic Sub for ', what);
  x[1].sub(val);
  var pass = check(x, x0, x0[1].read() - val);
  if !pass && verbose then writeln('atomic Sub failed for ', what);
  return pass;
}


proc test_and(ref x: ?t, in val, in what) {
  if !do_op_and then return true;
  var x0: t;
  for i in x.domain do x0[i].write(x[i].read());
  if verbose then writeln('test atomic And for ', what);
  x[1].and(val);
  var pass = check(x, x0, x0[1].read() & val);
  if !pass && verbose then writeln('atomic And failed for ', what);
  return pass;
}


proc test_or(ref x: ?t, in val, in what) {
  if !do_op_or then return true;
  var x0: t;
  for i in x.domain do x0[i].write(x[i].read());
  if verbose then writeln('test atomic Or for ', what);
  x[1].or(val);
  var pass = check(x, x0, x0[1].read() | val);
  if !pass && verbose then writeln('atomic Or failed for ', what);
  return pass;
}


proc test_xor(ref x: ?t, in val, in what) {
  if !do_op_xor then return true;
  var x0: t;
  for i in x.domain do x0[i].write(x[i].read());
  if verbose then writeln('test atomic Xor for ', what);
  x[1].xor(val);
  var pass = check(x, x0, x0[1].read() ^ val);
  if !pass && verbose then writeln('atomic Xor failed for ', what);
  return pass;
}


proc test_fadd(ref x: ?t, in val, in what) {
  if !do_op_fadd then return true;
  var x0: t;
  for i in x.domain do x0[i].write(x[i].read());
  if verbose then writeln('test atomic FAdd for ', what);
  var val0 = x[1].fetchAdd(val);
  var pass = check(x, x0, x0[1].read() + val) && (val0 == x0[1].read());
  if !pass && verbose then writeln('atomic FAdd failed for ', what);
  return pass;
}


proc test_fsub(ref x: ?t, in val, in what) {
  if !do_op_fsub then return true;
  var x0: t;
  for i in x.domain do x0[i].write(x[i].read());
  if verbose then writeln('test atomic FSub for ', what);
  var val0 = x[1].fetchSub(val);
  var pass = check(x, x0, x0[1].read() - val) && (val0 == x0[1].read());
  if !pass && verbose then writeln('atomic FSub failed for ', what);
  return pass;
}


proc test_fand(ref x: ?t, in val, in what) {
  if !do_op_fand then return true;
  var x0: t;
  for i in x.domain do x0[i].write(x[i].read());
  if verbose then writeln('test atomic FAnd for ', what);
  var val0 = x[1].fetchAnd(val);
  var pass = check(x, x0, x0[1].read() & val) && (val0 == x0[1].read());
  if !pass && verbose then writeln('atomic FAnd failed for ', what);
  return pass;
}


proc test_for(ref x: ?t, in val, in what) {
  if !do_op_for then return true;
  var x0: t;
  for i in x.domain do x0[i].write(x[i].read());
  if verbose then writeln('test atomic FOr for ', what);
  var val0 = x[1].fetchOr(val);
  var pass = check(x, x0, x0[1].read() | val) && (val0 == x0[1].read());
  if !pass && verbose then writeln('atomic FOr failed for ', what);
  return pass;
}


proc test_fxor(ref x: ?t, in val, in what) {
  if !do_op_fxor then return true;
  var x0: t;
  for i in x.domain do x0[i].write(x[i].read());
  if verbose then writeln('test atomic FXor for ', what);
  var val0 = x[1].fetchXor(val);
  var pass = check(x, x0, x0[1].read() ^ val) && (val0 == x0[1].read());
  if !pass && verbose then writeln('atomic FXor failed for ', what);
  return pass;
}


proc check(ref x, ref x0, expect) {
  const x_0  = x[0].read();
  const x0_0 = x0[0].read();
  const x_1  = x[1].read();
  const x_2  = x[2].read();
  const x0_2 = x0[2].read();

  const pass = (x_0 == x0_0 && x_1 == expect && x_2 == x0_2);

  const fmt = if isFloatType(x.type) then '%er' else '%@xu';
  
  if !pass && verbose_failures {
    writef('!(' + fmt + '==' + fmt + ' && '
                + fmt + '==' + fmt + ' && '
                + fmt + '==' + fmt + ')\n',
           x_0, x0_0, x_1, expect, x_2, x0_2);
  }

  return pass;
}
