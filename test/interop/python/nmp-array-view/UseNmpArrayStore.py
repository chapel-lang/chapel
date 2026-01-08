#
#
#
import ctypes
from enum import Enum
import NmpArrayStore
import numpy

# TODO: Move this into package initializer or something.
_once_for_package = False

class NmpArrayDataType(Enum):
  INT64 = 1
  REAL64 = 2

def _maybe_initialize_chpl():
  global _once_for_package
  if not _once_for_package:
    NmpArrayStore.chpl_setup()
    _once_for_package = True
  return

def _create_array_on_dtype(sdtype, srank, sdims):
  if sdtype == NmpArrayDataType.INT64:
    return NmpArrayStore.nmpArrayCreateInt64(srank, list(sdims))
  elif sdtype == NmpArrayDataType.REAL64:
    return NmpArrayStore.nmpArrayCreateReal64(srank, list(sdims))
  else:
    raise Exception('invalid array type: ', sdtype)
  return

def _dtype_to_ctypes_pointer_type(sdtype):
  if sdtype == NmpArrayDataType.INT64:
    return ctypes.POINTER(ctypes.c_int64)
  if sdtype == NmpArrayDataType.REAL64:
    return ctypes.POINTER(ctypes.c_double)
  raise Exception('invalid array type: ', sdtype)
  return

def _cast_int_to_ptr(val, sdtype):
  pointer_type = _dtype_to_ctypes_pointer_type(sdtype)
  ret = ctypes.cast(val, pointer_type)
  return ret

def _create_np_array_view(store_id, dtype, dims):
  val = NmpArrayStore.nmpArrayBufferPtrAsInt(store_id)
  ptr = _cast_int_to_ptr(val, dtype)
  ret = numpy.ctypeslib.as_array(ptr, shape=dims)
  return ret

def _sanitize_dtype(dtype):
  if dtype == int or dtype == numpy.int64:
    return NmpArrayDataType.INT64
  if dtype == float or dtype == numpy.float64 or dtype == ctypes.c_double:
    return NmpArrayDataType.REAL64
  raise Exception('unrecognized data type: ', dtype)
  return

def _sanitize_rank(rank):
  ret = int(rank)
  if ret <= 0:
    raise Exception('invalid rank value: ', rank)
  return ret

def _sanitize_dims_to_tuple(dims):
  if type(dims) == int: return (dims,)
  ret = tuple(list(dims))
  return ret

class NmpArray(object):
  def __init__(self, dtype, rank, dims):
    _maybe_initialize_chpl()
    sdtype = _sanitize_dtype(dtype)
    srank = _sanitize_rank(rank)
    sdims = _sanitize_dims_to_tuple(dims)
    self._store_id = _create_array_on_dtype(sdtype, srank, sdims)
    self._dtype = sdtype
    if self._store_id == -1:
      raise Exception('Failed to get array descriptor')
    self._view = _create_np_array_view(self._store_id, sdtype, sdims)
    return

  def __del__(self):
    NmpArrayStore.nmpArrayDestroy(self._store_id)
    return

  def view(self):
    return self._view

  def display(self):
    NmpArrayStore.nmpArrayPrint(self._store_id)
    return

def test(dtype, rank, dims):
  array = NmpArray(dtype, rank, dims)
  view = array.view()
  # TODO: How to walk dims in numpy?
  for i in range(0, len(view)):
    view[i] = dtype(i)
  array.display()
  return

def main():
  test(int, 1, 16)
  test(float, 1, 16)
  test(numpy.int64, 1, 16)
  test(numpy.float64, 1, 16)
  test(int, 2, (3, 3))
  test(int, 3, (2, 2, 2))
  return

if __name__ == '__main__':
  main()

