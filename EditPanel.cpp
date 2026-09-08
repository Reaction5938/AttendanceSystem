#include <wx/wx.h>
#include <wx/datectrl.h>
#include <wx/timectrl.h>
#include "EditPanel.h"
#include "CustomButton.h"
#include "MainFrame.h"

EditPanel::EditPanel(wxWindow* parent) : wxPanel(parent){
    SetBackgroundColour(*wxWHITE);

    new wxStaticText(
        this,
        wxID_ANY,
        wxString::FromUTF8("勤怠修正画面"),
        wxPoint(80, 60)
    );

    wxButton* menuButton = new wxButton(
        this,
        wxID_ANY,
        wxString::FromUTF8("≡"),
        wxPoint(10, 10),
        wxSize(40, 40)
    );

    menuButton->Bind(
        wxEVT_BUTTON,
        &EditPanel::OnMenuClick,
        this
    );

    attendanceType = new wxRadioBox(
        this,
        wxID_ANY,
        wxString::FromUTF8("勤怠種別"),
        wxPoint(80, 120),
        wxSize(250, 65),
        2,
        new wxString[]{
            wxString::FromUTF8("出勤 "),
            wxString::FromUTF8("退勤 ")
        },
        2,
        wxRA_SPECIFY_COLS
    );

    new wxStaticText(
        this,
        wxID_ANY,
        wxString::FromUTF8("日付"),
        wxPoint(80, 205)
    );

    datePicker = new wxDatePickerCtrl(
        this,
        wxID_ANY,
        wxDateTime::Today(),
        wxPoint(150, 200),
        wxSize(150, 30)
    );

    new wxStaticText(
        this,
        wxID_ANY,
        wxString::FromUTF8("時刻"),
        wxPoint(80, 250)
    );

    hourSpinCtrl = new wxSpinCtrl(
        this,
        wxID_ANY,
        wxString::FromUTF8(""),
        wxPoint(150, 245),
        wxSize(55, 30),
        wxSP_ARROW_KEYS,
        0,
        23,
        wxDateTime::Now().GetHour()
    );

    new wxStaticText(
        this,
        wxID_ANY,
        wxString::FromUTF8("時"),
        wxPoint(208, 250)
    );

    minuteSpinCtrl = new wxSpinCtrl(
        this,
        wxID_ANY,
        wxString::FromUTF8(""),
        wxPoint(230, 245),
        wxSize(55, 30),
        wxSP_ARROW_KEYS,
        0,
        59,
        wxDateTime::Now().GetMinute()
    );

    new wxStaticText(
        this,
        wxID_ANY,
        wxString::FromUTF8("分"),
        wxPoint(288, 250)
    );

    secondSpinCtrl = new wxSpinCtrl(
        this,
        wxID_ANY,
        wxString::FromUTF8(""),
        wxPoint(310, 245),
        wxSize(55, 30),
        wxSP_ARROW_KEYS,
        0,
        59,
        wxDateTime::Now().GetSecond()
    );

    new wxStaticText(
        this,
        wxID_ANY,
        wxString::FromUTF8("秒"),
        wxPoint(368, 250)
    );

    new wxStaticText(
        this,
        wxID_ANY,
        wxString::FromUTF8("修正メッセージ"),
        wxPoint(80, 300)
    );

    correctMessageText = new wxTextCtrl(
        this,
        wxID_ANY,
        wxString::FromUTF8(""),
        wxPoint(80, 325),
        wxSize(350, 75),
        wxTE_MULTILINE
    );

    CustomButton* saveButton = new CustomButton(
        this,
        wxString::FromUTF8("修正内容を保存"),
        wxPoint(80, 425),
        wxSize(250, 45),
        wxColour(0, 128, 0),
        wxColour(0, 168, 0),
        ButtonType::EditSaveButton
    );

    Bind(
        EVT_CUSTOM_BUTTON_CLICKED,
        &EditPanel::OnSaveEdit,
        this
    );
}

void EditPanel::OnMenuClick(wxCommandEvent& event){
    wxCommandEvent menuEvent(EVT_CUSTOM_BUTTON_CLICKED);

    menuEvent.SetInt(static_cast<int>(ButtonType::MenuButton));

    wxPostEvent(GetParent(), menuEvent);
}



void EditPanel::OnSaveEdit(wxCommandEvent& event){

    if(event.GetInt() != static_cast<int>(ButtonType::EditSaveButton)){

        return;
    }

    wxString time = wxString::Format(
        "%02d:%02d:%02d",
        hourSpinCtrl->GetValue(),
        minuteSpinCtrl->GetValue(),
        secondSpinCtrl->GetValue()
    );

    wxString message = correctMessageText->GetValue();

    if(message.IsEmpty()){

        wxMessageBox(
            wxString::FromUTF8("修正内容が入力されていません。"),
            wxString::FromUTF8("入力エラー"),
            wxOK | wxICON_ERROR
        );

        return;
    }

    MainFrame* frame = dynamic_cast<MainFrame*>(GetParent());

    if(frame == nullptr){
        return;
    }

    bool updateClockIn = attendanceType->GetSelection() == 0;

    bool success = frame->UpdateAttendance(
        datePicker->GetValue().FormatISODate(),
        time,
        updateClockIn,
        message
    );

    if(success){
        wxMessageBox(
            wxString::FromUTF8("修正内容を保存しました。"),
            wxString::FromUTF8("勤怠修正")
        );
    }
}
