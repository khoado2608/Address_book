#include "address_book.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

// ====================== Helper functions (internal) ======================

static inline bool is_ws(unsigned char c) { return std::isspace(c); }

static inline bool is_sep(unsigned char c) {
    return c == ' ' || c == '_' || c == '-' || c == '/' || c == ',' || c == '.';
}

static std::string toLower(std::string s) {
    for (auto& c : s) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return s;
}

// Capitalize first letter of each word
static std::string capitalizeEachWord(std::string s) {
    bool newWord = true;
    for (auto& c : s) {
        if (std::isspace(c) || c == '_' || c == '-') {
            newWord = true;
        }
        else {
            if (newWord) {
                c = static_cast<char>(std::toupper(c));
                newWord = false;
            }
            else {
                c = static_cast<char>(std::tolower(c));
            }
        }
    }
    return s;
}

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

static std::string normalizeName(std::string s) {
    auto first = std::find_if_not(s.begin(), s.end(), is_ws);
    if (first == s.end()) return "";
    auto last = std::find_if_not(s.rbegin(), s.rend(), is_ws).base();
    std::string t(first, last);

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

static std::string normalizeTitleKey(const std::string& s) {
    return normalizeKey(s);
}

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

    dataEmployee e = emp;                       // copy ra bản local
    e.address = capitalizeEachWord(e.address);  // chuẩn hóa address
    e.title = capitalizeEachWord(e.title);    // chuẩn hóa title

    data[key] = e;
}


bool PeopleBook::updateData(const std::string& name, const dataEmployee& emp) {
    std::string key = normalizeName(name);
    auto it = data.find(key);
    if (it == data.end()) return false;

    dataEmployee e = emp;
    e.address = capitalizeEachWord(e.address);
    e.title = capitalizeEachWord(e.title);

    it->second = e;
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
        std::getline(ss, salary);

        if (name.empty()) continue;

        dataEmployee emp;
        try {
            emp.age = ageStr.empty() ? 0 : std::stoi(ageStr);
        }
        catch (...) {
            emp.age = 0;
        }

        emp.address = capitalizeEachWord(addr);
        emp.phone = phone;
        emp.title = capitalizeEachWord(title);
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
