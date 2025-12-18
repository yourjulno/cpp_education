#include <iostream>
#include <vector>
#include <memory>
#include <cmath>


class Shape
{
public:
    virtual ~Shape() = default;

    virtual double perimeter() const = 0;
    virtual double area() const = 0;

    virtual void test()
    {
        std::cout << "Shape::test()" << std::endl;
    }
};

class Entity_v1 : public Shape
{
public:
    void test() override
    {
        std::cout << "Entity_v1::test()" << std::endl;
    }
};

class Entity_v2 : public Shape
{
public:
    void test() override
    {
        std::cout << "Entity_v2::test()" << std::endl;
    }
};

class Adapter_v1 : public Entity_v1
{
public:
    virtual void test_v1()
    {
        Entity_v1::test();
    }

    void test() override
    {
        test_v1();
    }
};

class Adapter_v2 : public Entity_v2
{
public:
    virtual void test_v2()
    {
        Entity_v2::test();
    }

    void test() override
    {
        test_v2();
    }
};

class Triangle: public Shape
{
private:

    double a, b, c;

public:
    Triangle(double side1, double side2, double side3) :
    a(side1), b(side2), c(side3) {}

    double perimeter() const override
    {
        return a + b + c;
    }

    double area() const override
    {
        double p = perimeter() / 2;
        return std::sqrt(p * (p - a) * (p - b) * (p - c));
    }

    void test() override final
    {
        std::cout << "Triangle::test(): "
                  << a << ", " << b << ", " << c << std::endl;
    }
};

class Square: public Shape
{
private:
    double side;

public:
    Square(double s) : side(s) {}

    double perimeter() const override final
    {
        return 4 * side;
    }

    double area() const override final
    {
        return side * side;
    }

    void test() override final
    {
        std::cout << "Square::test():  " << side << std::endl;
    }
};

class Circle final : public Shape
{
private:
    double radius;

public:
    Circle(double r) : radius(r) {}

    double perimeter() const override final
    {
        return 2 * M_PI * radius;
    }

    double area() const override final
    {
        return M_PI * radius * radius;
    }

    void test() override final
    {
        std::cout << "Circle::test():  " << radius << std::endl;
    }
};

class Client : public Adapter_v1, public Adapter_v2
{
private:
    std::vector<std::shared_ptr<Shape>> shapes;

public:

    double perimeter() const override
    {
        return 0.0;
    }

    double area() const override
    {
        return 0.0;
    }

    void addShape(std::shared_ptr<Shape> shape)
    {
        shapes.push_back(shape);
    }

    void print_all_shapes() const
    {
        for (size_t i = 0; i < shapes.size(); ++i)
        {
            std::cout << "Shape " << i + 1 << ":" << std::endl;
            std::cout << "  Area: " << shapes[i]->perimeter() << std::endl;
            std::cout << "  Square: " << shapes[i]->area() << std::endl;
            shapes[i]->test();
            std::cout << std::endl;
        }
    }

    void call_v1_test()
    {
        Adapter_v1::test_v1();
    }

    void call_v2_test()
    {
        Adapter_v2::test_v2();
    }
};

int main()
{
    Client client;

    client.call_v1_test();
    client.call_v2_test();

    client.addShape(std::make_shared<Triangle>(3, 4, 5));
    client.addShape(std::make_shared<Square>(4));
    client.addShape(std::make_shared<Circle>(2));
    client.addShape(std::make_shared<Triangle>(6, 8, 10));

    client.print_all_shapes();

    return 0;
}