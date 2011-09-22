// ChapelRange.chpl
//

use ChapelRangeBase;

//
// range type
//
//   parameterized by:
//        1) an integral element type,
//        2) whether low and/or high bounds exist, and
//        3) whether the stride is one or not.
//
// The type of the stride is a signed type with the same number of bits as the
// index type.  This means that for an unsigned index type, a maximal range can
// be reached in no less than two strides.
//
// The _aligned flag is used to track whether a range has a non-ambiguous alignment.
// All range literals have ambiguous alignment, but since they have a stride of 1
// their isAmbiguous() is false.
// The _aligned bit can also be set through the application of an explicit alignment.
//

pragma "range"
record range
{
  type idxType = int;                            // element type
  param boundedType: BoundedRangeType = BoundedRangeType.bounded; // bounded or not
  param stridable: bool = false;                 // range can be strided

  var _base : rangeBase(idxType, boundedType, stridable);
  var _aligned : bool = false;

  // TODO: hilde 2011/03/31
  // This should be a pragma and not a var declaration.  
  var _promotionType: idxType;                   // enables promotion
  proc strType type return chpl__signedType(idxType);
  
  pragma "inline" proc low return _base.low;       // public getter for low bound
  pragma "inline" proc high return _base.high;     // public getter for high bound
  pragma "inline" proc stride where stridable return _base.stride; // public getter for stride
  proc stride param where !stridable return 1 : strType;
  // public getter for alignment
  pragma "inline" proc alignment return _base.alignment;
  // public getter for the ambiguous alignment flag.
  pragma "inline" proc aligned return _aligned;

  // Returns the starting index in the sequence.
  pragma "inline" proc first return _base.first;
  // Returns the ending index.
  pragma "inline" proc last return _base.last;
  // Returns the low index, properly aligned.
  pragma "inline" proc alignedLow : idxType return _base.alignedLow;
  // Returns the high index, properly aligned.
  pragma "inline" proc alignedHigh : idxType return _base.alignedHigh;
  // Returns the number of elements in this range.
  pragma "inline" proc length return _base.length;
}

//################################################################################
//# Constructors
//#

// Declare this as constructor, so we can capture range creation.
// If it is not a constructor, then the user can still create a maximal range
// (for example) without being warned.
//
proc range.range(type idxType = int,
                 param boundedType : BoundedRangeType = BoundedRangeType.bounded,
                 param stridable : bool = false,
                 _low : idxType = 1,
                 _high : idxType = 0,
                 _stride : chpl__signedType(idxType) = 1,
                 _alignment : idxType = 0,
                 _aligned : bool = false)
{
  _base = new rangeBase(idxType, boundedType, stridable, _low, _high, _stride, _alignment);
  this._aligned = _aligned;
}

proc range.range(type idxType = int,
                 param boundedType = BoundedRangeType.bounded,
                 param stridable : bool = false,
                 _base : rangeBase(idxType, boundedType, stridable),
                 _aligned : bool = false)
{
  this._base = _base;
  this._aligned = _aligned;
}

/////////////////////////////////
// for debugging

proc range.displayRepresentation(msg: string = ""): void {
  writeln(msg, "(", typeToString(idxType), ",", boundedType, ",", stridable,
          " : ", low, ",", high, ",", stride, ",",
          if aligned then alignment:string else "?", ")");
}
 

//////////////////////////////////////////////////////////////////////////////////
// Range builders for bounded ranges
// Range builders are used by the parser to create literal ranges.
// hilde sez: I think that these need to be spelled out explicitly
// because param loop unrolling is performed before generic expansion.
//
proc _build_range(low: int, high: int)
  return new range(idxType = int, _low = low, _high = high);
proc _build_range(low: uint, high: uint)
  return new range(uint, _low = low, _high = high);
proc _build_range(low: int(64), high: int(64))
  return new range(int(64), _low = low, _high = high);
proc _build_range(low: uint(64), high: uint(64))
  return new range(uint(64), _low = low, _high = high);
