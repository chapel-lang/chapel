/* This class uses the type field as the type of another field */
class Bar {
  /* To be used */
  type t;
  /* User #1 */
  var a: t;
  /* User #2 */
  const b: t;
  /* User #3 */
  param c: t;

}
