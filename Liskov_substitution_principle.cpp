// ============================================================
// Liskov Substitution Principle (LSP)
// "Subtypes must be substitutable for their base types
//  without altering the correctness of the program."
// ============================================================
//
// BAD EXAMPLE  – Square inherits from Rectangle but breaks the
//                expected behaviour when width/height are set
//                independently.
//
// GOOD EXAMPLE – Separate Shape hierarchy where each class
//                correctly implements its own area calculation.
// ============================================================

#include <iostream>
#include <cassert>
#include <memory>
#include <vector>

// ===================== BAD EXAMPLE ===========================
// Square IS-A Rectangle mathematically, but the code contract
// of setWidth/setHeight is violated — callers expect they are
// independent.

namespace Bad
{

  class Rectangle
  {
  protected:
    double width, height;

  public:
    Rectangle(double w, double h) : width(w), height(h) {}
    virtual ~Rectangle() = default;

    virtual void setWidth(double w) { width = w; }
    virtual void setHeight(double h) { height = h; }

    double getWidth() const { return width; }
    double getHeight() const { return height; }
    double area() const { return width * height; }
  };

  // Square overrides setters so both dimensions stay equal,
  // but this breaks callers who assume independent setters.
  class Square : public Rectangle
  {
  public:
    explicit Square(double side) : Rectangle(side, side) {}

    void setWidth(double w) override
    {
      width = w;
      height = w; // side-effect: also changes height!
    }
    void setHeight(double h) override
    {
      width = h; // side-effect: also changes width!
      height = h;
    }
  };

  // This function works correctly for Rectangle but BREAKS for Square.
  void increaseRectangleWidth(Rectangle &r)
  {
    double oldHeight = r.getHeight();
    r.setWidth(r.getWidth() + 1);

    // Expectation: height should not have changed.
    // With Square, this assertion FAILS because setWidth also changed height.
    if (r.getHeight() != oldHeight)
    {
      std::cout << "  [BUG] Height changed unexpectedly! LSP violated.\n";
    }
  }

} // namespace Bad

// ===================== GOOD EXAMPLE ==========================
// Use a common Shape interface. Rectangle and Square are siblings,
// not parent-child.  Each computes area correctly on its own.

namespace Good
{

  class Shape
  {
  public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual std::string name() const = 0;
  };

  class Rectangle : public Shape
  {
    double width, height;

  public:
    Rectangle(double w, double h) : width(w), height(h) {}

    void setWidth(double w) { width = w; }
    void setHeight(double h) { height = h; }

    double getWidth() const { return width; }
    double getHeight() const { return height; }

    double area() const override { return width * height; }
    std::string name() const override { return "Rectangle"; }
  };

  class Square : public Shape
  {
    double side;

  public:
    explicit Square(double s) : side(s) {}

    void setSide(double s) { side = s; }
    double getSide() const { return side; }

    double area() const override { return side * side; }
    std::string name() const override { return "Square"; }
  };

  // Works correctly with ANY Shape — LSP satisfied.
  void printArea(const Shape &s)
  {
    std::cout << "  " << s.name() << " area = " << s.area() << "\n";
  }

} // namespace Good

// ======================== MAIN ===============================
int main()
{
  std::cout << "=== BAD EXAMPLE (violates LSP) ===\n";
  {
    Bad::Rectangle rect(10, 5);
    Bad::Square sq(5);

    std::cout << "Rectangle:\n";
    Bad::increaseRectangleWidth(rect); // OK

    std::cout << "Square (passed as Rectangle&):\n";
    Bad::increaseRectangleWidth(sq); // BUG — LSP violation
  }

  std::cout << "\n=== GOOD EXAMPLE (follows LSP) ===\n";
  {
    Good::Rectangle rect(10, 5);
    Good::Square sq(5);

    Good::printArea(rect); // Rectangle area = 50
    Good::printArea(sq);   // Square area = 25

    // Each type is used through the Shape interface correctly.
    std::vector<std::unique_ptr<Good::Shape>> shapes;
    shapes.push_back(std::make_unique<Good::Rectangle>(4, 6));
    shapes.push_back(std::make_unique<Good::Square>(7));

    std::cout << "All shapes:\n";
    for (const auto &s : shapes)
      Good::printArea(*s);
  }

  return 0;
}
