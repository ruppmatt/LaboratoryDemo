/**
 * @file LaboratoryView.cpp
 * @author Charles Owen
 */

#include "pch.h"

#include <wx/dcbuffer.h>
#include <wx/xrc/xmlres.h>

#include "LaboratoryView.h"
#include "LaboratoryDemoMainFrame.h"
#include "ILaboratoryIsolator.h"


/// View width
const int ViewWidth = 1200;

/// View height
const int ViewHeight = 800;

/// Amount we offset the X origin into the window in pixel
const int OffsetX = 50;

/// Amount we offset the Y origin into the window in pixel
const int OffsetY = 50;

/// How long to draw the coordinate axis in pixels
const int AxisLength = 100;

/// Length of the coordinate axis arrows in pixels
const int ArrowLength = 10;

/// Width of the coordinate axis arrows in pixels
const int ArrowWidth = 10;

/**
 * Constructor
 * @param mainFrame Parent MainFrame object
 * @param resourcesDir Directory containing program resources
 */
LaboratoryView::LaboratoryView(LaboratoryDemoMainFrame* mainFrame,
        std::shared_ptr<ILaboratoryIsolator> laboratoryIsolatorIsolator, std::wstring resourcesDir) :
        wxScrolledCanvas(mainFrame, wxID_ANY), mLaboratoryIsolator(laboratoryIsolatorIsolator)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);

    Bind(wxEVT_PAINT, &LaboratoryView::OnPaint, this);


    mainFrame->Bind(wxEVT_COMMAND_MENU_SELECTED, &LaboratoryView::OnViewWhite, this, XRCID("ViewWhite"));
    mainFrame->Bind(wxEVT_UPDATE_UI, &LaboratoryView::OnUpdateUIViewWhite, this, XRCID("ViewWhite"));

    SetupLaboratory();
}

/**
 * View>White menu event handler
 * @param event Menu event
 */
void LaboratoryView::OnViewWhite(wxCommandEvent &event)
{
    mWhite = !mWhite;
    Refresh();
}

void LaboratoryView::OnUpdateUIViewWhite(wxUpdateUIEvent &event)
{
    event.Check(mWhite);
}

/**
 * Paint event, draws the window.
 * @param event Paint event object
 */
void LaboratoryView::OnPaint(wxPaintEvent& event)
{
    auto size = wxSize((int)(ViewWidth * mZoom), (int)(ViewHeight * mZoom));
    SetVirtualSize(size.GetWidth(), size.GetHeight());

    SetScrollRate(1, 1);

    auto clientSize = GetClientSize();

    if(mFirstDraw)
    {
        Scroll((size.GetWidth() - clientSize.GetWidth()) / 2,
                (size.GetHeight() - clientSize.GetHeight()) / 2);

    }

    auto pos = GetScrollPos(wxHORIZONTAL);

    wxAutoBufferedPaintDC dc(this);
    DoPrepareDC(dc);

    if(mWhite)
    {
        dc.SetBackground(*wxWHITE_BRUSH);
    }
    else
    {
        wxBrush background(wxColour(20, 20, 20));
        dc.SetBackground(background);
    }

    dc.Clear();

    // Create a graphics context
    auto graphics = std::shared_ptr<wxGraphicsContext>(wxGraphicsContext::Create( dc ));

    graphics->PushState();
    graphics->Scale(mZoom, mZoom);
    graphics->Translate(OffsetX, OffsetY);

    mLaboratoryIsolator->DrawLaboratory(graphics);

    DrawAxis(graphics);
    DrawBottomCorner(graphics);

    graphics->PopState();
    mFirstDraw = false;
}

/**
 * Draw the x,y coordinate axis.
 * @param graphics Graphics object to draw on
 */
