#ifndef DATABASE_H
#define DATABASE_H

#include <wx/wx.h>
#include <libpq-fe.h>
#include "AttendanceRecord.h"

class Database{
    public:
        Database();

        bool Connect();

        // ユーザー登録
        bool AddUser(
            const wxString& userName,
            const wxString& password
        );

        // ユーザー削除
        bool DeleteUser(
            const wxString& userName,
            const wxString& password
        );

        // ユーザーログイン
        bool Login(
            const wxString& userName,
            const wxString& password
        );

        wxArrayString GetUsers();

        // 出勤レコード
        bool ClockIn(
            const AttendanceRecord& record
        );

        // 退勤レコード
        bool ClockOut(
            const AttendanceRecord& record
        );

        bool UpdateAttendance(
            int userId,
            const wxString& date,
            const wxString& time,
            bool updateClockIn,
            const wxString& correctionMessage

        );

        bool SubmitVacationLeave(
            const wxString& userName,
            const wxString& date,
            const wxString& message
        );

        bool SubmitOvertime(
            const wxString& userName,
            const wxString& date,
            const wxString& message
        );



        int GetUserId(const wxString& userName, const wxString& password);

        wxVector<AttendanceRecord> GetAttendanceRecords();

        wxArrayString GetMissingClockOutDates(int userId);

        wxArrayString GetOvertimeDates(int userId);
        

    private:
        
        PGconn* conn;

};

#endif