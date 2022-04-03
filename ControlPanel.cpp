/**
 * @file ControlPanel.cpp
 * @author Charles Owen
 */

#include "pch.h"

#include <wx/xrc/xmlres.h>

#include "ControlPanel.h"
#include "LaboratoryDemoMainFrame.h"
#include "LaboratoryView.h"

#include "ILaboratoryIsolator.h"

/// Maximum frame we play to
const int MaxFrame = 900;

/// Minimum possible zoom factor
const double MinZoom = 0.5;

/// Maximum possible zoom factor
const double MaxZoom = 4.0;

/**
 * Constructor
 * @param mainFrame Parent window
 */
ControlPanel::ControlPanel(LaboratoryDemoMainFrame* mainFrame)
{
    mLaboratoryView = mainFrame->GetLaboratoryView();
    auto lab = mLaboratoryView->GetLaboratoryIsolator();

    wxXmlResource::Get()->LoadPanel(this, mainFrame, L"ControlPanel");

    //
    // Frame choice slider
    //
    mFrameSlider = XRCCTRL(*this, "FrameSlider", wxSlider);
    mFrameNumberCtrl = XRCCTRL(*this, "FrameNumber", wxStaticText);
    mFrameSlider->SetMax(MaxFrame);

    Bind(wxEVT_SLIDER, &ControlPanel::OnFrameSliderChanged, this, XRCID("FrameSlider"));

    mLaboratoryView->SetFrame(mFrameSlider->GetValue());

    //
    // Zoom slider
    //
    mZoomSlider = XRCCTRL(*this, "ZoomSlider", wxSlider);
    mZoomSlider->SetMax(200);
    mZoomSlider->SetValue((1.0 - MinZoom) * 200 / (MaxZoom - MinZoom));

    Bind(wxEVT_SLIDER, &ControlPanel::OnZoomSliderChanged, this, XRCID("ZoomSlider"));

    //
    // Laboratory selection
    //
    mLaboratoryNumberCtrl = XRCCTRL(*this, "LaboratoryNumber", wxStaticText);
    mLaboratoryNumberCtrl->SetLabel(wxString::Format(wxT("%i"), lab->GetLaboratoryNumber()));

    Bind(wxEVT_BUTTON, &ControlPanel::OnSelectLaboratory, this, XRCID("SelectLaboratory"));

    //
    // Time
    //
    mLaboratoryTimeCtrl = XRCCTRL(*this, "LabTime", wxStaticText);
    mLaboratoryTimeCtrl->SetLabel(wxString::Format(wxT("%0.2f"), lab->GetLaboratoryTime()));

    //
    // Playback
    //
    Bind(wxEVT_BUTTON, &ControlPanel::OnRewind, this, XRCID("Rewind"));
    Bind(wxEVT_BUTTON, &ControlPanel::OnPlay, this, XRCID("Play"));
    Bind(wxEVT_BUTTON, &ControlPanel::OnStop, this, XRCID("Stop"));
    Bind(wxEVT_TIMER, &ControlPanel::OnTimer, this);

    mTimer.SetOwner(this);
    mStopWatch.Start(0);
    mStopWatch.Pause();
}




/**
 * Handle changes to the frame slider
 * @param event Command event for slider value change
 */
void ControlPanel::OnFrameSliderChanged(wxCommandEvent &event)
{
    auto frame = mFrameSlider->GetValue();
    SetLaboratoryFrame(frame);
    Stop();
}

/**
 * Set the current frame in the laboratory
 * Sets the laboratory frame and the current frame and time controls
 * @param frame Frame to set
 */
void ControlPanel::SetLaboratoryFrame(int frame)
{
    auto time = mLaboratoryView->SetFrame(frame);
    mFrameNumberCtrl->SetLabel(wxString::Format(wxT("%i"),frame));
    mLaboratoryTimeCtrl->SetLabel(wxString::Format(wxT("%0.2f"), time));
}

/**
 * Handle changes to the frame slider
 * @param event Command event for slider value change
 */
void ControlPanel::OnZoomSliderChanged(wxCommandEvent &event)
{
    mLaboratoryView->SetZoom(MinZoom + mZoomSlider->GetValue() *(MaxZoom - MinZoom) / mZoomSlider->GetMax());
}

/**
 * Handle a Select Laboratory button press
 * @param event Button press command event
 */
void ControlPanel::OnSelectLaboratory(wxCommandEvent& event)
{
    auto lab = mLaboratoryView->GetLaboratoryIsolator();
    if(lab->DoDialog(this->GetParent()))
    {
        mLaboratoryNumberCtrl->SetLabel(wxString::Format(wxT("%i"), lab->GetLaboratoryNumber()));
        mLaboratoryView->SetupLaboratory();
    }
}

/**
 * Handle a rewind button event.
 *
 * Stop any playing that may be active and reset the frame to zero
 *
 * @param event Button event
 */
void ControlPanel::OnRewind(wxCommandEvent& event)
{
    Stop();
    mFrameSlider->SetValue(0);
    SetLaboratoryFrame(0);
}

/**
 * Handle a play button press
 * @param event Button event
 */
void ControlPanel::OnPlay(wxCommandEvent& event)
{
    if(mPlaying)
    {
        // If already playing
        return;
    }

    auto frameRate = mLaboratoryView->GetFrameRate();
    auto time = mLaboratoryView->GetLaboratoryTime();

    mStopWatch.Start(lround(time * 1000));
    mTimer.Start(1000 / frameRate);
}

/**
 * Handle a Stop button press
 * @param event Button event
 */
void ControlPanel::OnStop(wxCommandEvent& event)
{
    Stop();
}

/**
 * Stop any playback animation
 */
void ControlPanel::Stop()
{
    mPlaying = false;
    mTimer.Stop();
    mStopWatch.Pause();
}

/**
 * Handle timer events
 * @param event timer event
 */
void ControlPanel::OnTimer(wxTimerEvent& event)
{
    auto newTime = mStopWatch.Time();
    auto frameRate = mLaboratoryView->GetFrameRate();

    int frame = (int)lround(newTime / 1000.0 * frameRate);
    if(frame >= MaxFrame)
    {
        frame = MaxFrame;
        Stop();
    }

    mFrameSlider->SetValue(frame);
    SetLaboratoryFrame(frame);
}

void ControlPanel::SetFrame(int frame)
{
    mFrameSlider->SetValue(frame);
    SetLaboratoryFrame(frame);
}

void ControlPanel::SetLaboratoryNumber(int laboratory)
{
    auto lab= mLaboratoryView->GetLaboratoryIsolator();
    lab->SetLaboratoryNumber(laboratory);
    mLaboratoryNumberCtrl->SetLabel(wxString::Format(wxT("%i"), lab->GetLaboratoryNumber()));
    mLaboratoryView->SetupLaboratory();

    mFrameSlider->SetValue(0);
    SetLaboratoryFrame(0);
}
