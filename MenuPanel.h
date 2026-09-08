#ifndef MENUPANEL_H
#define MENUPANEL_H

#include <wx/wx.h>
#include "CustomButton.h"

wxDECLARE_EVENT(EVT_MENU_BUTTON_CLICKED, wxCommandEvent);

class MenuPanel : public wxPanel{
    public:
        MenuPanel(wxWindow* parent);
        
        void UpdateLoginButtons(bool inLoggedIn);
    private:
        //各画面に移動するためのカスタムボタン宣言

        CustomButton* closeButton;//閉じるボタン
        CustomButton* homeButton;//勤怠入力ボタン
        CustomButton* historyButton;//勤怠履歴ボタン
        CustomButton* editButton;//勤怠修正ボタン
        CustomButton* vacationButton;//休暇申請ボタン
        CustomButton* overtimeButton;
        CustomButton* userButton;   //ユーザー登録ボタン
        CustomButton* loginButton;  //ログインボタン
        CustomButton* logoutButton;

        void OnCustomButtonClick(wxCommandEvent& event);

        
};

#endif