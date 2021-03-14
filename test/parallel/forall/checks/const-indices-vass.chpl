
var globInt:    int;
var globString: string;
var globRange:  range;
var globDomain: domain(1);
var globArray:  [globDomain] int;

proc mutate(ref obj) { }

iter serDflt(ref arg)                      { yield arg; }
iter parDflt(ref arg)                      { yield arg; }
iter parDflt(ref arg, param tag)           { yield arg; }

iter serConst(ref arg) const               { yield arg; }
iter parConst(ref arg) const               { yield arg; }
iter parConst(ref arg, param tag) const    { yield arg; }

iter serRef(ref arg) ref                   { yield arg; }
iter parRef(ref arg) ref                   { yield arg; }
iter parRef(ref arg, param tag) ref        { yield arg; }

iter sercref(ref arg) const ref            { yield arg; }
iter parcref(ref arg) const ref            { yield arg; }
iter parcref(ref arg, param tag) const ref { yield arg; }

/// the mutate() calls are illegal except as indicated ///

/// default intent ///

for IDX in serDflt(globInt) do
  mutate(IDX);

for IDX in serDflt(globString) do
  mutate(IDX);

for IDX in serDflt(globRange) do
  mutate(IDX);

for IDX in serDflt(globDomain) do
  mutate(IDX);

for IDX in serDflt(globArray) do
  mutate(IDX);  // allowed, see #13268

///

[IDX in serDflt(globInt)]
  mutate(IDX);

[IDX in serDflt(globString)]
  mutate(IDX);

[IDX in serDflt(globRange)]
  mutate(IDX);

[IDX in serDflt(globDomain)]
  mutate(IDX);

[IDX in serDflt(globArray)]
  mutate(IDX);  // allowed, see #13268

///
forall IDX in parDflt(globInt) do
  mutate(IDX);

forall IDX in parDflt(globString) do
  mutate(IDX);

forall IDX in parDflt(globRange) do
  mutate(IDX);

forall IDX in parDflt(globDomain) do
  mutate(IDX);

forall IDX in parDflt(globArray) do
  mutate(IDX);  // allowed, see #13268

/// const intent ///

for IDX in serConst(globInt) do
  mutate(IDX);

for IDX in serConst(globString) do
  mutate(IDX);

for IDX in serConst(globRange) do
  mutate(IDX);

for IDX in serConst(globDomain) do
  mutate(IDX);

for IDX in serConst(globArray) do
  mutate(IDX);  // allowed, see #13268

///

[IDX in serConst(globInt)]
  mutate(IDX);

[IDX in serConst(globString)]
  mutate(IDX);

[IDX in serConst(globRange)]
  mutate(IDX);

[IDX in serConst(globDomain)]
  mutate(IDX);

[IDX in serConst(globArray)]
  mutate(IDX);  // allowed, see #13268

///

forall IDX in parConst(globInt) do
  mutate(IDX);

forall IDX in parConst(globString) do
  mutate(IDX);

forall IDX in parConst(globRange) do
  mutate(IDX);

forall IDX in parConst(globDomain) do
  mutate(IDX);

forall IDX in parConst(globArray) do
  mutate(IDX);  // allowed, see #13268

/// ref intent -- all cases are legal ///

for IDX in serRef(globInt) do
  mutate(IDX);

for IDX in serRef(globString) do
  mutate(IDX);

for IDX in serRef(globRange) do
  mutate(IDX);

for IDX in serRef(globDomain) do
  mutate(IDX);

for IDX in serRef(globArray) do
  mutate(IDX);

///

[IDX in serRef(globInt)]
  mutate(IDX);

[IDX in serRef(globString)]
  mutate(IDX);

[IDX in serRef(globRange)]
  mutate(IDX);

[IDX in serRef(globDomain)]
  mutate(IDX);

[IDX in serRef(globArray)]
  mutate(IDX);

///

forall IDX in parRef(globInt) do
  mutate(IDX);

forall IDX in parRef(globString) do
  mutate(IDX);

forall IDX in parRef(globRange) do
  mutate(IDX);

forall IDX in parRef(globDomain) do
  mutate(IDX);

forall IDX in parRef(globArray) do
  mutate(IDX);

/// const ref intent ///

for IDX in sercref(globInt) do
  mutate(IDX);  // incorrectly allowed at present, #6142

for IDX in sercref(globString) do
  mutate(IDX);  // incorrectly allowed at present, #6142

for IDX in sercref(globRange) do
  mutate(IDX);  // incorrectly allowed at present, #6142

for IDX in sercref(globDomain) do
  mutate(IDX);  // incorrectly allowed at present, #6142

for IDX in sercref(globArray) do
  mutate(IDX);  // incorrectly allowed at present, #6142

///

[IDX in sercref(globInt)]
  mutate(IDX);

[IDX in sercref(globString)]
  mutate(IDX);

[IDX in sercref(globRange)]
  mutate(IDX);

[IDX in sercref(globDomain)]
  mutate(IDX);

[IDX in sercref(globArray)]
  mutate(IDX);

///

forall IDX in parcref(globInt) do
  mutate(IDX);

forall IDX in parcref(globString) do
  mutate(IDX);

forall IDX in parcref(globRange) do
  mutate(IDX);

forall IDX in parcref(globDomain) do
  mutate(IDX);

forall IDX in parcref(globArray) do
  mutate(IDX);
