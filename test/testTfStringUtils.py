#!/pxrpythonsubst
#
# Copyright 2016 Pixar
#
# Licensed under the terms set forth in the LICENSE.txt file available at
# https://openusd.org/license.
#
# Modified by Jeremy Retailleau.

from pxr import Tf
import logging
import unittest

class TestStringUtils(unittest.TestCase):
    """
    Test Tf String Utils (The python wrapped porting of the utility functions).
    """
    def setUp(self):
        self.log = logging.getLogger()

    def test_StringSplit(self):
        """Testing StringSplit() function. This function is supposed to behave
        like the split method on python string objects."""

        self.log.info("Testing string split cases")

        self.assertEqual([], Tf.StringSplit("",""))
        self.assertEqual([], Tf.StringSplit("abcd",""))
        self.assertEqual([], Tf.StringSplit("","ccc"))

        s = "abcd"
        self.assertEqual(s.split("a"), Tf.StringSplit(s, "a"))
        self.assertEqual(s.split("b"), Tf.StringSplit(s, "b"))
        self.assertEqual(s.split("c"), Tf.StringSplit(s, "c"))
        self.assertEqual(s.split("d"), Tf.StringSplit(s, "d"))
        self.assertEqual(s.split("abcd"), Tf.StringSplit(s, "abcd"))
        self.assertEqual(s.split("ab"), Tf.StringSplit(s, "ab"))

        s = "a:+b:+c:+d"
        self.assertEqual(s.split(":+"), Tf.StringSplit(s, ":+"))

        s = "a:+b:+c:d"
        self.assertEqual(s.split(":+"), Tf.StringSplit(s, ":+"))

    def test_Unicode(self):
        """Testing that we can pass python unicode objects to wrapped
        functions expecting std::string"""
        self.log.info("Testing unicode calls")
        self.assertEqual(Tf.StringSplit('123', '2'), ['1', '3'])
        self.assertEqual(Tf.StringSplit('123', u'2'), ['1', '3'])
        self.assertEqual(Tf.StringSplit(u'123', '2'), ['1', '3'])
        self.assertEqual(Tf.StringSplit(u'123', u'2'), ['1', '3'])

        self.assertEqual(Tf.DictionaryStrcmp('apple', 'banana'), -1)
        self.assertEqual(Tf.DictionaryStrcmp('apple', u'banana'), -1)
        self.assertEqual(Tf.DictionaryStrcmp(u'apple', 'banana'), -1)
        self.assertEqual(Tf.DictionaryStrcmp(u'apple', u'banana'), -1)

        # U+393B < U+393C
        # U+393B > U+393A
        # U+393B == U+393B
        # U+00FC > U+0030 because 0 is a digit and hence the word
        # prefix is less than that of `aü`
        self.assertEqual(Tf.DictionaryStrcmp('apple㤻', 'apple㤼'), -1)
        self.assertEqual(Tf.DictionaryStrcmp('apple㤻', 'apple㤺'), 1)
        self.assertEqual(Tf.DictionaryStrcmp('apple㤻', 'apple㤻'), 0)
        self.assertEqual(Tf.DictionaryStrcmp('aü', 'a0'), 1)

    def test_StringToLong(self):

        def checks(val):
            self.assertEqual(Tf.StringToLong(repr(val)), val)
        def checku(val):
            self.assertEqual(Tf.StringToULong(repr(val)), val)

        # A range of valid values.
        for i in range(1000000):
            checku(i)
        for i in range(-500000, 500000):
            checks(i)

        # A wider range of valid values.
        for i in range(0, 1000000000, 9337):
            checks(i)
        for i in range(-500000000, 500000000, 9337):
            checks(i)

        # Get the max/min values.
        ulmax, lmax, lmin = (
            Tf._GetULongMax(), Tf._GetLongMax(), Tf._GetLongMin())

        # Check the extrema and one before to ensure they work.
        for n in [ulmax-1, ulmax]:
            checku(n)

        for n in [lmin, lmin+1, lmax-1, lmax]:
            checks(n)

        # Check that some beyond the extrema over/underflow.
        #
        # Unsigned overflow.
        for i in range(1, 1000):
            with self.assertRaises(ValueError):
                checku(ulmax + i)
            with self.assertRaises(ValueError):
                checks(lmax + i)
            with self.assertRaises(ValueError):
                checks(lmin - i)

    def test_Identifiers(self):
        self.assertFalse(Tf.IsValidIdentifier(''))
        self.assertTrue(Tf.IsValidIdentifier('hello9'))
        self.assertFalse(Tf.IsValidIdentifier('9hello'))
        self.assertTrue(Tf.IsValidIdentifier('hello_world'))
        self.assertTrue(Tf.IsValidIdentifier('HELLO_WORLD'))
        self.assertTrue(Tf.IsValidIdentifier('hello_world_1234'))
        self.assertFalse(Tf.IsValidIdentifier('hello_#world#_1234'))
        self.assertFalse(Tf.IsValidIdentifier('h e l l o'))

        self.assertEqual(Tf.MakeValidIdentifier(''), '_')
        self.assertEqual(Tf.MakeValidIdentifier('hello9'), 'hello9')
        self.assertEqual(Tf.MakeValidIdentifier('9hello'), '_hello')
        self.assertEqual(
            Tf.MakeValidIdentifier('hello_#world#_1234'), 'hello__world__1234')
        self.assertFalse(Tf.IsValidIdentifier('h e l l o'), 'h_e_l_l_o')
        self.assertFalse(Tf.IsValidIdentifier('!@#$%'), '_____')


if __name__ == '__main__':
    unittest.main()
