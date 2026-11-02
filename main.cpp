#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>
using namespace std;

const int LOW_STOCK_LIMIT = 5;

// =======================================================
// Product: represents a single product
// =======================================================
class Product {
private:
  int id;
  string name;
  double price;
  int quantity;

public:
    Product(int id, const string& name, double price, int quantity)
      : id(id), name(name), price(price), quantity(quantity) {}
      //Getters
      int getId() const {return id; }
      string getName() const {return name; }
      double getPrice() const {return price; }
      int getQuantity() const {return quantity; }
      double getTotalValue() const {return price * quantity; }

      //Setters
      void setPrice(double newPrice) {price = newPrice; }

      //Add (positive) or remove (negative) stock.
      //Returns false if stock would become negative.
      bool changeQuantity(int amount) {
          if (quantity + amount < 0)return false;
          quantity += amount;
          return true;
          }

          //Convert to / from one line of the data file
          string toFileLine() const {
          return to_string(id) + ";" + name + ";" + to_string(price) + ";" + to_string(quantity);
          }
          static Product fromFileLine(const string& line){
              stringstream ss(line);
              string idStr , name , priceStr , qtyStr;
              getline(ss, idStr, ';');
              getline(ss, name, ';');
              getline(ss, priceStr, ';');
              getline(ss, qtyStr, ';');
              return Product(stoi(idStr), name, stod(priceStr), stoi(qtyStr));
          }
          void printRow() const {
          cout << left << setw(6) << id << setw(25) << name << right << setw(12) << fixed << setprecision(2) << price << setw(8) << quantity << "\n";
          }
};
// =======================================================
// Inventory: stores products + saves/loads them
// =======================================================
class Inventory {
private:
    vector<Product> products;
    string fileName;

    int findIndexById(int id) const{
        for (size_t i = 0; i < products.size();i++){
           if (products[i].getId() == id) return (int)i;
        }
    return - 1;
    }

    int nextId() const {
       int maxId = 0;
       for (const Product& p : products) {
            if (p.getId() > maxId) maxId = p.getId();
       }
       return maxId + 1;
    }
    static string toLower(string text){
        transform(text.begin(), text.end(), text.begin(),
                   [](unsigned char c) {return tolower(c); });
        return text;
    }
    void load() {
    ifstream file(fileName);
    string line;
    while (getline(file, line)){
        if (!line.empty()){
                products.push_back(Product::fromFileLine(line));
         }
      }
  }
    void save() const{
      ofstream file (fileName);
      for (const Product& p :products){
        file << p.toFileLine() << "\n";
      }
}
public:
    Inventory(const string& fileName) : fileName(fileName){
        load();
    }
    int addProduct(const string& name, double price, int quantity) {
         int id = nextId();
         products.push_back(Product(id, name, price, quantity));
         save();
         return id;
    }
    bool updatePrice(int id, double newPrice){
         int index = findIndexById(id);
         if (index == -1)return false;
         products[index].setPrice(newPrice);
         save();
         return true;
    }
    bool changeStock(int id, int amount){
        int index = findIndexById(id);
        if (index == -1) return false;
        if (!products[index].changeQuantity(amount)) return false;
        save();
        return true;
    }
    bool removeProduct(int id){
        int index = findIndexById(id);
        if(index == -1) return false;
        products.erase(products.begin() + index);
        save();
        return true;
    }
    vector<Product> search(const string& keyword)const {
         vector<Product> result;
         string key = toLower(keyword);
         for (const Product& p : products){
            if (toLower(p.getName()).find(key) != string::npos){
                result.push_back(p);
            }
         }
         return result;
    }
    vector<Product> getLowStock(int limit)const{
          vector<Product> result;
          for (const Product& p : products) {
            if (p.getQuantity() <= limit) result.push_back(p);
          }
          return result;
    }
    double getTotalValue() const {
          double total = 0;
          for (const Product& p : products) total += p.getTotalValue();
          return total;
    }
    const vector<Product>& getAll() const {return products; }
};

