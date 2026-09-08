#include <wx/wx.h>
#include "UserPanel.h"
#include "CustomButton.h"
#include "MainFrame.h"

UserPanel::UserPanel(wxWindow* parent) : wxPanel(parent){
    
    SetBackgroundColour(*wxWHITE);

    new wxStaticText(
        this,
        wxID_ANY,
        wxString::FromUTF8("ユーザー登録"),
        wxPoint(80, 40)
    );

    new wxStaticText(
        this,
        wxID_ANY,
        wxString::FromUTF8("ユーザー名"),
        wxPoint(80, 80)
    ); 

    // ユーザー名入力欄
    userNameText = new wxTextCtrl(
        this,
        wxID_ANY,
        wxString::FromUTF8(""),
        wxPoint(160, 75),
        wxSize(180, 30)
    );

    // パスワードラベル
    new wxStaticText(
        this,
        wxID_ANY,
        wxString::FromUTF8("パスワード"),
        wxPoint(80, 125)
    );

    passwordText = new wxTextCtrl(
        this,
        wxID_ANY,
        wxString::FromUTF8(""),
        wxPoint(160, 120),
        wxSize(180, 30),
        wxTE_PASSWORD
    );

    // 登録ボタン
    registerButton = new wxButton(
        this,
        wxID_ANY,
        wxString::FromUTF8("登録"),
        wxPoint(360, 95),
        wxSize(80, 35)
    );

    // 削除ボタン
    deleteButton = new wxButton(
        this,
        wxID_ANY,
        wxString::FromUTF8("削除"),
        wxPoint(360, 180),
        wxSize(80, 35)
    );

    // ユーザー選択ラベル
    new wxStaticText(
        this,
        wxID_ANY,
        wxString::FromUTF8("登録済みユーザー"),
        wxPoint(65, 185)
    );

    // ユーザー選択プルダウン
    userChoice = new wxChoice(
        this,
        wxID_ANY,
        wxPoint(160, 180),
        wxSize(180, 30)
    );

    wxButton* menuButton = new wxButton(
        this,
        wxID_ANY,
        wxString::FromUTF8("≡"),
        wxPoint(10, 10),
        wxSize(40, 40)
    );

    registerButton->Bind(
        wxEVT_BUTTON,
        &UserPanel::OnRegister,
        this
    );

    deleteButton->Bind(
        wxEVT_BUTTON,
        &UserPanel::OnDelete,
        this
    );

    menuButton->Bind(
        wxEVT_BUTTON,
        &UserPanel::OnMenuClick,
        this
    );
}

void UserPanel::OnRegister(wxCommandEvent& event){
    
    wxString name = userNameText->GetValue();

    wxString password = passwordText->GetValue();

    if(name.IsEmpty()){
        wxMessageBox(
            wxString::FromUTF8("ユーザー名を入力してください"),
            wxString::FromUTF8("登録")
        );

        return;
    }

    if(password.IsEmpty()){
        wxMessageBox(
            wxString::FromUTF8("パスワードを入力してください"),
            wxString::FromUTF8("登録")
        );

        return;
    }



    MainFrame* frame = dynamic_cast<MainFrame*>(GetParent());

    if(frame){
        
        if(!frame->RegisterUser(name, password)){

            wxMessageBox(wxString::FromUTF8("データベースへの登録に失敗しました。"),
            wxString::FromUTF8("エラー"));

            return;
        }
    }

    

    userNameText->Clear();

    passwordText->Clear();

    wxMessageBox(
        wxString::FromUTF8("ユーザーを登録しました。"),
        wxString::FromUTF8("登録")
    );
}

void UserPanel::OnDelete(wxCommandEvent& event){

    int selection = userChoice->GetSelection();

    wxString password = passwordText->GetValue();

    if(selection == wxNOT_FOUND){
        wxMessageBox(
            wxString::FromUTF8("削除するユーザーを選択してください。"),
            wxString::FromUTF8("削除")
        );

        return;
    }

    if(password.IsEmpty()){

        wxMessageBox(
            wxString::FromUTF8("パスワードを入力してください"),
            wxString::FromUTF8("削除")
        );

        return;
    }

    wxString name = userChoice->GetString(selection);

    MainFrame* frame = dynamic_cast<MainFrame*>(GetParent());

    if(frame){
        
        if(!frame->DeleteUser(name, password)){

            return;
        }
    }

    passwordText->Clear();

    wxMessageBox(
        name + wxString::FromUTF8("を削除しました。"),
        wxString::FromUTF8("削除")
    );
}


void UserPanel::OnMenuClick(wxCommandEvent& event){

    wxCommandEvent menuEvent(EVT_CUSTOM_BUTTON_CLICKED);

    menuEvent.SetInt(static_cast<int>(ButtonType::MenuButton));

    wxPostEvent(GetParent(), menuEvent);
}

void UserPanel::AddUser(const wxString& name){

    users.Add(name);

    userChoice->Append(name);
}

void UserPanel::RemoveUser(const wxString& name){

    int index = userChoice->FindString(name);

    if(index != wxNOT_FOUND){

        userChoice->Delete(index);
    }

    int userIndex = users.Index(name);

    if(userIndex != wxNOT_FOUND){

        users.RemoveAt(userIndex);
    }
}   