// run_backend.cpp
#include <iostream>
#include <string>
#include "address_book.h"

using namespace std;

int main() {
    PeopleBook book;

    while (true) {
        cout << "\n1. Nhap (them moi)\n"
            << "2. Xoa\n"
            << "3. Thay doi (ghi de ho so)\n"
            << "4. Xuat\n"
            << "5. Filter\n"
            << "0. Thoat\n";
        cout << "Chon: ";

        int option;
        if (!(cin >> option)) break;

        string name;
        switch (option) {
        case 1: { // add
            dataEmployee p;
            cout << "Name: ";       getline(cin >> ws, name);
            cout << "Age: ";        cin >> p.age;
            cout << "Address: ";    getline(cin >> ws, p.address);
            cout << "Phone: ";      getline(cin, p.phone);
            cout << "Title: ";      getline(cin, p.title);
            cout << "Salary: ";     getline(cin, p.salary);

            book.addData(name, p);
            break;
        }

        case 2: { // delete
            cout << "Name to delete: ";
            getline(cin >> ws, name);
            book.removeData(name);
            break;
        }

        case 3: { // update (overwrite whole profile)
            dataEmployee p;
            cout << "Name to change: "; getline(cin >> ws, name);
            cout << "New Age: ";        cin >> p.age;
            cout << "New Address: ";    getline(cin >> ws, p.address);
            cout << "New Phone: ";      getline(cin, p.phone);
            cout << "New Title: ";      getline(cin, p.title);
            cout << "New Salary: ";     getline(cin, p.salary);

            book.updateData(name, p); // hàm đã tự báo nếu không tìm thấy
            break;
        }

        case 4:
            cout << "\n=== ADDRESS BOOK ===\n";
            book.printData();
            break;

        case 5: {
            cout << "1. Loc theo do tuoi\n"
                << "2. Loc theo chuc danh (title)\n"
                << "3. Loc theo tu khoa dia chi\n";
            int sub; cin >> sub;

            if (sub == 1) {
                int minA, maxA;
                cout << "Min age: "; cin >> minA;
                cout << "Max age: "; cin >> maxA;
                auto list = book.filterByAgeRange(minA, maxA);
                for (auto& [k, emp] : list) {
                    cout << "Name: " << k
                        << ", Age: " << emp.age
                        << ", Addr: " << emp.address
                        << ", Phone: " << emp.phone
                        << ", Title: " << emp.title
                        << ", Salary: " << emp.salary << '\n';
                }
            }
            else if (sub == 2) {
                string title;
                cout << "Title: ";
                getline(cin >> ws, title);
                auto list = book.filterByTitle(title);
                for (auto& [k, emp] : list) {
                    cout << "Name: " << k
                        << ", Age: " << emp.age
                        << ", Addr: " << emp.address
                        << ", Phone: " << emp.phone
                        << ", Title: " << emp.title
                        << ", Salary: " << emp.salary << '\n';
                }
            }
            else if (sub == 3) {
                string kw;
                cout << "Keyword in address: ";
                getline(cin >> ws, kw);
                auto list = book.filterByAddressKeyword(kw);
                for (auto& [k, emp] : list) {
                    cout << "Name: " << k
                        << ", Age: " << emp.age
                        << ", Addr: " << emp.address
                        << ", Phone: " << emp.phone
                        << ", Title: " << emp.title
                        << ", Salary: " << emp.salary << '\n';
                }
            }
            break;
        }

        case 0:
            cout << "Thoat chuong trinh...\n";
            return 0;

        default:
            cout << "Lua chon khong hop le!\n";
            break;
        }
    }

    return 0;
}
