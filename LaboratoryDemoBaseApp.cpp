/**
 * @file LaboratoryDemoBaseApp.cpp
 * @author Charles Owen
 */

#include "pch.h"

#include <wx/xrc/xmlres.h>
#include <wx/stdpaths.h>
#include <wx/cmdline.h>

#include "LaboratoryDemoBaseApp.h"
#include "LaboratoryDemoMainFrame.h"

/**
 * Initialize the application
 * @return True if successful
 */
bool LaboratoryDemoBaseApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    // Add image type handlers
    wxInitAllImageHandlers();

    // Get pointer to XML resource system
    auto xmlResource = wxXmlResource::Get();

    // Initialize XRC handlers
    xmlResource->InitAllHandlers();

    // Load all XRC resources from the program resources
    auto standardPaths = wxStandardPaths::Get();
    auto resourcesDir = standardPaths.GetResourcesDir();
    if (!wxXmlResource::Get()->LoadAllFiles(resourcesDir + "/xrc"))
    {
        return false;
    }

    // Create the laboratory isolator containing a laboratory
    auto laboratory = CreateIsolator(resourcesDir.ToStdWstring());

    auto frame = new LaboratoryDemoMainFrame(laboratory, &mController);
    frame->Show(true);

    mController.Execute();
    return true;
}

void LaboratoryDemoBaseApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    wxAppBase::OnInitCmdLine(parser);
    mController.OnInitCmdLine(parser);
}

bool LaboratoryDemoBaseApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    if(!mController.OnCmdLineParsed(parser))
    {
        return false;
    }

    return wxAppBase::OnCmdLineParsed(parser);
}
