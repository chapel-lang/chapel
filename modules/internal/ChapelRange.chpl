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
// A range can only be aligned if it is strided.  Since alignments are taken
// modulo the stride, if the stride is one, even a nonzero alignment has no effect.
// The alignment of a range is relative to the low bound if it exists, otherwise
// relative to the high bound if it exists, otherwise absolute.
//
// The type of the stride is a signed type with the same number of bits as the
// index type.  This means that for an unsigned index type, a maximal range can
// be reached in no less than two strides.
//
// Ranges are usually naturally aligned; the _unaligned flag indicates when the
// alignment has been set explicitly -- thus making it "unaligned" w.r.t. its
// natural alignment.
//
// Some ranges have ambiguous alignment -- by definition:
//  Those which lack an upper bound and have a stride less than -1; or
//  Those which lack a lower bound and have a stride greater than 1.
// Ambiguous alignment is propagated by some operations, such as striding and slicing.
// Ambiguous alignment is erased if the alignment is set explicitly.
//
// The _aligned flag is used to track whether a range has been explicitly aligned.
// Initially, all ranges are unaligned, but since they have a stride of 1
// their alignment is not ambiguous.
// When a stride is applied, we check to see if any of the above cases is created.
// If not, the _aligned flag is set indicating that the alignment for that range
// has been determined.  Further applications of the "by" operator preserve that bit.
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
  
  pragma "inline" proc low return _base.low;       // public getter for low bound
  pragma "inline" proc high return _base.high;     // public getter for high bound
  pragma "inline" proc stride return _base.stride; // public getter for stride
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
 

//////////////////////////////////////////////////////////////////////////////////
// Range builders for bounded ranges
// Range builders are used by the parser to create literal ranges.
//
proc _build_range(low: int, high: int)
  return new range(idxType = int, _low = low, _high = high);
proc _build_range(low: uint, high: uint)
  return new range(uint, _low = low, _high = high);
proc _build_range(low: int(64), high: int(64))
  return new range(int(64), _low = low, _high = high);
proc _build_range(low: uint(64), high: uint(64))
  return new range(uint(64), _low = low, _high = high);


//////////////////////////////////////////////////////////////////////////////////
// Range builders for unbounded ranges
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

// Returns true if this range has a low bound.
proc range.hasLowBound() param
  return boundedType == BoundedRangeType.bounded ||
         boundedType == BoundedRangeType.boundedLow;

// Returns true if this range has a high bound.
proc range.hasHighBound() param
  return boundedType == BoundedRangeType.bounded ||
         boundedType == BoundedRangeType.boundedHigh;

pragma "inline" 
proc range.hasFirst() 
  return if isAmbiguous() then false else _base.hasFirst();
    
pragma "inline"
proc range.hasLast()
  return if isAmbiguous() then false else _base.hasLast();

// Returns true if this range is naturally aligned, false otherwise.
// Note that this does not indicate if the range is ambiguously aligned.
pragma "inline"
proc range.isAligned()
  return _base.isAligned();

// Returns true if the alignment of this range is ambiguous.
// After a stride whose absolute value is 2 or greater is applied, 
// the alignment must be positively established as evidenced by the _aligned
// flag being set to "true".
pragma "inline"
proc range.isAmbiguous()
{
  // If the range is not strided, then its alignment is unambiguously zero.
  if !stridable then return false;
  else return abs(stride) >= 2 && !aligned;
}

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

// Returns true if this range is equivalent to the other.
// Equivalent ranges produce the same index set.
//
// This routine relies on the assumption that
// the stride of an unstrided range is set to 1.
proc ==(r1: range(?), r2: range(?))
{
  // Ambiguous ranges cannot be equal even if all their parameters match.
  if r1.isAmbiguous() || r2.isAmbiguous() then
    return false;
  else
    return r1._base == r2._base;
}

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
proc ident(r1: range(?), r2: range(?))
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

// Returns the ordinal of this index within the sequence described by this range.
proc range.indexOrder(i: idxType)
{
  if this.isAmbiguous() then
    __primitive("chpl_error", "indexOrder -- Undefined on a range with ambiguous alignment.");

  return _base.indexOrder(i);
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

// This is the syntax processing routine for the "by" keyword.
// The by operator always clobbers an existing alignment.
proc by(r : range(?), str)
{
  var temp = chpl__by(r._base, str);
  var result = new range(temp.idxType, temp.boundedType, temp.stridable);
  result._base = temp;
  result._aligned = r._aligned || chpl__hasAlignment(result);
  return result;
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
  var result = new range(temp.idxType, temp.boundedType, temp.stridable);
  result._base = temp;
  result._aligned = !ambig && (this._aligned || other._aligned);
  return result;
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
  var result = new range(temp.idxType, temp.boundedType, temp.stridable);
  result._base = temp;
  result._aligned = r._aligned;
  return result;
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
  _base.writeThis(f);
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

