///////////////////////////////////////////////////////////////

// chapter : Memory Management

///////////////////////////////////////////////////////////////

// section : Resource Handlers

///////////////////////////////////////////////////////////////

// content : Resource Handlers
//
// content : Pattern Resource Acquisition Is Initialization

///////////////////////////////////////////////////////////////

#include <boost/noncopyable.hpp>
#include <iostream>
#include <source_location>
#include <string_view>
#include <cassert>
///////////////////////////////////////////////////////////////

template < typename T > class RAII : private boost::noncopyable
{
public :

	RAII(T x) : m_x(new T(x)) {}

//  ----------------------------

   ~RAII() 
	{ 
		delete m_x;
	}

private :

	T * m_x = nullptr;
};


namespace semester_second {

class Tracer
{
public:
    explicit Tracer(std::source_location loc = std::source_location::current())
        : m_loc(loc)
    {
        print("starts");
    }

    ~Tracer()
    {
        print("ends");
    }

    Tracer(const Tracer&) = delete;
    Tracer& operator=(const Tracer&) = delete;
    Tracer(Tracer&&) = delete;
    Tracer& operator=(Tracer&&) = delete;

private:
    std::source_location m_loc;

    void print(std::string_view msg) const
    {
        std::cout
            << m_loc.function_name()
            << " (" << m_loc.file_name() << ":" << m_loc.line() << ") "
            << msg
            << '\n';
    }
};


int foo()
{
    auto tracer = Tracer();
    std::cout << "Checking tracer" << std::endl;
    int a = 10;
    int b = 10;
    return a + b;
}

} // namespace semester_second

#ifndef NDEBUG
    #define trace() ::semester_second::Tracer tracer_##__LINE__{}
#else
    #define trace() ((void)0)
#endif

///////////////////////////////////////////////////////////////

int main()
{
    RAII < int > raii(1);
    int result = semester_second::foo();
    assert(result == 20);
}

///////////////////////////////////////////////////////////////