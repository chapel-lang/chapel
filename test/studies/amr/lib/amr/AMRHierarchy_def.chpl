public use CoarseningTransfer;
public use RefiningTransfer;
public use BergerRigoutsosClustering;

private use IO;


//|\"""""""""""""""""""""""""""|\
//| >    AMRHierarchy class    | >
//|/___________________________|/

class AMRHierarchy {

  //---- Geometric descriptors ----
  
  const x_low, x_high:    dimension*real;
  const n_coarsest_cells: dimension*int;
  const n_ghost_cells:    dimension*int;
  const ref_ratio:        dimension*int;



  //---- Level indexing ----
  
  var level_indices:   domain(1) = {1..1};
  const max_n_levels:  int;



  //---- Spatial structure ----

  var levels:              [level_indices] unmanaged Level?;
  var invalid_regions:     [level_indices] unmanaged LevelInvalidRegion?;
  var cf_ghost_regions:    [level_indices] unmanaged LevelCFGhostRegion?;
  var physical_boundaries: [level_indices] unmanaged PhysicalBoundary?;



  //---- Solution information ----

  var level_solutions:    [level_indices] unmanaged LevelSolution?;
  var cf_ghost_solutions: [level_indices] unmanaged LevelCFGhostSolution?;
  var time:               real;



  //---- Regridding ----
  
  const flagger:             unmanaged Flagger;  // Flagger class is defined below  
  const target_efficiency:   real;
  const steps_before_regrid: int = 2;
  var   regrid_counters:     [level_indices] int;




  

  proc n_levels { return level_indices.high; };
    


  //|\''''''''''''''''''''|\
  //| >    initializer    | >
  //|/....................|/
  proc init (
    x_low:             dimension*real,
    x_high:            dimension*real,
    n_coarsest_cells:  dimension*int,
    n_ghost_cells:     dimension*int,
    max_n_levels:      int,
    ref_ratio:         dimension*int,
    target_efficiency: real,
    flagger:           unmanaged Flagger,
    initialCondition:  func(dimension*real, real) )
  {

    this.x_low             = x_low;
    this.x_high            = x_high;
    this.n_coarsest_cells  = n_coarsest_cells;
    this.n_ghost_cells     = n_ghost_cells;
    this.ref_ratio         = ref_ratio;
    this.max_n_levels      = max_n_levels;
    this.flagger           = flagger;
    this.target_efficiency = target_efficiency;
    this.complete();


    //---- Create the base level ----
    
    levels(1) = new unmanaged Level(x_low = x_low,  x_high = x_high,
                          n_cells       = n_coarsest_cells,
                          n_ghost_cells = n_ghost_cells);
    writeln("Adding grid to level 1.");         
    levels(1)!.addGrid(levels(1)!.possible_cells);
    levels(1)!.complete();
    physical_boundaries(1) = new unmanaged PhysicalBoundary(levels(1)!);


    //---- Create base solution ----
    
    level_solutions(1) = new unmanaged LevelSolution(levels(1)!);
    level_solutions(1)!.setToFunction(initialCondition, time);
    
    
    //===> Create refined levels and solutions as needed ===>
    
    var i_finest = 1;
    
    while i_finest < max_n_levels {
      
      writeln("Refining level ", i_finest, ".");
      const new_level = buildRefinedLevel(i_finest);
            
      if new_level.grids.numIndices>0 {
        
        //---- Extend the level indices ----
        i_finest += 1;
        level_indices = {1..i_finest};
        
        
        //---- Create new level ----
        levels(i_finest) = new_level;


        //---- Create new solution ----
        level_solutions(i_finest) = new unmanaged LevelSolution(new_level);
        level_solutions(i_finest)!.setToFunction( initialCondition, time );
        
      
        //---- Create new boundary structures ----
        createBoundaryStructures(i_finest);
      
      
        //---- Initialize the regrid counter ----
        regrid_counters(i_finest) = steps_before_regrid;
      }

      else {
        writeln("Refinement unnecessary.");
        break;
      }
      
      writeln("Done with initial refinement.");
      
    }
    //<=== Create refined levels and solutions as needed <===
    

  }
  // /|''''''''''''''''''''/|
  //< |    initializer    < |
  // \|....................\|

