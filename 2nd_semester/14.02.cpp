#include <cassert>
#include <exception>
#include <stdexcept>
#include <thread>
#include <utility>

template <typename F>
class Task
{
public:
    Task(F function, std::exception_ptr& exception)
        : function(std::move(function)), exception(&exception)
    {
    }

    void operator()() const
    {
        try
        {
            function();
        }
        catch (...)
        {
            *exception = std::current_exception();
        }
    }

private:
    F function;
    std::exception_ptr* exception;
};

int main()
{
    std::exception_ptr exception;

    auto function = []()
    {
        throw std::runtime_error("thread exception");
    };

    std::thread thread{Task<decltype(function)>(function, exception)};

    thread.join();

    bool handled = false;

    try
    {
        if (exception)
        {
            std::rethrow_exception(exception);
        }
    }
    catch (const std::runtime_error&)
    {
        handled = true;
    }

    assert(handled);
}
