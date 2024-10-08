// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include <pxr/tf/regTest.h>
#include <pxr/tf/diagnostic.h>
#include <pxr/tf/enum.h>
#include <algorithm>
#include <cstdio>
#include <string>

using namespace std;
using namespace pxr;

enum Condiment {
    SALT,
    PEPPER = 13,
    KETCHUP,
    NO_NAME
};

// Declare an enumerated type with some values:
enum Season {
    SPRING,
    SUMMER = 3, // It's ok to have initializers
    AUTUMN,
    WINTER
};

static bool
Test_TfEnum()
{
    // add enum names and display names
    TF_ADD_ENUM_NAME(SALT, "Salt");
    TF_ADD_ENUM_NAME(PEPPER, "Pepper");
    TF_ADD_ENUM_NAME(KETCHUP, "Ketchup");

    // omit display names
    TF_ADD_ENUM_NAME(SPRING);
    TF_ADD_ENUM_NAME(SUMMER);
    TF_ADD_ENUM_NAME(AUTUMN);
    TF_ADD_ENUM_NAME(WINTER);

    bool foundIt;

    Condiment c;
    c = PEPPER;
    printf("GetName(PEPPER) returns %s\n", TfEnum::GetName(c).c_str());
    printf("GetFullName(PEPPER) returns %s\n", TfEnum::GetFullName(c).c_str());
    printf("GetDisplayName(PEPPER) returns %s\n", TfEnum::GetDisplayName(c).c_str());

    c = TfEnum::GetValueFromName<Condiment>("KETCHUP", &foundIt);
    printf("GetValueFromName(\"KETCHUP\") returns %s: %d\n",
           (foundIt ? "true" : "false"), c);
    TfEnum i = TfEnum::GetValueFromFullName("Condiment::KETCHUP", &foundIt);
    printf("GetValueFromFullName(\"Condiment::KETCHUP\") returns %s: %d\n",
           (foundIt ? "true" : "false"), i.GetValueAsInt());

    // Error testing
    c = NO_NAME;
    printf("GetName(NO_NAME) returns %s\n", TfEnum::GetName(c).c_str());
    printf("GetFullName(NO_NAME) returns %s\n",
           TfEnum::GetFullName(c).c_str());
    c = TfEnum::GetValueFromName<Condiment>("SQUID", &foundIt);
    printf("GetValueFromName(\"SQUID\") returns %s: %d\n",
           (foundIt ? "true" : "false"), c);
    i = TfEnum::GetValueFromFullName("Condiment::SQUID", &foundIt);
    printf("GetValueFromFullName(\"Condiment::SQUID\") returns %s: %d\n",
           (foundIt ? "true" : "false"), i.GetValueAsInt());

    // Look up the name for a value:
    string name1 = TfEnum::GetName(SUMMER);      // Returns "SUMMER"
    printf("name1 = \"%s\"\n", name1.c_str());
    string name2 = TfEnum::GetFullName(SUMMER); // Returns "Season::SUMMER"
    printf("name2 = \"%s\"\n", name2.c_str());
    // look up display name, which hasn't been specified. 
    // this should match the name
    string name3 = TfEnum::GetDisplayName(SUMMER); // Returns "SUMMER"
    printf("name3 = \"%s\"\n", name3.c_str());
    
    // Look up the value for a name:
    bool found;
    Season s1 = TfEnum::GetValueFromName<Season>("AUTUMN", &found);
    printf("s1 = %d, found = %s\n", s1, (found ? "true" : "false"));
    // Returns -1, sets found to \c false
    Season s2 = TfEnum::GetValueFromName<Season>("MONDAY", &found);
    printf("s2 = %d, found = %s\n", s2, (found ? "true" : "false"));
    // Returns 4, sets found to \c true
    TfEnum type(s2);
    TfEnum s3 = TfEnum::GetValueFromName(type.GetType(), "AUTUMN", &found);
    printf("s3 = %d, full name = %s, found = %s\n", s3.GetValueAsInt(),
           TfEnum::GetFullName(s3).c_str(), (found ? "true" : "false"));
    // Returns -1, sets found to \c false
    TfEnum s4 = TfEnum::GetValueFromFullName("Season::WINTER", &found);
    // Returns 5, sets found to \c true
    printf("s4 = %d, found = %s\n", s4.GetValueAsInt(),
           (found ? "true" : "false"));

    Season s5 = TfEnum::GetValueFromName<Season>("SALT", &found);
    // Returns -1, sets found to \c false
    printf("s5 = %d, found = %s\n", s5, (found ? "true" : "false"));

    vector<string> lookupNames;
    lookupNames.push_back("Season");
    lookupNames.push_back("Summer");
    lookupNames.push_back("Condiment");
    lookupNames.push_back("Sandwich");
    for(vector<string>::const_iterator n = lookupNames.begin();
        n != lookupNames.end(); ++ n)
        printf("type name \"%s\" is %s\n", n->c_str(),
               TfEnum::IsKnownEnumType(*n) ? "known" : "unknown");

    vector<string> names = TfEnum::GetAllNames<Condiment>();
    std::sort(names.begin(), names.end());
    printf("names associated with Condiment:\n");
    for(vector<string>::const_iterator n = names.begin();
        n != names.end(); ++ n)
        printf("%s\n", n->c_str());

    names = TfEnum::GetAllNames(SUMMER);
    std::sort(names.begin(), names.end());
    printf("names associated with SUMMER:\n");
    for(vector<string>::const_iterator n = names.begin();
        n != names.end(); ++ n)
        printf("%s\n", n->c_str());

    names = TfEnum::GetAllNames(*TfEnum::GetTypeFromName("Season"));
    std::sort(names.begin(), names.end());
    printf("names associated with \"Summer\":\n");
    for(vector<string>::const_iterator n = names.begin();
        n != names.end(); ++ n)
        printf("%s\n", n->c_str());

    TfEnum e(SUMMER);
    TF_AXIOM(e.IsA<Season>());
    TF_AXIOM(e.GetValueAsInt() == 3);
    TF_AXIOM(e.GetValue<Season>() == SUMMER);

    // Test coverage for operators
    TF_AXIOM(TfEnum(SUMMER) == TfEnum(SUMMER));
    TF_AXIOM(TfEnum(SUMMER) <= TfEnum(SUMMER));
    TF_AXIOM(TfEnum(SUMMER) >= TfEnum(SUMMER));
    TF_AXIOM(TfEnum(SUMMER) != TfEnum(SPRING));
    TF_AXIOM(TfEnum(SUMMER) > TfEnum(SPRING));
    TF_AXIOM(TfEnum(SUMMER) >= TfEnum(SPRING));
    TF_AXIOM(TfEnum(SPRING) <= TfEnum(SUMMER));
    TF_AXIOM(TfEnum(SPRING) < TfEnum(SUMMER));

    return true;
}

TF_ADD_REGTEST(TfEnum);
