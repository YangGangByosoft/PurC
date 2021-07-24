#include "purc.h"

#include <stdio.h>
#include <errno.h>
#include <gtest/gtest.h>

// to test: serialize a null
TEST(variant, serialize_null)
{
    int ret = purc_init ("cn.fmsoft.hybridos.test", "variant", NULL);
    ASSERT_EQ (ret, PURC_ERROR_OK);

    purc_variant_t my_null = purc_variant_make_null();
    ASSERT_NE(my_null, PURC_VARIANT_INVALID);

    char buf[8];
    purc_rwstream_t my_rws = purc_rwstream_new_from_mem(buf, sizeof(buf) - 1);
    ASSERT_NE(my_rws, nullptr);

    size_t len_expected = 0;
    ssize_t n = purc_variant_serialize(my_null, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_PLAIN, &len_expected);
    ASSERT_EQ(len_expected, 4);
    ASSERT_EQ(n, 4);

    buf[4] = 0;
    ASSERT_STREQ(buf, "null");

    len_expected = 0;
    n = purc_variant_serialize(my_null, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_IGNORE_ERRORS, &len_expected);
    ASSERT_EQ(n, 3);
    ASSERT_EQ(len_expected, 4);

    buf[7] = 0;
    ASSERT_STREQ(buf, "nullnul");

    purc_cleanup ();
}

// to test: serialize an undefined
TEST(variant, serialize_undefined)
{
    int ret = purc_init ("cn.fmsoft.hybridos.test", "variant", NULL);
    ASSERT_EQ (ret, PURC_ERROR_OK);

    purc_variant_t my_undefined = purc_variant_make_undefined();
    ASSERT_NE(my_undefined, PURC_VARIANT_INVALID);

    char buf[18];
    purc_rwstream_t my_rws = purc_rwstream_new_from_mem(buf, sizeof(buf) - 1);
    ASSERT_NE(my_rws, nullptr);

    size_t len_expected = 0;
    ssize_t n = purc_variant_serialize(my_undefined, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_PLAIN, &len_expected);
    ASSERT_GE(len_expected, 9);
    ASSERT_EQ(n, 9);

    buf[n] = 0;
    ASSERT_STREQ(buf, "undefined");

    len_expected = 0;
    n = purc_variant_serialize(my_undefined, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_PLAIN, &len_expected);
    ASSERT_GE(len_expected, 9);
    ASSERT_EQ(n, -1);

    purc_cleanup ();
}

// to test: serialize a number
TEST(variant, serialize_number)
{
    int ret = purc_init ("cn.fmsoft.hybridos.test", "variant", NULL);
    ASSERT_EQ (ret, PURC_ERROR_OK);

    purc_variant_t my_variant = purc_variant_make_number(123.0);
    ASSERT_NE(my_variant, PURC_VARIANT_INVALID);

    char buf[32];
    purc_rwstream_t my_rws = purc_rwstream_new_from_mem(buf, sizeof(buf) - 1);
    ASSERT_NE(my_rws, nullptr);

    size_t len_expected = 0;
    ssize_t n = purc_variant_serialize(my_variant, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_PLAIN, &len_expected);
    ASSERT_GT(n, 0);

    buf[n] = 0;
    ASSERT_STREQ(buf, "123");

    my_variant = purc_variant_make_number(123.456000000);
    ASSERT_NE(my_variant, PURC_VARIANT_INVALID);

    purc_variant_unref(my_variant);

    purc_rwstream_seek(my_rws, 0, SEEK_SET);

    len_expected = 0;
    n = purc_variant_serialize(my_variant, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_NOZERO, &len_expected);
    ASSERT_GT(n, 0);

    buf[n] = 0;
    ASSERT_STREQ(buf, "123.456");

    purc_cleanup ();
}

