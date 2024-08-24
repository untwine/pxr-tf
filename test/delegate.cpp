// Copyright 2017 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include <pxr/tf/diagnosticMgr.h>
#include <pxr/tf/diagnostic.h>
#include <pxr/tf/refPtr.h>
#include <pxr/tf/regTest.h>

#include <iostream>
#include <string>

namespace pxr {

class TestDelegate : public TfDiagnosticMgr::Delegate {
    public:
        TestDelegate(const std::string& ident) : _ident(ident) {}
        virtual ~TestDelegate() {}

        virtual void IssueError(const TfError &err) override {
            std::cout << "[" << _ident << "]: Error issued\n";
        }

        virtual void IssueFatalError(const TfCallContext &context,
                                     const std::string &msg) override {
            std::cout << "[" << _ident << "]: Fatal error issued\n";
        }

        virtual void IssueStatus(const TfStatus &status) override {
            std::cout << "[" << _ident << "]: Status issued\n";
        }

        virtual void IssueWarning(const TfWarning &warning) override {
            std::cout << "[" << _ident << "]: Warning issued\n";
        }

    private:
        std::string _ident;
};

// Create an RAII style wrapper using AddDelegate/RemoveDelegate
class TestDelegateWrapper {
    public:
        TestDelegateWrapper(TestDelegate* testDelegate) 
            : _testDelegate(testDelegate) {
            TfDiagnosticMgr::GetInstance().AddDelegate(_testDelegate);
        }

        ~TestDelegateWrapper() {
            TfDiagnosticMgr::GetInstance().RemoveDelegate(_testDelegate);
        }

    private:
        TestDelegate* _testDelegate;
};

static bool
Test_TfDelegateAddRemove() {
    TestDelegate testDelegate("delegate_1");
    TestDelegateWrapper testDelegateWrapper(&testDelegate);
    TF_STATUS("."); 
    TF_WARN(".");

    // Add a second delegate
    {
        TestDelegate testDelegate2 ("delegate_2");
        TestDelegateWrapper testDelegateWrapper2(&testDelegate2);
        TF_STATUS("."); 
        TF_WARN(".");
    }
    
    // Second delegate is gone bc its out of scope now
    TF_STATUS(".");
    TF_WARN(".");

    return true;
}

TF_ADD_REGTEST(TfDelegateAddRemove);

}  // namespace pxr
