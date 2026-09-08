#ifndef EDITPANEL_H
#define EDITPANEL_H

#include <wx/datectrl.h>
#include <wx/timectrl.h>
#include <wx/spinctrl.h>

class EditPanel : public wxPanel{
    public:
        EditPanel(wxWindow* parent);

    
    private:
        wxRadioBox* attendanceType;

        wxDatePickerCtrl* datePicker;
        
        wxSpinCtrl* hourSpinCtrl;
        
        wxSpinCtrl* minuteSpinCtrl;

        wxSpinCtrl* secondSpinCtrl;

        wxTextCtrl* correctMessageText;

        void OnMenuClick(wxCommandEvent& event);

        void OnSaveEdit(wxCommandEvent& event);
};

#endif