// to test: serialize a long integer
TEST(variant, serialize_longint)
{
    purc_variant_t my_variant;
    purc_rwstream_t my_rws;
    size_t len_expected;
    ssize_t n;
    char buf[64];

    int ret = purc_init ("cn.fmsoft.hybridos.test", "variant", NULL);
    ASSERT_EQ (ret, PURC_ERROR_OK);

    my_rws = purc_rwstream_new_from_mem(buf, sizeof(buf) - 1);
    ASSERT_NE(my_rws, nullptr);

    /* case 1: unsigned long int */
    my_variant = purc_variant_make_longint(123456789L);
    ASSERT_NE(my_variant, PURC_VARIANT_INVALID);

    purc_rwstream_seek(my_rws, 0, SEEK_SET);
    len_expected = 0;
    n = purc_variant_serialize(my_variant, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_PLAIN, &len_expected);
    ASSERT_GT(n, 0);

    buf[n] = 0;
    ASSERT_STREQ(buf, "123456789L");

    purc_variant_unref(my_variant);

    /* case 2: unsigned long int */
    my_variant = purc_variant_make_ulongint(123456789UL);
    ASSERT_NE(my_variant, PURC_VARIANT_INVALID);

    purc_rwstream_seek(my_rws, 0, SEEK_SET);
    len_expected = 0;
    n = purc_variant_serialize(my_variant, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_NOZERO, &len_expected);
    ASSERT_GT(n, 0);

    buf[n] = 0;
    ASSERT_STREQ(buf, "123456789UL");

    purc_variant_unref(my_variant);

    purc_rwstream_destroy(my_rws);
    purc_cleanup ();
}

// to test: serialize a long double
TEST(variant, serialize_longdouble)
{
    purc_variant_t my_variant;
    purc_rwstream_t my_rws;
    size_t len_expected;
    ssize_t n;
    char buf[128];

    int ret = purc_init ("cn.fmsoft.hybridos.test", "variant", NULL);
    ASSERT_EQ (ret, PURC_ERROR_OK);

    my_rws = purc_rwstream_new_from_mem(buf, sizeof(buf) - 1);
    ASSERT_NE(my_rws, nullptr);

    /* case 1: long double */
    my_variant = purc_variant_make_longdouble(123456789.2345);
    ASSERT_NE(my_variant, PURC_VARIANT_INVALID);

    purc_rwstream_seek(my_rws, 0, SEEK_SET);
    len_expected = 0;
    n = purc_variant_serialize(my_variant, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_PLAIN, &len_expected);
    ASSERT_GT(n, 0);

    buf[n] = 0;
    ASSERT_STREQ(buf, "123456789.23450001FL");

    purc_variant_unref(my_variant);

    purc_rwstream_destroy(my_rws);
    purc_cleanup ();
}

static purc_variant_t my_getter(purc_variant_t root,
        int nr_args, purc_variant_t arg0, ...)
{
    (void)root;

    if (nr_args > 0)
        return arg0;

    return purc_variant_make_undefined();
}

static purc_variant_t my_setter(purc_variant_t root,
        int nr_args, purc_variant_t arg0, ...)
{
    (void)(root);
    (void)(nr_args);
    (void)(arg0);

    return purc_variant_make_boolean(false);
}

// to test: serialize a dynamic value
TEST(variant, serialize_dynamic)
{
    purc_variant_t my_variant;
    purc_rwstream_t my_rws;
    size_t len_expected;
    ssize_t n;
    char buf[128];

    int ret = purc_init ("cn.fmsoft.hybridos.test", "variant", NULL);
    ASSERT_EQ (ret, PURC_ERROR_OK);

    my_rws = purc_rwstream_new_from_mem(buf, sizeof(buf) - 1);
    ASSERT_NE(my_rws, nullptr);

    my_variant = purc_variant_make_dynamic(my_getter, my_setter);
    ASSERT_NE(my_variant, PURC_VARIANT_INVALID);

    purc_rwstream_seek(my_rws, 0, SEEK_SET);
    len_expected = 0;
    n = purc_variant_serialize(my_variant, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_PLAIN, &len_expected);
    ASSERT_GT(n, 0);

    buf[n] = 0;
    ASSERT_STREQ(buf, "<dynamic>");

    purc_variant_unref(my_variant);

    purc_rwstream_destroy(my_rws);
    purc_cleanup ();
}

static bool my_releaser (void* native_entity)
{
    printf ("my_releaser is called\n");
    free (native_entity);
    return true;
}

// to test: serialize a native entity
TEST(variant, serialize_native)
{
    purc_variant_t my_variant;
    purc_rwstream_t my_rws;
    size_t len_expected;
    ssize_t n;
    char buf[128];

    int ret = purc_init ("cn.fmsoft.hybridos.test", "variant", NULL);
    ASSERT_EQ (ret, PURC_ERROR_OK);

    my_rws = purc_rwstream_new_from_mem(buf, sizeof(buf) - 1);
    ASSERT_NE(my_rws, nullptr);

    my_variant = purc_variant_make_native(strdup("HVML"), my_releaser);
    ASSERT_NE(my_variant, PURC_VARIANT_INVALID);

    purc_rwstream_seek(my_rws, 0, SEEK_SET);
    len_expected = 0;
    n = purc_variant_serialize(my_variant, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_PLAIN, &len_expected);
    ASSERT_GT(n, 0);

    buf[n] = 0;
    ASSERT_STREQ(buf, "<native>");

    purc_variant_unref(my_variant);

    purc_rwstream_destroy(my_rws);
    purc_cleanup ();
}

