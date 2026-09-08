#ifndef ATTENDANCERECORD_H
#define ATTENDANCERECORD_H

#include <wx/wx.h>

// 勤怠履歴を出すため構造体
struct AttendanceRecord{

    int attendanceId;

    int userId;

    wxString userName;

    wxString clockInTime;

    wxString clockOutTime;

    wxString correctMessage;

    wxString vacationMessage;

    wxString overtimeMessage;
};

#endif