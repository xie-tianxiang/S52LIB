#include "s52plib.h"

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "S57viewr.h"

class S57App : public wxApp
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(S57App);

bool S57App::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    // 创建主窗口
    S57Viewer* frame = new S57Viewer("S-57 Chart Viewer", wxPoint(50, 50), wxSize(1024, 768));
    frame->Show(true);
    
    return true;
}
