// ============================================================
// Open/Closed Principle (OCP)
// "Software entities should be open for extension,
//  but closed for modification."
// ============================================================
//
// BAD EXAMPLE  – Adding a new shape requires modifying the
//                existing AreaCalculator class.
//
// GOOD EXAMPLE – New shapes are added by creating new classes
//                that inherit from Shape. No existing code changes.
// ============================================================

#include <iostream>
#include <vector>
#include <memory>
#include <cmath>

// ===================== BAD EXAMPLE ===========================
// Every time a new shape is added, we must modify AreaCalculator.

namespace Bad
{

  enum class ShapeType
  {
    Circle,
    Rectangle
  };

  struct Shape
  {
    ShapeType type;
    double width, height, radius; // fields for all shapes crammed together
  };

  class AreaCalculator
  {
  public:
    double totalArea(const std::vector<Shape> &shapes) const
    {
      double total = 0;
      for (const auto &s : shapes)
      {
        // Must add a new case here for every new shape — violates OCP
        switch (s.type)
        {
        case ShapeType::Circle:
          total += M_PI * s.radius * s.radius;
          break;
        case ShapeType::Rectangle:
          total += s.width * s.height;
          break;
        }
      }
      return total;
    }
  };

} // namespace Bad

// ===================== GOOD EXAMPLE ==========================
// Shape is an abstract base. New shapes extend it without
// touching existing code.

namespace Good
{

  class Shape
  {
  public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual std::string name() const = 0;
  };

  class Circle : public Shape
  {
    double radius;

  public:
    explicit Circle(double r) : radius(r) {}
    double area() const override { return M_PI * radius * radius; }
    std::string name() const override { return "Circle"; }
  };

  class Rectangle : public Shape
  {
    double width, height;

  public:
    Rectangle(double w, double h) : width(w), height(h) {}
    double area() const override { return width * height; }
    std::string name() const override { return "Rectangle"; }
  };

  // Adding a new shape — NO modification to any existing class!
  class Triangle : public Shape
  {
    double base, height;

  public:
    Triangle(double b, double h) : base(b), height(h) {}
    double area() const override { return 0.5 * base * height; }
    std::string name() const override { return "Triangle"; }
  };

  // Works with ANY shape, present or future.
  class AreaCalculator
  {
  public:
    double totalArea(const std::vector<std::unique_ptr<Shape>> &shapes) const
    {
      double total = 0;
      for (const auto &s : shapes)
        total += s->area();
      return total;
    }
  };

} // namespace Good

// ======================== MAIN ===============================
int main()
{
  std::cout << "=== BAD EXAMPLE (violates OCP) ===\n";
  {
    Bad::Shape circle{Bad::ShapeType::Circle, 0, 0, 5.0};
    Bad::Shape rectangle{Bad::ShapeType::Rectangle, 4.0, 6.0, 0};
    Bad::AreaCalculator calc;
    std::cout << "Total area: "
              << calc.totalArea({circle, rectangle}) << "\n\n";
  }

  std::cout << "=== GOOD EXAMPLE (follows OCP) ===\n";
  {
    std::vector<std::unique_ptr<Good::Shape>> shapes;
    shapes.push_back(std::make_unique<Good::Circle>(5.0));
    shapes.push_back(std::make_unique<Good::Rectangle>(4.0, 6.0));
    shapes.push_back(std::make_unique<Good::Triangle>(3.0, 7.0));

    Good::AreaCalculator calc;
    for (const auto &s : shapes)
      std::cout << s->name() << " area: " << s->area() << "\n";
    std::cout << "Total area: " << calc.totalArea(shapes) << "\n";
  }

  return 0;
}
