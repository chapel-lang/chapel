record FScoreComparator {
  var fScore :  borrowed [..] real;
  proc init(fScore : borrowed [..] real) {
    this.fScore = fScore;
  }
}

proc FScoreComparator.key(i) {
  var D = this.fScore.domain;
  if D.contains(i) then
    return this.fScore[i];
  else
    return max(real);
}
