#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <wx/wx.h>

// 各ボタンの種類別宣言
enum class ButtonType{
    ClockIn,
    ClockOut,
    MenuButton,
    HomeButton,
    HistoryButton,
    EditButton,
    VacationButton,
    OvertimeButton,
    CloseButton,
    UserButton,
    LoginButton,
    LogoutButton,
    EditSaveButton,
    VacationSaveButton,
    OvertimeSaveButton
};

// ボタンの描画やイベント処理の宣言
class CustomButton : public wxPanel
{
public:
    CustomButton(wxWindow* parent,
                 const wxString& label,
                 const wxPoint& pos,
                 const wxSize& size,
                 const wxColour& backgroundcolour,
                 const wxColour& hovercolour,
                 ButtonType type);

private:
    // 各ボタンの種類別を見る宣言
    ButtonType m_type;
    
    //文字をlabelに取る宣言
    wxString m_label;
    
    // ボタンの背景色を取るための宣言
    wxColour m_backgroundColour;
    // カーソルがボタンに入ったときの色を取るための宣言
    wxColour m_hoverColour;

    //カーソルがボタンに入ったか確認する宣言
    bool m_isHover;

    //ボタンの所で左クリックしたか確認する宣言
    bool m_isPressed;

    
    void OnPaint(wxPaintEvent& event);
    void OnMouseEnter(wxMouseEvent& event);
    void OnMouseLeave(wxMouseEvent& event);
    void OnLeftDown(wxMouseEvent& event);
    void OnLeftUp(wxMouseEvent& event);
};

wxDECLARE_EVENT(EVT_CUSTOM_BUTTON_CLICKED, wxCommandEvent);

#endif