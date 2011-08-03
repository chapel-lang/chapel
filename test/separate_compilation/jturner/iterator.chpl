use ChapelBase;
use ChapelIO;
use ChapelArray;
use DefaultRectangular;
use DefaultAssociative;

pragma "export" iter dothis(n:int) : int {
  for i in 1..n do
    yield i * i;
}
