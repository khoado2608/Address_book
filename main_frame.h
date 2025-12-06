#pragma once
#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "address_book.h"

// ============= Frame hiển thị toàn bộ danh sách =============
class ShowAllFrame : public wxFrame {
public:
    ShowAllFrame(wxWindow* parent,
        const std::vector<std::pair<std::string, dataEmployee>>& allData);
};

// =================== Frame chính của ứng dụng ===================
class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);

private:
    // Backend
    PeopleBook m_book;

    // Controls
    wxTextCtrl* txtName = nullptr;
    wxTextCtrl* txtAge = nullptr;
    wxTextCtrl* txtAddress = nullptr;
    wxTextCtrl* txtPhone = nullptr;
    wxTextCtrl* txtTitle = nullptr;
    wxTextCtrl* txtSalary = nullptr;

    wxTextCtrl* txtLog = nullptr;

    // Hàm tiện ích
    static std::string ToStd(const wxString& s);

    //  HÀM KIỂM TRA ĐỦ THÔNG TIN
    bool CheckAllInputs();

    // Event handlers
    void OnAdd(wxCommandEvent& evt);
    void OnUpdate(wxCommandEvent& evt);
    void OnRemove(wxCommandEvent& evt);
    void OnShowAll(wxCommandEvent& evt);

    // Filter handlers
    void OnFilterByAge(wxCommandEvent& evt);
    void OnFilterByTitle(wxCommandEvent& evt);
    void OnFilterBySalary(wxCommandEvent& evt);

    void OnClose(wxCloseEvent& evt);
};

#endif // MAINFRAME_H
