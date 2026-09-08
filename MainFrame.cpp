#include "MainFrame.h"
#include "HomePanel.h"
#include "HistoryPanel.h"
#include "EditPanel.h"
#include "VacationPanel.h"
#include "UserPanel.h"

MainFrame::MainFrame() : wxFrame(
    // タイトル名
    nullptr,
    wxID_ANY,
    wxString::FromUTF8("勤怠管理システム"),
    wxDefaultPosition,
    wxSize(800, 600)),
    m_timer(this)
    {
        
        homePanel = new HomePanel(this);
        homePanel->SetSize(GetClientSize());

        menuPanel = new MenuPanel(this);
        //メニュー画面の幅は250px、高さはウィンドウと同じ
        menuPanel->SetSize(250, GetClientSize().GetHeight());
        
        //メニュー画面の初期位置(幅, 高さ)
        menuPanel->Move(-250, 0);
        menuPanel->Hide();        

        historyPanel = new HistoryPanel(this);
        historyPanel->SetSize(GetClientSize());
        historyPanel->Hide();

        editPanel = new EditPanel(this);
        editPanel->SetSize(GetClientSize());
        editPanel->Hide();

        vacationPanel = new VacationPanel(this);
        vacationPanel->SetSize(GetClientSize());
        vacationPanel->Hide();

        overtimePanel = new OvertimePanel(this);
        overtimePanel->SetSize(GetClientSize());
        overtimePanel->Hide();

        userPanel = new UserPanel(this);
        userPanel->SetSize(GetClientSize());
        userPanel->Hide();

        loginPanel = new LoginPanel(this);
        loginPanel->SetSize(GetClientSize());
        loginPanel->Hide();

        if(database.Connect()){
            
            wxMessageBox(wxString::FromUTF8("データベース接続成功"));

            wxArrayString dbUsers = database.GetUsers();

            for(const auto& user : dbUsers){

                AddUser(user);
            }
        }
        else{

            wxMessageBox(wxString::FromUTF8("データベース接続失敗"));
        }

        m_currentScreen = ScreenType::Login;

        homePanel->Hide();
        historyPanel->Hide();
        editPanel->Hide();
        vacationPanel->Hide();
        overtimePanel->Hide();
        userPanel->Hide();
        menuPanel->Hide();

        loginPanel->Show();

        // タイマーイベントの登録
        Bind(wxEVT_TIMER,&MainFrame::OnTimer,this);

        // メニュー画面の登録
        Bind(EVT_MENU_BUTTON_CLICKED, &MainFrame::OnMenuButtonClicked, this);

        // 各画面でのメニュー画面の登録
        Bind(EVT_CUSTOM_BUTTON_CLICKED, &MainFrame::OnMenuButtonClicked, this);
        
    }

// メニュー画面に移動する時
void MainFrame::ShowMenu(){

    m_menuState = MenuState::Opening;

    // メニュー画面の初期座標
    m_menuX = -250;

    // メニュー画面を動かしていく
    menuPanel->Move(m_menuX, 0);

    //メニュー画面を表示する
    menuPanel->Show();

    homePanel->Hide();

    historyPanel->Hide();

    editPanel->Hide();

    vacationPanel->Hide();

    overtimePanel->Hide();

    userPanel->Hide();

    loginPanel->Hide();

    // 0.01ミリ秒ごとに呼ばれる
    m_timer.Start(10);


}

//ホーム画面に移動する時
void MainFrame::Cansel(){

    m_menuState = MenuState::Closing;


    m_timer.Start(10);

}

void MainFrame::ShowHome(){

    if(!IsLoggedIn()){
        wxMessageBox(
            wxString::FromUTF8("ログインしてください。"),
            wxString::FromUTF8("エラー")
        );

        ShowLogin();

        return;
    }
    
    m_currentScreen = ScreenType::Home;

    homePanel->Show();

    menuPanel->Hide();

    historyPanel->Hide();

    editPanel->Hide();

    vacationPanel->Hide();

    overtimePanel->Hide();

    userPanel->Hide();

    loginPanel->Hide();
}

void MainFrame::ShowHistory(){

    m_currentScreen = ScreenType::History;
    
    homePanel->Hide();

    menuPanel->Hide();

    historyPanel->Show();

    editPanel->Hide();

    vacationPanel->Hide();

    overtimePanel->Hide();

    userPanel->Hide();

    loginPanel->Hide();

    wxVector<AttendanceRecord> records = GetAttendanceRecords();

    size_t currentUserRecordCount = 0;

    for(const auto& record : records){

        if(record.userName == GetCurrentUser()){
            
            currentUserRecordCount++;
        }
    }

    wxMessageBox(
        wxString::Format(
            wxString::FromUTF8("ログインユーザーの勤怠データ件数: %zu"),
            currentUserRecordCount
        ),
        
        wxString::FromUTF8("確認")
    );

    historyPanel->SetLoggedInUser(GetCurrentUser());

    historyPanel->SetAttendanceRecords(
        records,
        GetCurrentUser()
    );
}

