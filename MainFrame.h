#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include <wx/arrstr.h>

#include "CustomButton.h"
#include "HomePanel.h"
#include "MenuPanel.h"
#include "HistoryPanel.h"
#include "EditPanel.h"
#include "VacationPanel.h"
#include "OvertimePanel.h"
#include "UserPanel.h"
#include "AttendanceRecord.h"
#include "Database.h"
#include "LoginPanel.h"

enum class MenuState{
    Opening,
    Open,
    Closing,
    Closed
};

enum class ScreenType{
    Home,
    History,
    Edit,
    Vacation,
    Overtime,
    User,
    Login
};

class MainFrame : public wxFrame{
    private:

        // 勤怠データを保存する配列
        wxVector<AttendanceRecord> attendanceRecords;

        wxVector<AttendanceRecord> GetAttendanceRecords();

        AttendanceRecord* FindAttendanceRecord(const wxString& name);

        // 各画面の宣言
        MenuPanel* menuPanel;
        HomePanel* homePanel;
        HistoryPanel* historyPanel;
        EditPanel* editPanel;
        VacationPanel* vacationPanel;
        OvertimePanel* overtimePanel;
        UserPanel* userPanel;
        LoginPanel* loginPanel;


        // ボタンの宣言
        CustomButton* clockInButton;
        CustomButton* clockOutButton;

        //マウスイベント処理の宣言
        void OnMouseEnter(wxMouseEvent& event);
        void OnMouseLeave(wxMouseEvent& event);

        void OnMenuClick(wxCommandEvent& event);
        void OnCloseMenu(wxCommandEvent& event);

        // ハンバーガーメニューのアニメーション宣言
        wxTimer m_timer;

        //x座標から徐々に表示される宣言
        int m_menuX;

        // メニュー画面が開いているか確認する宣言
        MenuState m_menuState;

        ScreenType m_currentScreen;

        wxArrayString users;

        // データベースの宣言
        Database database;

        // 現在ログイン中のユーザー
        wxString currentUser;

        // アニメーションイベントの宣言
        void OnTimer(wxTimerEvent& event);
        //ハンバーガーメニューが押された時のイベント宣言
        void OnMenuButtonClicked(wxCommandEvent& event);
        // 各画面の切り替えイベントの宣言
        void OnPanelMenuClick(wxCommandEvent& event);

        int currentUserId = -1;

        
    public:
        MainFrame();

        wxString GetCurrentUser() const;

        bool IsLoggedIn() const;

        bool RegisterUser(const wxString& name, const wxString& password);

        bool LoginUser(const wxString& userName, const wxString& password);

        bool DeleteUser(const wxString& name, const wxString& password);

        bool ClockIn(const AttendanceRecord& record);

        bool ClockOut(const AttendanceRecord& record);

        bool UpdateAttendance(const wxString& date, const wxString& time, bool updateClockIn, const wxString& correctionMessage);

        bool SubmitVacationLeave(const wxString& userName, const wxString& date, const wxString& message);

        bool SubmitOvertime(const wxString& userName, const wxString& date, const wxString& message);

        int GetCurrentUserId() const;

        // 各画面にユーザー登録する
        void AddUser(const wxString& name);

        // 各画面にユーザー削除する
        void RemoveUser(const wxString & name);

        void Logout();

        // 勤怠登録するレコード
        void AddAttendanceRecord(const AttendanceRecord& record);

        //勤怠を最後のレコードに更新する
        void UpdateAttendanceRecord(const AttendanceRecord& record);

        // 各画面表示の定義
        void ShowMenu();
        void Cansel();
        void ShowHome();
        void ShowHistory();
        void ShowEdit();
        void ShowVacation();
        void ShowOvertime();
        void ShowUser();
        void ShowLogin();
};

#endif