void LaboratoryView::DrawAxis(std::shared_ptr<wxGraphicsContext> graphics)
{
    graphics->SetPen(*wxWHITE_PEN);
    graphics->StrokeLine(0, 0, AxisLength, 0);
    graphics->StrokeLine(AxisLength-ArrowLength, 0-ArrowWidth/2, AxisLength, 0);
    graphics->StrokeLine(AxisLength-ArrowLength, 0+ArrowWidth/2, AxisLength, 0);

    graphics->StrokeLine(0, 0, 0, AxisLength);
    graphics->StrokeLine(0-ArrowWidth/2, AxisLength-ArrowLength, 0, AxisLength);
    graphics->StrokeLine(0+ArrowWidth/2, AxisLength-ArrowLength, 0, AxisLength);

    wxFont font(wxSize(0, 20),
            wxFONTFAMILY_SWISS,
            wxFONTSTYLE_NORMAL,
            wxFONTWEIGHT_NORMAL);
    graphics->SetFont(font, *wxWHITE);

    graphics->DrawText(L"X", AxisLength + 5, -8);
    graphics->DrawText(L"Y", -6, AxisLength + 5);

    wxFont font2(wxSize(0, 10),
            wxFONTFAMILY_SWISS,
            wxFONTSTYLE_NORMAL,
            wxFONTWEIGHT_NORMAL);
    graphics->SetFont(font2, *wxWHITE);

    graphics->DrawText(L"(0,0)", 3, -14);

}

/**
 * Draw the x,y for the bottom right corner in space
 * @param graphics Graphics object to draw on
 */
void LaboratoryView::DrawBottomCorner(std::shared_ptr<wxGraphicsContext> graphics)
{
    int x = ViewWidth;
    int y = ViewHeight;

    graphics->SetPen(*wxWHITE_PEN);
    graphics->StrokeLine(x, y, x - AxisLength, y);
    graphics->StrokeLine(x, y, x, y - AxisLength);
//    graphics->StrokeLine(AxisLength-ArrowLength, 0-ArrowWidth/2, AxisLength, 0);
//    graphics->StrokeLine(AxisLength-ArrowLength, 0+ArrowWidth/2, AxisLength, 0);
//
//    graphics->StrokeLine(0, 0, 0, AxisLength);
//    graphics->StrokeLine(0-ArrowWidth/2, AxisLength-ArrowLength, 0, AxisLength);
//    graphics->StrokeLine(0+ArrowWidth/2, AxisLength-ArrowLength, 0, AxisLength);
}


/**
 * Set the laboratory frame
 * @param frame Frame number
 * @return The new computed laboratory time
 */
double LaboratoryView::SetFrame(int frame)
{
    mFrame = frame;
    mLaboratoryIsolator->SetLaboratoryFrame(mFrame);
    Refresh();

    return mLaboratoryIsolator->GetLaboratoryTime();
}


/**
 * Set the zoom factor.
 *
 * This attempts to keep the center of the image the same during zoom.
 * @param zoom New zoom factor
 */
void LaboratoryView::SetZoom(double zoom)
{
    // What is the center of the window?
    auto windowSize = GetClientSize();
    auto center = CalcUnscrolledPosition(wxPoint(windowSize.GetWidth() / 2, windowSize.GetHeight()/2));

    auto z1 = mZoom;
    auto z2 = zoom;

    auto viewStart = GetViewStart();

    // Set the new virtual size
    mZoom = zoom;
    auto size = wxSize((int)(ViewWidth * mZoom), (int)(ViewHeight * mZoom));
    SetVirtualSize(size.GetWidth(), size.GetHeight());

    auto viewStartX = lround(viewStart.x + center.x * (z2 - z1));
    if(viewStartX < 0)
    {
        viewStartX = 0;
    }

    auto viewStartY = lround(viewStart.y + center.y * (z2 - z1));
    if(viewStartY < 0)
    {
        viewStartY = 0;
    }

    Scroll(viewStartX, viewStartY);

    Refresh();
}

/**
 * Get the current laboratory time
 * @return Laboratory time in seconds
 */
double LaboratoryView::GetLaboratoryTime()
{
    return mLaboratoryIsolator->GetLaboratoryTime();
}

/**
 * Set up the laboratory:
 *  Location on screen
 *  Frame rate
 *  Current frame
 */
void LaboratoryView::SetupLaboratory()
{
    mLaboratoryIsolator->SetLocation(wxPoint(ViewWidth/2, ViewHeight));

    mLaboratoryIsolator->SetFrameRate(mFrameRate);
    mLaboratoryIsolator->SetLaboratoryFrame(mFrame);

    Refresh();
}
