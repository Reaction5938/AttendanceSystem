#ifndef HISTORYPANEL_H
#define HISTORYPANEL_H

#include <wx/listctrl.h>

#include "CustomButton.h"

#include "AttendanceRecord.h"

class HistoryPanel : public wxPanel{

    public:
        HistoryPanel(wxWindow* parent);

        void AddUser(const wxString& name);

        void RemoveUser(const wxString& name);

        void OnUserChanged(wxCommandEvent& event);

        void SetAttendanceRecords(const wxVector<AttendanceRecord>& records,
            const wxString& userName
        );

        void SetLoggedInUser(const wxString& name);

    private:
        wxChoice* userChoice;

        wxListCtrl* attendanceList;

        wxVector<AttendanceRecord> m_records;

        void OnMenuClick(wxCommandEvent& event);   
};

wxDECLARE_EVENT(EVT_CUSTOM_BUTTON_CLICKED, wxCommandEvent);
#endif