// address_book.cpp
#include "address_book.h"
#include <algorithm>
#include <cctype>
#include <iostream>

using namespace std;

static inline bool is_ws(unsigned char c) { return std::isspace(c); }
static inline bool is_word_sep(unsigned char c) {
    return c == '/' || c == '-' || c == ',' || c == '.';
}

// ================== normalizeAddress ==================

string PeopleBook::normalizeAddress(string s) {
    // 1) Trim theo khoảng trắng
    auto first = find_if_not(s.begin(), s.end(), is_ws);
    if (first == s.end()) return "";
    auto last = find_if_not(s.rbegin(), s.rend(), is_ws).base();
    string t(first, last);

    // 2) Collapse khoảng trắng liên tiếp thành 1 space
    string out;
    out.reserve(t.size());
    bool prev_space = false;
    for (unsigned char c : t) {
        if (is_ws(c)) {
            if (!prev_space) {
                out.push_back(' ');
                prev_space = true;
            }
        }
        else {
            out.push_back(c);
            prev_space = false;
        }
    }

    // 3) Viết hoa từng từ; coi space và / - , . là ranh giới từ
    bool newWord = true;
    for (size_t i = 0; i < out.size(); ++i) {
        unsigned char c = out[i];
        if (out[i] == ' ') {          // khoảng trắng
            newWord = true;
        }
        else if (is_word_sep(c)) {    // / - , .
            newWord = true;           // sau ký tự này là từ mới
        }
        else if (std::isalpha(c)) {   // chỉ đổi hoa/thường cho chữ
            out[i] = static_cast<char>(newWord ? std::toupper(c)
                : std::tolower(c));
            newWord = false;
        }
        else {
            // số/khác giữ nguyên, và không reset newWord (ví dụ "48/3")
            newWord = false;
        }
    }
    return out;
}

// ================== normalizeName ==================

string PeopleBook::normalizeName(string s) {
    string res;

    auto is_space = [](unsigned char c) { return std::isspace(c); };

    // trim
    auto first = find_if_not(s.begin(), s.end(), is_space);
    if (first == s.end()) return "";
    auto last = find_if_not(s.rbegin(), s.rend(), is_space).base();
    string trimmed(first, last);

    // gom nhiều space về 1 space
    bool prev_space = true;
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

    // viết hoa chữ cái đầu mỗi từ
    bool newWord = true;
    for (size_t i = 0; i < res.size(); ++i) {
        unsigned char c = res[i];
        if (std::isspace(c)) {
            newWord = true;
        }
        else {
            res[i] = static_cast<char>(newWord ? std::toupper(c)
                : std::tolower(c));
            newWord = false;
        }
    }
    return res;
}

// ================== toLower helper ==================

string PeopleBook::toLower(string s) {
    for (char& c : s)
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}
// ================== normalizeTitle ==================
string PeopleBook::normalizeTitle(string s) {
    // coi space và '_' là separator
    auto is_sep = [](unsigned char c) {
        return std::isspace(c) || c == '_';
        };

    // 1) trim separator ở đầu/cuối
    auto first = find_if_not(s.begin(), s.end(), is_sep);
    if (first == s.end()) return "";
    auto last = find_if_not(s.rbegin(), s.rend(), is_sep).base();
    string t(first, last);

    // 2) gom nhiều separator thành 1 dấu '_'
    string out;
    bool prev_sep = false;
    for (unsigned char c : t) {
        if (is_sep(c)) {
            if (!prev_sep) {        // chỉ chèn 1 '_'
                out.push_back(' ');
                prev_sep = true;
            }
        }
        else {
            out.push_back(c);
            prev_sep = false;
        }
    }

    // 3) Viết hoa từng từ, từ cách nhau bởi '_'
    bool newWord = true;
    for (size_t i = 0; i < out.size(); ++i) {
        unsigned char c = out[i];
        if (out[i] == ' ') {
            newWord = true;
        }
        else if (std::isalpha(c)) {
            out[i] = static_cast<char>(newWord ? std::toupper(c)
                : std::tolower(c));
            newWord = false;
        }
        else {
            newWord = false; // số/ký tự khác giữ nguyên
        }
    }
    return out;
}
// ================== API chính ==================

void PeopleBook::addData(const string& name, const dataEmployee& profile) {
    string key = normalizeName(name);
    dataEmployee p = profile;
    p.address = normalizeAddress(p.address);
    p.title = normalizeTitle(p.title);
    data[key] = p; // chèn hoặc ghi đè
}

bool PeopleBook::updateData(const string& name, const dataEmployee& profile) {
    string key = normalizeName(name);
    auto it = data.find(key);
    if (it == data.end()) {
        cout << "Khong tim thay \"" << key << "\" de thay doi.\n";
        return false;
    }
    dataEmployee p = profile;
    p.address = normalizeAddress(p.address);
    p.title = normalizeTitle(p.title);
    it->second = p;
    cout << "Da cap nhat ho so cho \"" << key << "\".\n";
    return true;
}

void PeopleBook::removeData(const string& name) {
    string key = normalizeName(name);
    size_t n = data.erase(key);
    if (n) cout << "\"" << key << "\" Deleted!\n";
    else   cout << "\"" << key << "\" khong ton tai.\n";
}

void PeopleBook::printData() const {
    if (data.empty()) {
        cout << "No data found!\n";
        return;
    }
    for (const auto& [k, emp] : data) {
        cout << "Name: " << k
            << ", Age: " << emp.age
            << ", Addr: " << emp.address
            << ", Phone: " << emp.phone
            << ", Title: " << emp.title
            << ", Salary: " << emp.salary
            << '\n';
    }
}

bool PeopleBook::getAge(const string& name, int& ageOut) const {
    string key = normalizeName(name);
    auto it = data.find(key);
    if (it == data.end()) return false;
    ageOut = it->second.age;
    return true;
}

bool PeopleBook::getEmployee(const string& name, dataEmployee& out) const {
    string key = normalizeName(name);
    auto it = data.find(key);
    if (it == data.end()) return false;
    out = it->second;
    return true;
}

// ================== FILTER ==================

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
    std::vector<std::pair<std::string, dataEmployee>> result;
    string key = normalizeTitle(title);    // chuẩn hóa input user

    for (const auto& [name, emp] : data) {
        if (emp.title == key) {           // so sánh dạng chuẩn
            result.emplace_back(name, emp);
        }
    }
    return result;
}

std::vector<std::pair<std::string, dataEmployee>>
PeopleBook::filterByAddressKeyword(const std::string& keyword) const {
    std::vector<std::pair<std::string, dataEmployee>> result;
    string key = toLower(keyword);

    for (const auto& [name, emp] : data) {
        if (toLower(emp.address).find(key) != std::string::npos) {
            result.emplace_back(name, emp);
        }
    }
    return result;
}