proc _build_range(low, high) {
  compilerError("Bounds of '..' must be integers of compatible types, when specified.");
}


//////////////////////////////////////////////////////////////////////////////////
// Range builders for unbounded ranges
// hilde sez: Ditto here: Generic types don't work.
//
proc _build_range(param bt: BoundedRangeType, bound: int)
  return new range(int, bt, false, bound, bound);
proc _build_range(param bt: BoundedRangeType, bound: uint)
  return new range(uint, bt, false, bound, bound);
proc _build_range(param bt: BoundedRangeType, bound: int(64))
  return new range(int(64), bt, false, bound, bound);
proc _build_range(param bt: BoundedRangeType, bound: uint(64))
  return new range(uint(64), bt, false, bound, bound);
proc _build_range(param bt: BoundedRangeType)
  return new range(int, bt);


//################################################################################
//# Predicates
//#

// isBoundedRange(r) = true if 'r' is a (fully) bounded range

proc isBoundedRange(r)           param
  return false;

proc isBoundedRange(r: range(?)) param
  return r.hasLowBound() && r.hasHighBound();

proc isBoundedRange(param B: BoundedRangeType) param
  return B == BoundedRangeType.bounded;

// Returns true if this range has a low bound.
proc range.hasLowBound() param
  return boundedType == BoundedRangeType.bounded ||
         boundedType == BoundedRangeType.boundedLow;

// Returns true if this range has a high bound.
proc range.hasHighBound() param
  return boundedType == BoundedRangeType.bounded ||
         boundedType == BoundedRangeType.boundedHigh;

// If the represented sequence is defined, reports whether it is empty.
// If it is not defined but one or both bounds is infinity, returns false.
// Otherwise the behavior is undefined.
inline
proc range.isEmpty()       where isBoundedRange(this) {
  if isAmbiguous() then
    halt("isEmpty() is invoked on an ambiguously-aligned range");
  else
    return this.alignedLow > this.alignedHigh;
}
proc range.isEmpty() param where !isBoundedRange(this)
  return false;

proc range.hasFirst() param where !stridable && !hasHighBound()
  return hasLowBound();

pragma "inline" 
proc range.hasFirst()
  return if isAmbiguous() then false else _base.hasFirst();
    
proc range.hasLast() param where !stridable && !hasLowBound()
  return hasHighBound();

pragma "inline"
proc range.hasLast()
  return if isAmbiguous() then false else _base.hasLast();

// Returns true if this range is naturally aligned, false otherwise.
// Note that this does not indicate if the range is ambiguously aligned.
pragma "inline"
proc range.isNaturallyAligned()
  return _base.isNaturallyAligned();

// Returns true if the range is ambiguously aligned.
proc range.isAmbiguous() param where !stridable
  return false;
proc range.isAmbiguous()       where stridable
  return !aligned && (stride > 1 || stride < -1);

// Returns true if i is in this range.
pragma "inline" proc range.member(i: idxType) 
{
  if this.isAmbiguous() then return false;
  return _base.member(i);
}

// Returns true if the other range is contained within this one.
pragma "inline" proc range.member(other: range(?)) 
{
  if this.isAmbiguous() || other.isAmbiguous() then return false;
  return _base.member(other);
}

// Returns true if the two ranges have the same represented sequence, or if
// both sequences are undefined but all four primary properties are identical.
proc ==(r1: range(?), r2: range(?)) param
  where r1.boundedType != r2.boundedType
return false;

proc ==(r1: range(?), r2: range(?)): bool
  where r1.boundedType == r2.boundedType
{
  // An ambiguous ranges cannot equal an unambiguous one
  //  even if all their parameters match.
  if r1.isAmbiguous() != r2.isAmbiguous() then return false;

  // As a special case, two ambiguous ranges compare equal 
  // if their representations are identical.
  if r1.isAmbiguous() then return ident(r1, r2);

  return r1._base == r2._base;
}

