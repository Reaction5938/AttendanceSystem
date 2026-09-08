#ifndef LOGINPANEL_H
#define LOGINPANEL_H

#include <wx/wx.h>
#include "CustomButton.h"

class LoginPanel: public wxPanel{
    private:
        wxTextCtrl* userNameText;

        wxTextCtrl* passwordText;

        wxButton* loginButton;

        void OnLogin(wxCommandEvent& event);

        void OnMenuClick(wxCommandEvent& event); 

    public:
        LoginPanel(wxWindow* parent);

        void ClearLogin();

        
};

#endif