// to test: serialize an atomstring
TEST(variant, serialize_atomstring)
{
    purc_variant_t my_variant;
    purc_rwstream_t my_rws;
    size_t len_expected;
    ssize_t n;
    char buf[64];

    int ret = purc_init ("cn.fmsoft.hybridos.test", "variant", NULL);
    ASSERT_EQ (ret, PURC_ERROR_OK);

    my_rws = purc_rwstream_new_from_mem(buf, sizeof(buf) - 1);
    ASSERT_NE(my_rws, nullptr);

    /* case 1: static string */
    my_variant = purc_variant_make_atom_string_static("HVML", false);
    ASSERT_NE(my_variant, PURC_VARIANT_INVALID);

    purc_rwstream_seek(my_rws, 0, SEEK_SET);
    len_expected = 0;
    n = purc_variant_serialize(my_variant, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_PLAIN, &len_expected);
    ASSERT_GT(n, 0);

    buf[n] = 0;
    ASSERT_STREQ(buf, "\"HVML\"");

    purc_variant_unref(my_variant);

    /* case 2: non-static string */
    my_variant = purc_variant_make_atom_string("PurC", false);
    ASSERT_NE(my_variant, PURC_VARIANT_INVALID);

    purc_rwstream_seek(my_rws, 0, SEEK_SET);
    len_expected = 0;
    n = purc_variant_serialize(my_variant, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_NOZERO, &len_expected);
    ASSERT_GT(n, 0);

    buf[n] = 0;
    ASSERT_STREQ(buf, "\"PurC\"");

    purc_variant_unref(my_variant);

    purc_rwstream_destroy(my_rws);
    purc_cleanup ();
}

// to test: serialize a string
TEST(variant, serialize_string)
{
    int ret = purc_init ("cn.fmsoft.hybridos.test", "variant", NULL);
    ASSERT_EQ (ret, PURC_ERROR_OK);

    purc_variant_t my_variant =
        purc_variant_make_string("\r\n\b\f\t\"\x1c'", false);
    ASSERT_NE(my_variant, PURC_VARIANT_INVALID);

    char buf[64] = { };
    purc_rwstream_t my_rws = purc_rwstream_new_from_mem(buf, sizeof(buf) - 1);
    ASSERT_NE(my_rws, nullptr);

    size_t len_expected = 0;
    ssize_t n = purc_variant_serialize(my_variant, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_PLAIN, &len_expected);
    ASSERT_GT(n, 0);
    puts("Serialized string with PCVARIANT_SERIALIZE_OPT_PLAIN flag:");
    puts(buf);

    buf[n] = 0;
    ASSERT_STREQ(buf, "\"\\r\\n\\b\\f\\t\\\"\\u001c'\"");

    purc_variant_unref(my_variant);

    memset(buf, '~', sizeof(buf));
    buf[sizeof(buf) - 1] = 0;
    purc_rwstream_seek(my_rws, 0, SEEK_SET);

    my_variant =
        purc_variant_make_string("这是一个很长的中文字符串", false);
    ASSERT_NE(my_variant, PURC_VARIANT_INVALID);

    n = purc_variant_serialize(my_variant, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_PLAIN, &len_expected);
    puts("Serialized string with PCVARIANT_SERIALIZE_OPT_PLAIN flag:");
    puts(buf);
    ASSERT_GT(n, 0);

    buf[n] = 0;
    ASSERT_STREQ(buf, "\"这是一个很长的中文字符串\"");
    purc_cleanup ();
}