  //|\''''''''''''''''''''|\
  //| >   deinitializer   | >
  //|/....................|/

  proc deinit() {
    for lvl in levels do if lvl then delete lvl;
    for reg in invalid_regions do if reg then delete reg;
    for cgr in cf_ghost_regions do if cgr then delete cgr;
    for pb in physical_boundaries do if pb then delete pb;
    for lvl in level_solutions do if lvl then delete lvl;
    for cgs in cf_ghost_solutions do if cgs then delete cgs;
  }

  // /|''''''''''''''''''''/|
  //< |   deinitializer   < |
  // \|....................\|


}
// /|"""""""""""""""""""""""""""/|
//< |    AMRHierarchy class    < |
// \|___________________________\|



//|\""""""""""""""""""""""""""""|\
//| >    AMRHierarchy.regrid    | >
//|/____________________________|/

//------------------------------------------------------------------
// This method regrids the AMRHierarchy, allowing the adjustment
// of all levels finer than that with the specified index 'i_base'.
//------------------------------------------------------------------

proc AMRHierarchy.regrid ( i_base: int )
{


  writeln("regrid called from base level ", i_base, ".");

  const i_finest_old = n_levels;
  var   i_finest     = i_base;


  //===> Create new level of refinement, if possible and necessary ===>
  //----------------------------------------------------------------
  // Only one new level of refinement may be added to the hierarchy
  // during any given regridding phase.
  //----------------------------------------------------------------
  
  if i_finest_old < max_n_levels {

    writeln("Refining level ", i_finest_old, ".");


    //---- Create new level ----
    const new_level = buildRefinedLevel( i_finest_old );

  
    //==== If level is nonempty, then add it to the hierarchy ====
    
    if new_level.grids.numIndices > 0 {

      //---- Update indices ----
      i_finest      = i_finest_old+1;
      level_indices = {1..i_finest};

      
      //---- Add new level ----
      levels(i_finest) = new_level;
      


      //---- Create and fill new solution ----
      level_solutions(i_finest) = new unmanaged LevelSolution( new_level );
      level_solutions(i_finest+1)!.initialFill( level_solutions(i_finest)! );


      //---- Create new boundary structures ----
      createBoundaryStructures( i_finest );
      
      
      //---- Set regrid counter ----
      regrid_counters(i_finest) = steps_before_regrid;    
      
      writeln("Level ", i_finest, " created!");  
    }
    else
      writeln("Refinement unnecessary.");

  }
  //<=== Create new level of refinement, if possible and necessary <===
  
  
  
  //===> Regrid existing levels ===>
  //------------------------------------------------------------------
  // Regridding starts at the old finest level.  Levels are regridded
  // one at a time, proceeding up the hierarchy, until level i_base+1
  // is regridded.
  //
  // Boundary structures are created at the very end of the regridding
  // process for all levels.  They could be created once each coarse/
  // fine pair of levels is finished, but it's easier just to handle
  // them all at the end.
  //------------------------------------------------------------------
  
  var i_regridding = i_finest_old;

  while i_regridding > i_base {

    writeln("Regridding level ", i_regridding, ".");

    
    //---- Remove old boundary structures ----
    deleteBoundaryStructures(i_regridding);
    

    
    //---- Create new level ----
    const regridded_level = buildRefinedLevel(i_regridding-1);
    writeln("Built regridded level.");


    //==== If regridded level is nonempty, add to the hierarchy ====

    if regridded_level.grids.numIndices > 0 {
      
      //---- Update the finest level index ----
      
      i_finest = max(i_finest, i_regridding);
            
            
      //---- Create solution on regridded level ----

      var regridded_level_solution = new unmanaged LevelSolution(regridded_level);
      regridded_level_solution.initialFill( level_solutions(i_regridding)!, 
                                            level_solutions(i_regridding-1)! );

      //---- Replace the old LevelSolution ----

      delete level_solutions(i_regridding);
      level_solutions(i_regridding) = regridded_level_solution;

      //---- Replace the level ----

      delete levels(i_regridding);
      levels(i_regridding) = regridded_level;

      
      //---- Set regrid counter ----
      regrid_counters(i_regridding) = steps_before_regrid;
      
      writeln("Level ", i_regridding, " successfully regridded.");
    }
    
    //==== Otherwise, delete the old level's data, and shrink the hierarchy ====
    
    else {

      //---- Clear old solution ----

      delete level_solutions(i_regridding);
      
      
      //---- Delete the old level ----

      delete levels(i_regridding);
      
      
      //---- Shrink the index space ----
      //--------------------------------------------------------------
      // Need to do this now rather than one shrinking operation at
      // the end, as buildRefinedLevel depends on each element of the
      // index space corresponding to an actual level.
      //--------------------------------------------------------------
      level_indices = {1..i_regridding-1};
      
      writeln("Level ", i_regridding, "no longer needed -- discarded.");
    }
    
    //==== Shift the regridding index up ====
    i_regridding -= 1;
  }
  //<=== Regrid existing levels <===
  
  
  //---- Create new boundary structures ----
  
  for i in i_base+1..i_finest do createBoundaryStructures(i);
 
   
}
// /|""""""""""""""""""""""""""""/|
//< |    AMRHierarchy.regrid    < |
// \|____________________________\|




