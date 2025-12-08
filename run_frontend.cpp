#include "main_frame.h"
#include <sstream>
#include <iostream>

// File CSV để lưu data
static const std::string CSV_PATH = "employees.csv";

// ====================================================
// ============= ShowAllFrame Implementation ===========
// ====================================================

ShowAllFrame::ShowAllFrame(wxWindow* parent,
    const std::vector<std::pair<std::string, dataEmployee>>& allData)
    : wxFrame(parent, wxID_ANY, "All Employees", wxDefaultPosition, wxSize(800, 500))
{
    wxPanel* panel = new wxPanel(this);

    wxListCtrl* list = new wxListCtrl(
        panel, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxLC_REPORT | wxLC_SINGLE_SEL
    );

    list->InsertColumn(0, "Name", wxLIST_FORMAT_LEFT, 150);
    list->InsertColumn(1, "Age", wxLIST_FORMAT_LEFT, 50);
    list->InsertColumn(2, "Address", wxLIST_FORMAT_LEFT, 200);
    list->InsertColumn(3, "Phone", wxLIST_FORMAT_LEFT, 120);
    list->InsertColumn(4, "Title", wxLIST_FORMAT_LEFT, 100);
    list->InsertColumn(5, "Salary", wxLIST_FORMAT_LEFT, 100);

    long index = 0;
    for (const auto& [name, emp] : allData)
    {
        index = list->InsertItem(index, name);
        list->SetItem(index, 1, std::to_string(emp.age));
        list->SetItem(index, 2, emp.address);
        list->SetItem(index, 3, emp.phone);
        list->SetItem(index, 4, emp.title);
        list->SetItem(index, 5, emp.salary);
    }

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(list, 1, wxALL | wxEXPAND, 10);
    panel->SetSizer(sizer);
}

// ====================================================
// ================= MainFrame utils ==================
// ====================================================

std::string MainFrame::ToStd(const wxString& s) {
    return std::string(s.mb_str());
}

//  KIỂM TRA CÁC Ô NHẬP CÓ ĐỦ HAY KHÔNG
bool MainFrame::CheckAllInputs()
{
    wxString missing;

    if (txtName->IsEmpty()) {
        if (!missing.IsEmpty()) missing += ", ";
        missing += "Name";
    }
    if (txtAge->IsEmpty()) {
        if (!missing.IsEmpty()) missing += ", ";
        missing += "Age";
    }
    if (txtAddress->IsEmpty()) {
        if (!missing.IsEmpty()) missing += ", ";
        missing += "Address";
    }
    if (txtPhone->IsEmpty()) {
        if (!missing.IsEmpty()) missing += ", ";
        missing += "Phone";
    }
    if (txtTitle->IsEmpty()) {
        if (!missing.IsEmpty()) missing += ", ";
        missing += "Title";
    }
    if (txtSalary->IsEmpty()) {
        if (!missing.IsEmpty()) missing += ", ";
        missing += "Salary";
    }

    if (!missing.IsEmpty()) {
        txtLog->AppendText("Thiếu: " + missing + ". Vui lòng nhập đủ thông tin.\n");
        return false;   // báo là không hợp lệ
    }

    return true;        // hợp lệ
}

