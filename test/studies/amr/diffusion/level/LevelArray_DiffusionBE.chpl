use GridArray_DiffusionBE;
use LevelArray_def;
use LevelBC_def;


//|""""""""""""""""""""""""""""""""""\
//|===> LevelArray.storeCGSolution ===>
//|__________________________________/
def LevelArray.storeCGSolution(
  rhs: LevelArray,
  bc:  LevelBC,
  diffusivity: real,
  dt: real,
  tolerance: real)
{
  
  //==== Initialize residual ====
  var residual = new LevelArray(level = level);
  bc.apply_Homogeneous(this);
  residual.storeBEOperator(this, diffusivity, dt);
  for grid in level.grids do
    residual(grid) = rhs(grid) - residual(grid);			       


  //==== Initialize search direction ====
  var search_dir = new LevelArray(level = level);
  for grid in level.grids do
    search_dir(grid,grid.cells) = residual(grid,grid.cells);
    //    search_dir(grid).value(grid.cells) = residual(grid).value(grid.cells);


  //==== Initialize residual update direction ====
  var residual_update = new LevelArray(level = level);
  bc.apply_Homogeneous(search_dir);
  residual_update.storeBEOperator(search_dir, diffusivity, dt);


  //==== Initialize scalars ====
  var grid_res_norm_squares: [level.grids] real;
  var level_res_norm_square: real;

  for grid in level.grids do
    grid_res_norm_squares(grid) = +reduce( residual(grid,grid.cells)**2 );
  level_res_norm_square = +reduce(grid_res_norm_squares);

  var grid_inner_products:       [level.grids] real;
  var alpha, beta:               real;
  var old_level_res_norm_square: real;
  const max_iterations: int = 100;


  //::::::::::::::::::::\
  //===> CG iteration ===>
  //::::::::::::::::::::/
  for iter in 1..max_iterations {

    //===> Update solution and residual ===>
    for grid in level.grids do
      grid_inner_products(grid) = 
        +reduce( residual_update(grid,grid.cells) * search_dir(grid,grid.cells) );

    alpha = +reduce(grid_inner_products);
    alpha = level_res_norm_square / alpha;

    for grid in level.grids {
      this(grid) += alpha * search_dir(grid);
      residual(grid) -= alpha * residual_update(grid);
      grid_res_norm_squares(grid) = +reduce( residual(grid).value(grid.cells)**2 );
    }
    //<=== Update solution and residual <===


    //==== Update residual norm, and check for convergence ====
    old_level_res_norm_square = level_res_norm_square;
    level_res_norm_square     = +reduce(grid_res_norm_squares);
    writeln("Iteration ", iter, ": residual_norm = ", sqrt(level_res_norm_square));
    if sqrt(level_res_norm_square) < tolerance then break;
    if iter == max_iterations then 
      writeln("Warning: conjugate gradient method failed to converge.");


    //==== Update directions for search and residual update ====
    beta = level_res_norm_square / old_level_res_norm_square;
    for grid in level.grids do
      search_dir(grid,grid.cells) = residual(grid,grid.cells) + beta*search_dir(grid,grid.cells);


    bc.apply_Homogeneous(search_dir);
    residual_update.storeBEOperator(search_dir, diffusivity, dt);

  }
  // /::::::::::::::::::::/
  //<=== CG iteration <===
  // \::::::::::::::::::::\
  
}
// /""""""""""""""""""""""""""""""""""/
//<=== LevelArray.storeCGSolution <==<
// \__________________________________\




//|""""""""""""""""""""""""""""""""""\
//|===> LevelArray.storeBEOperator ===>
//|__________________________________/
def LevelArray.storeBEOperator(
  q:           LevelArray,
  diffusivity: real,
  dt:          real)
{
  
  storeFluxDivergence(q, diffusivity);

  for grid in level.grids {
    this(grid,grid.cells) = q(grid,grid.cells) + dt * this(grid,grid.cells);
  }  

}
// /""""""""""""""""""""""""""""""""""/
//<=== LevelArray.storeBEOperator <==<
// \__________________________________\



def LevelArray.storeFluxDivergence(
  q:           LevelArray,
  diffusivity: real)
{
  q.fillOverlapRegions();
  
  for grid in level.grids do
    this(grid).storeFluxDivergence(q(grid), diffusivity);
}