//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

use Crypto;

/* START_EXAMPLE_0 */
var h = new Hash(Digest.SHA256);
/* STOP_EXAMPLE_0 */

/* START_EXAMPLE_1 */
var aes = new AES(256, CryptoChainMode.cbc);
/* STOP_EXAMPLE_1 */

/* START_EXAMPLE_2 */
var bf = new Blowfish(CryptoChainMode.cbc);
/* STOP_EXAMPLE_2 */