// ====================================================
// ================ MainFrame Constructor ==============
// ====================================================

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(700, 550))
{
    wxPanel* panel = new wxPanel(this);

    txtName = new wxTextCtrl(panel, wxID_ANY);
    txtAge = new wxTextCtrl(panel, wxID_ANY);
    txtAddress = new wxTextCtrl(panel, wxID_ANY);
    txtPhone = new wxTextCtrl(panel, wxID_ANY);
    txtTitle = new wxTextCtrl(panel, wxID_ANY);
    txtSalary = new wxTextCtrl(panel, wxID_ANY);

    txtLog = new wxTextCtrl(panel, wxID_ANY, "",
        wxDefaultPosition, wxDefaultSize,
        wxTE_MULTILINE | wxTE_READONLY);

    wxButton* btnAdd = new wxButton(panel, wxID_ANY, "Add");
    wxButton* btnUpdate = new wxButton(panel, wxID_ANY, "Update");
    wxButton* btnRemove = new wxButton(panel, wxID_ANY, "Remove");
    wxButton* btnShow = new wxButton(panel, wxID_ANY, "Show All");

    // ==== CÁC NÚT FILTER ====
    wxButton* btnFilterAge = new wxButton(panel, wxID_ANY, "Filter Age");
    wxButton* btnFilterTitle = new wxButton(panel, wxID_ANY, "Filter Title");
    wxButton* btnFilterSalary = new wxButton(panel, wxID_ANY, "Filter Salary");

    // ---- Layout ----
    wxFlexGridSizer* form = new wxFlexGridSizer(2, 6, 8, 8);
    form->AddGrowableCol(1, 1);

    form->Add(new wxStaticText(panel, wxID_ANY, "Name:"));
    form->Add(txtName, 1, wxEXPAND);

    form->Add(new wxStaticText(panel, wxID_ANY, "Age:"));
    form->Add(txtAge, 1, wxEXPAND);

    form->Add(new wxStaticText(panel, wxID_ANY, "Address:"));
    form->Add(txtAddress, 1, wxEXPAND);

    form->Add(new wxStaticText(panel, wxID_ANY, "Phone:"));
    form->Add(txtPhone, 1, wxEXPAND);

    form->Add(new wxStaticText(panel, wxID_ANY, "Title:"));
    form->Add(txtTitle, 1, wxEXPAND);

    form->Add(new wxStaticText(panel, wxID_ANY, "Salary:"));
    form->Add(txtSalary, 1, wxEXPAND);

    wxBoxSizer* btns = new wxBoxSizer(wxHORIZONTAL);
    btns->Add(btnAdd, 0, wxALL, 5);
    btns->Add(btnUpdate, 0, wxALL, 5);
    btns->Add(btnRemove, 0, wxALL, 5);

    btns->Add(btnFilterAge, 0, wxALL, 5);
    btns->Add(btnFilterTitle, 0, wxALL, 5);
    btns->Add(btnFilterSalary, 0, wxALL, 5);

    btns->AddStretchSpacer(1);
    btns->Add(btnShow, 0, wxALL, 5);

    wxBoxSizer* main = new wxBoxSizer(wxVERTICAL);
    main->Add(form, 0, wxALL | wxEXPAND, 12);
    main->Add(btns, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 12);
    main->Add(new wxStaticText(panel, wxID_ANY, "Log:"), 0, wxLEFT | wxRIGHT, 12);
    main->Add(txtLog, 1, wxALL | wxEXPAND, 12);

    panel->SetSizer(main);

    // ---- Bind events ----
    btnAdd->Bind(wxEVT_BUTTON, &MainFrame::OnAdd, this);
    btnUpdate->Bind(wxEVT_BUTTON, &MainFrame::OnUpdate, this);
    btnRemove->Bind(wxEVT_BUTTON, &MainFrame::OnRemove, this);
    btnShow->Bind(wxEVT_BUTTON, &MainFrame::OnShowAll, this);

    btnFilterAge->Bind(wxEVT_BUTTON, &MainFrame::OnFilterByAge, this);
    btnFilterTitle->Bind(wxEVT_BUTTON, &MainFrame::OnFilterByTitle, this);
    btnFilterSalary->Bind(wxEVT_BUTTON, &MainFrame::OnFilterBySalary, this);

    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);

    // ==== LOAD DỮ LIỆU TỪ CSV LÚC MỚI MỞ APP ====
    if (m_book.loadFromCsv(CSV_PATH)) {
        txtLog->AppendText("Đã load dữ liệu từ file employees.csv\n");
    }
    else {
        txtLog->AppendText("Không tìm thấy employees.csv, bắt đầu với danh sách rỗng.\n");
    }
}

// ====================================================
// ================ Event Handler Logic ================
// ====================================================

void MainFrame::OnAdd(wxCommandEvent& evt)
{
    if (!CheckAllInputs())
        return;

    std::string name = ToStd(txtName->GetValue());
    if (name.empty()) {
        txtLog->AppendText("Name is empty → Cannot Add.\n");
        return;
    }

    dataEmployee emp;
    long age;
    if (!txtAge->GetValue().ToLong(&age) || age < 0) {
        txtLog->AppendText("Age is invalid.\n");
        return;
    }
    emp.age = static_cast<int>(age);
    emp.address = ToStd(txtAddress->GetValue());
    emp.phone = ToStd(txtPhone->GetValue());
    emp.title = ToStd(txtTitle->GetValue());
    emp.salary = ToStd(txtSalary->GetValue());

    m_book.addData(name, emp);

    txtLog->AppendText("Added: " + txtName->GetValue() + "\n");
}

void MainFrame::OnUpdate(wxCommandEvent& evt)
{
    if (!CheckAllInputs())
        return;

    std::string name = ToStd(txtName->GetValue());
    if (name.empty()) {
        txtLog->AppendText("Name empty → Cannot Update.\n");
        return;
    }

    dataEmployee emp;
    emp.age = txtAge->IsEmpty() ? 0 : std::stoi(ToStd(txtAge->GetValue()));
    emp.address = ToStd(txtAddress->GetValue());
    emp.phone = ToStd(txtPhone->GetValue());
    emp.title = ToStd(txtTitle->GetValue());
    emp.salary = ToStd(txtSalary->GetValue());

    if (m_book.updateData(name, emp)) {
        txtLog->AppendText("Updated: " + txtName->GetValue() + "\n");
    }
    else {
        txtLog->AppendText("Not found → Cannot Update.\n");
    }
}

