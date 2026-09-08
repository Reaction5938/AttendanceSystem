#ifndef OVERTIMEPANEL_H
#define OVERTIMEPANEL_H

#include <wx/wx.h>
#include <wx/datectrl.h>

class OvertimePanel : public wxPanel{

    public:
        OvertimePanel(wxWindow* parent);

        void SetLoggedInUser(const wxString& userName);

    private:
        wxStaticText* userLabel;
        wxDatePickerCtrl* datePicker;
        wxTextCtrl* messageTextCtrl;
        wxString loggedInUser;

        void OnMenuClick(wxCommandEvent& event);
        void OnSubmitClick(wxCommandEvent& event);

        
};

#endif