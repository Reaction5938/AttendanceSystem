#include <wx/wx.h>
#include <wx/datectrl.h>

#include "VacationPanel.h"
#include "CustomButton.h"
#include "MainFrame.h"

VacationPanel::VacationPanel(wxWindow* parent) : wxPanel(parent){
    SetBackgroundColour(*wxWHITE);

    new wxStaticText(
        this,
        wxID_ANY,
        wxString::FromUTF8("休暇申請画面"),
        wxPoint(80, 60)
    );

    wxButton* menuButton = new wxButton(
        this,
        wxID_ANY,
        wxString::FromUTF8("≡"),
        wxPoint(10, 10),
        wxSize(40, 40)
    );

    new wxStaticText(
        this,
        wxID_ANY,
        wxString::FromUTF8("申請者"),
        wxPoint(80, 110)
    );

    userLabel = new wxStaticText(
        this,
        wxID_ANY,
        wxString::FromUTF8(""),
        wxPoint(150, 110)
    );

    new wxStaticText(
        this,
        wxID_ANY,
        wxString::FromUTF8("休暇日"),
        wxPoint(80, 160)
    );

    datePicker = new wxDatePickerCtrl(
        this,
        wxID_ANY,
        wxDateTime::Today(),
        wxPoint(150, 155),
        wxSize(180, 30)
    );

    new wxStaticText(
        this,
        wxID_ANY,
        wxString::FromUTF8("休暇理由"),
        wxPoint(80, 210)
    );

    messageText = new wxTextCtrl(
        this,
        wxID_ANY,
        wxString::FromUTF8(""),
        wxPoint(80, 235),
        wxSize(430, 120),
        wxTE_MULTILINE
    );

    CustomButton* submitButton = new CustomButton(
        this,
        wxString::FromUTF8("休暇届を提出する"),
        wxPoint(80, 380),
        wxSize(220, 45),
        wxColour(0, 128, 0),
        wxColour(0, 200, 0),
        ButtonType::VacationSaveButton
    );

    menuButton->Bind(
        wxEVT_BUTTON,
        &VacationPanel::OnMenuClick,
        this
    );

    Bind(
        EVT_CUSTOM_BUTTON_CLICKED,
        &VacationPanel::OnSubmitVacation,
        this
    );
}

void VacationPanel::OnMenuClick(wxCommandEvent& event){
    wxCommandEvent menuEvent(EVT_CUSTOM_BUTTON_CLICKED);

    menuEvent.SetInt(static_cast<int>(ButtonType::MenuButton));

    wxPostEvent(GetParent(), menuEvent);
}

void VacationPanel::AddUser(const wxString& name){
    (void)name;
}

void VacationPanel::RemoveUser(const wxString& name){
    (void)name;
}

void VacationPanel::SetLoggedInUser(const wxString& name){
    loggedInUser = name;

    if(userLabel != nullptr){
        userLabel->SetLabel(name);
    }
}

void VacationPanel::OnSubmitVacation(wxCommandEvent& event){

    if(event.GetInt() != static_cast<int>(ButtonType::VacationSaveButton)){
        return;
    }

    wxString userName = loggedInUser;

    wxString message = messageText->GetValue();

    if(userName.IsEmpty() || message.Trim().IsEmpty()){
        wxMessageBox(
            wxString::FromUTF8("ユーザー名と休暇理由を入力してください。"),
            wxString::FromUTF8("入力エラー"),
            wxOK | wxICON_ERROR
        );

        return;
    }

    MainFrame* frame = dynamic_cast<MainFrame*>(GetParent());
    if(frame == nullptr){
        return;
    }

    bool success = frame->SubmitVacationLeave(
        userName,
        datePicker->GetValue().FormatISODate(),
        message
    );

    if(success){
        wxMessageBox(
            wxString::FromUTF8("休暇届を提出しました。"),
            wxString::FromUTF8("休暇申請")
        );
        messageText->Clear();
    }
}