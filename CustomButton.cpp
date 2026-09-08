#include "CustomButton.h"

wxDEFINE_EVENT(EVT_CUSTOM_BUTTON_CLICKED, wxCommandEvent);
// コンストラクタ
CustomButton::CustomButton(
    // 親ウィンドウを受け取る
    wxWindow* parent,
    //ボタンに表示する文字
    const wxString& label,
    //ボタンの座標
    const wxPoint& pos,
    // ボタンの横幅と高さ
    const wxSize& size,
    // ボタンの背景色
    const wxColour& backgroundcolour,
    //ボタンの乗った際の色
    const wxColour& hovercolour,
    // どのボタンか判別するため
    ButtonType type

) : wxPanel(parent, wxID_ANY, pos, size), //
    m_label(label),
    m_backgroundColour(backgroundcolour),
    m_hoverColour(hovercolour),
    m_isHover(false),
    m_type(type),
    m_isPressed(false)
{

    //イベントが起きたら各関数など実行をする
    Bind(wxEVT_PAINT, &CustomButton::OnPaint, this);//背景色や文字サイズなどの描画
    Bind(wxEVT_ENTER_WINDOW, &CustomButton::OnMouseEnter, this);//カーソルがボタン触れている時のイベント
    Bind(wxEVT_LEAVE_WINDOW, &CustomButton::OnMouseLeave, this);//カーソルがボタンから離れた時のイベント
    Bind(wxEVT_LEFT_DOWN, &CustomButton::OnLeftDown, this);//左クリックしたときのイベント
    Bind(wxEVT_LEFT_UP, &CustomButton::OnLeftUp, this);//左クリックした後のイベント
}

void CustomButton::OnPaint(wxPaintEvent& event){
    wxPaintDC dc(this);

    // ボタン背景色
    wxColour colour;

    //各ボタンが押された時の色の変更
    if(m_isPressed){
        // 出勤ボタン
        if(m_type == ButtonType::ClockIn){
            colour = wxColour(0, 100, 0);

        // 退勤ボタン
        }else if(m_type == ButtonType::ClockOut){
            colour = wxColour(100, 0, 0);

        // ホーム画面ボタン
        }else if(m_type == ButtonType::HomeButton){
            colour = wxColour(0, 0, 100);

        //勤怠履歴ボタン 
        }else if(m_type == ButtonType::HistoryButton){
            colour = wxColour(101, 67, 33);
        
        // 勤怠修正ボタン
        }else if(m_type == ButtonType::EditButton){
            colour = wxColour(190, 0, 190);
        
        //休暇申請ボタン 
        }else if(m_type == ButtonType::VacationButton){
            colour = wxColour(0, 100, 100);
        
        // 閉じるボタン
        }else if(m_type == ButtonType::CloseButton){
            colour = wxColour(50, 50, 50);
        
        }else if(m_type == ButtonType::UserButton){
            colour = wxColour(180, 100, 0);
        }

        else if(m_type == ButtonType::LoginButton){
            colour = wxColour(140, 10, 79);
        }
        else if(m_type == ButtonType::LogoutButton){
            colour = wxColour(255, 150, 150);
        }
        else if(m_type == ButtonType::EditSaveButton){
            colour = wxColour(0, 100, 0);
        }
        else if(m_type == ButtonType::VacationSaveButton){
            colour = wxColour(0, 100, 0);
        }
        else if(m_type == ButtonType::OvertimeSaveButton){
            colour = wxColour(130, 65, 0);
        }
        

    // ホバーした際の色
    }else if(m_isHover){
        colour = m_hoverColour;
    }else{
        colour = m_backgroundColour;
    }

    dc.SetBrush(wxBrush(colour));//塗りつぶし

    dc.SetPen(*wxTRANSPARENT_PEN);//枠線の描画(枠線無し)
    dc.DrawRectangle(GetClientRect());

    // 文字サイズを取得
    wxSize textSize = dc.GetTextExtent(m_label);

    // ボタンサイズを取得
    wxSize buttonSize = GetClientSize();
    
    //文字サイズとボタンサイズの横幅と高さで引き、2で割ることでボタンの中で中央揃えにして表示できる
    int x = (buttonSize.GetWidth() - textSize.GetWidth()) / 2;
    int y = (buttonSize.GetHeight() - textSize.GetHeight()) / 2;

    //ボタンをクリックした際に、文字を右下1pxをずらすようにする。
    if(m_isPressed){
        x += 1;
        y += 1;
    }

    //ウィンドウの背景色の設定
    dc.SetTextForeground(*wxWHITE);

    //文字(label)を座標(x, y)に描画する
    dc.DrawText(m_label, x, y);

}

//カーソルがボタンに入る
void CustomButton::OnMouseEnter(wxMouseEvent& event){
    //ホバーがされている
    m_isHover = true;

    // 再描画する。ここの場合は色を再描画を行っている
    //Refresh()がない場合、描画することはない
    Refresh();

    //イベント処理を他のイベントや標準処理にも渡す
    event.Skip();
}

// カーソルがボタンから出る
void CustomButton::OnMouseLeave(wxMouseEvent& event){
    // ホバーをしない
    m_isHover = false;

    Refresh();

    event.Skip();
}

// 左クリックでボタンに押した際の処理
void CustomButton::OnLeftDown(wxMouseEvent& event){
    //クリック処理を行う
    m_isPressed = true;

    Refresh();
    
    event.Skip();
}

//左クリックでボタンを離した際の処理
void CustomButton::OnLeftUp(wxMouseEvent& event){
    // 左クリック処理を行わない
    m_isPressed = false;

    Refresh();

    // CustomButtonがクリックしたときのイベントを作る
    wxCommandEvent buttonEvent(EVT_CUSTOM_BUTTON_CLICKED);

    //各ボタンにイベントを入れる
    buttonEvent.SetInt(static_cast<int>(m_type));

    //MenuPanelやHomePanelに通知する
    wxPostEvent(GetParent(), buttonEvent);

    event.Skip();
}