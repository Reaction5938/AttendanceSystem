#include "MenuPanel.h"
#include "CustomButton.h"

wxDEFINE_EVENT(EVT_MENU_BUTTON_CLICKED, wxCommandEvent);

MenuPanel::MenuPanel(wxWindow* parent) : wxPanel(parent){
    SetBackgroundColour(wxColour(144, 238, 144));

    //テキスト文描画
    new wxStaticText(
        this,
        wxID_ANY,
        wxString::FromUTF8("メニュー 一覧"),
        wxPoint(70, 40)
    );

    // ボタンの描画
    closeButton = new CustomButton(
        this,
        wxString::FromUTF8("✕"),
        wxPoint(10, 10),
        wxSize(40, 40),
        wxColour(0, 0, 0),
        wxColour(120, 120, 120),
        ButtonType::CloseButton
    );

    homeButton = new CustomButton(
        this,
        wxString::FromUTF8("出退勤入力"),
        wxPoint(30, 60),
        wxSize(200, 40),
        wxColour(0, 0, 170),
        wxColour(0, 0, 220),
        ButtonType::HomeButton
    );

    historyButton = new CustomButton(
        this,
        wxString::FromUTF8("勤怠履歴"),
        wxPoint(30, 120),
        wxSize(200, 40),
        wxColour(139, 69, 19),
        wxColour(160, 82, 45),
        ButtonType::HistoryButton
    );

    editButton = new CustomButton(
        this,
        wxString::FromUTF8("勤怠入力修正"),
        wxPoint(30, 180),
        wxSize(200, 40),
        wxColour(200, 0, 200),
        wxColour(250, 0, 250),
        ButtonType::EditButton
    );

    vacationButton = new CustomButton(
        this,
        wxString::FromUTF8("休暇申請"),
        wxPoint(30, 240),
        wxSize(200, 40),
        wxColour(0, 170, 170),
        wxColour(0, 220, 220),
        ButtonType::VacationButton
    );

    overtimeButton = new CustomButton(
        this,
        wxString::FromUTF8("残業申請"),
        wxPoint(30, 300),
        wxSize(200, 40),
        wxColour(170, 85, 0),
        wxColour(220, 120, 0),
        ButtonType::OvertimeButton
    );

    userButton = new CustomButton(
        this,
        wxString::FromUTF8("ユーザー登録"),
        wxPoint(30, 360),
        wxSize(200, 40),
        wxColour(230, 140, 0),
        wxColour(255, 170, 30),
        ButtonType::UserButton
    );

    Bind(
        EVT_CUSTOM_BUTTON_CLICKED,
        &MenuPanel::OnCustomButtonClick,
        this
    );

    loginButton = new CustomButton(
        this,
        wxString::FromUTF8("ログイン"),
        wxPoint(30, 420),
        wxSize(200, 40),
        wxColour(189, 26, 120),
        wxColour(219, 56, 150),
        ButtonType::LoginButton
    );

    logoutButton = new CustomButton(
        this,
        wxString::FromUTF8("ログアウト"),
        wxPoint(30, 480),
        wxSize(200, 40),
        wxColour(255, 200, 200),
        wxColour(225, 170, 170),
        ButtonType::LogoutButton
    );

    UpdateLoginButtons(false);
}

void MenuPanel::OnCustomButtonClick(wxCommandEvent& event){
    
    //MenuPanelから送るイベント
    wxCommandEvent menuEvent(EVT_MENU_BUTTON_CLICKED);

    //ButtonTypeをコピーする
    menuEvent.SetInt(event.GetInt());

    // MenuPanelからMainFrameへイベントを送る
    wxPostEvent(GetParent(), menuEvent);
}

void MenuPanel::UpdateLoginButtons(bool isLoggedIn){

    if(isLoggedIn){
        
        loginButton->Hide();
        logoutButton->Show();
    }
    else{

        loginButton->Show();
        logoutButton->Hide();
    }

    Layout();
}