use GridArray_advection;
use LevelArray_def;


def LevelArray.storeCTUOperator(
  q:        LevelArray,
  velocity: dimension*real,
  dt:       real)
{
  
  for grid in level.grids do
    this(grid).storeCTUOperator(q(grid), velocity, dt);
  
}