#include <string>
#include <utility>

class Person
{
public:
    std::string name;
    int age;
    int grade;

public:
    Person()
        : name()
        , age(0)
        , grade(0)
    {
    }
};


class Builder
{
private:
    Person person_;

public:
    Builder()
        : person_()
    {}

    Builder& name(std::string value)
    {
        person_.name = std::move(value);
        return *this;
    }

    Builder& age(int value)
    {
        person_.age = value;
        return *this;
    }

    Builder& grade(int value)
    {
        person_.grade = value;
        return *this;
    }

    Person get() const
    {
        return person_;
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    Builder builder;

    auto person =
        builder
            .name("Ivan")
            .age(25)
            .grade(10)
            .get();

    return 0;
}
