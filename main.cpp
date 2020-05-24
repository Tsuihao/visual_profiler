#include <iostream>
#include "VisualProfiler.hpp"


void dummyPrintOut()
{
    PROFILE_FUNCTION();
    for(int i = 0; i < 1000; i++)
    {
        std::cout << "print #" << i+1 << std::endl;
    }
}

void dummySum()
{
    PROFILE_FUNCTION();
    long result = 0;
    for(int i = 0; i < 1000; i++)
    {
        result += (i+1);
    }
    std::cout << "Sum result = " << result << std::endl;
}

void dummyFactorial()
{
    PROFILE_FUNCTION();
    double result = 1;
    for(int i = 0; i < 100; i++)
    {
        result *= (i+1);
    }
    std::cout << "Factorial result = " << result << std::endl;
}


int main()
{
    Instrumentor::get().beginSession("Profile");  // insert beginSession()
    dummyPrintOut();
    dummySum();
    dummyFactorial();
    Instrumentor::get().endSession();
    
}