// to test: serialize a byte sequence
TEST(variant, serialize_bsequence)
{
    int ret = purc_init ("cn.fmsoft.hybridos.test", "variant", NULL);
    ASSERT_EQ (ret, PURC_ERROR_OK);

    purc_variant_t my_variant =
        purc_variant_make_byte_sequence("\x59\x1C\x88\xAF", 4);
    ASSERT_NE(my_variant, PURC_VARIANT_INVALID);

    char buf[128];
    size_t len_expected;
    ssize_t n;

    purc_rwstream_t my_rws = purc_rwstream_new_from_mem(buf, sizeof(buf) - 1);
    ASSERT_NE(my_rws, nullptr);

    /* case 1: hex */
    memset(buf, '~', sizeof(buf));
    buf[sizeof(buf) - 1] = 0;
    purc_rwstream_seek(my_rws, 0, SEEK_SET);
    len_expected = 0;
    n = purc_variant_serialize(my_variant, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_BSEQUECE_HEX, &len_expected);
    ASSERT_GT(n, 0);
    puts("Serialized byte sequence with PCVARIANT_SERIALIZE_OPT_BSEQUECE_HEX flag:");
    puts(buf);
    buf[n] = 0;
    ASSERT_STREQ(buf, "bx591c88af");

    /* case 2: binary */
    memset(buf, '~', sizeof(buf));
    buf[sizeof(buf) - 1] = 0;
    purc_rwstream_seek(my_rws, 0, SEEK_SET);
    len_expected = 0;
    n = purc_variant_serialize(my_variant, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_BSEQUECE_BIN, &len_expected);
    ASSERT_GT(n, 0);
    puts("Serialized byte sequence with PCVARIANT_SERIALIZE_OPT_BSEQUECE_BIN flag:");
    puts(buf);
    buf[n] = 0;
    ASSERT_STREQ(buf, "bb01011001000111001000100010101111");

    /* case 3: binary with dot */
    memset(buf, '~', sizeof(buf));
    buf[sizeof(buf) - 1] = 0;
    purc_rwstream_seek(my_rws, 0, SEEK_SET);
    len_expected = 0;
    n = purc_variant_serialize(my_variant, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_BSEQUECE_BIN |
            PCVARIANT_SERIALIZE_OPT_BSEQUENCE_BIN_DOT, &len_expected);
    ASSERT_GT(n, 0);
    puts("Serialized byte sequence with PCVARIANT_SERIALIZE_OPT_BSEQUECE_BIN | PCVARIANT_SERIALIZE_OPT_BSEQUENCE_BIN_DOT flag:");
    puts(buf);
    buf[n] = 0;
    ASSERT_STREQ(buf, "bb0101.1001.0001.1100.1000.1000.1010.1111");

    /* case 4: Base64 */
    memset(buf, '~', sizeof(buf));
    buf[sizeof(buf) - 1] = 0;
    purc_rwstream_seek(my_rws, 0, SEEK_SET);
    len_expected = 0;
    n = purc_variant_serialize(my_variant, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_BSEQUECE_BASE64, &len_expected);
    ASSERT_GT(n, 0);
    puts("Serialized byte sequence with PCVARIANT_SERIALIZE_OPT_BSEQUECE_BASE64 flag:");
    puts(buf);
    buf[n] = 0;
    ASSERT_STREQ(buf, "b64WRyIrw==");

    purc_variant_unref(my_variant);

    /* case 4: long sequence */
    my_variant =
        purc_variant_make_byte_sequence("\x59\x1C\x88\xAF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\xEF\x00", 55);
    ASSERT_NE(my_variant, PURC_VARIANT_INVALID);

    memset(buf, '~', sizeof(buf));
    buf[sizeof(buf) - 1] = 0;
    purc_rwstream_seek(my_rws, 0, SEEK_SET);
    len_expected = 0;
    n = purc_variant_serialize(my_variant, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_BSEQUECE_HEX, &len_expected);
    ASSERT_GT(n, 0);
    puts("Serialized byte sequence with PCVARIANT_SERIALIZE_OPT_BSEQUECE_HEX flag:");
    puts(buf);
    buf[n] = 0;
    ASSERT_STREQ(buf, "bx591c88afefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefef00");

    purc_variant_unref(my_variant);

    purc_cleanup ();
}

