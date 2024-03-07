// from test/domains/vass/no-dom-field-error.chpl
// and _distribution.newRectangularDom()
// we want res1 and res2 both to be 'true'

param rank = 1;
type idxType = int;
param strides = strideKind.one;
const ranges: 1*range;
const _value = new unmanaged GlobalDistribution();

param res1 = __primitive("method call resolves",
        _value, "dsiNewRectangularDom", strides.toStridable());

var x = _value.dsiNewRectangularDom(strides.toStridable());

// this works correctly if I replace 'strides.toStridable()' with 'false'
param res2 = __primitive("resolves",
        _value.dsiNewRectangularDom(strides.toStridable()) );

compilerWarning("res0 = ", res1:string, 0);
compilerWarning("res1 = ", res2:string, 0);
compilerWarning("x : ", x.type:string);
compilerError("done");

class GlobalDistribution {
  proc dsiNewRectangularDom(param stridable: bool) do return 0;
}

@chpldoc.nodoc proc param strideKind.toStridable() param
  do return this != strideKind.one;

