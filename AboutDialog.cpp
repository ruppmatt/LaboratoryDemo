/**
 * @file AboutDialog.cpp
 * @author Charles Owen
 */

#include "pch.h"
#include <wx/xrc/xmlres.h>
#include <wx/stdpaths.h>
#include <sstream>
#include <iomanip>

#include "AboutDialog.h"
#include "Config.h"

using namespace std;

/**
 * Constructor
 * @param parent Parent window
 */
AboutDialog::AboutDialog(wxWindow* parent)
{
    wxXmlResource::Get()->LoadDialog(this, parent, L"AboutDialog");

    auto versionCtrl = XRCCTRL(*this, "LaboratoryDemoVersion", wxStaticText);

    wstringstream str;
    str << L"Version " << LaboratoryDemoVersionMajor << L"." <<
        setfill(L'0') << setw(2) << LaboratoryDemoVersionMinor << L"." <<
        setfill(L'0') << setw(2) << LaboratoryDemoVersionPatch;
    versionCtrl->SetLabel(str.str());
}
