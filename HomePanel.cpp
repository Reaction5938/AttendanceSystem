#include "HomePanel.h"
#include "MainFrame.h"
#include "AttendanceRecord.h"
#include <chrono>
#include <ctime>


using namespace std;

HomePanel::HomePanel(wxWindow* parent) : wxPanel(parent), m_clockTimer(this){
     // 背景色
        SetBackgroundColour(*wxWHITE);
        
        // 現在日時表示
        dateTimeText = new wxStaticText(
            this,
            wxID_ANY,
            wxString::FromUTF8("現在日時"),
            wxPoint(250, 50)
        );

        m_clockTimer.Start(1000);

        Bind(wxEVT_TIMER, &HomePanel::OnTimer, this);

        // ハンバーガーメニュー
        menuButton = new wxButton(
            this,
            wxID_ANY,
            wxString::FromUTF8("≡"),
            wxPoint(10, 10),
            wxSize(40, 40)
        );

        menuButton->Bind(
            wxEVT_BUTTON,
            &HomePanel::OnMenuClick,
            this
        );

        loginUserLabel = new wxStaticText(
            this,
            wxID_ANY,
            wxString::FromUTF8("ログインユーザー : "),
            wxPoint(100, 100)
        );


        // 出勤ボタン
        clockInButton = new CustomButton(
            this,
            wxString::FromUTF8("出勤"), //日本語表示するためFromUTF8()でwxStringに変換している labelで取得する
            wxPoint(200, 200),//座標表示(x, y) wxPointでposとして取得する
            wxSize(100, 40),//横幅と高さの設定(横幅, 高さ)
            wxColour(0, 170, 0),
            wxColour(0, 220, 0),
            ButtonType::ClockIn
        );

        // 退勤ボタン
        clockOutButton = new CustomButton(
            this,
            wxString::FromUTF8("退勤"),
            wxPoint(350, 200),
            wxSize(100, 40),
            wxColour(170, 0, 0),
            wxColour(220, 0, 0),
            ButtonType::ClockOut
        );


        Bind(
            EVT_CUSTOM_BUTTON_CLICKED,
            &HomePanel::OnCustomButtonClick,
            this
        );
}

void HomePanel::OnMenuClick(wxCommandEvent& event){
    MainFrame* frame = dynamic_cast<MainFrame*>(GetParent());

    if(frame){
        frame->ShowMenu();
    }
}

// 現在日時を画面に表示する
void HomePanel::OnTimer(wxTimerEvent& event){
    
    // 現在時刻の取得
    auto now = chrono::system_clock::now();
    
    //nowをtime_tに変換する処理
    time_t currentTime = chrono::system_clock::to_time_t(now);

    tm localTime{};

    localtime_s(&localTime, &currentTime);

    wxString dateTime = wxString::Format(
        
        // 現在日時のフォーマット
        wxString::FromUTF8("%04d年%02d月%02d日 %02d:%02d:%02d"),

        // 年・月・日・時・分・秒を取り出している
        // tm_yearは1900年からの経過年数を表示するため + 1900をすることで現在の西暦になる。
        localTime.tm_year + 1900,

        //tm_monでは、0から11の値を表示する(0が1月となる)ため + 1をすることで現在の月を表示することができる。
        localTime.tm_mon + 1,

        localTime.tm_mday,
        localTime.tm_hour,
        localTime.tm_min,
        localTime.tm_sec
    );

    //dateTimeTextにdataTimeの文字列を表示する
    dateTimeText->SetLabel(dateTime);
}

void HomePanel::SetLoginUser(const wxString& userName){

    loginUserLabel->SetLabel(
        wxString::FromUTF8("ログインユーザー : ") + userName
    );
}


//出勤ボタンを押した際のイベント
void HomePanel::OnClockIn(wxCommandEvent& event){

    MainFrame* frame = dynamic_cast<MainFrame*>(GetParent());

    if(frame == nullptr){

        return;
    }

    wxString name = frame->GetCurrentUser();

    if(name.IsEmpty()){
        wxMessageBox(
            wxString::FromUTF8("ログインしてください。"),
            wxString::FromUTF8("出勤")
        );

        return;
    }

    auto now = chrono::system_clock::now();

    time_t currentTime = chrono::system_clock::to_time_t(now);

    tm localTime{};

    localtime_s(&localTime, &currentTime);

    wxString dateTime = wxString::Format(

        wxString::FromUTF8("%04d-%02d-%02d %02d:%02d:%02d"),

        localTime.tm_year + 1900,
        localTime.tm_mon + 1,
        localTime.tm_mday,
        localTime.tm_hour,
        localTime.tm_min,
        localTime.tm_sec
    );

    // 勤怠データに保存
    m_attendanceRecord.userId = frame->GetCurrentUserId();

    m_attendanceRecord.userName = name;
    m_attendanceRecord.clockInTime = dateTime;


    frame->AddAttendanceRecord(m_attendanceRecord);

    frame->ClockIn(m_attendanceRecord);

    //メッセージボックスに出勤時間の後に、dataTimeを入れることで、押された時の時刻を表示することができる。
    wxMessageBox(
        name + wxString::FromUTF8("  出勤時刻\n") + dateTime,
        wxString::FromUTF8("出勤")
    );
}

void HomePanel::OnClockOut(wxCommandEvent& event){


    MainFrame* frame = dynamic_cast<MainFrame*>(GetParent());

    if(frame == nullptr){

        return;
    }

    wxString name = frame->GetCurrentUser();

    if(name.IsEmpty()){
        
        wxMessageBox(
            wxString::FromUTF8("ログインをしてください。"),
            wxString::FromUTF8("退勤")
        );

        return;
    }

    auto now = chrono::system_clock::now();

    time_t currentTime = chrono::system_clock::to_time_t(now);

    tm localTime{};

    localtime_s(&localTime, &currentTime);

    wxString dateTime = wxString::Format(

        wxString::FromUTF8("%04d-%02d-%02d %02d:%02d:%02d"),

        localTime.tm_year + 1900,
        localTime.tm_mon + 1,
        localTime.tm_mday,
        localTime.tm_hour,
        localTime.tm_min,
        localTime.tm_sec
    );

    m_attendanceRecord.userId = frame->GetCurrentUserId();

    m_attendanceRecord.userName = name;

    m_attendanceRecord.clockOutTime = dateTime;


    frame->UpdateAttendanceRecord(m_attendanceRecord);

    frame->ClockOut(m_attendanceRecord);

    wxMessageBox(
        name + wxString::FromUTF8("  退勤時刻\n") + dateTime,
        wxString::FromUTF8("退勤")
    );
}

void HomePanel::OnCustomButtonClick(wxCommandEvent& event){

    ButtonType type =
        static_cast<ButtonType>(event.GetInt());

    if(type == ButtonType::ClockIn){
        OnClockIn(event);

    }else if(type == ButtonType::ClockOut){
        OnClockOut(event);
    }
}