//|\"""""""""""""""""""""""""""""""""""""""|\
//| >    AMRHierarchy.buildRefinedLevel    | >
//|/_______________________________________|/
//
//--------------------------------------------------------
// Builds a refined level below level i_refining.
//--------------------------------------------------------

proc AMRHierarchy.buildRefinedLevel ( i_refining: int )
{
  
  proc buffer(flags: [?cells] bool) {
    var buffered_flags: [cells] bool;

    for cell in cells do
      if flags(cell) {
        var ranges: dimension*range(stridable=true);
        for d in dimensions do ranges(d) = cell(d)-2 .. cell(d)+2 by 2;
        var neighborhood: domain(dimension,stridable=true) = ranges;
        for nbr in cells(neighborhood) do
          buffered_flags(nbr) = true;
      }
          
    return buffered_flags;
  }
  
  
  
  //---- Flag the level being refined ----
  
  const coarse_level = levels(i_refining)!;
  var flags: [coarse_level.possible_cells] bool;
  flagger.setFlags(level_solutions(i_refining)!, flags);
  
  
  //---- Add flags for the level below the new one, if needed ----

  if i_refining+2 <= n_levels 
  {    
    for super_fine_grid in levels(i_refining+2)!.grids {
      var cells_to_flag = coarsen( coarsen(super_fine_grid.cells, ref_ratio), ref_ratio);
      flags(cells_to_flag) = true;
    }
  }
  
  
  //---- Add buffer region ----

  var buffered_flags = buffer(flags);
  
    
  //---- Cluster ----

  var min_width: dimension*int;
  min_width = min_width + 2;
  var cluster_domains = clusterFlags( buffered_flags, target_efficiency, min_width );
  
    
  
  //===> Ensure proper nesting ===>

  var domains_to_refine = new unmanaged List( domain(dimension,stridable=true) );  
  
  
  //---- Form exterior of coarse level ----
  
  var coarse_exterior = new unmanaged MultiDomain(dimension,stridable=true);
  coarse_exterior.add( coarse_level.possible_cells );
  
  for grid in coarse_level.grids do
    coarse_exterior.subtract( grid.cells );
    
    
    
  for D in cluster_domains
  {
    
    //---- Remove any portion of D within 1 cell of coarse_exterior ----
    
    var properly_nested_domains = new unmanaged MultiDomain(dimension,stridable=true);
    properly_nested_domains.add(D);

    for exterior_D in coarse_exterior do 
      properly_nested_domains.subtract( exterior_D.expand(2) );
    
    
    
    //---- Add results to the list of domains to refine ----
    
    for E in properly_nested_domains do
      domains_to_refine.add( E );
      
      
    //---- Clean up ----
      
    delete properly_nested_domains;
    
  }

  delete coarse_exterior;
  delete cluster_domains;

  //<=== Ensure proper nesting <===
  
  
  
  //===> Create new level, and return ===>
  
  var new_level = new unmanaged Level(x_low   = this.x_low,
                            x_high  = this.x_high,
                            n_cells = levels(i_refining)!.n_cells * ref_ratio,
                            n_ghost_cells = this.n_ghost_cells);
                            
  for domain_to_refine in domains_to_refine do
    new_level.addGrid( refine(domain_to_refine, ref_ratio) );
  
  
  delete domains_to_refine;
  
  
  new_level.complete();
  

  return new_level;
  
  //<=== Create new level, and return <===
  
  
}
// /|"""""""""""""""""""""""""""""""""""""""/|
//< |    AMRHierarchy.buildRefinedLevel    < |
// \|_______________________________________\|




