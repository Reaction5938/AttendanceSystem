#include <wx/wx.h>
#include <wx/datectrl.h>

#include "OvertimePanel.h"
#include "CustomButton.h"
#include "MainFrame.h"

OvertimePanel::OvertimePanel(wxWindow* parent) : wxPanel(parent){
    
    SetBackgroundColour(*wxWHITE);

    new wxStaticText(
        this,
        wxID_ANY,
        wxString::FromUTF8("残業申請画面"),
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
        wxString::FromUTF8("残業日"),
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
        wxString::FromUTF8("申請理由"),
        wxPoint(80, 210)
    );

    messageTextCtrl = new wxTextCtrl(
        this,
        wxID_ANY,
        wxString::FromUTF8(""),
        wxPoint(80, 235),
        wxSize(430, 120),
        wxTE_MULTILINE
    );

    CustomButton* submitButton = new CustomButton(
        this,
        wxString::FromUTF8("残業申請を提出する"),
        wxPoint(80, 380),
        wxSize(220, 45),
        wxColour(170, 85, 0),
        wxColour(220, 120, 0),
        ButtonType::OvertimeSaveButton
    );

    menuButton->Bind(
        wxEVT_BUTTON,
        &OvertimePanel::OnMenuClick,
        this
    );

    Bind(
        EVT_CUSTOM_BUTTON_CLICKED,
        &OvertimePanel::OnSubmitClick,
        this
    );
}

void OvertimePanel::OnMenuClick(wxCommandEvent& event){

    wxCommandEvent menuEvent(EVT_CUSTOM_BUTTON_CLICKED);

    menuEvent.SetInt(
        static_cast<int>(ButtonType::MenuButton)
    );

    wxPostEvent(GetParent(), menuEvent);
}

void OvertimePanel::SetLoggedInUser(const wxString& name){

    loggedInUser = name;

    if(userLabel != nullptr){
        userLabel->SetLabel(name);
    }
}

void OvertimePanel::OnSubmitClick(wxCommandEvent& event){
    
    wxString dateStr = datePicker->GetValue().Format("%Y-%m-%d");
    wxString message = messageTextCtrl->GetValue();

    if(message.IsEmpty()){
        wxMessageBox(
            wxString::FromUTF8("残業理由の入力してください。"),
            wxString::FromUTF8("エラー")
        );

        return;
    }

    if(loggedInUser.IsEmpty()){
        wxMessageBox(
            wxString::FromUTF8("ログイン情報がありません。再度ログインしてください。"),
            wxString::FromUTF8("エラー")
        );

        return;
    }

    MainFrame* frame = dynamic_cast<MainFrame*>(GetParent());

    if(frame){
        if(frame->SubmitOvertime(loggedInUser, dateStr, message)){
            wxMessageBox(wxString::FromUTF8("残業申請を受け付けました。"),
                         wxString::FromUTF8("成功"));
            messageTextCtrl->Clear();
        }else{
            wxMessageBox(wxString::FromUTF8("残業申請の送信に失敗しました。"),
                         wxString::FromUTF8("エラー"));
        }
    }
}