proc !=(r1: range(?), r2: range(?))  return !(r1 == r2);


// This makes use of the fact that the default values for the
// stride and alignment fields are consistent, whether the two ranges
// being compared are stridable or not.
proc ident(r1: range(?), r2: range(?))
  where r1.idxType == r2.idxType &&
  r1.boundedType == r2.boundedType &&
  r1.stridable == r2.stridable
{
  if r1._aligned != r2._aligned then return false;
  return ident(r1._base, r2._base);
}

// If the parameters don't match, then the two ranges cannot be identical.
proc ident(r1: range(?), r2: range(?)) param
  return false;


//////////////////////////////////////////////////////////////////////////////////
// Bounds checking
//
// returns true if other is in bounds of this for all specified
// bounds; these functions are used to determine if an array slice is
// valid.  We break out the boundedNone case in order to permit
// unbounded ranges to slice ranges of various index types -- otherwise
// we get a compiler error in the boundsCheck function.
//

pragma "inline"
proc range.boundsCheck(other: range(?e,?b,?s))
  where b == BoundedRangeType.boundedNone
{
  if this.isAmbiguous() || other.isAmbiguous()
    then return false;

  return true;
}

pragma "inline"
proc range.boundsCheck(other: range(?e,?b,?s))
{
  if this.isAmbiguous() || other.isAmbiguous()
    then return false;

  return _base.boundsCheck(other._base);
}

pragma "inline"
proc range.boundsCheck(other: idxType)
  return member(other);


//################################################################################
//# Member functions
//#

// Moves the low bound of the range up to the next alignment point.
proc range.alignLow() 
{
  if this.isAmbiguous() then
    __primitive("chpl_error", "alignLow -- Canot be applied to a range with ambiguous alignment.");

  _base.alignLow();
  return this;
}

// Moves the high bound of the range down to the next alignment point.
proc range.alignHigh()
{
  if this.isAmbiguous() then
    __primitive("chpl_error", "alignHigh -- Canot be applied to a range with ambiguous alignment.");

  _base.alignHigh();
  return this;
}

// Returns the ordinal of 'i' within this range's represented sequence.
proc range.indexOrder(i: idxType)
{
  if this.isAmbiguous() then
    __primitive("chpl_error", "indexOrder -- Undefined on a range with ambiguous alignment.");

  return _base.indexOrder(i);
}

// Opposite of indexOrder: returns the ord-th element of this range's
// represented sequence.
proc range.orderToIndex(ord: integral): idxType
{
  if isAmbiguous() then
    halt("invoking orderToIndex on a range that is ambigously aligned");

  return _base.orderToIndex(ord);
}

//////////////////////////////////////////////////////////////////////////////////
// translate the indices in this range by i
// A range can be translated even if its alignment is ambiguous.
//
pragma "inline" 
proc range.translate(i: integral)
  return new range(idxType, boundedType, stridable,
                   _base.translate(i), _aligned);

pragma "inline" 
proc range.translate(i)
{
  compilerError("offsets must be of integral type");
}

// Return an interior portion of this range.
pragma "inline" 
proc range.interior(i: idxType)
  return new range(idxType, boundedType, stridable,
                    _base.interior(i), _aligned);

// Return an exterior portion of this range.
pragma "inline" 
proc range.exterior(i: idxType)
  return new range(idxType, boundedType, stridable,
                   _base.exterior(i), _aligned);

// Returns an expanded range, or a contracted range if i < 0.
// The existing absolute alignment is preserved.
pragma "inline" 
proc range.expand(i: idxType)
  return new range(idxType, boundedType, stridable,
                   _base.expand(i), _aligned);


//################################################################################
//# Syntax Functions
//# These are special functions known to the compiler parser.
//#

// Assignment
pragma "inline"
proc =(r1: range(stridable=?s1), r2: range(stridable=?s2))
{
  r1._base = r2._base;
  r1._aligned = r2._aligned;
  return r1;
}

