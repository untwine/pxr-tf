// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include <pxr/tf/regTest.h>
#include <pxr/tf/stacked.h>
#include <pxr/tf/instantiateStacked.h>
#include <pxr/tf/diagnostic.h>
#include <pxr/tf/iterator.h>

#include <pxr/arch/demangle.h>

#include <iostream>
#include <thread>
#include <cstdio>

namespace pxr {

class Tf_SafeStacked : public TfStacked<Tf_SafeStacked, true> {
public:
    explicit Tf_SafeStacked(int v) : value(v) {}
    int value;
};
TF_INSTANTIATE_STACKED(Tf_SafeStacked);

TF_DEFINE_STACKED(Tf_UnsafeStacked, false, ) {
public:
    explicit Tf_UnsafeStacked(int v) : value(v) {}
    int value;
};
TF_INSTANTIATE_DEFINED_STACKED(Tf_UnsafeStacked);


TF_DEFINE_STACKED(Tf_FallbackStacked, true, ) {
public:
    explicit Tf_FallbackStacked(int v) : value(v) {}
    int value;

private:
    friend class TfStackedAccess;
    static void _InitializeStack() {
        new Tf_FallbackStacked(-2);
        new Tf_FallbackStacked(-1);
    }
};
TF_INSTANTIATE_DEFINED_STACKED(Tf_FallbackStacked);


template <class T>
static void PrintStack() {
    printf("%s : ", ArchGetDemangled<T>().c_str());
    TF_FOR_ALL(i, T::GetStack())
        printf("%d, ", (*i)->value);
    printf("\n");
}


template <class Stacked>
static void Test()
{
    typedef typename Stacked::Stack Stack;
    
    PrintStack<Stacked>();

    {
        TF_AXIOM(Stacked::GetStackTop() == 0);
        TF_AXIOM(Stacked::GetStackPrevious() == 0);
        Stack const &stack = Stacked::GetStack();
        TF_AXIOM(stack.size() == 0);
    }
        
    {
        Stacked a(1);
        Stacked b(2);

        {
            PrintStack<Stacked>();
            TF_AXIOM(Stacked::GetStackTop()->value == 2);
            TF_AXIOM(Stacked::GetStackPrevious()->value == 1);
            
            Stack const &stack = Stacked::GetStack();
            TF_AXIOM(stack.size() == 2);
            TF_AXIOM(stack[0]->value == 1 &&
                     stack[1]->value == 2);
        }

        {
            Stacked c(3);
            Stacked d(4);
            Stacked e(5);
            PrintStack<Stacked>();
            
            TF_AXIOM(Stacked::GetStackTop()->value == 5);
            TF_AXIOM(Stacked::GetStackPrevious()->value == 4);
            
            Stack const &stack = Stacked::GetStack();
            TF_AXIOM(stack.size() == 5);
            TF_AXIOM(stack[0]->value == 1 && 
                     stack[1]->value == 2 && 
                     stack[2]->value == 3 && 
                     stack[3]->value == 4 && 
                     stack[4]->value == 5);
        }

        {
            PrintStack<Stacked>();
            
            TF_AXIOM(Stacked::GetStackTop()->value == 2);
            TF_AXIOM(Stacked::GetStackPrevious()->value == 1);
            
            Stack const &stack = Stacked::GetStack();
            TF_AXIOM(stack.size() == 2);
            TF_AXIOM(stack[0]->value == 1 && 
                     stack[1]->value == 2);
        }

    }

    PrintStack<Stacked>();

    {
        TF_AXIOM(Stacked::GetStackTop() == 0);
        TF_AXIOM(Stacked::GetStackPrevious() == 0);
        Stack const &stack = Stacked::GetStack();
        TF_AXIOM(stack.size() == 0);
    }
}

struct TestFallbackStackedInAThread
{
    void operator()() {
        // This will run in multiple threads to make sure that the 
        // stack initializers have run once per-thread.
        TF_AXIOM(Tf_FallbackStacked::GetStackTop()->value == -1);
        TF_AXIOM(Tf_FallbackStacked::GetStackPrevious()->value == -2);
    }
};


static bool
Test_TfStacked() {

    Test<Tf_SafeStacked>();
    Test<Tf_UnsafeStacked>();

    // Test SafeStacked in multiple threads
    TF_AXIOM(std::thread::hardware_concurrency() > 1);
    std::vector<std::thread> threads;
    for (size_t i = 0, n = std::thread::hardware_concurrency(); i != n; ++i) {
        threads.emplace_back(TestFallbackStackedInAThread());
    }
    for (size_t i = 0, n = std::thread::hardware_concurrency(); i != n; ++i) {
        threads[i].join();
    }


    // Fallback values.
    PrintStack<Tf_FallbackStacked>();
    TF_AXIOM(Tf_FallbackStacked::GetStackTop()->value == -1);
    TF_AXIOM(Tf_FallbackStacked::GetStackPrevious()->value == -2);
    {
        Tf_FallbackStacked s(1234);
        PrintStack<Tf_FallbackStacked>();
        TF_AXIOM(Tf_FallbackStacked::GetStackTop()->value == 1234);
        TF_AXIOM(Tf_FallbackStacked::GetStackPrevious()->value == -1);

        {
            Tf_FallbackStacked s(2345);
            PrintStack<Tf_FallbackStacked>();
            TF_AXIOM(Tf_FallbackStacked::GetStackTop()->value == 2345);
            TF_AXIOM(Tf_FallbackStacked::GetStackPrevious()->value == 1234);
        }
            
        PrintStack<Tf_FallbackStacked>();
        TF_AXIOM(Tf_FallbackStacked::GetStackTop()->value == 1234);
        TF_AXIOM(Tf_FallbackStacked::GetStackPrevious()->value == -1);
    }
    PrintStack<Tf_FallbackStacked>();
    TF_AXIOM(Tf_FallbackStacked::GetStackTop()->value == -1);
    TF_AXIOM(Tf_FallbackStacked::GetStackPrevious()->value == -2);
    
    
    return true;
}

TF_ADD_REGTEST(TfStacked);

}  // namespace pxr
