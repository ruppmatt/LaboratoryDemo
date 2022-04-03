/**
 * @file ControlPanel.h
 * @author Charles Owen
 *
 * The laboratory demonstrator control panel
 */

#ifndef LABORATORYDEMO_CONTROLPANEL_H
#define LABORATORYDEMO_CONTROLPANEL_H

class LaboratoryDemoMainFrame;
class LaboratoryView;


/**
 * The laboratory demonstrator control panel
 */
class ControlPanel final : public wxPanel {
private:
    void OnFrameSliderChanged(wxCommandEvent& event);
    void OnZoomSliderChanged(wxCommandEvent& event);
    void OnSelectLaboratory(wxCommandEvent& event);
    void OnRewind(wxCommandEvent& event);
    void OnPlay(wxCommandEvent& event);
    void OnStop(wxCommandEvent& event);
    void OnTimer(wxTimerEvent& event);

    void SetLaboratoryFrame(int frame);

    /// Pointer to the laboratory view
    LaboratoryView *mLaboratoryView;

    /// The frame choice slider
    wxSlider *mFrameSlider;

    /// The static control that displays the current frame
    wxStaticText *mFrameNumberCtrl;

    /// The zoom slider
    wxSlider *mZoomSlider;

    /// The static control that displays the laboratory number
    wxStaticText *mLaboratoryNumberCtrl;

    /// Laboratory time control
    wxStaticText *mLaboratoryTimeCtrl;

    /// The timer that allows for animation
    wxTimer mTimer;

    /// Stopwatch used to measure elapsed time
    wxStopWatch mStopWatch;

    /// Are we playing?
    bool mPlaying = false;

public:
    ControlPanel(LaboratoryDemoMainFrame *mainFrame);

    void Stop();

    void SetFrame(int frame);
    void SetLaboratoryNumber(int laboratory);
};

#endif //LABORATORYDEMO_CONTROLPANEL_H
