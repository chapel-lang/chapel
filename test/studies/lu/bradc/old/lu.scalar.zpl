config const numRows: integer = 16;
config const numCols: integer = 16;

region MatReg = [1..numRows, 1..numCols];

var Mat: [MatReg] float;

const numDiags: integer = min(numRows, numCols);

for d in 1 to numDiags do
  region Reduced = [d..numRows, d..numCols];
  region TopRow = [d, d..numCols];
  region LeftCol = [d..numRows, d];

  var pivotVal: float;
  var pivotLoc: array [1..2] integer;

  [LeftCol] begin
              pivotVal := max<< Mat;
              pivotLoc := maxloc<< Mat;
            end;

  region PivotRow = [pivotLoc[1], d..numCols];

  -- swap rows using remap operator
  var TmpRow: [PivotRow] float;
  [PivotRow] TmpRow := Mat;
  [PivotRow] Mat := Mat#[d, ];
  [TopRow]   Mat := TmpRow#[pivotLoc[1], ];

  region ScaleCol = [d+1..numRows, d];

  [ScaleCol] Mat /= -pivotVal;

  -- use flood operator to broadcast replicated values
  region RepRedRow = [*, d..numCols];
  region RepRedCol = [d..numRows, *];
  var RepRow: [RepRedRow] float;
  var RepCol: [RepRedCol] float;
  [RepRedRow] RepRow := >>[d, ] Mat;
  [RepRedCol] RepCol := >>[ , d] Mat;
  [Reduced] Mat += RepRow * RepCol;
end;
