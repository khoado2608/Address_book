#include "address_book.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

// ====================== Helper functions (internal) ======================

static inline bool is_ws(unsigned char c) { return std::isspace(c); }

// dùng để tách từ theo khoảng trắng / _ / - / . / ,
static inline bool is_sep(unsigned char c) {
    return c == ' ' || c == '_' || c == '-' || c == '/' || c == ',' || c == '.';
}

static std::string toLower(std::string s) {
    for (unsigned char c : s) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return s;
}


// Chuẩn hóa key: gộp mọi loại separator thành '_', bỏ separator đầu/cuối, lowercase.
// Ví dụ:
//  "Giam_Doc"   -> "giam_doc"
//  "giam doc "  -> "giam_doc"
//  "giam__DOC_" -> "giam_doc"
static std::string normalizeKey(const std::string& s) {
    std::string out;
    out.reserve(s.size());

    bool prev_sep = false;
    for (unsigned char ch : s) {
        if (is_sep(ch)) {
            if (!prev_sep && !out.empty()) {
                out.push_back('_');
            }
            prev_sep = true;
        }
        else {
            out.push_back(static_cast<char>(std::tolower(ch)));
            prev_sep = false;
        }
    }

    if (!out.empty() && out.back() == '_')
        out.pop_back();

    return out;
}

// Chuẩn hóa Name để lưu key trong map:
// - trim khoảng trắng
// - collapse nhiều khoảng trắng thành 1 space
// - mỗi từ viết hoa chữ cái đầu, các chữ sau lowercase
// - vẫn giữ dấu '_' nếu có (VD: Pham_Thanh_Vinh)
static std::string normalizeName(std::string s) {
    // trim
    auto first = std::find_if_not(s.begin(), s.end(), is_ws);
    if (first == s.end()) return "";
    auto last = std::find_if_not(s.rbegin(), s.rend(), is_ws).base();
    std::string t(first, last);

    // collapse space
    std::string tmp;
    tmp.reserve(t.size());
    bool prev_space = false;
    for (unsigned char c : t) {
        if (is_ws(c)) {
            if (!prev_space) {
                tmp.push_back(' ');
                prev_space = true;
            }
        }
        else {
            tmp.push_back(static_cast<char>(c));
            prev_space = false;
        }
    }

    // capitalize mỗi từ (ngăn bởi space hoặc '_')
    std::string out;
    out.reserve(tmp.size());
    bool newWord = true;
    for (unsigned char c : tmp) {
        if (c == ' ' || c == '_') {
            out.push_back(static_cast<char>(c));
            newWord = true;
        }
        else {
            if (newWord) {
                out.push_back(static_cast<char>(std::toupper(c)));
                newWord = false;
            }
            else {
                out.push_back(static_cast<char>(std::tolower(c)));
            }
        }
    }
    return out;
}

// dùng cho so sánh title
static std::string normalizeTitleKey(const std::string& s) {
    return normalizeKey(s);
}

// parse lương: giữ lại toàn bộ chữ số, bỏ dấu . , khoảng trắng, chữ
// "10.000.000"   -> 10000000
// "12,500,000đ"  -> 12500000
static long long parseSalary(const std::string& s) {
    std::string digits;
    digits.reserve(s.size());
    for (unsigned char c : s) {
        if (std::isdigit(c)) {
            digits.push_back(static_cast<char>(c));
        }
    }
    if (digits.empty()) return -1;

    try {
        return std::stoll(digits);
    }
    catch (...) {
        return -1;
    }
}

// ====================== PeopleBook - CRUD ======================

void PeopleBook::addData(const std::string& name, const dataEmployee& emp) {
    std::string key = normalizeName(name);
    data[key] = emp;
}

bool PeopleBook::updateData(const std::string& name, const dataEmployee& emp) {
    std::string key = normalizeName(name);
    auto it = data.find(key);
    if (it == data.end()) return false;
    it->second = emp;
    return true;
}

bool PeopleBook::removeData(const std::string& name) {
    std::string key = normalizeName(name);
    return data.erase(key) > 0;
}

void PeopleBook::printData() const {
    if (data.empty()) {
        std::cout << "No data.\n";
        return;
    }
    for (const auto& [name, emp] : data) {
        std::cout << "Name: " << name
            << ", Age: " << emp.age
            << ", Address: " << emp.address
            << ", Phone: " << emp.phone
            << ", Title: " << emp.title
            << ", Salary: " << emp.salary
            << '\n';
    }
}

