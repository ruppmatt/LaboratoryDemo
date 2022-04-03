/**
 * @file LaboratoryDemoMainFrame.cpp
 * @author Charles Owen
 */

#include "pch.h"

#include <wx/xrc/xmlres.h>
#include <wx/stdpaths.h>

#include "LaboratoryDemoMainFrame.h"
#include "ControlPanel.h"
#include "LaboratoryView.h"
#include "AboutDialog.h"
#include "Controller.h"


/**
 * Constructor
 */
LaboratoryDemoMainFrame::LaboratoryDemoMainFrame(std::shared_ptr<ILaboratoryIsolator> laboratoryIsolator,
        Controller* controller)
{
    wxXmlResource::Get()->LoadFrame(this, nullptr, L"MainFrame");
#ifdef WIN32
    SetIcon(wxIcon(L"mainframe", wxBITMAP_TYPE_ICO_RESOURCE));
#endif

    // Where do the resources come from?
    auto standardPaths = wxStandardPaths::Get();
    auto resourcesDir = standardPaths.GetResourcesDir();

    auto sizer = new wxBoxSizer( wxVERTICAL );

    mLaboratoryView = new LaboratoryView(this, laboratoryIsolator, resourcesDir.ToStdWstring());
    mControlPanel = new ControlPanel(this);

    sizer->Add(mLaboratoryView, 1, wxEXPAND | wxALL);
    sizer->Add(mControlPanel, 0, wxEXPAND | wxALL);

    SetSizer( sizer );
    Layout();

    controller->SetWindows(this, mLaboratoryView, mControlPanel);

    Bind(wxEVT_COMMAND_MENU_SELECTED, &LaboratoryDemoMainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &LaboratoryDemoMainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_CLOSE_WINDOW, &LaboratoryDemoMainFrame::OnClose, this);
}

/**
 * Exit menu option handlers
 * @param event Menu event
 */
void LaboratoryDemoMainFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

/**
 * Application about box menu handler
 * @param event Menu event
 */
void LaboratoryDemoMainFrame::OnAbout(wxCommandEvent& event)
{
    AboutDialog aboutDlg(this);
    aboutDlg.ShowModal();
}

/**
 * Handle a close event. Stop the animation and destroy this window.
 * @param event The Close event
 */
void LaboratoryDemoMainFrame::OnClose(wxCloseEvent& event)
{
    mControlPanel->Stop();
    Destroy();
}





