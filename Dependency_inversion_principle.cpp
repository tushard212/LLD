// ============================================================
// Dependency Inversion Principle (DIP)
// "High-level modules should not depend on low-level modules.
//  Both should depend on abstractions."
// ============================================================
//
// BAD EXAMPLE  – NotificationService directly depends on
//                concrete EmailSender — tightly coupled.
//
// GOOD EXAMPLE – NotificationService depends on an abstract
//                IMessageSender. Concrete senders are injected.
// ============================================================

#include <iostream>
#include <string>
#include <memory>
#include <vector>

// ===================== BAD EXAMPLE ===========================
// High-level module (NotificationService) directly creates and
// uses a low-level module (EmailSender).
// Adding SMS support requires modifying NotificationService.

namespace Bad
{

  class EmailSender
  {
  public:
    void sendEmail(const std::string &to, const std::string &msg)
    {
      std::cout << "  [Email] To: " << to << " | " << msg << "\n";
    }
  };

  class NotificationService
  {
    EmailSender emailSender; // Direct dependency on concrete class!
  public:
    void notify(const std::string &to, const std::string &msg)
    {
      emailSender.sendEmail(to, msg);
      // What if we also want SMS? Must modify THIS class — violates DIP.
    }
  };

} // namespace Bad

// ===================== GOOD EXAMPLE ==========================
// Both high-level and low-level modules depend on an abstraction.
// New senders can be added without changing NotificationService.

namespace Good
{

  // Abstraction — the contract
  class IMessageSender
  {
  public:
    virtual ~IMessageSender() = default;
    virtual void send(const std::string &to, const std::string &msg) = 0;
    virtual std::string channelName() const = 0;
  };

  // Low-level module 1
  class EmailSender : public IMessageSender
  {
  public:
    void send(const std::string &to, const std::string &msg) override
    {
      std::cout << "  [Email] To: " << to << " | " << msg << "\n";
    }
    std::string channelName() const override { return "Email"; }
  };

  // Low-level module 2
  class SmsSender : public IMessageSender
  {
  public:
    void send(const std::string &to, const std::string &msg) override
    {
      std::cout << "  [SMS]   To: " << to << " | " << msg << "\n";
    }
    std::string channelName() const override { return "SMS"; }
  };

  // Low-level module 3 — added later, NO change to existing code
  class PushNotificationSender : public IMessageSender
  {
  public:
    void send(const std::string &to, const std::string &msg) override
    {
      std::cout << "  [Push]  To: " << to << " | " << msg << "\n";
    }
    std::string channelName() const override { return "Push Notification"; }
  };

  // High-level module — depends only on abstraction (IMessageSender)
  class NotificationService
  {
    std::vector<std::shared_ptr<IMessageSender>> senders;

  public:
    void addChannel(std::shared_ptr<IMessageSender> sender)
    {
      senders.push_back(std::move(sender));
    }

    void notify(const std::string &to, const std::string &msg)
    {
      for (const auto &sender : senders)
      {
        sender->send(to, msg);
      }
    }

    void listChannels() const
    {
      std::cout << "  Active channels: ";
      for (size_t i = 0; i < senders.size(); ++i)
      {
        if (i > 0)
          std::cout << ", ";
        std::cout << senders[i]->channelName();
      }
      std::cout << "\n";
    }
  };

} // namespace Good

// ======================== MAIN ===============================
int main()
{
  std::cout << "=== BAD EXAMPLE (violates DIP) ===\n";
  {
    Bad::NotificationService service;
    service.notify("alice@example.com", "Hello from bad design!");
    // Can only send emails. Adding SMS means editing the class.
  }

  std::cout << "\n=== GOOD EXAMPLE (follows DIP) ===\n";
  {
    Good::NotificationService service;

    // Inject dependencies — easy to add/remove channels
    service.addChannel(std::make_shared<Good::EmailSender>());
    service.addChannel(std::make_shared<Good::SmsSender>());
    service.addChannel(std::make_shared<Good::PushNotificationSender>());

    service.listChannels();
    service.notify("alice@example.com", "Hello from good design!");
  }

  return 0;
}
