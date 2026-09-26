#include <iostream>
#include <iomanip>
#include <limits>
#include <map>
#include <string>

using namespace std;

class BankAccount {
private:
    long long accountNumber;
    string holderName;
    string pin;
    double balance;

public:
    BankAccount() : accountNumber(0), balance(0.0) {}

    BankAccount(long long number, const string& name, const string& accountPin,
                double initialBalance)
        : accountNumber(number), holderName(name), pin(accountPin), balance(initialBalance) {}

    long long getAccountNumber() const { return accountNumber; }
    const string& getHolderName() const { return holderName; }
    double getBalance() const { return balance; }

    bool authenticate(const string& enteredPin) const {
        return pin == enteredPin;
    }

    void deposit(double amount) {
        balance += amount;
    }

    bool withdraw(double amount) {
        if (amount <= 0 || amount > balance) {
            return false;
        }
        balance -= amount;
        return true;
    }

    void display() const {
        cout << "\nAccount number : " << accountNumber
             << "\nAccount holder : " << holderName
             << "\nBalance        : $" << fixed << setprecision(2) << balance << '\n';
    }
};

class Bank {
private:
    map<long long, BankAccount> accounts;
    long long nextAccountNumber = 1001;

    static void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    long long readAccountNumber() const {
        long long number;
        while (true) {
            cout << "Enter account number: ";
            if (cin >> number && number > 0) {
                return number;
            }
            cout << "Invalid account number. Try again.\n";
            clearInput();
        }
    }

    double readPositiveAmount(const string& prompt) const {
        double amount;
        while (true) {
            cout << prompt;
            if (cin >> amount && amount > 0) {
                return amount;
            }
            cout << "Amount must be greater than zero. Try again.\n";
            clearInput();
        }
    }

    BankAccount* login() {
        long long number = readAccountNumber();
        auto account = accounts.find(number);

        if (account == accounts.end()) {
            cout << "Account not found.\n";
            return nullptr;
        }

        string enteredPin;
        cout << "Enter PIN: ";
        cin >> enteredPin;
        if (!account->second.authenticate(enteredPin)) {
            cout << "Incorrect PIN.\n";
            return nullptr;
        }

        return &account->second;
    }

public:
    void createAccount() {
        string name;
        string pin;

        clearInput();
        cout << "Enter account holder name: ";
        getline(cin, name);

        do {
            cout << "Create a 4-digit PIN: ";
            cin >> pin;
            if (pin.size() != 4 || pin.find_first_not_of("0123456789") != string::npos) {
                cout << "PIN must contain exactly 4 digits.\n";
            }
        } while (pin.size() != 4 || pin.find_first_not_of("0123456789") != string::npos);

        double initialDeposit;
        while (true) {
            cout << "Enter initial deposit (0 allowed): ";
            if (cin >> initialDeposit && initialDeposit >= 0) {
                break;
            }
            cout << "Deposit cannot be negative. Try again.\n";
            clearInput();
        }

        long long number = nextAccountNumber++;
        accounts.emplace(number, BankAccount(number, name, pin, initialDeposit));

        cout << "\nAccount created successfully!\n"
             << "Your account number is: " << number << '\n';
    }

    void depositMoney() {
        BankAccount* account = login();
        if (account == nullptr) return;

        double amount = readPositiveAmount("Enter deposit amount: $");
        account->deposit(amount);
        cout << "Deposit successful. New balance: $"
             << fixed << setprecision(2) << account->getBalance() << '\n';
    }

    void withdrawMoney() {
        BankAccount* account = login();
        if (account == nullptr) return;

        double amount = readPositiveAmount("Enter withdrawal amount: $");
        if (account->withdraw(amount)) {
            cout << "Withdrawal successful. New balance: $"
                 << fixed << setprecision(2) << account->getBalance() << '\n';
        } else {
            cout << "Withdrawal failed: insufficient funds.\n";
        }
    }

    void checkBalance() {
        BankAccount* account = login();
        if (account != nullptr) {
            cout << "Current balance: $" << fixed << setprecision(2)
                 << account->getBalance() << '\n';
        }
    }

    void showAccount() {
        BankAccount* account = login();
        if (account != nullptr) {
            account->display();
        }
    }

    void run() {
        int choice;

        do {
            cout << "\n========== BANKING SYSTEM ==========\n"
                 << "1. Create account\n"
                 << "2. Deposit money\n"
                 << "3. Withdraw money\n"
                 << "4. Check balance\n"
                 << "5. Show account details\n"
                 << "0. Exit\n"
                 << "====================================\n"
                 << "Choose an option: ";

            if (!(cin >> choice)) {
                cout << "Please enter a valid menu option.\n";
                clearInput();
                continue;
            }

            switch (choice) {
                case 1: createAccount(); break;
                case 2: depositMoney(); break;
                case 3: withdrawMoney(); break;
                case 4: checkBalance(); break;
                case 5: showAccount(); break;
                case 0: cout << "Thank you for using the banking system.\n"; break;
                default: cout << "Invalid option. Try again.\n";
            }
        } while (choice != 0);
    }
};

int main() {
    Bank bank;
    bank.run();
    return 0;
}
