
/***************************************************
 * Parallel DPLL implementation
 * by Tayfun Elmas 
 ***************************************************/

/***************************************************
 * Modules referenced
 ***************************************************/

use Time; // for measuring the elapsed time

/***************************************************
 * Configuration variables
 ***************************************************/
 
config param DEBUG = false; // enables/disables extra debug messages to the console
config param STAT = false;  // enables/disables collecting statistics while searching

config const FILENAME : string; // input file path

config const MAXPROCS : int = 5; // maximum number of processors to be assigned search branches
config const printTiming = true;

/***************************************************
 * Global variables
 ***************************************************/

var numVars : int(32); // number of variables
var numClss : sync int(32); // number of clauses

var domVars : domain(1) dmapped (Block); // domain for variables
var domClss : domain(1) dmapped (Block); // domain for clauses

var clauses : [domClss] Clause; // clauses in the formula

// used to synchronize when a processor is adding a new clause to the global clauses as a result of a conflict
var clauses_lock : sync bool;

// result of the search, UNKNOWN means the search is still going on
enum sat_result {SAT, UNSAT, UNKNOWN};

// result of the search. when any processor finds out the solution, it is set. 
var glb_sat : sat_result = sat_result.UNKNOWN;
var glb_model : Assignment; // the complete assignment if the formula is satisfiable

var numProcs : sync int = 0; // current number of processors assigned a search branch

var timer : Timer; // timer to measure the elapsed time

var stats : Statistics; // statistics object to collect measurements

/***************************************************
 * Classes
 ***************************************************/

/***************************************************
 * class to store measurements during the search
 ***************************************************/
class Statistics {
	var nClauses : int = 0;
	var nVariables : int = 0;
	var nConflicts : int = 0;
	var nProcesses : int = 0;
	var nDecisions : int = 0;
	var nDeductions : int = 0;
	var maxDecisionLevel : int = 0;
	
	// prints out the entire assignment
	proc writeThis(w : Writer) throws {
		w.write("Statistics\n");
		w.write("Num clauses: ", nClauses, "\n");
		w.write("Num conflicts: ", nVariables, "\n");
		w.write("Num processes: ", nConflicts, "\n");
		w.write("Num decisions: ", nProcesses, "\n");
		w.write("Num deductions: ", nDeductions, "\n");
		w.write("Max decision levels: ", maxDecisionLevel, "\n");
  	}
}

/***************************************************
 * represents an assignment to a variable. it also keeps track of previous assignments that implied this one.
 ***************************************************/
class IGNode {
	var varid  : int; // id of the variable
	var dlevel : int; // decision level at which the assignment is peformed 
	var value  : bool; // value assigned
	var isdecision : bool; // was this a decision or as a result of a deduction
	var antecedents : List(IGNode); // previous assignments that implied this assignment (empty if isdecision == true)
	
	proc init(i:int, dl:int, v:bool, dv:bool) {
		this.varid = i;
		this.dlevel = dl;
		this.value = v;
		this.isdecision = dv;
		this.antecedents = new List(IGNode);
	} 	
	
	// initialize this node using the information stored in a given node
	proc initwith(node : IGNode) {
		this.init(node.varid, node.dlevel, node.value, node.isdecision);
	} 
}

/***************************************************
 * class to store the current set of assignments to the variables
 ***************************************************/
class Assignment {
	var values : [domVars] IGNode; // map from variable id to the assignment node (nil if the variable has not been assigned)
	var dlevel : int; // current decision level
	var slevel : int; // the decision level (of the parent processor) when this assignment is created (0 for the first processor)

	proc init() {
		for i in domVars do {
			values(i) = nil;	
		}	
		dlevel = 0;
		slevel = 0; // the only process with slevel is the starting process
	}
	
	// initializes the assignment by copying the individual assignments from a given one, which is the parent
	proc init(A : Assignment) {
		for i in domVars do {
			values(i) = A(i);	
		}	
		dlevel = A.dlevel;
		slevel = A.dlevel;
	}
	
	// increase the decision level to the next one
	proc next_dlevel() {
		dlevel = dlevel + 1;
	}
	
	// reset the decision level to the given one
	proc reset_dlevel(dl : int) {
		dlevel = dl;
	}
	
	// returns the IGNode (representing the assignment) associated with the variable
	proc this(i : int) ref {
		assert(domVars.contains(i), "Check if i is member of assignment.");
		return values(i);
	}
	
