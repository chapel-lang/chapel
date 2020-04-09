use cell_test;

writeln("Starting Cell class tests");
cell_test.gNEmpty();
cell_test.gNHasOne();
cell_test.gNHasSome();
cell_test.gNFull();  
cell_test.sNPlaceOne();
cell_test.pInitially();
cell_test.pAfterSet();
cell_test.gNClear();
cell_test.sNClear();
cell_test.iPClear();
cell_test.sPClear();
cell_test.cleanUp();
writeln("Finished Cell class tests\n");

use piececell_test;

writeln("Starting PieceCell class tests");
piececell_test.gNEmpty();
piececell_test.gNHasOne();
piececell_test.gNHasSome();
piececell_test.gNFull();  
piececell_test.sNPlaceOne();
piececell_test.pInitially();
piececell_test.pAfterSet();
piececell_test.gNClear();
piececell_test.sNClear();
piececell_test.iPClear();
piececell_test.sPClear();
flip();
rotate();
piececell_test.cleanUp();
writeln("Finished PieceCell class tests\n");

use piecelist_test;

writeln("Starting PieceList class tests");
empty();
bFirst();
bMiddleLine();
bEndLine();
bTaken();
bUnattached();
bAllUpdated();
piecelist_test.cleanUp();
writeln("Finished PieceList class tests\n");

use piece_test;

writeln("Starting Piece class tests");
initialize();
iUnprocessed();
iPerm0();
writeln("Straight test:");
fP(straight);
roPi(straight);
writeln("Circular test:");
fP(circular);
roPi(circular);
writeln("Jagged test:");
fP(jagged);
roPi(jagged);
writeln("Block test:");
fP(block);
roPi(block);
writeln("Mjolnir test:");
fP(mjolnir);
roPi(mjolnir);
writeln("Horse test:");
fP(horse);
roPi(horse);
writeln("Testing nextPermutation:");
writeln("When the permutation is from 0 to 4, rotation should occur normally"); 
nPNormal(straight);
writeln("When the permutation is 5 or 11, piece should be flipped " +
	"instead of rotated");
nPFlipNext(circular);
nPFlipNext(jagged);
nPNormalFlipped();
rePr();
piece_test.cleanUp();
writeln("Finished Piece class tests");