//|\""""""""""""""""""""""""""""""""""""""""""""""""""""""|\
//| >    method: AMRHierarchy.createBoundaryStructures    | >
//|/______________________________________________________|/
//
//--------------------------------------------------------------------------
// This method creates the boundary structures associated with the addition
// of a level at index 'i'.  This includes the LevelInvalidRegion for the
// level at i-1, and the LevelCFGhostRegion, LevelCFGhostSolution, and
// PhysicalBoundary for level i.
//--------------------------------------------------------------------------

proc AMRHierarchy.createBoundaryStructures( i: int )
{

  invalid_regions(i-1)   = new unmanaged LevelInvalidRegion( levels(i-1)!, levels(i)! );
  
  cf_ghost_regions(i)    = new unmanaged LevelCFGhostRegion( levels(i)!, levels(i-1)! );

  cf_ghost_solutions(i)  = new unmanaged LevelCFGhostSolution( cf_ghost_regions(i)!, levels(i)! );

  physical_boundaries(i) = new unmanaged PhysicalBoundary( levels(i)! );
  
  
}

// /|""""""""""""""""""""""""""""""""""""""""""""""""""""""/|
//< |    method: AMRHierarchy.createBoundaryStructures    < |
// \|______________________________________________________\|





//|\""""""""""""""""""""""""""""""""""""""""""""""""""""""|\
//| >    method: AMRHierarchy.deleteBoundaryStructures    | >
//|/______________________________________________________|/
//
//-----------------------------------------------------------------------
// This method clears and deletes all boundary structures associated
// with fine level 'i'.  Meant to be called in conjunction with removing
// level i during a regrid.
//-----------------------------------------------------------------------

proc AMRHierarchy.deleteBoundaryStructures( i: int )
{
  
  delete invalid_regions(i-1);
  
  delete cf_ghost_regions(i);
  
  delete cf_ghost_solutions(i);
  
  delete physical_boundaries(i);

}
// /|""""""""""""""""""""""""""""""""""""""""""""""""""""""/|
//< |    method: AMRHierarchy.deleteBoundaryStructures    < |
// \|______________________________________________________\|





//|\"""""""""""""""""""""""""""""""|\
//| >    PhysicalBoundary class    | >
//|/_______________________________|/
//
//--------------------------------------------------------------
// Describes the physical boundary of a level, situated in
// the rectangular (mathematical) domain of the full hierarchy.
//--------------------------------------------------------------

class PhysicalBoundary
{

  const grids:        domain(unmanaged Grid);
  const multidomains: [grids] unmanaged MultiDomain(dimension,stridable=true)?;



  //|\''''''''''''''''''''|\
  //| >    initializer    | >
  //|/....................|/
  
  proc init ( level: unmanaged Level ) 
  {
    this.complete();
    for grid in level.grids {

      var boundary_multidomain = new unmanaged MultiDomain(dimension,stridable=true);

      for D in grid.ghost_domains do boundary_multidomain.add( D );

      boundary_multidomain.subtract(level.possible_cells);

      if !boundary_multidomain.isEmpty() {
        grids.add(grid);
        multidomains(grid) = boundary_multidomain;
      } 
    } // end for grid in level.grids

  }
  // /|''''''''''''''''''''/|
  //< |    initializer    < |
  // \|....................\|



  //|\'''''''''''''''''''|\
  //| >  deinitializer   | >
  //|/...................|/
  