	// have all the variables been assigned?
	proc is_complete() : bool {
		return (dlevel == numVars);
	}
	
	// make an implication edge from the source to the target, as a result of a deduction
	proc make_edge(from_node : IGNode, to_node : IGNode) {
		var ans = new to_node.antecedents;
		ans.insert(from_node);
	}
	
	// assign a variable
	proc assign(x : int, v : bool, dv : bool) : IGNode {
		var node = new IGNode();
		
		assert(values(x) == nil, "Variables must be unassigned.");
		
		node.init(x, dlevel, v, dv);
		values(x) = node;
		
		return node;
	} 
	
	// assign a unit variable (+literal means assign true, -literal means assign false)
	proc assign_unit(unit : int) {
		return assign_literal(unit, false);
	}
	
	// assign a literal (+literal means assign true, -literal means assign false)
	proc assign_literal(literal : int, dv : bool) : IGNode {
		if literal > 0 {
			return assign(literal, true, dv);
		} else {
			return assign(abs(literal), false, dv);
		}	
	}
	
	// assign a decision variable (+literal means assign true, -literal means assign false)
	proc assign_decision(literal : int) : IGNode {
		return assign_literal(literal, true);	
	}
	
	// remove the given assignment 
	proc unassign(node : IGNode) {
		values(node.varid) = nil;
	}
	
	// replace the old node with a new node with the complement value
	proc reassign(x : int, v : bool) {
		var node : IGNode();
		var newnode = new IGNode();
		
		node = values(x);
		newnode.initwith(node);
		newnode.value = v;
		values(x) = newnode; 
	} 
	
	// prints out the entire assignment
	proc writeThis(w : Writer) throws {
		var node : IGNode = nil;
		
  		w.write("[");
  		forall node in values {
  			if node == nil {
  				write("?");
  			} else {
  				write(node.varid, ": ");
  				if node.value == true then
  					write("1");
  				else
  					write("0");
  				write(" , ");
  			}
  		}
  		w.write("]");
  	}
}
/***************************************************
 * class to represent a clause (disjunction of literals)
 ***************************************************/
class Clause {
	var domLtrs  : sparse subdomain (domVars); // subdomain points only to the variables whose literals are in this clause
	var posneg   : [domLtrs] bool; // map from variable id to true/false. posneg[id]==true means +id is in the clause, posneg[id]==false means -id is in the clause
	
	proc init() {
		domLtrs.clear();
	}
	
	// number of literals in this clause
	proc numLtrs() {
		return domLtrs.numIndices;
	}
	
	// iterator over the literals in the clause
	iter these() {
	    for ltr in domLtrs do
	      yield ltr;
  	}
  	
  	// check if this contains a given literal
  	proc has_literal(l : int) {
  		for ltr in domLtrs {
  			if ltr == l then
  				return true;
  		} 
  		return false;
  	}
  	
  	// check if this contains a given variable either in positive or negative form
  	proc has_variable(l : int) {
  		for ltr in domLtrs {
  			if abs(ltr) == l then
  				return true;
  		} 
  		return false;
  	}
  	
  	// prints out this clause
  	proc writeThis(w : Writer) throws {
  		w.write("[");
  		forall ltr in domLtrs {
  			if posneg(ltr) == false then
  				w.write("-");
  			
  			w.write(ltr);
  			w.write(" ");
  		}
  		w.write("]");
  	}
  	
  	// does the given assignment satisfy this clause?
	proc check_sat(A : Assignment) : bool {
		var issat = false;
		var ltr : int;
		var node : IGNode;
		var ispos : bool;
		
		forall ltr in domLtrs {
			if issat == false {
				node = A(ltr);
				if node != nil {
					ispos = posneg(ltr);
					if (ispos == node.value) {
						issat = true;
						// break;
					}
				}
			}
		}
		return issat;
	}
	
	// does the given assignment makes this clause unsatisfiable?
	proc check_conflict(A : Assignment) : bool {
		var conflict = true;
		var ltr : int;
		var node : IGNode;
		var ispos : bool;
		
		forall ltr in domLtrs {
			if conflict == true {
				node = A(ltr);
				if node != nil {
					ispos = posneg(ltr);
					if (ispos == node.value) {
						conflict = false;
						// break;
					}
				} else {
					conflict = false;
					// break;
				}
			}
		}
		return conflict;
	}
	
