#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "argparsing.h"

#include <qthread/qthread.h>
#include <qthread/dictionary.h>
#include <qthread/hash.h>

#define EXPECTED_ENTRIES 4

int my_key_equals(void *first,
                  void *second);
int my_hashcode(void *string);

int my_key_equals(void *first,
                  void *second)
{
    //iprintf("Comparing %s %s\n", (char *)first, (char *)second);
    return !strcmp(first, second);
}

int my_hashcode(void *string)
{
    return qt_hash_bytes(string, strlen((char*)string), GOLDEN_RATIO);
}

void my_destructor(void *key, void *val)
{
    iprintf("\tdeleting value key=%p (%s), val=%p (%s)\n", key, key, val, val);
}

int main(int    argc,
         char **argv)
{
    void *ret_code;

    CHECK_VERBOSE();

    qthread_initialize();
    qt_dictionary *dict   = qt_dictionary_create(my_key_equals, my_hashcode, my_destructor);
    char          *mykey1 = "k1";
    char          *myval1 = "v1";

    char *mykey2 = "k2";
    char *myval2 = "v2";

    char *mykey3 = "k3";
    char *myval3 = "v3";
    // first, tests for a single bucket:

    ret_code = qt_dictionary_put(dict, mykey1, myval1);
    iprintf(" 1. Put exited with code %p\n", ret_code);
    assert(ret_code != NULL);

    void *retrieved = qt_dictionary_get(dict, "k1");
    iprintf(" 2. v1=%s\n", (char *)retrieved);
    assert(my_key_equals("v1", (char *)retrieved));

    ret_code = qt_dictionary_put(dict, mykey2, myval2);
    iprintf(" 3. Put exited with code %p(%s)\n", ret_code, (char *)ret_code);
    assert(ret_code != NULL);

    retrieved = qt_dictionary_get(dict, "k1");
    iprintf(" 4. v1=%s\n", (char *)retrieved);
    assert(my_key_equals("v1", (char *)retrieved));

    retrieved = qt_dictionary_get(dict, "k2");
    assert(retrieved);
    iprintf(" 5. v2=%s\n", (char *)retrieved);
    assert(my_key_equals("v2", (char *)retrieved));

    retrieved = qt_dictionary_get(dict, "k3");
    iprintf(" 6. null=%s\n", (char *)retrieved);
    assert(retrieved == NULL);

    retrieved = qt_dictionary_get(dict, "k2");
    iprintf(" 7. v2=%s\n", (char *)retrieved);
    assert(my_key_equals("v2", (char *)retrieved));

    void *ret_code2 = qt_dictionary_put(dict, mykey2, "newv2");
    iprintf(" 8. Put exited with code %p\n", ret_code2);
    assert(ret_code2 != NULL);

    retrieved = qt_dictionary_get(dict, "k2");
    iprintf(" 9. newv2=%s\n", (char *)retrieved);
    assert(my_key_equals("newv2", (char *)retrieved));

    ret_code = qt_dictionary_put_if_absent(dict, mykey2, "updatev2");
    iprintf("10. Put exited with code %p\n", ret_code);
    assert(ret_code == ret_code2);

    retrieved = qt_dictionary_get(dict, "k2");
    iprintf("11. newv2=%s\n", (char *)retrieved);
    assert(my_key_equals("newv2", (char *)retrieved));

    ret_code = qt_dictionary_put_if_absent(dict, mykey3, myval3);
    iprintf("12. Put exited with code %p\n", ret_code);
    assert(ret_code != NULL);

    retrieved = qt_dictionary_get(dict, "k3");
    iprintf("13. v3=%s\n", (char *)retrieved);
    assert(my_key_equals("v3", (char *)retrieved));

    // tests for another bucket:
    ret_code = qt_dictionary_put(dict, "c1", myval1);
    iprintf("14. Put exited with code %p\n", ret_code);
    assert(ret_code != NULL);

    retrieved = qt_dictionary_get(dict, "c1");
    iprintf("15. v1=%s\n", (char *)retrieved);
    assert(my_key_equals("v1", (char *)retrieved));

    retrieved = qt_dictionary_get(dict, "k1");
    iprintf("16. v1=%s\n", (char *)retrieved);
    assert(my_key_equals("v1", (char *)retrieved));

    // test iterator behavior
    int                     no_entries = 0;
    qt_dictionary_iterator *it         = qt_dictionary_iterator_create(dict);
    while(NULL != qt_dictionary_iterator_next(it)) {
        list_entry *le = qt_dictionary_iterator_get(it);
        no_entries++;
        assert(le != NULL);
        iprintf("17. Pair:(%s,%s)\n", (char *)le->key, (char *)le->value);
    }
    iprintf("Found %d entries\n", no_entries);
    assert(no_entries == EXPECTED_ENTRIES);
    qt_dictionary_iterator_destroy(it);

    // test iterator equality function
    qt_dictionary_iterator *it1 = qt_dictionary_iterator_create(dict);
    qt_dictionary_iterator *it2 = qt_dictionary_iterator_create(dict);
    assert(qt_dictionary_iterator_equals(it1, it2));
    qt_dictionary_iterator_next(it1);
    qt_dictionary_iterator_next(it2);
    assert(qt_dictionary_iterator_equals(it1, it2));
    qt_dictionary_iterator_next(it2);
    assert(!qt_dictionary_iterator_equals(it1, it2));

    ret_code2 = qt_dictionary_get(dict, mykey2);
    iprintf("18. Get exited with code %p (%s)\n", ret_code2, ret_code2);
    assert(ret_code2 != NULL);

    void *val = qt_dictionary_delete(dict, mykey2);
    assert(val != NULL);
    assert(my_key_equals("newv2", (char *)val));

    ret_code2 = qt_dictionary_get(dict, mykey2);
    iprintf("19. Get exited with code %p\n", ret_code2);
    assert(ret_code2 == NULL);

    val = qt_dictionary_delete(dict, mykey2);
    iprintf("20. Delete exited with code %p\n", val);
    assert(val == NULL);

    qt_dictionary_iterator_destroy(it1);
    qt_dictionary_iterator_destroy(it2);
    qt_dictionary_destroy(dict);

    return 0;
}

/* vim:set expandtab: */