  proc deinit () 
  {
    for multidomain in multidomains do delete multidomain;
  }
  // /|'''''''''''''''''''/|
  //< |  deinitializer   < |
  // \|...................\|


  
  //|\''''''''''''''''''''''''''''''|\
  //| >    special method: these    | >
  //|/..............................|/
  
  iter these() 
  {
    for grid in grids do
      yield (grid, multidomains(grid));
  }
  // /|''''''''''''''''''''''''''''''/|
  //< |    special method: these    < |
  // \|..............................\|
  
}
// /|"""""""""""""""""""""""""""""""/|
//< |    PhysicalBoundary class    < |
// \|_______________________________\|




//|\""""""""""""""""""""""|\
//| >    Flagger class    | >
//|/______________________|/

//----------------------------------------------------------
// Container for the setFlags routine.  This class is meant
// to be derived, and the setFlags routine overridden with
// something user-specified.
//----------------------------------------------------------

class Flagger {
  proc setFlags( level_solution: unmanaged LevelSolution, 
                 flags: [level_solution.level.possible_cells] bool ) {}
}
// /|""""""""""""""""""""""/|
//< |    Flagger class    < |
// \|______________________\|



//|\"""""""""""""""""""""""""""""""""""""""""""""|\
//| >    AMRHierarchy initializer, file-based    | >
//|/_____________________________________________|/
//
//-----------------------------------------------------------------
// Alternate initializer in which all numerical parameters for the
// hierarchy are provided using an input file.  This allows those
// parameters to be changed without recompiling the code.
//-----------------------------------------------------------------

proc AMRHierarchy.init (
  file_name:  string,
  flagger:    unmanaged Flagger,
  inputIC:    func(dimension*real,real))
{

  const parameter_file = open(file_name, iomode.r).reader();

  //==== Safety check the dimension ====
  var dim_input: int;
  parameter_file.readln(dim_input);
  assert(dim_input == dimension,
         "error: dimension of input file inconsistent with compiled dimension.");
  parameter_file.readln(); // skip line


  //==== Read parameters from file ====
  var x_low, x_high:     dimension*real;
  var n_coarsest_cells:  dimension*int;
  var n_ghost_cells:     dimension*int;
  var max_n_levels:      int;
  var ref_ratio:         dimension*int;
  var target_efficiency: real;

  parameter_file.readln( (...x_low) );
  parameter_file.readln( (...x_high) );
  parameter_file.readln( (...n_coarsest_cells) );
  parameter_file.readln( (...n_ghost_cells) );
  parameter_file.readln( (...ref_ratio) );
  parameter_file.readln( max_n_levels );
  parameter_file.readln( target_efficiency );
  parameter_file.close();

  //==== Create and return hierarchy ====
  this.init(
            x_low,
            x_high,
            n_coarsest_cells,
            n_ghost_cells,
            max_n_levels,
            ref_ratio,
            target_efficiency,
            flagger,
            inputIC);

}
// /|"""""""""""""""""""""""""""""""""""""""""""""/|
//< |    AMRHierarchy initializer, file-based    < |
// \|_____________________________________________\|





//|\""""""""""""""""""""""""""""""""""|\
//| >    LevelSolution.initialFill    | >
//|/__________________________________|/
//
//-----------------------------------------------------
// This method fills a recently-created LevelSolution.
//-----------------------------------------------------

//----------------------------------------------------------------------
// This version copies data from 'old_solution' of the same refinement, 
// and  interpolatesfrom 'coarse_solution' at the next-coarsest level 
// where possible.
//----------------------------------------------------------------------

proc LevelSolution.initialFill (
  old_solution:    unmanaged LevelSolution,
  coarse_solution: unmanaged LevelSolution )
{
  assert( abs(old_solution.current_time - coarse_solution.current_time) < 1.0e-8);
  
  current_data.initialFill(old_solution.current_data, coarse_solution.current_data);
  current_time = old_solution.current_time;
  old_time     = current_time;
}



//--------------------------------------------------------------------------
// This version purely interpolates data from 'coarse_solution'.
//--------------------------------------------------------------------------

