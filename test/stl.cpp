// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include <pxr/tf/regTest.h>
#include <pxr/tf/stl.h>
#include <pxr/tf/hash.h>

#include <pxr/tf/hashmap.h>

#include <tuple>
#include <utility>
#include <vector>

using std::map;
using std::pair;
using std::string;
using std::vector;

using namespace pxr;

static void testSetDifferences()
{
    int a1[] = {1, 3, 3, 1};
    int a2[] = {2, 3, 2};

    int e1[] = {1, 3, 1};
    int e2[] = {1};
    
    vector<int> v1(a1, a1 + (sizeof(a1)/sizeof(a1[0])));
    vector<int> v2(a2, a2 + (sizeof(a2)/sizeof(a2[0])));

    vector<int> expected1(e1, e1 + (sizeof(e1)/sizeof(e1[0])));
    vector<int> expected2(e2, e2 + (sizeof(e2)/sizeof(e2[0])));

    
    vector<int> result1 =
        TfOrderedSetDifferenceToContainer<vector<int> >(v1.begin(), v1.end(),
                                                        v2.begin(), v2.end());
    TF_AXIOM(result1 == expected1);

    vector<int> result2 =
        TfOrderedUniquingSetDifferenceToContainer<vector<int> >
        (v1.begin(), v1.end(), v2.begin(), v2.end());
    TF_AXIOM(result2 == expected2);
}

static void testGetPair()
{
    {
        pair<int, std::string> testPair(1, "A");

        using return_type_0 = decltype(TfGet<0>()(testPair));
        static_assert(
            std::is_same<return_type_0, int&>::value,
            "return type should be int&");

        using return_type_1 = decltype(TfGet<1>()(testPair));
        static_assert(
            std::is_same<return_type_1, std::string&>::value,
            "return type should be string&");

        TF_AXIOM(TfGet<0>()(testPair) == 1);
        TF_AXIOM(TfGet<1>()(testPair) == "A");
    }

    {
        const pair<int, std::string> testPair(2, "B");

        using return_type_0 = decltype(TfGet<0>()(testPair));
        static_assert(
            std::is_same<return_type_0, const int&>::value,
            "return type should be const int&");

        using return_type_1 = decltype(TfGet<1>()(testPair));
        static_assert(
            std::is_same<return_type_1, const std::string&>::value,
            "return type should be const string&");

        TF_AXIOM(TfGet<0>()(testPair) == 2);
        TF_AXIOM(TfGet<1>()(testPair) == "B");
    }

    {
        auto make_test_pair = []() -> pair<int, std::string> {
            return std::make_pair(3, "C");
        };

        using return_type_0 = decltype(TfGet<0>()(make_test_pair()));
        static_assert(
            std::is_same<return_type_0, int&&>::value,
            "return type should be int&&");

        using return_type_1 = decltype(TfGet<1>()(make_test_pair()));
        static_assert(
            std::is_same<return_type_1, std::string&&>::value,
            "return type should be string&&");


        TF_AXIOM(TfGet<0>()(make_test_pair()) == 3);
        TF_AXIOM(TfGet<1>()(make_test_pair()) == "C");
    }

    {
        const std::vector<pair<int, std::string>> pairs = {
            std::make_pair(1, "A"), 
            std::make_pair(2, "B"), 
            std::make_pair(3, "C"), 
            std::make_pair(4, "D")
        };

        std::vector<int> intsOnly(pairs.size());
        std::transform(pairs.begin(), pairs.end(), intsOnly.begin(),
                       TfGet<0>());

        TF_AXIOM((intsOnly == std::vector<int>{ 1, 2, 3, 4 }));
    }
}

static void testGetTuple()
{
    {
        std::tuple<int, std::string> testTuple(1, "A");

        using return_type_0 = decltype(TfGet<0>()(testTuple));
        static_assert(
            std::is_same<return_type_0, int&>::value,
            "return type should be int&");

        using return_type_1 = decltype(TfGet<1>()(testTuple));
        static_assert(
            std::is_same<return_type_1, std::string&>::value,
            "return type should be string&");

        TF_AXIOM(TfGet<0>()(testTuple) == 1);
        TF_AXIOM(TfGet<1>()(testTuple) == "A");
    }

    {
        const std::tuple<int, std::string> testTuple(2, "B");

        using return_type_0 = decltype(TfGet<0>()(testTuple));
        static_assert(
            std::is_same<return_type_0, const int&>::value,
            "return type should be const int&");

        using return_type_1 = decltype(TfGet<1>()(testTuple));
        static_assert(
            std::is_same<return_type_1, const std::string&>::value,
            "return type should be const string&");

        TF_AXIOM(TfGet<0>()(testTuple) == 2);
        TF_AXIOM(TfGet<1>()(testTuple) == "B");
    }

    {
        auto make_test_tuple = []() -> std::tuple<int, std::string> {
            return std::make_tuple(3, "C");
        };

        using return_type_0 = decltype(TfGet<0>()(make_test_tuple()));
        static_assert(
            std::is_same<return_type_0, int&&>::value,
            "return type should be int&&");

        using return_type_1 = decltype(TfGet<1>()(make_test_tuple()));
        static_assert(
            std::is_same<return_type_1, std::string&&>::value,
            "return type should be string&&");


        TF_AXIOM(TfGet<0>()(make_test_tuple()) == 3);
        TF_AXIOM(TfGet<1>()(make_test_tuple()) == "C");
    }

    {
        const std::vector<std::tuple<int, std::string>> tuples = {
            std::make_tuple(1, std::string("A")),
            std::make_tuple(2, std::string("B")), 
            std::make_tuple(3, std::string("C")), 
            std::make_tuple(4, std::string("D"))
        };

        std::vector<int> intsOnly(tuples.size());
        std::transform(tuples.begin(), tuples.end(), intsOnly.begin(),
                       TfGet<0>());

        TF_AXIOM((intsOnly == std::vector<int>{ 1, 2, 3, 4 }));
    }
}

static bool
Test_TfStl()
{
    testSetDifferences();
    testGetPair();
    testGetTuple();
    
    TfHashMap<string, int, TfHash> hm;
    map<string, int> m;

    const TfHashMap<string, int, TfHash>& chm = hm;
    const map<string, int>& cm = m;

    int hvalue = 0, mvalue = 0;
    string key("key");
    string badKey("blah");
    
    hm[key] = 1;
    m[key] = 1;

    TF_AXIOM(TfMapLookup(hm, key, &hvalue) && hvalue == 1);
    TF_AXIOM(TfMapLookup(m, key, &mvalue) && mvalue == 1);
    TF_AXIOM(TfMapLookupPtr(m, key) == &m[key]);
    TF_AXIOM(TfMapLookupPtr(hm, key) == &hm[key]);

    TF_AXIOM(TfMapLookupPtr(cm, key) == &m[key]);
    TF_AXIOM(TfMapLookupPtr(chm, key) == &hm[key]);


    TF_AXIOM(!TfMapLookup(m, badKey, &mvalue));
    TF_AXIOM(!TfMapLookup(hm, badKey, &hvalue));
    TF_AXIOM(!TfMapLookupPtr(m, badKey));
    TF_AXIOM(!TfMapLookupPtr(hm, badKey));
    TF_AXIOM(!TfMapLookupPtr(cm, badKey));
    TF_AXIOM(!TfMapLookupPtr(chm, badKey));

    TF_AXIOM(TfOrderedPair(1, 2) == TfOrderedPair(2, 1));
    TF_AXIOM((TfOrderedPair(2, 1) == pair<int,int>(1,2)));

    return true;
}

TF_ADD_REGTEST(TfStl);
