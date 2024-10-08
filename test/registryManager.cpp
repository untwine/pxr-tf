// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include <pxr/tf/regTest.h>
#include <pxr/tf/registryManager.h>
#include <pxr/tf/instantiateSingleton.h>

#include <string>
#include <map>

using std::string;
using std::map;

using namespace pxr;

/*
 * This is an example and test of the TF_REGISTRY_FUNCTION facility.
 * RegRegistry is a registry that keeps a map from strings to pointers to
 * an interface class.  There are two classes derived from the interface
 * class.  They use TF_REGISTRY_FUNCTION to add themselves to the registry.
 * Then the test attempts to get those pointers back out of the registry
 * and use them.
 */



/*
 * Interface class
 */
class RegBase {
public:
    virtual int Get() = 0;
    virtual ~RegBase() {};
};



/*
 * Registry, allows registering of pointers to RegBase and lookup by name.
 */
class RegRegistry {
public:
    static RegRegistry& GetInstance() {
        return TfSingleton<RegRegistry>::GetInstance();
    }

    void Register(const std::string &name, RegBase *i) {
        _registered[name] = i;
    }

    RegBase *Get(const std::string &name) {
        map<string, RegBase *>::iterator i = _registered.find(name);
        if(i == _registered.end())
            return NULL;
        return i->second;
    }

private:
    RegRegistry() {
        TfSingleton<RegRegistry>::SetInstanceConstructed(*this);  
        TfRegistryManager::GetInstance().SubscribeTo<RegRegistry>();
    }

    RegRegistry(const RegRegistry &);
    const RegRegistry &operator=(const RegRegistry &);

    friend class TfSingleton<RegRegistry>;

    std::map<std::string, RegBase *> _registered;
};

TF_INSTANTIATE_SINGLETON(RegRegistry);



/*
 * Derived classes, implement RegBase and register themselves
 * with the RegRegistry.
 */
class RegDerived1 : public RegBase {
public:
    virtual int Get() { return 1; }
    virtual ~RegDerived1() {};
};

TF_REGISTRY_FUNCTION(RegRegistry) {
    RegRegistry::GetInstance().Register("one", new RegDerived1());
}


class RegDerived2 : public RegBase {
public:
    virtual int Get() { return 2; }
    virtual ~RegDerived2() {};
};

TF_REGISTRY_FUNCTION(RegRegistry) {
    RegRegistry::GetInstance().Register("two", new RegDerived2());
}



/*
 * Try to get the derived classes out of the registry by name.
 */
static bool
Test_TfRegistryManager()
{
    RegBase *x;

    x = RegRegistry::GetInstance().Get("one");
    TF_AXIOM(x);
    TF_AXIOM(x->Get() == 1);

    x = RegRegistry::GetInstance().Get("two");
    TF_AXIOM(x);
    TF_AXIOM(x->Get() == 2);

    x = RegRegistry::GetInstance().Get("three");
    TF_AXIOM(!x);

    TfSingleton<RegRegistry>::DeleteInstance();
    return true;
}

TF_ADD_REGTEST(TfRegistryManager);