bool PeopleBook::getAge(const std::string& name, int& ageOut) const {
    std::string key = normalizeName(name);
    auto it = data.find(key);
    if (it == data.end()) return false;
    ageOut = it->second.age;
    return true;
}

bool PeopleBook::getEmployee(const std::string& name, dataEmployee& out) const {
    std::string key = normalizeName(name);
    auto it = data.find(key);
    if (it == data.end()) return false;
    out = it->second;
    return true;
}

std::vector<std::pair<std::string, dataEmployee>> PeopleBook::getAll() const {
    std::vector<std::pair<std::string, dataEmployee>> res;
    res.reserve(data.size());
    for (const auto& p : data) {
        res.push_back(p);
    }
    return res;
}

// ====================== PeopleBook - filters ======================

std::vector<std::pair<std::string, dataEmployee>>
PeopleBook::filterByAgeRange(int minAge, int maxAge) const {
    if (minAge > maxAge) std::swap(minAge, maxAge);

    std::vector<std::pair<std::string, dataEmployee>> result;
    for (const auto& [name, emp] : data) {
        if (emp.age >= minAge && emp.age <= maxAge) {
            result.emplace_back(name, emp);
        }
    }
    return result;
}

std::vector<std::pair<std::string, dataEmployee>>
PeopleBook::filterByTitle(const std::string& title) const {
    std::string key = normalizeTitleKey(title);

    std::vector<std::pair<std::string, dataEmployee>> result;
    for (const auto& [name, emp] : data) {
        if (normalizeTitleKey(emp.title) == key) {
            result.emplace_back(name, emp);
        }
    }
    return result;
}

std::vector<std::pair<std::string, dataEmployee>>
PeopleBook::filterByAddressKeyword(const std::string& keyword) const {
    std::string needle = toLower(keyword);

    std::vector<std::pair<std::string, dataEmployee>> result;
    for (const auto& [name, emp] : data) {
        std::string hay = toLower(emp.address);
        if (hay.find(needle) != std::string::npos) {
            result.emplace_back(name, emp);
        }
    }
    return result;
}

std::vector<std::pair<std::string, dataEmployee>>
PeopleBook::filterBySalaryRange(long long minSalary, long long maxSalary) const {
    if (minSalary > maxSalary) std::swap(minSalary, maxSalary);

    std::vector<std::pair<std::string, dataEmployee>> result;
    for (const auto& [name, emp] : data) {
        long long s = parseSalary(emp.salary);
        if (s < 0) continue;
        if (s >= minSalary && s <= maxSalary) {
            result.emplace_back(name, emp);
        }
    }
    return result;
}

// ====================== PeopleBook - CSV ======================
// Định dạng CSV: Name,Age,Address,Phone,Title,Salary

bool PeopleBook::loadFromCsv(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        return false;
    }

    data.clear();

    std::string line;
    bool first = true;
    while (std::getline(in, line)) {
        if (line.empty()) continue;

        // bỏ dòng header nếu có
        if (first) {
            first = false;
            if (line.rfind("Name,", 0) == 0) {
                continue;
            }
        }

        std::stringstream ss(line);
        std::string name, ageStr, addr, phone, title, salary;

        std::getline(ss, name, ',');
        std::getline(ss, ageStr, ',');
        std::getline(ss, addr, ',');
        std::getline(ss, phone, ',');
        std::getline(ss, title, ',');
        std::getline(ss, salary);           // phần còn lại

        if (name.empty()) continue;

        dataEmployee emp;
        try {
            emp.age = ageStr.empty() ? 0 : std::stoi(ageStr);
        }
        catch (...) {
            emp.age = 0;
        }

        emp.address = addr;
        emp.phone = phone;
        emp.title = title;
        emp.salary = salary;

        addData(name, emp);
    }

    return true;
}

bool PeopleBook::saveToCsv(const std::string& path) const {
    std::ofstream out(path);
    if (!out.is_open()) {
        return false;
    }

    out << "Name,Age,Address,Phone,Title,Salary\n";
    for (const auto& [name, emp] : data) {
        out << name << ','
            << emp.age << ','
            << emp.address << ','
            << emp.phone << ','
            << emp.title << ','
            << emp.salary << '\n';
    }

    return true;
}
