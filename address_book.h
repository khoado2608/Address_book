// address_book.h
#pragma once
#include <string>
#include <map>
#include <vector>

struct dataEmployee {
    int age{};
    std::string address;
    std::string phone;
    std::string title;
    std::string salary;
};

class PeopleBook {
private:
    std::map<std::string, dataEmployee> data;

    static std::string normalizeName(std::string s);
    static std::string normalizeAddress(std::string s);
    static std::string toLower(std::string s);
    static std::string normalizeTitle(std::string s);

public:
    void addData(const std::string& name, const dataEmployee& profile);
    bool updateData(const std::string& name, const dataEmployee& profile);
    void removeData(const std::string& name);
    void printData() const;

    bool getAge(const std::string& name, int& ageOut) const;
    bool getEmployee(const std::string& name, dataEmployee& out) const;

    // ====== các hàm filter ======
    std::vector<std::pair<std::string, dataEmployee>>
        filterByAgeRange(int minAge, int maxAge) const;

    std::vector<std::pair<std::string, dataEmployee>>
        filterByTitle(const std::string& title) const;

    std::vector<std::pair<std::string, dataEmployee>>
        filterByAddressKeyword(const std::string& keyword) const;
};
