#ifndef S57_VIEWER_H
#define S57_VIEWER_H

#include <wx/wx.h>
#include "s52plib.h"

class S57Viewer : public wxFrame
{
public:
    S57Viewer(const wxString& title, const wxPoint& pos, const wxSize& size);
    ~S57Viewer();

    bool LoadS57File(const wxString& filePath);

private:
    void OnPaint(wxPaintEvent& event);
    void OnOpenFile(wxCommandEvent& event);
    void OnClose(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    S52PLibHandle m_s52Handle;
    S57Dataset* m_s57Dataset;
    S57Bounds m_chartBounds;
    wxBitmap m_chartBitmap;

    wxDECLARE_EVENT_TABLE();
};

enum
{
    ID_Open = 1,
    ID_About,
    ID_Quit
};

#endif // S57_VIEWER_H