void MainFrame::ShowEdit(){

    m_currentScreen = ScreenType::Edit;

    homePanel->Hide();

    menuPanel->Hide();

    historyPanel->Hide();

    editPanel->Show();

    vacationPanel->Hide();

    overtimePanel->Hide();

    userPanel->Hide();

    loginPanel->Hide();
}

void MainFrame::ShowVacation(){

    m_currentScreen = ScreenType::Vacation;

    homePanel->Hide();

    menuPanel->Hide();

    historyPanel->Hide();

    editPanel->Hide();

    vacationPanel->SetLoggedInUser(GetCurrentUser());
    vacationPanel->Show();

    overtimePanel->Hide();

    userPanel->Hide();

    loginPanel->Hide();
}

void MainFrame::ShowOvertime(){

    m_currentScreen = ScreenType::Overtime;

    homePanel->Hide();

    menuPanel->Hide();
    
    historyPanel->Hide();

    editPanel->Hide();

    vacationPanel->Hide();

    overtimePanel->SetLoggedInUser(GetCurrentUser());
    overtimePanel->Show();

    userPanel->Hide();

    loginPanel->Hide();
}

void MainFrame::ShowUser(){

    m_currentScreen = ScreenType::User;

    homePanel->Hide();

    menuPanel->Hide();

    historyPanel->Hide();

    editPanel->Hide();

    vacationPanel->Hide();

    overtimePanel->Hide();

    userPanel->Show();

    loginPanel->Hide();
}

void MainFrame::ShowLogin(){

    m_currentScreen = ScreenType::Login;

    homePanel->Hide();

    menuPanel->Hide();

    historyPanel->Hide();

    editPanel->Hide();

    vacationPanel->Hide();

    overtimePanel->Hide();

    userPanel->Hide();

    loginPanel->Show();
}

// アニメーションを行う時
void MainFrame::OnTimer(wxTimerEvent& event){
    // メニュー画面を開く処理
    if(m_menuState == MenuState::Opening){

        //10ずつ足している
        m_menuX += 10;

        //x座標を10pxずつ右に動かしていく
        menuPanel->Move(m_menuX, 0);


        // x座標が0以上になった時の条件
        if(m_menuX >= 0){

            //0にする
            m_menuX = 0;

            //x座標を動かしていたものを停止する
            menuPanel->Move(m_menuX, 0);
            
            // メニュー画面を開いた状態にする
            m_menuState = MenuState::Open;


            // タイマーを停止する
            m_timer.Stop();
        }
    
    // メニュー画面を閉じる時の処理
    }else if(m_menuState == MenuState::Closing){

        // 10ずつ引かせている
        m_menuX -= 10;

        // x座標を10pxずつ左に動かしていく
        menuPanel->Move(m_menuX, 0);

        // x座標が-250以下になった時の条件
        if(m_menuX <= -250){

            //-250にする
            m_menuX = -250;

            //x座標を-250pxに固定する
            menuPanel->Move(m_menuX, 0);

            //メニュー画面を非表示にする
            menuPanel->Hide();

            if(m_currentScreen == ScreenType::Home){
                
                homePanel->Show();
            }
            else if(m_currentScreen == ScreenType::History){

                historyPanel->Show();
            }
            else if(m_currentScreen == ScreenType::Edit){

                editPanel->Show();
            }
            else if(m_currentScreen == ScreenType::Vacation){

                vacationPanel->Show();
            
            }else if(m_currentScreen == ScreenType::Overtime){
                
                overtimePanel->Show();

            }else if(m_currentScreen == ScreenType::User){

                userPanel->Show();
            }

            else if(m_currentScreen == ScreenType::Login){

                loginPanel->Show();
            }

            // メニュー画面を閉じた状態にする
            m_menuState = MenuState::Closed;

            //タイマーを停止する
            m_timer.Stop();
        }
    }
}


void MainFrame::OnMenuClick(wxCommandEvent& event){

    if(m_menuState == MenuState::Closed){
        ShowMenu();

    }
}

// 各メニュー画面のボタンが押された処理を行う
void MainFrame::OnMenuButtonClicked(wxCommandEvent& event){

    ButtonType type = static_cast<ButtonType>(event.GetInt());

    if(type == ButtonType::CloseButton){
        Cansel();
    }else if(type == ButtonType::HomeButton){
        ShowHome();
    }
    else if(type == ButtonType::HistoryButton){
        ShowHistory();
    }
    else if(type == ButtonType::EditButton){
        ShowEdit();
    }
    else if(type == ButtonType::VacationButton){
        ShowVacation();
    }
    else if(type == ButtonType::OvertimeButton){
        ShowOvertime();
    }
    else if(type == ButtonType::MenuButton){
        ShowMenu();
    }
    else if(type == ButtonType::UserButton){
        ShowUser();
    }
    else if(type == ButtonType::LoginButton){

        ShowLogin();
    }
    else if(type == ButtonType::LogoutButton){
        Logout();
    }
}