	// does the given assignment leaves this clause unit? (unit clause: the clause is unsatisfied and there is exactly one literal unassigned)
	proc check_unit(A : Assignment) : int {
		var unit = 0;
		var node : IGNode;
		var ltr : int;
		var ispos : bool;
		var isunit = true;
		
		forall ltr in domLtrs {
			if isunit == true {
				ispos = posneg(ltr);
				node = A(ltr);
				if node == nil {
					if unit == 0 {
						if ispos then
							unit = ltr;
						else
							unit = -ltr;
					} else {
						unit = 0;
						isunit = false;
						// break;
					}
				} else if node.value == ispos {
					unit = 0;
					isunit = false;
					// break;
				}
			}
		}
		return unit;		
	}
}

/***************************************************
 * Top-level global functions
 ***************************************************/

/***************************************************
 * selects an unassigned variable to decide whose value in the current decision level
 * this basically selects the one with lowest id
 ***************************************************/
proc select_dvar_basic(A : Assignment) {
	var node : IGNode;
	var dvar = 0;
	
	// simply select the first unassigned variable
	forall i in domVars {
		if dvar == 0 {
			node = A(i);
			if node == nil {
				dvar = i;
			}
		}		
	}
	
	return dvar;
}

/***************************************************
 * selects an unassigned variable to decide whose value in the current decision level
 * this selects the one whose assignment will satisfy maximum number of clauses 
 ***************************************************/
 
proc select_dvar(A : Assignment) {
	var node : IGNode;
	var maxcls : int = 0;
	var maxltr : int = 0;
	var clause : Clause;
	
	forall i in domVars {
		node = A(i);
		if node == nil {
			var pos = 0;
			var neg = 0;
			var ltr = i;
			forall c in domClss {
				clause = clauses(c);
				if clause.check_sat(A) == false {
					if clause.has_variable(i) {
						if clause.posneg(i) {
							pos = pos + 1;
						} else {
							neg = neg + 1;
						}
					}
				}
			}
			if (pos + neg) > 0 {
				if neg > pos then
					ltr = -ltr;
				pos = max(pos,neg);
				if maxltr == 0 || pos > maxcls {
					maxcls = pos;
					maxltr = ltr;
				}
			}
		}		
	}
	
	assert(maxltr != 0, "A decision variables should be selected.");
	
	return maxltr;
}
/***************************************************
 * decides the value of an unassigned variable
 ***************************************************/
proc decide(A : Assignment) {

	var node : IGNode;
	var dltr : int;
	
	dltr = select_dvar_basic(A);

	A.next_dlevel(); // increase the decision level here.
	node = A.assign_decision(dltr); // assigns true
	
	if STAT {
		stats.nDecisions = stats.nDecisions + 1;
		stats.maxDecisionLevel = max(A.dlevel, stats.maxDecisionLevel); 	
	} 
	
	return node.varid;
}

/***************************************************
 * check whether the entire formula has been satisfied by the given assignment 
 ***************************************************/
proc check_sat(A : Assignment) : bool {
	
	var clause : Clause;
	var issat = true;
	
	// first check the global variable
	if glb_sat != sat_result.UNKNOWN then
		return true;
	
	forall c in domClss do {
		if issat == true {
			clause = clauses(c);
			if clause.check_sat(A) == false {
				issat = false;
				// break;
			}
		}
	}
	
	if issat == true {
		glb_sat = sat_result.SAT;
		glb_model = A;	
	}
	
	return issat;
}

/***************************************************
 * checks whether there is any unsatisfied clauses by the given assignment
 ***************************************************/
proc check_conflict(A : Assignment) : Clause {
	
	var conflict_clause : Clause = nil;
	var clause : Clause = nil;
	
	forall c in domClss {
		if conflict_clause == nil {
			clause = clauses(c);
			if clause.check_conflict(A) == true {
				conflict_clause = clause;
				
				if STAT then
					stats.nConflicts = stats.nConflicts + 1; 
				
				// break;
			}
		}
	}
	
	return conflict_clause;
}

/***************************************************
 * makes deductions. deduction detects unit clauses (unsatisfied and having exactly one unassigned literal) and assigns the unassigned literals in them
 ***************************************************/
