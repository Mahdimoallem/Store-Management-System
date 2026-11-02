# 🛒 Store Management System

A console-based Store Management System written in C++ using Object-Oriented Programming. It lets you manage products, prices and inventory stock, and saves all data automatically to a text file.

---

## ✨ Features

- ➕ Add new products (name, price, quantity)
- 📋 Show all products in a clean table
- 💲 Update product prices
- 📦 Increase or decrease stock (negative stock is prevented)
- 🗑️ Remove products
- 🔍 Search products by name (case-insensitive)
- ⚠️ Low stock report
- 💰 Total inventory value
- 💾 Automatic saving and loading from products.txt
- ✅ Input validation (invalid input never crashes the program)

---

## 🧱 Project Design

The project is split into three classes, each with a single responsibility:

| Class | Responsibility |
|-------|----------------|
| Product | Represents one product (id, name, price, quantity) and converts itself to/from a file line |
| Inventory | Stores all products, performs operations (add, update, remove, search) and handles file saving/loading |
| StoreApp | Shows the menu, reads user input and calls Inventory |

OOP concepts used:

- Classes and objects
- Encapsulation (private data with getters/setters)
- Constructors and initializer lists
- static member functions
- const correctness

---

## 🚀 Getting Started

### Requirements

- A C++ compiler with C++11 support (GCC, MinGW, ...)
- Or an IDE such as Code::Blocks

### Build and Run

Using g++:

g++ -std=c++11 main.cpp -o store
./store
Using Code::Blocks:

1. Open main.cpp (or create a new Console Application project)
2. Press F9 (Build and Run)

---

## 📖 Usage

When the program starts, you will see this menu:

`
===== Store Man