proc LevelSolution.initialFill ( coarse_solution: unmanaged LevelSolution )
{
  current_data.initialFill( coarse_solution.current_data );
  current_time = coarse_solution.current_time;
  old_time     = current_time;
}
// /|""""""""""""""""""""""""""""""""""/|
//< |    LevelSolution.initialFill    < |
// \|__________________________________\|



//|\""""""""""""""""""""""""""""""""""|\
//| >    LevelVariable.initialFill    | >
//|/__________________________________|/
//
//----------------------------------------------------------------
// This method fills a freshly-created LevelVariable.
//
// The input 'q_old' is a LevelVariable at the same layer of
// refinement,  from which data will be copied if possible.  This 
// input may be nil if no such LevelVariable is available.
//
// In remaining locations, data is obtained by interpolation from
// thee input 'q_coarse', a LevelVariable at the next-coarsest
// level.
//----------------------------------------------------------------

proc LevelVariable.initialFill (
  q_old:     unmanaged LevelVariable?,
  q_coarse:  unmanaged LevelVariable)
{

  //---- Safety check ----

  if q_old != nil then assert(this.level.n_cells == q_old!.level.n_cells);

  
  //---- Refinement ratio ----
  
  const ref_ratio = refinementRatio(q_coarse.level, this.level);

  
  //===> Fill grid-by-grid ===>

  for grid in level.grids {

    //---- Initialize structure of unfilled cell blocks ----
    
    var unfilled_region = new unmanaged MultiDomain(dimension, true);
    unfilled_region.add( grid.cells );


    //===> Copy from q_old where possible ===>
    //-------------------------------------------------------
    // Possible speedup if old level's grid cells are stored
    // as a MultiDomain. #inefficiency
    //-------------------------------------------------------

    if q_old != nil
    {

      for old_grid in q_old!.level.grids
      {
      
        var overlap = grid.cells( old_grid.cells );

        if overlap.numIndices > 0 
        {
          this(grid,overlap) = q_old!(old_grid, overlap);
          unfilled_region.subtract( overlap );
        }
        
      }
      
    }

    //<=== Copy from q_old where possible <===

  
    //===> Interpolate from q_coarse everywhere else ===>
    //------------------------------------------------
    // This looks really inefficient... #inefficiency
    //------------------------------------------------------------------
    // Yep, the part that begins 'for block in unfilled_region'
    // is exactly the kind of comprehensive iteration I've been trying
    // to avoid.  Ideally, I'd like a combined intersect/subtract
    // operation on MultiDomains.  Can I make sense out of that?
    //------------------------------------------------------------------

    for coarse_grid in q_coarse.level.grids {
      
      //---- Calculate full overlap on the fine grid ----
      
      var refined_coarse = grid.cells( refine(coarse_grid.cells, ref_ratio) );
      
      
      //----If there is an overlap, remove fragments that coincide with unfilled_region ----
      
      if refined_coarse.numIndices > 0 
      {

       {
        var unfilled_intersection = unfilled_region.copy();
        unfilled_intersection.intersect( refined_coarse );
        
        for D in unfilled_intersection do
          this(grid,D) = q_coarse(coarse_grid)!.refineValues(D, ref_ratio);
        
        delete unfilled_intersection;
       }
        
        unfilled_region.subtract( refined_coarse );
        if unfilled_region.isEmpty() then break;
        
        
        // for block in unfilled_region {
        // 
        //   var unfilled_overlap = refined_coarse(block);
        // 
        //   if unfilled_overlap.numIndices > 0 then
        //     this(grid,unfilled_overlap) 
        //         = q_coarse(coarse_grid).refineValues( unfilled_overlap, ref_ratio );
        // }
        // 
        // unfilled_region.subtract(refined_coarse);
      }
      
    }
    
    delete unfilled_region;
    
    //<=== Interpolate from q_coarse everywhere else <===

  }
  //<=== Fill grid-by-grd <===
}



//----------------------------------------------------------------------
// This version of the method is for use when there is no LevelVariable
// available at the same layer of refinement as the LevelVariable being
// filled.  This simply passes q_old as 'nil' to the version of
// LevelVariable.initialFill defined above
//----------------------------------------------------------------------

