#ifndef HOMEPANEL_H
#define HOMEPANEL_H

#include <wx/wx.h>
#include "CustomButton.h"
#include "AttendanceRecord.h"

class HomePanel : public wxPanel{
    public:
        HomePanel(wxWindow* parent);

        void SetLoginUser(const wxString& userName);
    private:

        wxStaticText* loginUserLabel;

        // カスタムボタン出勤ボタン宣言
        CustomButton* clockInButton;
        // カスタムボタン退勤ボタン宣言
        CustomButton* clockOutButton;

        // ハンバーガーメニューボタン宣言
        wxButton* menuButton;
        
        // 現在日時表示
        wxStaticText* dateTimeText;

        //現在日時更新タイマー
        wxTimer m_clockTimer;

        // ユーザー選択ブルダウン
        wxChoice* userChoice;

        // 勤怠情報
        AttendanceRecord m_attendanceRecord;

        void OnClockIn(wxCommandEvent& event);

        void OnClockOut(wxCommandEvent& event);

        // 現在日時の更新
        void OnTimer(wxTimerEvent& event);

        void OnCustomButtonClick(wxCommandEvent& event);

        // ハンバーガーメニューのイベント宣言
        void OnMenuClick(wxCommandEvent& event);

        void SendAttendanceRecord();
};

#endif