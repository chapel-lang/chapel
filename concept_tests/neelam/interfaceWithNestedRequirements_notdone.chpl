interface LinearTransformation(Mat){
	type Vec;
	type eltType;
	Vec implements VectorSpace;
	Vec.eltType == eltType;
	proc *(A : Mat, x:Vec):Vec;
	proc *(alpha : eltType, A: Mat):Mat;
}