// =======================================================
// StoreApp: menu and user interaction
// =======================================================
class StoreApp{
private:
    Inventory inventory;

    // ---- input helpers ----
    static int readInt(const string& prompt, int minValue = 0) {
         int value;
         while (true){
            cout << prompt;
            if (cin >> value && value >= minValue){
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
            cout << "Invalid input, try again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
         }
    }
    static double readDouble(const string& prompt, double minValue = 0){
         double value;
         while (true) {
            cout << prompt;
            if (cin >> value && value >= minValue){
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                return value;
            }
            cout << "Invalid input, try again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
         }
    }
    static string readtext(const string& prompt){
         string text;
         while (true){
            cout << prompt;
            getline(cin, text);
            if (!text.empty()) {
                replace(text.begin(), text.end(), ';', ' ');
                return text;
            }
            cout << "This field cannot be empty.\n";
         }
    }
    // ---- display ----
    static void printTable(const vector<Product>& list){
        if (list.empty()){
            cout << "\nNo products found.\n";
            return;
        }
        cout << "\n" << left << setw(6) << "ID" << setw(25) << "Name" << right << setw(12) << "Price" << setw(8) << "Stock" << "\n";
        cout << string(51, '-') << "\n";
        for (const Product& p : list) p.printRow();
    }
    static void showMenu(){
        cout << "\n===== Store Management System =====\n"
             << "1. Add product\n"
             << "2. Show all products\n"
             << "3. Update price\n"
             << "4. Change stock\n"
             << "5. Remove product\n"
             << "6. Search product\n"
             << "7. Low stock report\n"
             << "8. Total inventory value\n"
             << "0. Exit\n";
    }
    // ---- menu actions ----
    void addProduct(){
        string name = readtext("Product name: ");
        double price = readDouble("price: ");
        int quantity = readInt("Quantity: ");
        int id = inventory.addProduct(name, price, quantity);
        cout << "product added with ID " << id << "\n";
    }

    void updatePrice(){
        int id = readInt("Product ID: ", 1);
        double newPrice = readDouble("New price: ");
        if (inventory.updatePrice(id, newPrice))
            cout << "Price updated.\n";
        else
            cout << "Price not found.\n";
    }

    void changeStock(){
        int id = readInt("Price ID: ", 1);
        int amount = readInt("Amount (negative to remove): ", -1000000);
        if (inventory.changeStock(id, amount))
            cout << "Stock updated.\n";
        else
            cout << "Product not found, or not enough stock,\n";
    }

    void removeProduct(){
        int id = readInt("Product ID: ", 1);
        if (inventory.removeProduct(id))
            cout << "Product removed.\n";
        else
            cout << "Product not found.\n";
    }

    void searchProduct(){
        string keyword = readtext("Search keyword: ");
        printTable(inventory.search(keyword));
    }

    void lowStockReport(){
        cout << "\nProducts with stock <= " << LOW_STOCK_LIMIT << ":";
        printTable(inventory.getLowStock(LOW_STOCK_LIMIT));
    }

    void totalValue(){
        cout << "\nTotal inventory value: " << fixed << setprecision(2) << inventory.getTotalValue() << "\n";
    }

public:
    StoreApp() : inventory("products.txt"){}

    void run(){
        int choice;
        do{
            showMenu();
            choice = readInt("Choose an option: ");

            switch(choice){
                 case 1: addProduct();                    break;
                 case 2: printTable(inventory.getAll());   break;
                 case 3: updatePrice();                    break;
                 case 4: changeStock();                    break;
                 case 5: removeProduct();                  break;
                 case 6: searchProduct();                  break;
                 case 7: lowStockReport();                 break;
                 case 8: totalValue();                     break;
                 case 0: cout << "Goodbye!\n";             break;
                 default: cout << "Invalid option.\n";
            }
        }while (choice != 0);
    }
};
int main(){
    StoreApp app;
    app.run();
    return 0;
}
