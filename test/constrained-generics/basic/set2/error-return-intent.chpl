/*
Errors tested by this test:
- a required function cannot have 'param' or 'type' intent (*)
- the return intent of the implementing function must match
  that of the required function (relaxing this restriction is future work)

(*) Allowing param/type interface functions does not blend well
with our "early resolution" strategy. For example, consider this
function that relies on the interface IFC below:
    proc cgFun(arg: IFC) {
      if fparam() > 0 then doSomething();
    }
We expect the branch to be param-folded. However we cannot do that
during early resolution, as the value returned by fparam() is unknown
and can differ between the implementations of IFC.
*/

interface IFC {
  proc fparam() param: int;
  proc ftype() type: int;
  proc fref() ref: int;
  proc fconstref() const ref: int;
  proc fdefault(): int;
}

int implements IFC;
proc fparam() param return 5;
proc ftype() type return string;
proc fref() return 5;
proc fconstref() return 5;
proc fdefault() const ref return numLocales;
