class Index {
}

class Array {
  pragma "clone_for_manifest_constants"      
  function this(i) {
    return __primitive("array_index", this, i);
  }
  pragma "clone_for_manifest_constants"      
  function this(i, j) {
    return __primitive("array_index", this, i, j);
  }
  pragma "clone_for_manifest_constants"      
  function this(i, j, k) {
    return __primitive("array_index", this, i, j, k);
  }
}

class Domain {
}

class Tuple {
  pragma "clone_for_manifest_constants"      
  function this(i) {
    return __primitive("index_object", this, i);
  }
}

function +(a : numeric, b : numeric) { return __primitive(a, "+", b); }
function -(a : numeric, b : numeric) { return __primitive(a, "-", b); }
function *(a : numeric, b : numeric) { return __primitive(a, "*", b); }
function /(a : numeric, b : numeric) { return __primitive(a, "/", b); }
function mod(a : numeric, b : numeric) { return __primitive(a, "%", b); }
function ==(a : numeric, b : numeric) { return __primitive(a, "==", b); }
function !=(a : numeric, b : numeric) { return __primitive(a, "!=", b); }
function <=(a : numeric, b : numeric) { return __primitive(a, "<=", b); }
function >=(a : numeric, b : numeric) { return __primitive(a, ">=", b); }
function <(a : numeric, b : numeric) { return __primitive(a, "<", b); }
function >(a : numeric, b : numeric) { return __primitive(a, ">", b); }
function &(a : numeric, b : numeric) { return __primitive(a, "&", b); }
function |(a : numeric, b : numeric) { return __primitive(a, "|", b); }
function ^(a : numeric, b : numeric) { return __primitive(a, "^", b); }
function and(a : numeric, b : numeric) { return __primitive(a, "&&", b); }
function or(a : numeric, b : numeric) { return __primitive(a, "||", b); }
function **(a : numeric, b : numeric) { return __primitive(a, "**", b); }
function by(a : numeric, b : numeric) { return __primitive(a, "by", b); }

function _chpl_malloc(number, size, description: string);
function _chpl_calloc(number, size, description: string);
function _chpl_free(memory);
function _chpl_realloc(memory, number, size);

function _UNION_CHECK(u, tag, filename, lineno);
function _UNION_SET(u, tag);

function _INIT_CONFIG(v, v_type, chapel_name, module_name);
function _INIT_ARRAY(rank, arr, dom, elt_type);
function _INIT_DOMAIN_DIM(dom, dim, lo, hi, str);
