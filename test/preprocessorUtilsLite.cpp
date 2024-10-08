// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#define TF_MAX_ARITY 24

#include <pxr/tf/diagnosticLite.h>
#include <pxr/tf/regTest.h>
#include <pxr/tf/preprocessorUtilsLite.h>
#include <string.h>

using namespace pxr;

static bool
TestTF_PP_VARIADIC_SIZE()
{
    static_assert(TF_PP_VARIADIC_SIZE(()) == 1, "");
    static_assert(TF_PP_VARIADIC_SIZE(f()) == 1, "");
    static_assert(TF_PP_VARIADIC_SIZE(f()()) == 1, "");
    static_assert(TF_PP_VARIADIC_SIZE((a)) == 1, "");
    static_assert(TF_PP_VARIADIC_SIZE(((a))) == 1, "");
    static_assert(TF_PP_VARIADIC_SIZE((()())) == 1, "");

    static_assert(TF_PP_VARIADIC_SIZE(a) == 1, "");
    static_assert(TF_PP_VARIADIC_SIZE(a, b) == 2, "");
    static_assert(TF_PP_VARIADIC_SIZE(a, b, c) == 3, "");
    static_assert(TF_PP_VARIADIC_SIZE(a, b, c, d) == 4, "");
    static_assert(TF_PP_VARIADIC_SIZE(a, b, c, d, e) == 5, "");
    static_assert(TF_PP_VARIADIC_SIZE(a, b, c, d, e, f) == 6, "");
    static_assert(TF_PP_VARIADIC_SIZE(a, b, c, d, e, f, g) == 7, "");
    static_assert(TF_PP_VARIADIC_SIZE(a, b, c, d, e, f, g, h) == 8, "");
    static_assert(TF_PP_VARIADIC_SIZE(a, b, c, d, e, f, g, h, i) == 9, "");
    static_assert(TF_PP_VARIADIC_SIZE(a, b, c, d, e, f, g, h, i, j) == 10, "");
    static_assert(TF_PP_VARIADIC_SIZE(a, b, c, d, e, f, g, h, i, j, k) == 11, "");
    static_assert(TF_PP_VARIADIC_SIZE(a, b, c, d, e, f, g, h, i, j, k, l) == 12, "");
    static_assert(TF_PP_VARIADIC_SIZE(a, b, c, d, e, f, g, h, i, j, k, l, m) == 13, "");
    static_assert(TF_PP_VARIADIC_SIZE(a, b, c, d, e, f, g, h, i, j, k, l, m, n) == 14, "");
    static_assert(TF_PP_VARIADIC_SIZE(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) == 15, "");
    static_assert(TF_PP_VARIADIC_SIZE(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) == 16, "");
    static_assert(TF_PP_VARIADIC_SIZE(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) == 17, "");
    static_assert(TF_PP_VARIADIC_SIZE(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r) == 18, "");
    static_assert(TF_PP_VARIADIC_SIZE(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s) == 19, "");
    static_assert(TF_PP_VARIADIC_SIZE(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t) == 20, "");
    static_assert(TF_PP_VARIADIC_SIZE(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) == 21, "");
    static_assert(TF_PP_VARIADIC_SIZE(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v) == 22, "");
    static_assert(TF_PP_VARIADIC_SIZE(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w) == 23, "");
    static_assert(TF_PP_VARIADIC_SIZE(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x) == 24, "");

    return true;
}

static bool
TestTF_PP_VARIADIC_ELEM()
{
    static_assert(TF_PP_VARIADIC_ELEM(0, 1) == 1, "");
    static_assert(TF_PP_VARIADIC_ELEM(0, 1, 2) == 1, "");
    static_assert(TF_PP_VARIADIC_ELEM(0, 1, 2, 3) == 1, "");
    static_assert(TF_PP_VARIADIC_ELEM(1, 1, 2) == 2, "");
    static_assert(TF_PP_VARIADIC_ELEM(1, 1, 2, 3) == 2, "");
    static_assert(TF_PP_VARIADIC_ELEM(2, 1, 2, 3) == 3, "");
    return true;
}

