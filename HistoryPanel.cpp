#include <wx/wx.h>
#include <wx/listctrl.h>
#include "HistoryPanel.h"
#include "CustomButton.h"
#include "AttendanceRecord.h"


HistoryPanel::HistoryPanel(wxWindow* parent) : wxPanel(parent){
    
    SetBackgroundColour(*wxWHITE);



    wxButton* menuButton = new wxButton(
        this,
        wxID_ANY,
        wxString::FromUTF8("≡"),
        wxPoint(10, 10),
        wxSize(40, 40)
    );

    userChoice = new wxChoice(
        this,
        wxID_ANY,
        wxPoint(120, 40),
        wxSize(100, 40)
    );

    attendanceList = new wxListCtrl(
        this,
        wxID_ANY,
        wxPoint(30, 80),
        wxSize(700, 400),
        wxLC_REPORT |
        wxLC_HRULES |
        wxLC_VRULES
    );

    userChoice->Bind(
        wxEVT_CHOICE,
        &HistoryPanel::OnUserChanged,
        this
    );

    userChoice->Append(
        wxString::FromUTF8("全ユーザー")
    );

    userChoice->SetSelection(0);

    attendanceList->InsertColumn(0, wxString::FromUTF8("ユーザー"), wxLIST_FORMAT_LEFT, 60);

    attendanceList->InsertColumn(1, wxString::FromUTF8("出勤"), wxLIST_FORMAT_LEFT, 150);

    attendanceList->InsertColumn(2, wxString::FromUTF8("退勤"), wxLIST_FORMAT_LEFT, 150);

    attendanceList->InsertColumn(3, wxString::FromUTF8("修正メッセージ"), wxLIST_FORMAT_LEFT, 180);

    attendanceList->InsertColumn(4, wxString::FromUTF8("休暇理由"), wxLIST_FORMAT_LEFT, 200);

    attendanceList->InsertColumn(5, wxString::FromUTF8("残業理由"), wxLIST_FORMAT_LEFT, 200);

    menuButton->Bind(
        wxEVT_BUTTON,
        &HistoryPanel::OnMenuClick,
        this
    );
}

void HistoryPanel::OnMenuClick(wxCommandEvent& event){

    wxCommandEvent menuEvent(EVT_CUSTOM_BUTTON_CLICKED);

    menuEvent.SetInt(static_cast<int>(ButtonType::MenuButton));

    wxPostEvent(GetParent(), menuEvent);
}

void HistoryPanel::AddUser(const wxString& name){

    userChoice->Append(name);
}

void HistoryPanel::RemoveUser(const wxString& name){

    int index = userChoice->FindString(name);

    if(index != wxNOT_FOUND){
        userChoice->Delete(index);
    }
}

void HistoryPanel::SetAttendanceRecords(const wxVector<AttendanceRecord>& records,
    const wxString& userName
){
    // データを保存する
    m_records = records;

    attendanceList->DeleteAllItems();

    const wxString allUsers = wxString::FromUTF8("全ユーザー");

    for(const auto& record : records){
        if(record.userName != userName && userName != allUsers){
            continue;
        }


        long row = attendanceList->InsertItem(
            attendanceList->GetItemCount(),
            record.userName
        );

        attendanceList->SetItem(
            row,
            1,
            record.clockInTime
        );

        attendanceList->SetItem(
            row,
            2,
            record.clockOutTime
        );

        attendanceList->SetItem(
            row,
            3,
            record.correctMessage
        );

        wxString vacationDisplay = record.vacationMessage;
        if(vacationDisplay.IsEmpty()){
            vacationDisplay = wxString::FromUTF8("-");
        }

        attendanceList->SetItem(
            row,
            4,
            vacationDisplay
        );
        
        wxString overtimeDisplay = record.overtimeMessage;
        if(overtimeDisplay.IsEmpty()){
            overtimeDisplay = wxString::FromUTF8("-");
        }

        attendanceList->SetItem(
            row,
            5,
            overtimeDisplay
        );
    }
}

void HistoryPanel::OnUserChanged(wxCommandEvent& event){

    wxString selectedUser = userChoice->GetStringSelection();

    SetAttendanceRecords(
        m_records,
        selectedUser
    );
}

void HistoryPanel::SetLoggedInUser(const wxString& name){
    
    userChoice->Clear();

    userChoice->Append(wxString::FromUTF8("全ユーザー"));

    if(!name.IsEmpty()){

        userChoice->Append(name);
    }

    userChoice->SetSelection(1);

    SetAttendanceRecords(m_records, name);
}