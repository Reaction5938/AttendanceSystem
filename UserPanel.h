#ifndef USERPANEL_H
#define USERPANEL_H

#include <wx/wx.h>
#include "CustomButton.h"

class UserPanel : public wxPanel{
    public:
        UserPanel(wxWindow* parent);

        void AddUser(const wxString& name);

        void RemoveUser(const wxString& name);

    private:

        // ユーザー名を入力する場所
        wxTextCtrl* userNameText;

        wxTextCtrl* passwordText;

        // 登録したユーザーを選択するプルダウンメニュー
        wxChoice* userChoice;

        //ユーザー登録ボタン 
        wxButton* registerButton;

        // ユーザー削除ボタン
        wxButton* deleteButton;

        // 登録したユーザー名を保存する場所
        wxArrayString users;

        // 登録ボタンを押した際のイベント処理
        void OnRegister(wxCommandEvent& event);

        // 削除ボタンを押した際のイベント処理
        void OnDelete(wxCommandEvent& event);

        void OnMenuClick(wxCommandEvent& event);   
};


#endif