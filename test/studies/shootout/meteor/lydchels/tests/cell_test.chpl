module cell_test {
  use Meteor;
  // Things to test:
  var emptyCell: Cell = new Cell();
  
  var singleCellE: Cell = new Cell();
  singleCellE.neighbors[EAST] = emptyCell;
  
  var singleCellSE: Cell = new Cell();
  singleCellSE.neighbors[SOUTHEAST] = emptyCell;

  var singleCellSW: Cell = new Cell();
  singleCellSW.neighbors[SOUTHWEST] = emptyCell;

  var singleCellW: Cell = new Cell();
  singleCellW.neighbors[WEST] = emptyCell;

  var singleCellNW: Cell = new Cell();
  singleCellNW.neighbors[NORTHWEST] = emptyCell;

  var singleCellNE: Cell = new Cell();
  singleCellNE.neighbors[NORTHEAST] = emptyCell;

  var multiCell: Cell = new Cell();
  multiCell.neighbors[EAST] = singleCellE;
  multiCell.neighbors[SOUTHWEST] = singleCellSW;
  multiCell.neighbors[WEST] = singleCellW;
  multiCell.neighbors[NORTHEAST] = singleCellNE;
  
  var fullCell: Cell = new Cell();
  fullCell.neighbors = singleCellE;
  // For some reason, parallel assignment breaks gNEmpty()

  var processedCell: Cell = new Cell(processed=true);

  //  Black Box
  //   getNeighbor
  //     Initially empty
  proc gNEmpty () {
    if !((emptyCell.getNeighbor(EAST) == nil) && 
	 (emptyCell.getNeighbor(SOUTHEAST) == nil) && 
	 (emptyCell.getNeighbor(SOUTHWEST) == nil) && 
	 (emptyCell.getNeighbor(WEST) == nil) && 
	 (emptyCell.getNeighbor(NORTHWEST) == nil) && 
	 (emptyCell.getNeighbor(NORTHEAST) == nil)) {
      writeln("Fails: Empty cell fails initiation, jedi master disapproves");
      writeln("EAST is empty - ", 
              (emptyCell.getNeighbor(EAST) == nil), ", SOUTHEAST is empty - ",
	      (emptyCell.getNeighbor(SOUTHEAST) == nil), 
	      ", SOUTHWEST is empty - ", 
	      (emptyCell.getNeighbor(SOUTHWEST) == nil), ", WEST is empty - ",
	      (emptyCell.getNeighbor(WEST) == nil), ", NORTHWEST is empty - ",
	      (emptyCell.getNeighbor(NORTHWEST) == nil), 
	      ", NORTHEAST is empty - ", 
	      (emptyCell.getNeighbor(NORTHEAST) == nil));
    } else {
      writeln("Passes: Empty cell is created empty");
    }
  }
  //     Has one
  //       That one is full
  //       Others still empty
  proc gNHasOne () {
    var list: [1..6] Cell = (singleCellE, singleCellSE, singleCellSW, 
			     singleCellW, singleCellNW, singleCellNE);
    forall (i, a) in (list, 1..6) { // forall affects this test, too   
      if !((i.getNeighbor(EAST) != nil) ^ (i.getNeighbor(SOUTHEAST) != nil) ^ 
	   (i.getNeighbor(SOUTHWEST) != nil) ^ (i.getNeighbor(WEST) != nil) ^ 
	   (i.getNeighbor(NORTHWEST) != nil) ^ 
	   (i.getNeighbor(NORTHEAST) != nil)) {
	writeln("Fails: singleCell ", a, " has incorrect number of neighbors:");
	writeln("\tEAST is empty - ", (i.getNeighbor(EAST) == nil),
		", SOUTHEAST is empty - ", (i.getNeighbor(SOUTHEAST) == nil), 
		", SOUTHWEST is empty - ", (i.getNeighbor(SOUTHWEST) == nil), 
		", WEST is empty - ", (i.getNeighbor(WEST) == nil), 
		", NORTHWEST is empty - ", (i.getNeighbor(NORTHWEST) == nil), 
		", NORTHEAST is empty - ", (i.getNeighbor(NORTHEAST) == nil));
      } else {
	writeln("Passes: singleCell ", a, " has correct number of neighbors");
      }
    }
  }

  proc gNHasSome () {
    if !((multiCell.getNeighbor(EAST) != nil) && 
	 (multiCell.getNeighbor(SOUTHEAST) == nil) &&
	 (multiCell.getNeighbor(SOUTHWEST) != nil) &&
	 (multiCell.getNeighbor(WEST) != nil) &&
	 (multiCell.getNeighbor(NORTHWEST) == nil) &&
	 (multiCell.getNeighbor(NORTHEAST) != nil)) {
      writeln("Fails: multiCell's neighbors were incorrect: ");
      writeln("\tEAST is empty - ", (multiCell.getNeighbor(EAST) == nil), 
	      ", SOUTHEAST is empty - ", 
	      (multiCell.getNeighbor(SOUTHEAST) == nil), 
	      ", SOUTHWEST is empty - ", 
	      (multiCell.getNeighbor(SOUTHWEST) == nil), 
	      ", WEST is empty - ", (multiCell.getNeighbor(WEST) == nil), 
	      ", NORTHWEST is empty - ", 
	      (multiCell.getNeighbor(NORTHWEST) == nil), 
	      ", NORTHEAST is empty - ",
	      (multiCell.getNeighbor(NORTHEAST) == nil));
    } else {
      writeln("Passes: multiCells' neighbors were correct");
    }
  }

  //     Full
  proc gNFull () {
    if (fullCell.getNeighbor(EAST) == nil) || 
      (fullCell.getNeighbor(SOUTHEAST) == nil) ||
      (fullCell.getNeighbor(SOUTHWEST) == nil) || 
      (fullCell.getNeighbor(WEST) == nil) ||
      (fullCell.getNeighbor(NORTHWEST) == nil) || 
      (fullCell.getNeighbor(NORTHEAST) == nil) {
      writeln("Fails: fullCell should have all six neighbors:");
      writeln("\tEAST is full - ", (fullCell.getNeighbor(EAST) != nil), 
	      ", SOUTHEAST is full - ", 
	      (fullCell.getNeighbor(SOUTHEAST) != nil), 
	      ", SOUTHWEST is full - ", 
	      (fullCell.getNeighbor(SOUTHWEST) != nil), 
	      ", WEST is full - ", (fullCell.getNeighbor(WEST) != nil), 
	      ", NORTHWEST is full - ",
	      (fullCell.getNeighbor(NORTHWEST) != nil), 
	      ", NORTHEAST is full - ",
	      (fullCell.getNeighbor(NORTHEAST) != nil));
    } else {
      writeln("Passes: fullCell's neighbors are all present");
    }
  }

  //   setNeighbor
  //     Place one
  proc sNPlaceOne () {
    multiCell.setNeighbor(SOUTHEAST, singleCellSW);
    if multiCell.getNeighbor(SOUTHEAST) != singleCellSW {
      writeln("Fails: was supposed to write to multiCell neighbor EAST");
    } else {
      writeln("Passes: set multiCell neighbor");
    }
  }
  //   isProcessed
  //     Initially false
  proc pInitially () {
    if emptyCell.isProcessed() {
      writeln("Fails: processed should initially be false");
    } else {
      writeln("Passes: processed is initially false");
    }
  }
  //     After setProcess set
  proc pAfterSet () {
    emptyCell.setProcessed(true);
    if !emptyCell.isProcessed() {
      writeln("Fails: setProcessed should have set processed");
    } else {
      emptyCell.setProcessed(false);
      if emptyCell.isProcessed() {
	writeln("Fails: setProcessed only set to true!");
      } else {
	writeln("Passes: setProcessed successfully set the process");
      }
    }
  }
  //  Clear Box
  //   getNeighbor
  //     getNeighbor(side) = neighbors[side]
  proc gNClear () {
    if singleCellNW.getNeighbor(NORTHWEST) != singleCellNW.neighbors[NORTHWEST]{
      writeln("Fails: getNeighbor does not reflect the current neighbor state");
      writeln("\t singleCellNW.getNeighbor(NORTHWEST) = ", 
	      singleCellNW.getNeighbor(NORTHWEST), " , should be ",
	      singleCellNW.neighbors[NORTHWEST]);
    } else if (singleCellNW.getNeighbor(NORTHEAST) != 
	       singleCellNW.neighbors[NORTHEAST]){
      writeln("Fails: getNeighbor does not reflect the current neighbor state");
      writeln("\t singleCellNW.getNeighbor(NORTHEAST) = ", 
	      singleCellNW.getNeighbor(NORTHEAST), " , should be ",
	      singleCellNW.neighbors[NORTHEAST]);
    } else if (singleCellNW.getNeighbor(EAST) != 
	       singleCellNW.neighbors[EAST]){
      writeln("Fails: getNeighbor does not reflect the current neighbor state");
      writeln("\t singleCellNW.getNeighbor(EAST) = ", 
	      singleCellNW.getNeighbor(EAST), " , should be ",
	      singleCellNW.neighbors[EAST]);
    } else if (singleCellNW.getNeighbor(SOUTHEAST) != 
	       singleCellNW.neighbors[SOUTHEAST]){
      writeln("Fails: getNeighbor does not reflect the current neighbor state");
      writeln("\t singleCellNW.getNeighbor(SOUTHEAST) = ", 
	      singleCellNW.getNeighbor(SOUTHEAST), " , should be ",
	      singleCellNW.neighbors[SOUTHEAST]);
    } else if (singleCellNW.getNeighbor(SOUTHWEST) != 
	       singleCellNW.neighbors[SOUTHWEST]){
      writeln("Fails: getNeighbor does not reflect the current neighbor state");
      writeln("\t singleCellNW.getNeighbor(SOUTHWEST) = ", 
	      singleCellNW.getNeighbor(SOUTHWEST), " , should be ",
	      singleCellNW.neighbors[SOUTHWEST]);
    } else if (singleCellNW.getNeighbor(WEST) != 
	       singleCellNW.neighbors[WEST]){
      writeln("Fails: getNeighbor does not reflect the current neighbor state");
      writeln("\t singleCellNW.getNeighbor(WEST) = ", 
	      singleCellNW.getNeighbor(WEST), " , should be ",
	      singleCellNW.neighbors[WEST]);
    } else {
      writeln("Passes: getNeighbor returns the correct data");
    }
  }
  //   setNeighbor
  //     neighbors[side] = cell afterwards
  proc sNClear () {
    multiCell.setNeighbor(NORTHEAST, singleCellNW);
    if (multiCell.neighbors[NORTHEAST] != singleCellNW) {
      writeln("Fails: setNeighbor did not correctly affect the neighbor state");
    } else {
      writeln("Passes: setNeighbor correctly alters the neighbor state");
    }
  }
  //   isProcessed
  //     reflects current state accurately
  proc iPClear () {
    if (processedCell.isProcessed() != processedCell.processed) ||
      (emptyCell.isProcessed() != emptyCell.processed) {
      writeln("Fails: isProcessed() does not reflect the current state");
    } else {
      writeln("Passes: isProcessed() reflects the current state");
    }
  }
  //   setProcessed
  //     actually changes processed
  proc sPClear() {
    emptyCell.setProcessed(true);
    if !emptyCell.processed {
      writeln("Fails: setProcessed should have set processed");
    } else {
      emptyCell.setProcessed(false);
      if emptyCell.processed {
	writeln("Fails: setProcessed only set to true!");
      } else {
	writeln("Passes: setProcessed successfully set the process");
      }
    }
  }

  // Clean up the garbage!
  proc cleanUp() {
    delete emptyCell;
    delete singleCellE; 
    delete singleCellSE;
    delete singleCellSW;
    delete singleCellW;
    delete singleCellNW;
    delete singleCellNE;
    delete multiCell; 
    delete fullCell;
    delete processedCell;
  }
}