// to test: serialize an array
TEST(variant, serialize_array)
{
    int ret = purc_init ("cn.fmsoft.hybridos.test", "variant", NULL);
    ASSERT_EQ (ret, PURC_ERROR_OK);

    purc_variant_t v1 = purc_variant_make_number(123.0);
    purc_variant_t v2 = purc_variant_make_number(123.456);

    purc_variant_t my_variant = purc_variant_make_array(2, v1, v2);
    ASSERT_NE(my_variant, PURC_VARIANT_INVALID);

    char buf[64] = { };
    purc_rwstream_t my_rws = purc_rwstream_new_from_mem(buf, sizeof(buf) - 1);
    ASSERT_NE(my_rws, nullptr);

    size_t len_expected = 0;
    ssize_t n = purc_variant_serialize(my_variant, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_PLAIN, &len_expected);
    puts("Serialized array with PCVARIANT_SERIALIZE_OPT_PLAIN flag:");
    puts(buf);
    ASSERT_GT(n, 0);
    ASSERT_GT(len_expected, 0);

    buf[n] = 0;
    ASSERT_STREQ(buf, "[123,123.456]");

    len_expected = 0;
    purc_rwstream_seek(my_rws, 0, SEEK_SET);
    memset(buf, 0, sizeof(buf));
    n = purc_variant_serialize(my_variant, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_SPACED, &len_expected);
    puts("Serialized array with PCVARIANT_SERIALIZE_OPT_SPACED flag:");
    puts(buf);
    ASSERT_GT(n, 0);

    buf[n] = 0;
    ASSERT_STREQ(buf, "[ 123, 123.456 ]");

    len_expected = 0;
    purc_rwstream_seek(my_rws, 0, SEEK_SET);
    memset(buf, 0, sizeof(buf));
    n = purc_variant_serialize(my_variant, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_NOZERO, &len_expected);
    puts("Serialized array with PCVARIANT_SERIALIZE_OPT_NOZERO flag:");
    puts(buf);
    ASSERT_GT(n, 0);

    buf[n] = 0;
    ASSERT_STREQ(buf, "[123,123.456]");

    len_expected = 0;
    purc_rwstream_seek(my_rws, 0, SEEK_SET);
    memset(buf, 0, sizeof(buf));
    n = purc_variant_serialize(my_variant, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_PRETTY, &len_expected);
    puts("Serialized array with PCVARIANT_SERIALIZE_OPT_PRETTY flag:");
    puts(buf);
    ASSERT_GT(n, 0);

    buf[n] = 0;
    ASSERT_STREQ(buf, "[\n  123,\n  123.456\n]");

    len_expected = 0;
    purc_rwstream_seek(my_rws, 0, SEEK_SET);
    memset(buf, 0, sizeof(buf));
    n = purc_variant_serialize(my_variant, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_PRETTY |
            PCVARIANT_SERIALIZE_OPT_PRETTY_TAB, &len_expected);
    puts("Serialized array with PCVARIANT_SERIALIZE_OPT_PRETTY and PCVARIANT_SERIALIZE_OPT_PRETTY_TAB flag:");
    puts(buf);
    ASSERT_GT(n, 0);

    buf[n] = 0;
    ASSERT_STREQ(buf, "[\n\t123,\n\t123.456\n]");

    purc_variant_unref(my_variant);
    purc_variant_unref(v1);
    purc_variant_unref(v2);

    purc_cleanup ();
}

// to test: serialize an object
TEST(variant, serialize_object)
{
    int ret = purc_init ("cn.fmsoft.hybridos.test", "variant", NULL);
    ASSERT_EQ (ret, PURC_ERROR_OK);

    purc_variant_t v1 = purc_variant_make_number(123.0);
    purc_variant_t v2 = purc_variant_make_number(123.456);

    purc_variant_t my_variant =
        purc_variant_make_object_c(2, "v1", v1, "v2", v2);
    ASSERT_NE(my_variant, PURC_VARIANT_INVALID);

    char buf[32];
    purc_rwstream_t my_rws = purc_rwstream_new_from_mem(buf, sizeof(buf) - 1);
    ASSERT_NE(my_rws, nullptr);

    size_t len_expected = 0;
    ssize_t n = purc_variant_serialize(my_variant, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_PLAIN, &len_expected);
    ASSERT_GT(n, 0);

    buf[n] = 0;
    ASSERT_STREQ(buf, "{\"v1\":123,\"v2\":123.456}");

    purc_rwstream_seek(my_rws, 0, SEEK_SET);

    len_expected = 0;
    n = purc_variant_serialize(my_variant, my_rws,
            0, PCVARIANT_SERIALIZE_OPT_NOZERO, &len_expected);
    ASSERT_GT(n, 0);

    buf[n] = 0;
    ASSERT_STREQ(buf, "{\"v1\":123,\"v2\":123.456}");

    purc_variant_unref(my_variant);
    purc_variant_unref(v1);
    purc_variant_unref(v2);

    purc_cleanup ();
}
