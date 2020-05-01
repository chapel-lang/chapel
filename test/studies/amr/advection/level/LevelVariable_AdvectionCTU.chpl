public use GridVariable_AdvectionCTU;
public use LevelVariable_def;


proc LevelVariable.storeCTUOperator(
  q:        unmanaged LevelVariable,
  velocity: dimension*real,
  dt:       real)
{
  
  for grid in level.grids do
    this(grid)!.storeCTUOperator(q(grid)!, velocity, dt);
  
}
