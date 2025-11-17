#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <cctype>

using namespace std;

static string normalizeName(string s) {
    auto is_space = [](unsigned char c) { return std::isspace(c); };    //lambda expression
    // trim left
    auto first = find_if_not(s.begin(), s.end(), is_space);
    if (first == s.end()) return ""; 
    // trim right
    auto last = find_if_not(s.rbegin(), s.rend(), is_space).base();
    string trimmed(first, last);
    string res;
    bool prev_space = false;
    for (unsigned char c : trimmed) {
        if (is_space(c)) {
            if (!prev_space) {          
                res.push_back(' ');
                prev_space = true;
            }
        }
        else {
            res.push_back(c);
            prev_space = false;
        }
    }
    return res;
}

class PeopleBook {
private:
    map<string, int> data;  // name -> age
public:
    void addData(const string& name, int age) {
        string key = normalizeName(name);
        data[key] = age;
    }

    void updateData(const string& name, int age) {
        string key = normalizeName(name);
        auto it = data.find(key);
        if (it == data.end()) {
            cout << "Khong tim thay \"" << key << "\" de thay doi.\n";
        }
        else {
            it->second = age;
            cout << "Da cap nhat tuoi cho \"" << key << "\".\n";
        }
    }

    void removeData(const string& name) {
        string key = normalizeName(name);
        size_t n = data.erase(key);   
        if (n) cout << "\"" << key << "\" Deleted!\n";
        else   cout << "\"" << key << "\" khong ton tai.\n";
    }

    void printData() const {
        if (data.empty()) {
            cout << "No data found!\n";
            return;
        }
        for (const auto& pair : data) {
            cout << "Name: " << pair.first << ", Age: " << pair.second << '\n';
        }
    }

    bool getAge(const string& name, int& ageOut) const {
        string key = normalizeName(name);
        auto it = data.find(key);
        if (it == data.end()) return false;
        ageOut = it->second;
        return true;
    }
};

int main() {
    PeopleBook book;
    int age = 0;
    int option;
    string name;

    while (true) {
        cout << "\n1. Nhap\n2. Xoa\n3. Thay doi\n4. Xuat\n0. Thoat\n";
        cout << "Chon: ";
        if (!(cin >> option)) break; // lỗi input

        switch (option) {
        case 1: // add
            cout << "Age: ";
            cin >> age;
            cout << "Name: ";
            getline(cin >> ws, name);
            book.addData(name, age);
            break;

        case 2: // delete
            cout << "Name to delete: ";
            getline(cin >> ws, name);
            book.removeData(name);
            break;

        case 3: // update
            cout << "Name to change age: ";
            getline(cin >> ws, name);
            cout << "New Age: ";
            cin >> age;
            book.updateData(name, age);
            break;

        case 4: // print
            cout << "\n=== ADDRESS BOOK ===\n";
            book.printData();
            break;

        case 0:
            return 0;

        default:
            cout << "Lua chon khong hop le!\n";
            break;
        }
    }
    return 0;
}
