#ifndef VACATIONPANEL_H
#define VACATIONPANEL_H

#include <wx/datectrl.h>

class VacationPanel : public wxPanel{
    public:
        VacationPanel(wxWindow* parent);

        void AddUser(const wxString& name);

        void RemoveUser(const wxString& name);

        void SetLoggedInUser(const wxString& name);
    
    private:
        wxStaticText* userLabel;
        wxDatePickerCtrl* datePicker;
        wxTextCtrl* messageText;
        wxString loggedInUser;
        

        void OnMenuClick(wxCommandEvent& event);
        void OnSubmitVacation(wxCommandEvent& event);
};

#endif