//////////////////////////////////////////////////////////////////////////////////
// Range arithmetic.
//
// Shifts and entire range to the right or left.
// Absolute alignment is not preserved
// (That is, the alignment shifts along with the range.)
//
pragma "inline"
proc +(r: range(?e, ?b, ?s), i: integral)
{
  var temp = r._base + i;
  return new range(temp.idxType, b, s, temp, r._aligned);
}

pragma "inline"
proc +(i:integral, r: range(?e,?b,?s))
  return r + i;

pragma "inline"
proc -(r: range(?e,?b,?s), i: integral)
{
  var temp = r._base - i;
  return new range(temp.idxType, b, s, temp, r._aligned);
}


proc by(r : range(?i,?b,?s), step)
{
  if step == 0 then
    __primitive("chpl_error", "the step argument of the 'by' operator is zero");

  if step.type != r.idxType && !chpl__legalIntCoerce(step.type, r.strType) then
    compilerError("in the 'by' operator, there is no implicit conversion from the type of the step argument, ", typeToString(step.type), ", to the type required by the range argument, ", typeToString(r.strType));

  const lw: i = r.low,
        hh: i = r.high,
        st: r.strType = r.stride * step:r.strType;

  const (ald, alt): (bool, i) =
    if r.isAmbiguous() then                   (false, r.alignment)
    else
      // we could talk about aligned bounds
      if      r.hasLowBound()  && st > 0 then (true, r.alignedLow)
      else if r.hasHighBound() && st < 0 then (true, r.alignedHigh)
      else                                    (r.aligned, r.alignment);

  return new range(i, b, true,  lw, hh, st, alt, ald);
}

proc by(r : range(?i,?b,true), param step)
{
  if step == 0 then
    compilerError("the step argument of the 'by' operator is zero");

  if step.type != r.idxType && !chpl__legalIntCoerce(step.type, r.strType) then
    compilerError("in the 'by' operator, there is no implicit conversion from the type of the step argument, ", typeToString(step.type), ", to the type required by the range argument, ", typeToString(r.strType));

  const lw: i = r.low,
        hh: i = r.high,
        st: r.strType = r.stride * step:r.strType;

  const (ald, alt): (bool, i) =
    if r.isAmbiguous() then                   (false, r.alignment)
    else
      // we could talk about aligned bounds
      if      r.hasLowBound()  && st > 0 then (true, r.alignedLow)
      else if r.hasHighBound() && st < 0 then (true, r.alignedHigh)
      else                                    (r.aligned, r.alignment);

  return new range(i, b, true,  lw, hh, st, alt, ald);
}

proc by(r : range(?i,?b,false), param step)
{
  if step == 0 then
    compilerError("the step argument of the 'by' operator is zero");

  if step.type != r.idxType && !chpl__legalIntCoerce(step.type, r.strType) then
    compilerError("in the 'by' operator, there is no implicit conversion from the type of the step argument, ", typeToString(step.type), ", to the type required by the range argument, ", typeToString(r.strType));

  const lw: i = r.low,
        hh: i = r.high;
  param st: r.strType = r.stride * step:r.strType;

  const (ald, alt): (bool, i) =
    if r.isAmbiguous() then                   (false, r.alignment)
    else
      // we could talk about aligned bounds
      if      r.hasLowBound()  && st > 0 then (true, r.alignedLow)
      else if r.hasHighBound() && st < 0 then (true, r.alignedHigh)
      else                                    (r.aligned, r.alignment);

  return new range(i, b, true,  lw, hh, st, alt, ald);
}

// This is the syntax processing routine for the "align" keyword.
// It produces a new range with the specified alignment.
// By definition, alignment is relative to the low bound of the range.
pragma "inline"
proc align(r : range(?e, ?b, ?s), algn)
  return new range(e, b, s, chpl__align(r._base, algn), true);

// Apply a natural alignment to an existing range.
proc range.offset(offs : integral)
{
  _base.offset(offs);
  this._aligned = true;
  return this;
}

