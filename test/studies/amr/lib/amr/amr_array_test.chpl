use amr_array_defs;



def initial_condition ( x: dimension*real ) {
  var f: real = 1.0;
  for d in dimensions do
    f *= exp(-10 * (x(d) + 0.0)**2);
    return f;
}




def main {

  var hierarchy = hierarchyFromInputFile("space.txt");
  var amr_array = new AMRArray(hierarchy = hierarchy);
  var level_array: LevelArray;

  for level in hierarchy.levels {
    level_array = new LevelArray(level = level);
    level_array.setToFunction(initial_condition);
    amr_array.level_arrays(level) = level_array;
  }

  amr_array.clawOutput(0.0, 0);


}