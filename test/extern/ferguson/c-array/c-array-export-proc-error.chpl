use CPtr;
extern proc bad_extern(): c_array(int, 2);
export proc bad_export(): c_array(int, 2) {
  return bad_extern();
}