static bool
TestTF_PP_FOR_EACH()
{
#define ADD(x) x +
    static_assert(TF_PP_FOR_EACH(ADD, 1) 1 == 2, "");
    static_assert(TF_PP_FOR_EACH(ADD, 1, 2) 1 == 4, "");
    static_assert(TF_PP_FOR_EACH(ADD, 1, 2, 3) 1 == 7, "");
#undef ADD
    return true;
}

static bool
TestTF_PP_EAT_PARENS()
{
    #define _STR TF_PP_EAT_PARENS_STR
    #define _EAT TF_PP_EAT_PARENS

    TF_AXIOM(!strcmp(_STR(), ""));
    TF_AXIOM(!strcmp(_STR(_EAT()), ""));
    TF_AXIOM(!strcmp(_STR(_EAT(())), ""));
    TF_AXIOM(!strcmp(_STR(_EAT(a)), "a"));
    TF_AXIOM(!strcmp(_STR(_EAT(a)), "a"));
    TF_AXIOM(!strcmp(_STR(_EAT((a))), "a"));
    TF_AXIOM(!strcmp(_STR(_EAT(((a)))), "(a)"));
    TF_AXIOM(!strcmp(_STR(_EAT(_EAT(((a))))), "a"));
    TF_AXIOM(!strcmp(_STR(_EAT(_EAT((((a)))))), "(a)"));
    TF_AXIOM(!strcmp(_STR(_EAT(_EAT(_EAT((((a))))))), "a"));

    TF_AXIOM(!strcmp(_STR((_EAT((<a, b>)))), "(<a, b>)"));
    TF_AXIOM(!strcmp(_STR((_EAT(_EAT(((<a, b>)))))), "(<a, b>)"));
    TF_AXIOM(!strcmp(_STR((_EAT(_EAT(_EAT((((<a, b>)))))))), "(<a, b>)"));

    TF_AXIOM(!strcmp(_STR(_EAT(f(a))), "f(a)"));

    TF_AXIOM(!strcmp(_STR(_EAT((x)(x))), "x(x)"));
    TF_AXIOM(!strcmp(_STR(_EAT((x)f(x))), "xf(x)"));
    TF_AXIOM(!strcmp(_STR(_EAT((x)(x)(x))), "x(x)(x)"));

    TF_AXIOM(!strcmp(_STR(_EAT(((x)(x)))), "(x)(x)"));
    TF_AXIOM(!strcmp(_STR(_EAT(((x)f(x)))), "(x)f(x)"));
    TF_AXIOM(!strcmp(_STR(_EAT(((x)(x)(x)))), "(x)(x)(x)"));

    #undef _STR
    #undef _EAT

    return true;
}


static bool
TestTF_PP_IS_TUPLE()
{
    #define _STR TF_PP_STRINGIZE

    TF_AXIOM(!strcmp(_STR(TF_PP_IS_TUPLE(())), "1"));
    TF_AXIOM(!strcmp(_STR(TF_PP_IS_TUPLE((a))), "1"));
    TF_AXIOM(!strcmp(_STR(TF_PP_IS_TUPLE(((a)))), "1"));
    TF_AXIOM(!strcmp(_STR(TF_PP_IS_TUPLE((a, b))), "1"));
    TF_AXIOM(!strcmp(_STR(TF_PP_IS_TUPLE((a, b, c))), "1"));

    TF_AXIOM(!strcmp(_STR(TF_PP_IS_TUPLE(a)), "0"));
    TF_AXIOM(!strcmp(_STR(TF_PP_IS_TUPLE(f(a))), "0"));
    TF_AXIOM(!strcmp(_STR(TF_PP_IS_TUPLE(This is a test)), "0"));

    #undef _STR

    return true;
}

static bool
TestTF_PP_TUPLE_ELEM()
{
    #define _STR(value) std::string(TF_PP_STRINGIZE(value))

    TF_AXIOM(_STR(TF_PP_TUPLE_ELEM(0, ())) == "");
    TF_AXIOM(_STR(TF_PP_TUPLE_ELEM(0, (a))) == "a");
    TF_AXIOM(_STR(TF_PP_TUPLE_ELEM(0, ((a)))) == "(a)");
    TF_AXIOM(_STR(TF_PP_TUPLE_ELEM(0, (((a))))) == "((a))");
    TF_AXIOM(_STR(TF_PP_TUPLE_ELEM(0, (a, b, c))) == "a");
    TF_AXIOM(_STR(TF_PP_TUPLE_ELEM(1, (a, b, c))) == "b");
    TF_AXIOM(_STR(TF_PP_TUPLE_ELEM(0, ((a, b, c)))) == "(a, b, c)");
    TF_AXIOM(_STR(TF_PP_TUPLE_ELEM(1, ((a, b, c)))) == "");
    TF_AXIOM(_STR(TF_PP_TUPLE_ELEM(1, (a, (b), c))) == "(b)");
    TF_AXIOM(_STR(TF_PP_TUPLE_ELEM(2, (a, b, c))) == "c");

    #undef _STR

    return true;
}

