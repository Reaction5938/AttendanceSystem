#include <wx/wx.h>
#include "MainFrame.h"

class MyApp : public wxApp
{
public:
    //アプリケーションが起動した直後に最初に一度だけ呼び出す関数
    bool OnInit() override
    {
        // ウィンドウ表示する際にMainFrameで定義した型を呼び出す
        MainFrame* frame = new MainFrame();

        //ウィンドウ上に表示する
        frame->Show();
        return true;
    }
};

//アプリケーションを開始するためのマクロ
wxIMPLEMENT_APP(MyApp);