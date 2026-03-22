// ============================================================
// Interface Segregation Principle (ISP)
// "Clients should not be forced to depend on interfaces
//  they do not use."
// ============================================================
//
// BAD EXAMPLE  – A single fat IWorker interface forces Robot
//                to implement eat(), which makes no sense.
//
// GOOD EXAMPLE – Split into small, cohesive interfaces so each
//                class only implements what it actually needs.
// ============================================================

#include <iostream>
#include <string>
#include <memory>
#include <vector>

// ===================== BAD EXAMPLE ===========================
// One fat interface — every worker must implement ALL methods,
// even if some are irrelevant.

namespace Bad
{

  class IWorker
  {
  public:
    virtual ~IWorker() = default;
    virtual void work() = 0;
    virtual void eat() = 0;   // Not every worker eats!
    virtual void sleep() = 0; // Not every worker sleeps!
  };

  class HumanWorker : public IWorker
  {
  public:
    void work() override { std::cout << "  Human working\n"; }
    void eat() override { std::cout << "  Human eating\n"; }
    void sleep() override { std::cout << "  Human sleeping\n"; }
  };

  class RobotWorker : public IWorker
  {
  public:
    void work() override { std::cout << "  Robot working\n"; }

    // Forced to implement methods that make no sense for a robot!
    void eat() override { /* does nothing — violates ISP */ }
    void sleep() override { /* does nothing — violates ISP */ }
  };

} // namespace Bad

// ===================== GOOD EXAMPLE ==========================
// Small, focused interfaces — classes implement only what they need.

namespace Good
{

  class IWorkable
  {
  public:
    virtual ~IWorkable() = default;
    virtual void work() = 0;
  };

  class IFeedable
  {
  public:
    virtual ~IFeedable() = default;
    virtual void eat() = 0;
  };

  class ISleepable
  {
  public:
    virtual ~ISleepable() = default;
    virtual void sleep() = 0;
  };

  // Human implements all three interfaces.
  class HumanWorker : public IWorkable, public IFeedable, public ISleepable
  {
  public:
    void work() override { std::cout << "  Human working\n"; }
    void eat() override { std::cout << "  Human eating\n"; }
    void sleep() override { std::cout << "  Human sleeping\n"; }
  };

  // Robot only implements IWorkable — no dummy methods needed.
  class RobotWorker : public IWorkable
  {
  public:
    void work() override { std::cout << "  Robot working\n"; }
  };

  // Functions depend only on the interface they actually need.
  void doWork(IWorkable &worker) { worker.work(); }
  void doEat(IFeedable &feeder) { feeder.eat(); }
  void doSleep(ISleepable &sleeper) { sleeper.sleep(); }

} // namespace Good

// ======================== MAIN ===============================
int main()
{
  std::cout << "=== BAD EXAMPLE (violates ISP) ===\n";
  {
    Bad::HumanWorker human;
    Bad::RobotWorker robot;

    std::cout << "Human:\n";
    human.work();
    human.eat();

    std::cout << "Robot:\n";
    robot.work();
    robot.eat(); // Compiles, but semantically wrong
  }

  std::cout << "\n=== GOOD EXAMPLE (follows ISP) ===\n";
  {
    Good::HumanWorker human;
    Good::RobotWorker robot;

    std::cout << "Human:\n";
    Good::doWork(human);
    Good::doEat(human);
    Good::doSleep(human);

    std::cout << "Robot:\n";
    Good::doWork(robot);
    // Good::doEat(robot);   // Won't compile! Robot is not IFeedable.
    // Good::doSleep(robot); // Won't compile! Robot is not ISleepable.
    std::cout << "  (Robot can't eat or sleep — and the compiler enforces it!)\n";
  }

  return 0;
}
