const NULL: integer = 0;  -- this needs to precede any class definitions


class Index {
}

class Array {
  function this(i) {
    return __primitive("array_index", this, i);
  }
  function this(i, j) {
    return __primitive("array_index", this, i, j);
  }
  function this(i, j, k) {
    return __primitive("array_index", this, i, j, k);
  }
}

class Domain {
}

class Tuple {
  function this(pragma "clone_for_constants" i) {
    return __primitive("index_object", this, i);
  }
}

pragma "builtin" function +(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function -(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function *(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function /(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function &(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function |(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function ^(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function ==(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function !=(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function <=(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function >=(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function <(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function >(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function and(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function or(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }
pragma "builtin" function **(a : Array, b : Array) { return __primitive("array_pointwise_op", a, b); }

pragma "builtin" function +(a : numeric, b : numeric) { return __primitive(a, "+", b); }
pragma "builtin" function -(a : numeric, b : numeric) { return __primitive(a, "-", b); }
pragma "builtin" function *(a : numeric, b : numeric) { return __primitive(a, "*", b); }
pragma "builtin" function /(a : numeric, b : numeric) { return __primitive(a, "/", b); }
pragma "builtin" function mod(a : numeric, b : numeric) { return __primitive(a, "%", b); }
pragma "builtin" function ==(a : numeric, b : numeric) { return __primitive(a, "==", b); } 
pragma "builtin" function !=(a : numeric, b : numeric) { return __primitive(a, "!=", b); }
pragma "builtin" function <=(a : numeric, b : numeric) { return __primitive(a, "<=", b); }
pragma "builtin" function >=(a : numeric, b : numeric) { return __primitive(a, ">=", b); }
pragma "builtin" function <(a : numeric, b : numeric) { return __primitive(a, "<", b); }
pragma "builtin" function >(a : numeric, b : numeric) { return __primitive(a, ">", b); }
pragma "builtin" function &(a : numeric, b : numeric) { return __primitive(a, "&", b); }
pragma "builtin" function |(a : numeric, b : numeric) { return __primitive(a, "|", b); }
pragma "builtin" function ^(a : numeric, b : numeric) { return __primitive(a, "^", b); }
pragma "builtin" function and(a : numeric, b : numeric) { return __primitive(a, "&&", b); }
pragma "builtin" function or(a : numeric, b : numeric) { return __primitive(a, "||", b); }
pragma "builtin" function **(a : numeric, b : numeric) { return __primitive(a, "**", b); }
pragma "builtin" function by(a : numeric, b : numeric) { return __primitive(a, "by", b); }
pragma "builtin" function ==(a : object, b : object) { return __primitive("ptr_eq", a, b); }
pragma "builtin" function !=(a : object, b : object) { return __primitive("ptr_neq", a, b); }

function _chpl_malloc(number, size, description: string);
function _chpl_calloc(number, size, description: string);
function _chpl_free(memory);
function _chpl_realloc(memory, number, size, description: string);

function _UNION_CHECK(u, tag, filename, lineno);
function _UNION_SET(u, tag);
function _UNION_CHECK_QUIET(u, tag): boolean;

function _INIT_CONFIG(v, v_type, chapel_name, module_name);
function _INIT_ARRAY(rank, arr, dom, elt_type);
function _INIT_DOMAIN_DIM(dom, dim, lo, hi, str);

function _SEQ_APPEND(s, t, type_name);
function _SEQ_INIT_NIL(s);


class _seq {
  type elementType;
  class _node {
    var element : elementType;
    var next : _node;
  }
  var length : integer;
  var first : _node;
  var last : _node;
}
/*
function _seq_pound(s : _seq, e : elementType) {
  var new like s.first;
  new.element = e;
  if length > 0 {
    last.next = new;
    last = last.next;
  } else {
    first = new;
    last = first;
  }
  length += 1;
  return s;
}

function _seq_pound(e : elementType, s : _seq) {
  var new like s.first;
  new.element = e;
  if length > 0 {
    new.next = first;
    first = new;
  } else {
    first = new;
    last = first;
  }
  length += 1;
  return s;
}

function _seq_pound(s1 : _seq, s2 : _seq) {
  s1.last.next = s2.first;
  return s1;
}

function _seq_copy(s : _seq) {
  var new like s; --assume compiler makes default value a new instance
  for e in s {
    new = _seq_pound(new, e);
  }
  return new;
}
*/