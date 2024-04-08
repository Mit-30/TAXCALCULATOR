#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <conio.h>  // Include the header for _getch

using namespace std;

string getPasswordInput();  // Declare the function here

bool createUserAccount() {
    string username, password;

    cout << "Enter a new username: ";
    cin >> username;

    ifstream userDataFile("user_data.txt");
    string line;

    while (getline(userDataFile, line)) {
        string storedUsername;
        istringstream iss(line);
        iss >> storedUsername;

        if (storedUsername == username) {
            userDataFile.close();
            cout << "Username already exists. Please choose a different username." << endl;
            return false;
        }
    }

    userDataFile.close();

    cout << "Enter a password: ";
    password = getPasswordInput();  // Call the function to get hidden password input

    ofstream userDataFileAppend("user_data.txt", ios::app);
    userDataFileAppend << username << " " << password << endl;
    userDataFileAppend.close();

    // Create a user-specific transaction history file
    ofstream historyFile((username + "_history.txt").c_str());
    historyFile.close();

    cout << "User account created successfully." << endl;
    return true;
}

bool login(string& username) {
    string password;

    cout << "Enter your username: ";
    cin >> username;

    cout << "Enter your password: ";
    password = getPasswordInput();  // Call the function to get hidden password input

    ifstream userDataFile("user_data.txt");
    string line;

    while (getline(userDataFile, line)) {
        string storedUsername, storedPassword;
        istringstream iss(line);
        iss >> storedUsername >> storedPassword;

        if (storedUsername == username && storedPassword == password) {
            userDataFile.close();
            cout << "Login successful. Welcome!" << endl;
            return true;
        }
    }

    userDataFile.close();
    cout << "Login failed. Please try again." << endl;
    return false;
}

string getPasswordInput() {
    string password;
    char ch;
    while (true) {
        ch = _getch();  // Read a character without displaying it
        if (ch == 13) // Enter key
        {
            cout << endl;
            break;
        }
        password.push_back(ch); // Add the character to the password
        cout << '*'; // Display an asterisk instead of the actual character
    }
    return password;
}

// The rest of the code remains unchanged.
void displayTaxResults(double salesTax, double incomeTax, double importTax, double payrollTax) {
    cout << "Tax Calculation Results:" << endl;
    cout << "----------------------------------------" << endl;
    cout << left << setw(20) << "Sales Tax:" << right << setw(12) << "$" << fixed << setprecision(2) << salesTax << endl;
    cout << left << setw(20) << "Income Tax:" << right << setw(12) << "$" << fixed << setprecision(2) << incomeTax << endl;
    cout << left << setw(20) << "Import Tax:" << right << setw(12) << "$" << fixed << setprecision(2) << importTax << endl;
    cout << left << setw(20) << "Payroll Tax:" << right << setw(12) << "$" << fixed << setprecision(2) << payrollTax << endl;
    cout << "----------------------------------------" << endl;
}

void displayTransactionHistory(const string& username) {
    ifstream historyFile((username + "_history.txt").c_str());
    string transaction;

    cout << "Transaction History for " << username << ":" << endl;
    cout << "----------------------------------------" << endl;

    while (getline(historyFile, transaction)) {
        cout << transaction << endl;
    }

    historyFile.close();
    cout << "----------------------------------------" << endl;
}

class TaxCalculator {
public:
    double calculateTax(double value, double rate) {
        return value * rate / 100.0;
    }
};

void processTaxData(TaxCalculator& calculator, double& salesTax, double& incomeTax, double& importTax, double& payrollTax, const string& username) {
    double purchaseAmount, salesTaxRate, income, incomeTaxRate, importValue, importTaxRate, salary;

    char inputChoice;

    cout << "Do you want to enter input manually (M) or read from a file (F)? ";
    cin >> inputChoice;

    if (inputChoice == 'M' || inputChoice == 'm') {
        // Manually enter tax data
        cout << "Enter purchase amount: ";
        cin >> purchaseAmount;
        cout << "Enter sales tax rate: ";
        cin >> salesTaxRate;
        cout << "Enter income: ";
        cin >> income;
        cout << "Enter income tax rate: ";
        cin >> incomeTaxRate;
        cout << "Enter import value: ";
        cin >> importValue;
        cout << "Enter import tax rate: ";
        cin >> importTaxRate;
        cout << "Enter salary: ";
        cin >> salary;
    } else if (inputChoice == 'F' || inputChoice == 'f') {
        ifstream inputFile("tax_input.txt");

        if (!inputFile.is_open()) {
            cout << "Error: Could not open the input file." << endl;
            return;
        }

                inputFile >> purchaseAmount >> salesTaxRate >> income >> incomeTaxRate >> importValue >> importTaxRate >> salary;
        inputFile.close();
    } else {
        cout << "Invalid input choice. Please select 'M' or 'F' for manual or file input." << endl;
        return;
    }

    salesTax = calculator.calculateTax(purchaseAmount, salesTaxRate);
    incomeTax = calculator.calculateTax(income, incomeTaxRate);
    importTax = calculator.calculateTax(importValue, importTaxRate);
    payrollTax = calculator.calculateTax(salary, 6.2);

    // Record the transaction in the user's transaction history
    ofstream historyFile((username + "_history.txt").c_str(), ios::app);
    historyFile << "Sales Tax: $" << salesTax << ", Income Tax: $" << incomeTax << ", Import Tax: $" << importTax
                << ", Payroll Tax: $" << payrollTax << endl;
    historyFile.close();

    displayTaxResults(salesTax, incomeTax, importTax, payrollTax);
}

int main() {
    cout << "Welcome to the Login System and Tax Calculator!" << endl;
    char choice;
    bool loggedIn = false;
    string username;

    srand(static_cast<unsigned>(time(0)));

    do {
        cout << "Options:" << endl;
        cout << "1. Register a new account" << endl;
        cout << "2. Log in" << endl;
        cout << "3. Calculate Tax" << endl;
        cout << "4. View Transaction History" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case '1':
                if (!loggedIn) {
                    createUserAccount();
                } else {
                    cout << "You are already logged in. Please log out to create a new account." << endl;
                }
                break;
            case '2':
                if (!loggedIn) {
                    if (login(username)) {
                        loggedIn = true;
                    }
                } else {
                    cout << "You are already logged in as " << username << ". Please log out to log in with a different account." << endl;
                }
                break;
            case '3':
                if (loggedIn) {
                    TaxCalculator taxCalculator;
                    double salesTax, incomeTax, importTax, payrollTax;

                    processTaxData(taxCalculator, salesTax, incomeTax, importTax, payrollTax, username);
                } else {
                    cout << "You must log in to calculate tax." << endl;
                }
                break;
            case '4':
                if (loggedIn) {
                    displayTransactionHistory(username);
                } else {
                    cout << "You must log in to view your transaction history." << endl;
                }
                break;
            case '5':
                cout << "Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    } while (choice != '5');

    return 0;
}