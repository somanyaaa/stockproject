#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <unordered_map>
#include <map>
#include <cmath>
#include <stdexcept>
using namespace std;

struct Product {
    int id;
    string name;
    int quantity;
    double price;
};

// Read an entire line, so incorrect input never poisons cin.
string readLine(const string& prompt) {
    cout << prompt;
    string s;
    if (!getline(cin, s))
        throw runtime_error("Input closed");
    return s;
}

int readInt(const string& prompt, int minimum) {
    while (true) {
        stringstream ss(readLine(prompt));
        int value; 
        char extra;
        if ((ss >> value) && !(ss >> extra) && value >= minimum)
            return value;
        cout << "Enter a whole number >= " << minimum << ".\n";
    }
}

double readPrice() {
    while (true) {
        stringstream ss(readLine("Unit price (INR): "));
        double value;
        char extra;

        if ((ss >> value) &&
            !(ss >> extra) &&
            isfinite(value) &&
            value >= 0 &&
            value <= 1000000000.0) {
            return value;
        }

        cout << "Enter a valid price from 0 to 1000000000.\n";
    }
}

class Inventory {
    unordered_map<int, Product> products;
    map<int, int> orderedIds;
    public:
        void add() {
            int id = readInt("Product ID: ", 1);
            if (products.count(id)) {
                cout << "Product ID already exists.\n";
                return;
            }
            string name;
            do {
                name = readLine("Product name: ");
                if (name.find_first_not_of(" \t\r") != string::npos) 
                    break;
                cout << "Name cannot be empty.\n";
            } 
            while (true);
            int quantity = readInt("Quantity: ", 0);
            double price = readPrice();
            products.emplace(id, Product{id, name, quantity, price});
            orderedIds.emplace(id, id);
            cout << "Product added successfully.\n";
        }

        void update() {
            int id = readInt("Product ID: ", 1);
            auto it = products.find(id);
            if (it == products.end()) { 
                cout << "Product not found.\n"; return; }
                int quantity = readInt("New quantity: ", 0);
            double price = readPrice();
            it->second.quantity = quantity;
            it->second.price = price;
            cout << "Stock and price updated.\n";
        }

        static void show(const Product& p) {
            cout << p.id << " | " << p.name << " | " << p.quantity << " | " << fixed << setprecision(2) << p.price << '\n';
        }

        void search() const {
            int id = readInt("Product ID: ", 1);
            auto it = products.find(id);
            if (it == products.end()) 
                cout << "Product not found.\n";
            else { 
                cout << "ID | Name | Quantity | Price (INR)\n"; show(it->second); 
            }
        }

        void remove() {
            int id = readInt("Product ID: ", 1);
            if (!products.erase(id)) { 
                cout << "Product not found.\n"; return; 
            }
            orderedIds.erase(id);
            cout << "Product deleted.\n";
        }

        void list() const {
        if (products.empty()) {
            cout << "Inventory is empty.\n"; return;
        }
        cout << "ID | Name | Quantity | Price (INR)\n";
        // Tree iteration yields ascending IDs without a separate sort.
        for (const auto& entry : orderedIds)
            show(products.at(entry.first));
        }

        void lowStock() const {
            int threshold = readInt("Low-stock threshold: ", 0);
            bool found = false;
            cout << "ID | Name | Quantity | Price (INR)\n";
            for (const auto& entry : orderedIds) {
                const auto& p = products.at(entry.first);
                if (p.quantity <= threshold) {
                    show(p); found = true;
                }
            }
            if (!found) cout << "No products meet the threshold.\n";
        }

        void value() const {
            long double total = 0;
            for (const auto& entry : products)
                total += static_cast<long double>(entry.second.quantity) * entry.second.price;
            cout << "Total inventory value (INR): " << fixed << setprecision(2) << total << '\n';
        }
    };

int main() {
    Inventory inventory;
    cout << "INVENTORY AND STOCK MANAGEMENT\n";
    try {
        while (true) {
            cout << "\n1. Add product\n2. Update stock and price\n"
            << "3. Search by ID\n4. Delete product\n5. List by ID\n"
            << "6. Low-stock report\n7. Total inventory value\n0. Exit\n";
            int choice = readInt("Choice: ", 0);
            switch (choice) {
                    case 1: inventory.add(); break;
                    case 2: inventory.update(); break;
                    case 3: inventory.search(); break;
                    case 4: inventory.remove(); break;
                    case 5: inventory.list(); break;
                    case 6: inventory.lowStock(); break;
                    case 7: inventory.value(); break;
                    case 0: cout << "Goodbye.\n"; return 0;
                    default: cout << "Choose an option from 0 to 7.\n";
                }
            }
    } 
    catch (const runtime_error&) {
        cout << "\nInput closed. Session ended.\n";
    }
}
