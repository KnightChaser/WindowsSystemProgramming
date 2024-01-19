#include <stdio.h>
#include <Windows.h>
#include <exception>
#include <memory>

class TestClass {
    public:
        ~TestClass() {
            printf("Destroying TestClass!\r\n");
        }
};

__declspec(noinline) void TestCPPEX()
{
#ifdef CPPEX
    printf("Throwing C++ exception\r\n");
    throw std::exception("");
#else
    printf("Triggering SEH exception\r\n");
    volatile int* pInt = 0x00000000;    // nullptr
    *pInt = 20;                         // triggers EXCEPTION_ACCESS_VIOLATION
#endif
}

__declspec(noinline) void TestExceptions()
{
    std::unique_ptr<TestClass> ptrTestClass = std::make_unique<TestClass>();
    TestCPPEX();
}

int main()
{
    __try
    {
        TestExceptions();
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        printf("Executing SEH __except block\r\n");
    }

    return 0;
}