proc deduce(dvar : int, A : Assignment) : Clause {
	var clause : Clause;
	var unit : int;
	var unode : IGNode;
	var conflict_clause : Clause = nil;
	
	var node : IGNode = A(dvar); 
	
	forall c in domClss {
		if conflict_clause == nil {
			clause = clauses(c);
			if clause.check_conflict(A) == true {
				conflict_clause = clause;
				// break;
			}
			if conflict_clause == nil { 
				unit = clause.check_unit(A);
				if unit != 0 {
					verbose("Deduced unit: " + unit + " from ");
					if DEBUG { clause.writeThis(stdout); writeln(); }
					unode = A.assign_unit(unit);
					A.make_edge(node,unode);
					
					if STAT then
						stats.nDeductions = stats.nDeductions + 1; 
					
					conflict_clause = deduce(unode.varid, A);
					// if conflict_clause != nil then
					// 		break;
				}	
			}
		}
	}
	
	return conflict_clause;
}

/***************************************************
 * collect the assignments that caused the given assignment (node) to be deducted
 ***************************************************/
proc compute_causes(node : IGNode, causes : List(IGNode)) {
	var antecedents = node.antecedents;
	
	forall anode in antecedents {
		assert(anode != nil, "Antecedents contains nil. Varid: " + node.varid);
		if anode.isdecision {
			causes.insert(anode);
		} else {
			compute_causes(anode, causes);
		}
	}
}

/***************************************************
 * collect the assignments that caused the given clause to be unsatisfied
 ***************************************************/
proc compute_conflict_clause(A : Assignment, clause : Clause) : List(IGNode) {
	var node : IGNode;
	var causes = new List(IGNode);
	
	forall ltr in clause {
		node = A(ltr);
		compute_causes(node, causes);
	}
	
	return causes;
}

/***************************************************
 * do an atomic addition of a clause to the global set of clauses
 ***************************************************/
proc atomic_add_clause(clause : Clause) {
	var nc = numClss; // acquire
	
	nc = nc + 1;
	domClss = {1..nc};
	clauses(nc) = clause;
	
	if STAT then
		stats.nClauses = nc; 
	
	numClss = nc; // release
}

/***************************************************
 * add a conflict induced clause to the global set of clauses as a learned lemma 
 ***************************************************/
proc add_conflict_lemma(causes : List(IGNode)) : Clause {
	var varid : int; 
	var clause = new Clause();
	
	clause.init();
	forall node in causes {
		varid = node.varid;
		clause.domLtrs.add(varid);
		if node.value then
			clause.posneg(varid) = false;
		else
			clause.posneg(varid) = true;
	}
	
	assert(clause.numLtrs() > 0, "Adding empty clause.");
	
	// now attach the new clause to the existing clauses
	atomic_add_clause(clause);
	
	return clause;
}

/***************************************************
 * after detecting a conflict and adding the conflict induced clause, this adds the required implications to the implication graph 
 ***************************************************/
proc deduce_btvar(A : Assignment, btvar : int, clause : Clause) {
	var unit = 0;
	var unitvar = 0;
	var nodef : IGNode;
	var nodet : IGNode; 
	
	unit = clause.check_unit(A);
	assert(unit == btvar, "Clause must be unit and the unit literal must be equal to btvar.");
	
	A.assign_literal(btvar, false);
	
	unitvar = abs(btvar);
	nodet = A(unitvar);
	forall ltr in clause {
		if ltr != unitvar {
			nodef = A(ltr);
			A.make_edge(nodef, nodet);
		}
	}
	
	return unitvar;
}

/***************************************************
 * backtrack to the given decision level, this may cause a subset of the assignments to be rolled back 
 ***************************************************/
proc backtrack(A : Assignment, btlevel : int) : int {
	var node : IGNode;

	// remove all nodes after btlevel
	for i in domVars {
		node = A(i);
		if node != nil {
			if node.dlevel >= btlevel {
				assert(node.varid == i, "Check node's variable id.");
				A.unassign(node);
				verbose("Unassigned variable: " + node.varid);
			}
		}
	}
	A.reset_dlevel(btlevel); // reset the decision level
}

/***************************************************
 * analyze a unsatisfied clause and determine the conflict induced clause (cause of the conflict) and which the decision level to backtrack
 ***************************************************/
