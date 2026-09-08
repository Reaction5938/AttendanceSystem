#include "LoginPanel.h"
#include "MainFrame.h"

LoginPanel::LoginPanel(wxWindow* parent) : wxPanel(parent){

    SetBackgroundColour(*wxWHITE);

    new wxStaticText(
        this,
        wxID_ANY,
        wxString::FromUTF8("勤怠管理システム"),
        wxPoint(140, 40)
    );

    wxButton* menuButton = new wxButton(
        this,
        wxID_ANY,
        wxString::FromUTF8("≡"),
        wxPoint(10, 10),
        wxSize(40, 40)
    );

    menuButton->Bind(
        wxEVT_BUTTON,
        &LoginPanel::OnMenuClick,
        this
    );

    new wxStaticText(
        this,
        wxID_ANY,
        wxString::FromUTF8("ユーザー名"),
        wxPoint(80, 100)
    );

    userNameText = new wxTextCtrl(
        this,
        wxID_ANY,
        wxString::FromUTF8(""),
        wxPoint(180, 95),
        wxSize(180, 30)
    );

    new wxStaticText(
        this,
        wxID_ANY,
        wxString::FromUTF8("パスワード"),
        wxPoint(80, 160)
    );

    passwordText = new wxTextCtrl(
        this,
        wxID_ANY,
        wxString::FromUTF8(""),
        wxPoint(180, 155),
        wxSize(100, 30),
        wxTE_PASSWORD
    );

    loginButton = new wxButton(
        this,
        wxID_ANY,
        wxString::FromUTF8("ログイン"),
        wxPoint(180, 230),
        wxSize(120, 40)
    );

    loginButton->Bind(
        wxEVT_BUTTON,
        &LoginPanel::OnLogin,
        this
    );
}

void LoginPanel::OnLogin(wxCommandEvent& event){

    wxString userName = userNameText->GetValue();
    
    wxString password = passwordText->GetValue();

    MainFrame* frame = dynamic_cast<MainFrame*>(GetParent());

    if(frame){

        if(frame->LoginUser(userName, password)){

            wxMessageBox(
                wxString::FromUTF8("ログイン成功"),
                wxString::FromUTF8("ログイン")
            );

            frame->ShowHome();
        }
        else{
            wxMessageBox(
                wxString::FromUTF8("ユーザー名またはパスワードが違います。"),
                wxString::FromUTF8("ログイン")
            );
        }
    }
}

void LoginPanel::ClearLogin(){

    userNameText->Clear();

    passwordText->Clear();
}

void LoginPanel::OnMenuClick(wxCommandEvent& event){
    wxCommandEvent menuEvent(EVT_CUSTOM_BUTTON_CLICKED);

    menuEvent.SetInt(static_cast<int>(ButtonType::MenuButton));

    wxPostEvent(GetParent(), menuEvent);
}