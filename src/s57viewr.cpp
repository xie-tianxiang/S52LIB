#include "S57viewr.h"
#include <wx/filedlg.h>
#include <wx/msgdlg.h>

wxBEGIN_EVENT_TABLE(S57Viewer, wxFrame)
    EVT_PAINT(S57Viewer::OnPaint)
    EVT_MENU(ID_Open, S57Viewer::OnOpenFile)
    EVT_MENU(ID_About, S57Viewer::OnAbout)
    EVT_MENU(ID_Quit, S57Viewer::OnClose)
wxEND_EVENT_TABLE()

S57Viewer::S57Viewer(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size), m_s52Handle(nullptr), m_s57Dataset(nullptr)
{
    // 创建菜单栏
    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(ID_Open, "&Open S-57 File...\tCtrl+O");
    fileMenu->AppendSeparator();
    fileMenu->Append(ID_Quit, "E&xit\tCtrl+Q");

    wxMenu* helpMenu = new wxMenu;
    helpMenu->Append(ID_About, "&About...");

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");
    SetMenuBar(menuBar);

    // 初始化s52plib
    if (s52plib_init(&m_s52Handle) != S52P_SUCCESS)
    {
        wxMessageBox("Failed to initialize s52plib", "Error", wxOK | wxICON_ERROR);
    }

    CreateStatusBar();
    SetStatusText("Welcome to S-57 Chart Viewer");
}

S57Viewer::~S57Viewer()
{
    // 释放S57数据集
    if (m_s57Dataset)
    {
        s52plib_free_dataset(m_s57Dataset);
    }
    
    // 清理s52plib
    if (m_s52Handle)
    {
        s52plib_cleanup(m_s52Handle);
    }
}

bool S57Viewer::LoadS57File(const wxString& filePath)
{
    // 释放之前加载的数据集
    if (m_s57Dataset)
    {
        s52plib_free_dataset(m_s57Dataset);
        m_s57Dataset = nullptr;
    }

    // 加载新的S-57文件
    m_s57Dataset = s52plib_load_dataset(m_s52Handle, filePath.ToUTF8().data());
    if (!m_s57Dataset)
    {
        wxMessageBox("Failed to load S-57 file: " + filePath, "Error", wxOK | wxICON_ERROR);
        return false;
    }

    // 获取海图边界
    s57_get_bounds(m_s57Dataset, &m_chartBounds);
    
    // 更新状态栏
    SetStatusText("Loaded: " + filePath);
    
    // 触发重绘
    Refresh();
    return true;
}

void S57Viewer::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    
    // 如果没有加载海图，显示提示信息
    if (!m_s57Dataset)
    {
        dc.DrawText("Please open an S-57 file using File -> Open", 50, 50);
        return;
    }

    // 获取窗口客户区大小
    wxSize clientSize = GetClientSize();
    
    // 配置渲染参数
    S52RenderParams params = {
        .viewport_width = clientSize.GetWidth(),
        .viewport_height = clientSize.GetHeight(),
        .bounds = m_chartBounds,
        .display_scale = 1.0,
        .color_schema = S52_SCHEMA_STANDARD
    };

    // 创建内存缓冲区用于渲染
    wxImage image(clientSize.GetWidth(), clientSize.GetHeight(), true);
    unsigned char* data = image.GetData();
    
    // 使用s52plib渲染海图到内存缓冲区
    if (s52plib_render_to_buffer(m_s52Handle, m_s57Dataset, &params, data, clientSize.GetWidth() * 3) == S52P_SUCCESS)
    {
        // 转换为wxBitmap并绘制
        m_chartBitmap = wxBitmap(image);
        dc.DrawBitmap(m_chartBitmap, 0, 0, false);
    }
    else
    {
        dc.DrawText("Failed to render chart", 50, 50);
    }
}

void S57Viewer::OnOpenFile(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(this, _("Open S-57 File"), "", "", 
                               "S-57 Files (*.s57;*.000)|*.s57;*.000|All Files (*)|*", 
                               wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    LoadS57File(openFileDialog.GetPath());
}

void S57Viewer::OnClose(wxCommandEvent& event)
{
    Close(true);
}

void S57Viewer::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("S-57 Chart Viewer\nBased on wxWidgets and s52plib", 
                 "About", wxOK | wxICON_INFORMATION);
}