// Composition
// Return the intersection of this and other.
proc range.this(other: range(?))
{
  // Two cases to consider:
  //  1) Both ranges unambiguously aligned
  //  2) One or both ranges ambiguously aligned.
  // In the latter case, we can obtain a result modulo alignment 
  // as long as the two strides are relatively prime.  
  // Otherwise, we can't know whether the two ranges intersect or not.
  var ambig = false;
  if this.isAmbiguous() || other.isAmbiguous()
  {
    var st1 = abs(this.stride);
    var st2 = abs(other.stride);
    var (g,x) = chpl__extendedEuclid(st1, st2);
    if g > 1 then
      __primitive("chpl_error", "Cannot slice ranges with ambiguous alignments unless their strides are relatively prime.");

    // OK, we can combine these two ranges, but the result is marked as ambiguous.
    ambig = true;
  }

  var temp = _base.this(other._base);
  return new range(temp.idxType, temp.boundedType, temp.stridable,
                   temp, !ambig && (this._aligned || other._aligned));
}


//////////////////////////////////////////////////////////////////////////////////
// syntax functions for counted ranges
//

// Creates a new range from an existing range, but with the specified number of
// elements.
// The new range has the same stride and alignment as the existing range.
// If the argument n is positive, the new range contains
// the first n elements in the existing range.
// If the argument n is negative, the new range contains
// the last abs(n) elements in the existing range.

proc #(r:range(?), i:integral)
{
  if r.isAmbiguous() then
    __primitive("chpl_error", "count -- Cannot count off elements from a range which is ambiguously aligned.");

  var temp = chpl__count(r._base, i);
  return new range(temp.idxType, temp.boundedType, temp.stridable,
                   temp, r._aligned);
}


//################################################################################
//# Iterators
//#
//# 

iter range.these()
{
  // TODO: hilde
  // Does this test nesting affect performance?
  // Inline and remove the test if so.
  if this.isAmbiguous() then
    __primitive("chpl_error", "these -- Attempt to iterate over a range with ambiguous alignment.");
  for i in _base.these() do yield i;
}

iter range.these(param tag: iterator) where tag == iterator.leader
{
  if this.isAmbiguous() then
    __primitive("chpl_error", "these -- Attempt to iterate over a range with ambiguous alignment.");
  for i in _base.these(iterator.leader) do yield i;
}

iter range.these(param tag: iterator, follower) where tag == iterator.follower
{
  if this.isAmbiguous() then
    __primitive("chpl_error", "these -- Attempt to iterate over a range with ambiguous alignment.");
  for i in _base.these(iterator.follower, follower) do yield i;
}


//################################################################################
//# Utilities
//#

// Write implementation for ranges
proc range.writeThis(f: Writer)
{
  if !aligned {
    // set things up so alignment does not get printed out
    _base._alignment =
      if isBoundedRange(this) then
        (if stride > 0 then _base._low else _base._high)
      else if this.boundedType == BoundedRangeType.boundedLow then
        _base._low
      else if this.boundedType == BoundedRangeType.boundedHigh then
        _base._high
      else
        0;
    // could verify that we succeeded:
    //assert(_base.isNaturallyAligned());
  }
  f.write(_base);
}

// Return a substring of a string with a range of indices.
proc string.substring(r: range(?))
{
  if r.isAmbiguous() then
    __primitive("chpl_error", "substring -- Cannot select from a string using a range with ambiguous alignment.");

  return this.substring(r._base);
}


//################################################################################
//# Internal helper functions.
//#

pragma "inline"
proc range.chpl__unTranslate(i: idxType)
  return this - i;

pragma "inline"
proc range.chpl__unTranslate(i)
{
  if _isSignedType(i.type) then
    return this - i;
  else
    return this + abs(i);
}

  // Determine if a strided range has a definite alignment.
proc chpl__hasAlignment(r : range(?))
{
  if r.hasLowBound() && r.stride >= 2 then return true;
  if r.hasHighBound() && r.stride <= -2 then return true;
  return false;
}