void MainFrame::OnRemove(wxCommandEvent& evt)
{
    std::string name = ToStd(txtName->GetValue());
    if (name.empty()) {
        txtLog->AppendText("Name empty → Cannot Remove.\n");
        return;
    }

    dataEmployee tmp;
    if (!m_book.getEmployee(name, tmp)) {
        txtLog->AppendText("Not found → Cannot Remove.\n");
        return;
    }

    m_book.removeData(name);

    txtLog->AppendText("Removed: " + txtName->GetValue() + "\n");
}

void MainFrame::OnShowAll(wxCommandEvent& evt)
{
    auto allData = m_book.getAll();
    ShowAllFrame* frame = new ShowAllFrame(this, allData);
    frame->Show(true);
}

// ==================== FILTER HANDLERS ====================

void MainFrame::OnFilterByAge(wxCommandEvent& evt)
{
    long minAge, maxAge;

    wxString sMin = wxGetTextFromUser(
        "Nhập tuổi tối thiểu:",
        "Filter by Age",
        "0",
        this
    );
    if (sMin.IsEmpty()) return;
    if (!sMin.ToLong(&minAge)) {
        txtLog->AppendText("Giá trị tuổi tối thiểu không hợp lệ.\n");
        return;
    }

    wxString sMax = wxGetTextFromUser(
        "Nhập tuổi tối đa:",
        "Filter by Age",
        "100",
        this
    );
    if (sMax.IsEmpty()) return;
    if (!sMax.ToLong(&maxAge)) {
        txtLog->AppendText("Giá trị tuổi tối đa không hợp lệ.\n");
        return;
    }

    auto result = m_book.filterByAgeRange((int)minAge, (int)maxAge);

    if (result.empty()) {
        txtLog->AppendText("Không tìm thấy nhân viên trong khoảng tuổi.\n");
    }
    else {
        txtLog->AppendText("Đã filter theo tuổi, mở cửa sổ kết quả.\n");
        ShowAllFrame* frame = new ShowAllFrame(this, result);
        frame->Show(true);
    }
}

void MainFrame::OnFilterByTitle(wxCommandEvent& evt)
{
    wxString title = wxGetTextFromUser(
        "Nhập chức vụ (ví dụ: Giam_Doc, Nhan_Vien...):",
        "Filter by Title",
        "",
        this
    );
    if (title.IsEmpty()) return;

    auto result = m_book.filterByTitle(ToStd(title));

    if (result.empty()) {
        txtLog->AppendText("Không tìm thấy nhân viên với chức vụ đó.\n");
    }
    else {
        txtLog->AppendText("Đã filter theo chức vụ, mở cửa sổ kết quả.\n");
        ShowAllFrame* frame = new ShowAllFrame(this, result);
        frame->Show(true);
    }
}

void MainFrame::OnFilterBySalary(wxCommandEvent& evt)
{
    long minS, maxS;

    wxString sMin = wxGetTextFromUser(
        "Nhập lương tối thiểu (VD: 5000000):",
        "Filter by Salary",
        "0",
        this
    );
    if (sMin.IsEmpty()) return;
    if (!sMin.ToLong(&minS) || minS < 0) {
        txtLog->AppendText("Giá trị lương tối thiểu không hợp lệ.\n");
        return;
    }

    wxString sMax = wxGetTextFromUser(
        "Nhập lương tối đa (VD: 20000000):",
        "Filter by Salary",
        "100000000",
        this
    );
    if (sMax.IsEmpty()) return;
    if (!sMax.ToLong(&maxS) || maxS < 0) {
        txtLog->AppendText("Invalid maximum number\n");
        return;
    }

    auto result = m_book.filterBySalaryRange(
        static_cast<long long>(minS),
        static_cast<long long>(maxS)
    );

    if (result.empty()) {
        txtLog->AppendText("Can't find suitable data with this filter.\n");
    }
    else {
        txtLog->AppendText("--Succesfully filter, result--.\n");
        ShowAllFrame* frame = new ShowAllFrame(this, result);
        frame->Show(true);
    }
}

// ====================================================
// ===================== OnClose ======================
// ====================================================

void MainFrame::OnClose(wxCloseEvent& evt)
{
    // Lưu dữ liệu ra CSV trước khi thoát
    if (!m_book.saveToCsv(CSV_PATH)) {
        std::cerr << "Can't store CSV file: " << CSV_PATH << "\n";
    }

    evt.Skip(); // cho wxWidgets xử lý tiếp (đóng cửa sổ)
}
