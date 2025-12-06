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
public:
    PeopleBook() = default;

    // ===== CRUD =====
    void addData(const std::string& name, const dataEmployee& emp);
    bool updateData(const std::string& name, const dataEmployee& emp);
    bool removeData(const std::string& name);

    void printData() const;

    bool getAge(const std::string& name, int& ageOut) const;
    bool getEmployee(const std::string& name, dataEmployee& out) const;

    // Lấy toàn bộ dữ liệu cho GUI
    std::vector<std::pair<std::string, dataEmployee>> getAll() const;

    // ===== Filter =====
    std::vector<std::pair<std::string, dataEmployee>>
        filterByAgeRange(int minAge, int maxAge) const;

    std::vector<std::pair<std::string, dataEmployee>>
        filterByTitle(const std::string& title) const;

    std::vector<std::pair<std::string, dataEmployee>>
        filterByAddressKeyword(const std::string& keyword) const;

    std::vector<std::pair<std::string, dataEmployee>>
        filterBySalaryRange(long long minSalary, long long maxSalary) const;

    // ===== CSV I/O =====
    bool loadFromCsv(const std::string& path);
    bool saveToCsv(const std::string& path) const;

private:
    std::map<std::string, dataEmployee> data;
};
