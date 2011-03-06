use GridVariable_AdvectionCTU;
use LevelVariable_def;


def LevelVariable.storeCTUOperator(
  q:        LevelVariable,
  velocity: dimension*real,
  dt:       real)
{
  
  for grid in level.grids do
    this(grid).storeCTUOperator(q(grid), velocity, dt);
  
}