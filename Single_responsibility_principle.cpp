// ============================================================
// Single Responsibility Principle (SRP)
// "A class should have only one reason to change."
// ============================================================
//
// BAD EXAMPLE  – Invoice class handles calculation, printing,
//                AND saving to a database. Three reasons to change!
//
// GOOD EXAMPLE – Each responsibility is split into its own class.
// ============================================================

#include <iostream>
#include <string>
#include <fstream>

// ===================== BAD EXAMPLE ===========================
// This class violates SRP because it handles:
//   1. Invoice data & calculation
//   2. Printing
//   3. Persistence (saving to file)

namespace Bad
{

  class Invoice
  {
    std::string customer;
    double amount;
    double taxRate;

  public:
    Invoice(const std::string &customer, double amount, double taxRate)
        : customer(customer), amount(amount), taxRate(taxRate) {}

    // Responsibility 1: Business logic
    double calculateTotal() const
    {
      return amount + (amount * taxRate);
    }

    // Responsibility 2: Presentation / printing
    void printInvoice() const
    {
      std::cout << "---- Invoice ----\n";
      std::cout << "Customer : " << customer << "\n";
      std::cout << "Amount   : " << amount << "\n";
      std::cout << "Tax      : " << (amount * taxRate) << "\n";
      std::cout << "Total    : " << calculateTotal() << "\n";
      std::cout << "-----------------\n";
    }

    // Responsibility 3: Persistence
    void saveToFile(const std::string &filename) const
    {
      std::ofstream file(filename);
      file << customer << "," << amount << "," << taxRate << "\n";
      std::cout << "Invoice saved to " << filename << "\n";
    }
  };

} // namespace Bad

// ===================== GOOD EXAMPLE ==========================
// Each class has exactly ONE responsibility.

namespace Good
{

  // Responsibility 1: Hold invoice data & business logic only
  class Invoice
  {
    std::string customer;
    double amount;
    double taxRate;

  public:
    Invoice(const std::string &customer, double amount, double taxRate)
        : customer(customer), amount(amount), taxRate(taxRate) {}

    double getAmount() const { return amount; }
    double getTaxRate() const { return taxRate; }
    std::string getCustomer() const { return customer; }

    double calculateTotal() const
    {
      return amount + (amount * taxRate);
    }
  };

  // Responsibility 2: Printing / presentation
  class InvoicePrinter
  {
  public:
    void print(const Invoice &inv) const
    {
      std::cout << "---- Invoice ----\n";
      std::cout << "Customer : " << inv.getCustomer() << "\n";
      std::cout << "Amount   : " << inv.getAmount() << "\n";
      std::cout << "Tax      : " << (inv.getAmount() * inv.getTaxRate()) << "\n";
      std::cout << "Total    : " << inv.calculateTotal() << "\n";
      std::cout << "-----------------\n";
    }
  };

  // Responsibility 3: Persistence / saving
  class InvoiceRepository
  {
  public:
    void saveToFile(const Invoice &inv, const std::string &filename) const
    {
      std::ofstream file(filename);
      file << inv.getCustomer() << ","
           << inv.getAmount() << ","
           << inv.getTaxRate() << "\n";
      std::cout << "Invoice saved to " << filename << "\n";
    }
  };

} // namespace Good

// ======================== MAIN ===============================
int main()
{
  std::cout << "=== BAD EXAMPLE (violates SRP) ===\n\n";
  {
    Bad::Invoice inv("Alice", 1000.0, 0.18);
    inv.printInvoice();
    inv.saveToFile("invoice_bad.csv");
  }

  std::cout << "\n=== GOOD EXAMPLE (follows SRP) ===\n\n";
  {
    Good::Invoice inv("Alice", 1000.0, 0.18);
    Good::InvoicePrinter printer;
    Good::InvoiceRepository repo;

    printer.print(inv);
    repo.saveToFile(inv, "invoice_good.csv");
  }

  return 0;
}