proc diagnose(A : Assignment, clause : Clause) : (int, int, List(IGNode)) {
	var ltr : int;
	var node : IGNode;
	var ispos : bool;
	var dlevel = 0;
	var causes : List(IGNode);
	var btvar : int;
	
	verbose("Computing conflict-induced clause");
	causes = compute_conflict_clause(A, clause);
	
	verbose("Detecting the decision level to backtrach");
	forall node in causes {
		assert(node != nil, "Checking nil node in diagnose");
		if node.dlevel > dlevel {
			dlevel = node.dlevel;
			
			if node.value then
				btvar = -(node.varid);
			else
				btvar = node.varid;
		}
	}	
	
	// assert(dlevel > 0, "Backtrack level invalid.");
	// assert(btvar != 0, "Backtrack variable invalid.");
	
	return (dlevel, btvar, causes);
}

/***************************************************
 * if the max number of processors has not been reached, allocate a new processor and start a new search on that  
 ***************************************************/
proc fork_search (A : Assignment, dvar : int) {
	var newA : Assignment;
	var node : IGNode;
	var newnode : IGNode;
	
	if alloc_proc() {
		newA = new Assignment();
		newA.init(A);
		
		// decide the complement of dvar
		node = A(dvar);
		if node.value == true then
			newA.reassign(dvar, false);
		else
			newA.reassign(dvar, true);
		
		verbose("Forking new search process with dvar: " + dvar);
		begin {
			search(newA, dvar);
		}
	}
}

/***************************************************
 * the main search procedure. performed by a processor starting from a decision level (0 if the main processor)
 * it terminates if it has to backtrack to a decision level from which it was forked (except the main one) 
 ***************************************************/
proc search(A : Assignment, dvar0 : int) {
	var conflict_clause : Clause;
	var conflict_induced_clause : Clause;
	var btlevel : int;
	var btvar : int;
	var done = false;
	var dvar : int;
	var causes : List(IGNode);
	var dodecide = true;
	
	verbose("Starting the search");
	
	if dvar0 != 0 then
		dvar = dvar0;
	
	while (!done) {
		
		if dodecide {
			dvar = decide(A);
			verbose("Decided the variable " + dvar);	
		}
		
		if check_sat(A) {
			done = true;
		} else {
		
			// try allocating a proc for the other decision
			if dodecide then
				fork_search(A, dvar);
			
			verbose("Assigning unit clauses and checking conflict");
			conflict_clause = deduce(dvar, A);
			verbose("Finished with unit clauses");
					
			if (conflict_clause != nil) {
				
				verbose("Detected a conflict, diagnosing");
				if DEBUG { conflict_clause.writeThis(stdout); writeln(); }
				
				// decide which level to backtrack 
				(btlevel, btvar, causes) = diagnose(A, conflict_clause);
				verbose("Level to backtrack: " + btlevel +
					 " Var to assign after backtracking: " + btvar);
					 
				// if the dlevel is less than the start level, then stop this process
				if (A.slevel == 0) || (btlevel > A.slevel) {
								
					backtrack(A, btlevel);
					
					verbose("Finished with backtracking");
					
					if btlevel > 0 {
						conflict_induced_clause = add_conflict_lemma(causes);
						verbose("Added the conflict-induced clause as a lemma:");
						if DEBUG { conflict_induced_clause.writeThis(stdout); writeln(); }
					
						dvar = deduce_btvar(A, btvar, conflict_induced_clause);
						dodecide = false;					
					} else {
						dodecide = true;
					}
				} else {
					done = true; // stop current search process
				}
			} else {
				verbose("No conflict, checking sat");
				if check_sat(A) {
					done = true;
				}
				dodecide = true;
			}
		}
	}
	verbose("Finished search.");
	free_proc();
}

/***************************************************
 * the main entry point 
 ***************************************************/
proc main() {
	var A : Assignment;
	
	if STAT then
		stats = new Statistics();
		
	timer = new Timer();
	
	parse();
	
	alloc_proc(); // allocate for this proc
	
	A = new Assignment();
	A.init();
	
	timer.start();
	
	search(A, 0);
	
	timer.stop();
	
	write("Result: ");
	if glb_sat == sat_result.SAT then
		writeln("SAT");
	else
		writeln("UNSAT");
	writeln("Model: ");
	glb_model.writeThis(stdout);
        writeln();
	
	if STAT {
		stats.writeThis(stdout);
	}
        if printTiming {
          writeln("\nElapsed time: ", timer.elapsed(), " seconds.");
        }
	
}
/***************************************************
 * reads the input file and initializes the formula 
 ***************************************************/