proc LevelVariable.initialFill ( q_coarse: unmanaged LevelVariable )
{
  const q_old: unmanaged LevelVariable?;
  initialFill( q_old, q_coarse );
}
// /|""""""""""""""""""""""""""""""""""/|
//< |    LevelVariable.initialFill    < |
// \|__________________________________\|







//|\""""""""""""""""""""""""""""""""""""|\
//| >    AMRHierarchy output methods    | >
//|/____________________________________|/

//------------------------------------------------------------------------
// Writes full Clawpack output at a given time.  Most of the work is done
// by the "write" method, which handles output of the spatial data.
//------------------------------------------------------------------------

proc AMRHierarchy.clawOutput(frame_number: int)
{

  //---- Names of output files ----
  
  var frame_string:       string = "%04i".format(frame_number);
  var time_file_name:     string = "_output/fort.t" + frame_string;
  var solution_file_name: string = "_output/fort.q" + frame_string;


  //---- Time file ----

  var n_grids: int = 0;
  for level in levels do n_grids += level!.grids.numIndices;

  const time_file = open(time_file_name, iomode.cw).writer();
  writeTimeFile(time, 1, n_grids, 1, time_file);
  time_file.close();


  //---- Solution file ----
  
  const solution_file = open(solution_file_name, iomode.cw).writer();
  this.writeData(solution_file);
  solution_file.close();

}



//----------------------------------------------------------------
// Proceeds down the indexed_levels, calling the LevelVariable.write
// method on each corresponding LevelVariable.
//----------------------------------------------------------------
proc AMRHierarchy.writeData(outfile: channel){

  var base_grid_number = 1;

  for i in level_indices {
    level_solutions(i)!.current_data.writeData(i, base_grid_number, outfile);
    base_grid_number += levels(i)!.grids.numIndices;
  }

}
// /|""""""""""""""""""""""""""""""""""""/|
//< |    AMRHierarchy output methods    < |
// \|____________________________________\|










proc main {
  
  //===> Flagger definition ===>
  class GradientFlagger: Flagger {
    
    const tolerance: real = 0.05;
    
    proc setFlags(
      level_solution: unmanaged LevelSolution, 
      flags:          [level_solution.level.possible_cells] bool)
    {
      const current_data = level_solution.current_data;
      current_data.extrapolateGhostData();
      
      for grid in level_solution.level.grids {
        for cell in grid.cells {
          var max_differential, differential: real;
          for nbr in neighbors(cell) {
            differential = abs(current_data(grid).value(cell) - current_data(grid).value(nbr));
            max_differential = max(differential, max_differential);
          }
          
          // writeln("cell: ", cell, ";  max_differential = ", max_differential);
          
          if max_differential > tolerance {
            flags(cell) = true;
            // writeln("Adding flag");
          }
        }
      }
      
    }
    
    iter neighbors(cell: dimension*int) {
      var shift: dimension*int;
      for d in dimensions {
        shift(d) = -2;
        yield cell+shift;
        shift(d) = 2;
        yield cell+shift;
        shift(d) = 0;
      }
    }
  }
  //<=== Flagger definition <===
  
  
  
  const x_low  = (-1.0,-1.0);
  const x_high = (1.0,1.0);
  const n_coarsest_cells = (40,40);
  const n_ghost_cells = (2,2);
  const max_n_levels = 4;
  const ref_ratio = (2,2);
  
  proc elevatedSquare (x: 2*real)
  {
    var f: real = 0.0;
    if x(1)<-0.6 && (x(2)>-0.8 && x(2)<-0.2) then f = 1.0;
    return f;
  }
  
  const flagger = new unmanaged GradientFlagger(tolerance = 0.1);
  const target_efficiency = 0.7;
  
  
  const hierarchy = new unmanaged AMRHierarchy(x_low,
                                     x_high,
                                     n_coarsest_cells,
                                     n_ghost_cells,
                                     max_n_levels,
                                     ref_ratio,
				     target_efficiency,
				     flagger,
				     elevatedSquare);
  
  hierarchy.clawOutput(0);

  delete hierarchy;
}