// 各画面にユーザー名を入れる
void MainFrame::AddUser(const wxString& name){
    users.Add(name);

    //各画面のプルダウンメニューに追加

    historyPanel->AddUser(name);

    vacationPanel->AddUser(name);

    userPanel->AddUser(name);

}

bool MainFrame::RegisterUser(const wxString& name, const wxString& password){

    // データベースへ登録する   
    if(!database.AddUser(name, password)){

        return false;
    }

    // データベース登録に成功したときに各画面に反映する
    AddUser(name);

    return true;

}

bool MainFrame::LoginUser(const wxString& userName, const wxString& password){

    if(!database.Login(userName, password)){
        return false;
    }

    currentUser = userName;

    currentUserId = database.GetUserId(userName, password);

    homePanel->SetLoginUser(userName);

    vacationPanel->SetLoggedInUser(userName);

    overtimePanel->SetLoggedInUser(userName);

    menuPanel->UpdateLoginButtons(true);

    wxArrayString missingClockOutDates = database.GetMissingClockOutDates(currentUserId);

    if(!missingClockOutDates.IsEmpty()){

        wxString message =
            wxString::FromUTF8("退勤が記録されていない日付があります。\n\n");

        for(const wxString& date : missingClockOutDates){

            message += date;
            message += "\n";
        }

        wxMessageBox(
            message,
            wxString::FromUTF8("退勤忘れの警告"),
            wxOK | wxICON_WARNING
        );
    }

    wxArrayString overtimeDates = database.GetOvertimeDates(currentUserId);

    if(!overtimeDates.IsEmpty()){

        wxString message =
            wxString::FromUTF8("既定の勤怠時間を超えている日付があります。\n\n");

            for(const wxString& date : overtimeDates){

                message += date;
                message += "\n";
            }

            wxMessageBox(
                message,
                wxString::FromUTF8("残業時間の警告"),
                wxOK | wxICON_WARNING
            );
    }

    ShowHome();

    return true;
}

bool MainFrame::ClockIn(const AttendanceRecord& record){

    return database.ClockIn(record);
}

bool MainFrame::ClockOut(const AttendanceRecord& record){

    return database.ClockOut(record);
}

bool MainFrame::UpdateAttendance(const wxString& date, const wxString& time, bool updateClockIn, const wxString& correctionMessage){

    return database.UpdateAttendance(currentUserId, date, time, updateClockIn, correctionMessage);
}

wxString MainFrame::GetCurrentUser() const{

    return currentUser;
}


bool MainFrame::IsLoggedIn() const{

    return !currentUser.IsEmpty();
}

void MainFrame::Logout(){

    // ログインユーザーをクリアにする。
    currentUser.Clear();

    currentUserId = -1;

    // HomePanelの表示もクリアにする。
    homePanel->SetLoginUser(wxString::FromUTF8(""));

    loginPanel->ClearLogin();

    menuPanel->UpdateLoginButtons(false);

    ShowLogin();
    
    wxMessageBox(
        wxString::FromUTF8("ログアウトしました。"),
        wxString::FromUTF8("ログアウト")
    );
}

// 各画面にユーザー名を削除する
void MainFrame::RemoveUser(const wxString& name){

    int index = users.Index(name);

    if(index != wxNOT_FOUND){
        users.RemoveAt(index);
    }

    historyPanel->RemoveUser(name);


    vacationPanel->RemoveUser(name);

    userPanel->RemoveUser(name);
}

bool MainFrame::DeleteUser(const wxString& name, const wxString& password){

    if(!database.DeleteUser(name, password)){

        return false;
    }

    RemoveUser(name);

    return true;
}

bool MainFrame::SubmitVacationLeave(const wxString& userName, const wxString& date, const wxString& message){

    return database.SubmitVacationLeave(userName, date, message);
}

bool MainFrame::SubmitOvertime(const wxString& userName, const wxString& date, const wxString& message){

    return database.SubmitOvertime(userName, date, message);
}

//勤怠レコードを登録する処理
void MainFrame::AddAttendanceRecord(const AttendanceRecord& record){

    attendanceRecords.push_back(record);

}

// 勤怠レコードを最後のレコードを更新する。
void MainFrame::UpdateAttendanceRecord(const AttendanceRecord& record){

    for(int i = attendanceRecords.size() - 1; i >= 0; i--){

        if(attendanceRecords[i].userName == record.userName){

            attendanceRecords[i].clockOutTime = record.clockOutTime;

            return;
        }
    }

}

wxVector<AttendanceRecord> MainFrame::GetAttendanceRecords(){

    return database.GetAttendanceRecords();
}

AttendanceRecord* MainFrame::FindAttendanceRecord(const wxString& name){

    for(auto& record : attendanceRecords){

        if(record.userName == name){

            return &record;
        }
    }

    return nullptr;
}

int MainFrame::GetCurrentUserId() const{

    return currentUserId;
}