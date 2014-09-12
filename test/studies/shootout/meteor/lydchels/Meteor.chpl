/**
 * This module stores all the data for the Meteor project, as
 * attempted by Rachel Sobel and Lydia Duncan, whilst referencing
 * the Java solution
 */
module Meteor {

  config const N = 2098;

  const NUMBEROFSIDES: int = 6;

  // The sides of a cell
  const EAST: int = 0;
  const SOUTHEAST: int = 1;
  const SOUTHWEST: int = 2;
  const WEST: int = 3;
  const NORTHWEST: int = 4;
  const NORTHEAST: int = 5;


  class Cell {
    
    var neighbors: [0..(NUMBEROFSIDES-1)] Cell;

    var processed: bool = false;
    
    /**
     * returns the neighbor found at the specified side
     */
    proc getNeighbor(side: int) ref {
      return neighbors[side];
    }

    /**
     * sets the neighbor at the specified side to be the provided Cell
     */
    proc setNeighbor(side: int, cell: Cell) {
      neighbors[side] = cell;
    }

    /**
     * returns true if the current Cell has been processed and false otherwise
     */
    proc isProcessed() {
      return processed;
    }

    /**
     * sets the Cell to have been processed or not depending on b
     */
    proc setProcessed(b: bool) {
      processed = b;
    }

    proc toString() {
      var ans: [0..(NUMBEROFSIDES-1)] bool;
      for i in 0..(NUMBEROFSIDES-1) {
	if neighbors[i] != nil {
	  ans[i] = true;
	}
      }
      return ans;
    }
  }

  /**
   * A subclass of Cell which is useful for dealing with Pieces
   */
  class PieceCell : Cell {
    // Flips the piece across the Prime Meridian (geography, BIRCHES!)
    proc flip() {
      var buffer: Cell = getNeighbor(NORTHEAST);
      setNeighbor(NORTHEAST, getNeighbor(NORTHWEST));
      setNeighbor(NORTHWEST, buffer);
      buffer = getNeighbor(EAST);
      setNeighbor(EAST, getNeighbor(WEST));
      setNeighbor(WEST, buffer);
      buffer = getNeighbor(SOUTHEAST);
      setNeighbor(SOUTHEAST, getNeighbor(SOUTHWEST));
      setNeighbor(SOUTHWEST, buffer);
    }

    // Clockwise rotation
    proc rotate() {
      var eastNeighbor: Cell = getNeighbor(EAST);
      setNeighbor(EAST, getNeighbor(NORTHEAST));
      setNeighbor(NORTHEAST, getNeighbor(NORTHWEST));
      setNeighbor(NORTHWEST, getNeighbor(WEST));
      setNeighbor(WEST, getNeighbor(SOUTHWEST));
      setNeighbor(SOUTHWEST, getNeighbor(SOUTHEAST));
      setNeighbor(SOUTHEAST, eastNeighbor);
    }
  }

  const NUMBEROFCELLS: int = 5;
  const NUMBEROFPERMUTATIONS: int = 12;

  /**
   * Class which contains the methods for dealing with Pieces on the board.
   */
  class Piece {
    var pieceCells: [1..NUMBEROFCELLS] PieceCell;

    proc Piece(list: PieceList) {
      if list.currentToAdd != 6 {
	this = nil;
      } else {
	forall i in 1..NUMBEROFCELLS {
	  pieceCells[i] = list.pieces[i];
	}
      }
    }

    var currentPermutation: int = 0;

    proc rotatePiece () {
      forall i in 1..NUMBEROFCELLS {
	pieceCells[i].rotate();
      }
    }

    proc flipPiece () {
      forall i in 1..NUMBEROFCELLS {
	pieceCells[i].flip();
      }
    }

    proc nextPermutation() {

    }

    proc resetProcessed() {

    }

  }

  /**
   * Class which should be used to build a Piece because it provides handy
   * methods to do so
   */
  class PieceList {
    var pieces: [1..NUMBEROFCELLS] PieceCell;
    var currentToAdd: int = 1; 
    // Goes from 1..NUMBEROFCELLS+1 (at which point it is full) 
    
    /**
     * Method which adds a PieceCell to an already existing list of PieceCells
     * if the neighbor and direction to add are correct.
     */
    proc build(next: PieceCell, neighborNum: int=0, direction: int=-1) const {
      if neighborNum == 0 || direction == -1 {
	if currentToAdd != 1 {
	  return false; // A direction hasn't been specified or a neighbor
	} else {
	  pieces[1] = next;
	  currentToAdd += 1;
	}
      } else if neighborNum >= currentToAdd || 
	  currentToAdd >= (NUMBEROFCELLS + 1) { 
	// If adding to out of bounds area, return false and don't do it
	return false;	
      } else {
	if pieces[neighborNum].getNeighbor(direction) != nil {
	  return false; // The space is occupied
	} else {
	  pieces[currentToAdd] = next;
	  pieces[neighborNum].setNeighbor(direction, pieces[currentToAdd]);
	  var curPos: int = (direction + 3) % 6;
	  pieces[currentToAdd].setNeighbor(curPos, pieces[neighborNum]);
	  // The specified neighbor is updated and added as a neighbor to next
	  
	  // Now we must check if next will get other neighbors
	  var upDir: int = (direction + 1) % 6;
	  var lowDir: int = (direction - 1) % 6;
	  if lowDir < 0 {
	    lowDir += 6;
	  }
	  
	  var upper: Cell = pieces[neighborNum].getNeighbor(upDir);
	  if upper != nil {  
	    // There is a neighbor one further around the compass
	    var upPos: int = (direction + 2) % 6; 
	    upper.setNeighbor(lowDir, pieces[currentToAdd]);
	    pieces[currentToAdd].setNeighbor(upPos, upper);
	  }

	  var lower: Cell = pieces[neighborNum].getNeighbor(lowDir);
	  if lower != nil {
	    // There is a neighbor one less around the compass
	    var lowPos: int = (direction + 4) % 6;
	    lower.setNeighbor(upDir, pieces[currentToAdd]);
	    pieces[currentToAdd].setNeighbor(lowPos, lower);
	  }
	  currentToAdd += 1;
	}
      }
      return true;
    }
  }

  class BoardCell : Cell {

  }

  class Board {

  }

  /*  var Board: [0..9, 0..4] int;

      writeln(Board); */
}