proc parse() {
	var line : string;
	var p : string;
	var format : string;
	var nv : int;
	var nc : int;
	var ltr : int;
	var varid : int;
	var clause : Clause;
	var c : int;
	
	var inputfile = open(FILENAME, iomode.r).reader();
	
	inputfile.read(p);
	while p != "p" {
		inputfile.readln();
		inputfile.read(p);
	}

	inputfile.readln(format, nv, nc);
	writeln("Input FileName: " + FILENAME + " NumVars: " + nv + " NumClss: " + nc); 
	
	// init the global variables
	
	domVars = {1..nv}; // domain for variables
	domClss = {1..nc}; // domain for clauses
	
	clauses = [domClss] Clause;
	
	c = 1;
	while c <= nc {
		clause = new Clause();
		clause.init();
		
		inputfile.read(ltr);
		while ltr != 0 {
			varid = abs(ltr);
			clause.domLtrs.add(varid);
			if ltr > 0 then
				clause.posneg(varid) = true;
			else
				clause.posneg(varid) = false;
			inputfile.read(ltr);
		}
		inputfile.readln();
		
		if clause.numLtrs() > 0 {
			clauses(c) = clause;
			c = c + 1;
			verbose("Read a clause: ");
			if DEBUG { clause.writeThis(stdout); writeln();}			
		}
	}
	
	inputfile.close();	
	
	// numClss is sync var, so write to it only at the end
	numVars = nv;
	numClss = nc;
	
	if STAT {
		stats.nVariables = nv;
		stats.nClauses = nc;
	}
}

/************************************************
 * Utility functions
 ************************************************/
 
/************************************************
 * if the max number of processors has not been reached, returns true and increases the current number of processors, returns false otherwise 
 ************************************************/
 proc alloc_proc () : bool {
 	var np = numProcs; // acquire
 	
 	if np < MAXPROCS {
 		np = np + 1;
 		numProcs = np; // release
 		return true;
 	} else {
 		numProcs = np; // release
 	}
 	
 	if STAT then
 		stats.nProcesses = np;
 	
 	return false;
 }
 
/************************************************
 * frees a processor and decreases the current number of processors when the search on a processor terminates  
 ************************************************/
 proc free_proc () {
 	var np = numProcs;
 	
 	np = np - 1;
 	numProcs = np;
 }

/************************************************
 * if DEBUG flag is set, prints out a message to the console 
 ************************************************/
proc verbose(message : string) {
	if DEBUG {
		writeln(message);
	}
}

/***********************************************
 * List library
 * NOTE: this library is taken from the chapel's examples directory
 ***********************************************/

config const useClockSeed = false; // Use the clock to seed the RandomStream?
config const randomSeed = 1; // If not using the clock as a seed
config const listSize = 10;  // Used when populating the list

//
// A generic linked list class
//
class List {
  type eltType;
  var head: Node(eltType);

  //
  // The Node class contains the list's values and references to the
  // following node in the list. To avoid namespace pollution and
  // increase abstraction, it is defined within the List class.
  //
  class Node {
    type eltType;
    var value: eltType;
    var next: Node(eltType);
  }

  //
  // Insert value into the front of the list
  //
  proc insert(value: eltType) {
    head = new Node(eltType, value, head);
  }

  //
  // Return true if the list has any elements that match the argument in value
  //
  proc contains(value: eltType) {
    for i in this do
      if i == value then
        return true;
    return false;
  }

  //
  // Remove the first occurrence of an element that matches the argument value
  //
  proc remove(value: eltType) {
    if head == nil then
      return;

    if head.value == value {
      head = head.next;
      return;
    }

    var current = head;
    while current.next != nil {
      if current.next.value == value {
        current.next = current.next.next;
        return;
      }
      current = current.next;
    }
  }

  // 
  // A default iterator.  Starting at head, follow next references
  // and yield the values of each node.
  //
  iter these() {
    var current = head;
    while current != nil {
      yield current.value;
      current = current.next;
    }
  }
  //
  // Define the style of the output when a list is passed to the write or
  // writeln functions. The values will be written separated by spaces.
  // 
  proc writeThis(x: Writer) throws {
    var first = true;
    for i in this do
      if first {
        x.write(i);
        first = false;
      } else {
        x.write(" " + i);
      }
  }
}