static bool
TestTF_PP_SEQ_SIZE()
{
    static_assert(TF_PP_SEQ_SIZE() == 0);
    #define EMPTY_SEQUENCE
    static_assert(TF_PP_SEQ_SIZE(EMPTY_SEQUENCE) == 0);
    #undef EMPTY_SEQUENCE
    static_assert(TF_PP_SEQ_SIZE(()) == 1);
    static_assert(TF_PP_SEQ_SIZE((((())))) == 1);
    static_assert(TF_PP_SEQ_SIZE((4)) == 1);
    static_assert(TF_PP_SEQ_SIZE((("hello", "world"))) == 1);
    static_assert(TF_PP_SEQ_SIZE((("hello", "world"))("goodbye")) == 2);
    static_assert(TF_PP_SEQ_SIZE((a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)) == 26);

    return true;
}

static bool
TestTF_PP_SEQ_FOR_EACH()
{
    #define EMPTY_SEQUENCE
    #define ADD(unused, x) x +
    static_assert(TF_PP_SEQ_FOR_EACH(ADD, ~, EMPTY_SEQUENCE) 1 == 1);
    static_assert(TF_PP_SEQ_FOR_EACH(ADD, ~, (1)) 1 == 2);
    static_assert(TF_PP_SEQ_FOR_EACH(ADD, ~, (1)(2)) 1 == 4);
    static_assert(TF_PP_SEQ_FOR_EACH(ADD, ~, (1)(2)(3)) 1 == 7);
    #undef ADD

    // Test the "data" argument
    #define SCALE_ADD(scale, x) (scale * x) +
    static_assert(TF_PP_SEQ_FOR_EACH(SCALE_ADD, 3, EMPTY_SEQUENCE) 1 == 1);
    static_assert(TF_PP_SEQ_FOR_EACH(SCALE_ADD, 3, (1)) 1 == 4);
    static_assert(TF_PP_SEQ_FOR_EACH(SCALE_ADD, 4, (1)(2)) 1 == 13);
    static_assert(TF_PP_SEQ_FOR_EACH(SCALE_ADD, 2, (1)(2)(3)) 1 == 13);
    #undef SCALE_ADD

    // Test generating sequences from sequences
    #define _MAP(unused, elem) ((TF_PP_CAT(elem, Key), "value"))
    #define _MAPPED_ELEMENTS TF_PP_SEQ_FOR_EACH(_MAP, ~, (x)(y)(z)(w))
    static_assert(TF_PP_SEQ_SIZE(_MAPPED_ELEMENTS) == 4);
    #define _MAP_AGAIN(unused, elem) TF_PP_TUPLE_ELEM(1, elem)
    TF_AXIOM(!strcmp(TF_PP_SEQ_FOR_EACH(_MAP_AGAIN, ~, _MAPPED_ELEMENTS), "valuevaluevaluevalue"));
    #undef _MAP_AGAIN
    #undef _MAPPED_ELEMENTS
    #undef _MAP
    #undef EMPTY_SEQUENCE

    return true;
}

static bool
Test_TfPreprocessorUtilsLite()
{
    return
        TestTF_PP_EAT_PARENS() &&
        TestTF_PP_IS_TUPLE() &&
        TestTF_PP_VARIADIC_SIZE() &&
        TestTF_PP_VARIADIC_ELEM() &&
        TestTF_PP_FOR_EACH() &&
        TestTF_PP_TUPLE_ELEM() &&
        TestTF_PP_SEQ_SIZE() &&
        TestTF_PP_SEQ_FOR_EACH()
        ;
}

TF_ADD_REGTEST(TfPreprocessorUtilsLite);
