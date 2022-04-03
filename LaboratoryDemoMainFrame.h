/**
 * @file LaboratoryDemoMainFrame.h
 * @author Charles Owen
 *
 * The top-level (main) frame of the application
 */

#ifndef LABORATORYDEMO_LABORATORYDEMOMAINFRAME_H
#define LABORATORYDEMO_LABORATORYDEMOMAINFRAME_H

class LaboratoryView;
class ControlPanel;
class ILaboratoryIsolator;
class Controller;

/**
 * The top-level (main) frame of the application
 */
class LaboratoryDemoMainFrame final : public wxFrame {
private:
    /// Pointer to the laboratory view window
    LaboratoryView* mLaboratoryView;

    /// Pointer to the control panel window
    ControlPanel* mControlPanel;

    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);

public:
    LaboratoryDemoMainFrame(std::shared_ptr<ILaboratoryIsolator> laboratoryIsolator, Controller* controller);

    /// Default constructor (disabled)
    LaboratoryDemoMainFrame() = delete;

    /// Copy constructor (disabled)
    LaboratoryDemoMainFrame(const LaboratoryDemoMainFrame &) = delete;

    /// Assignment operator
    void operator=(const LaboratoryDemoMainFrame &) = delete;

    /**
     * Get a pointer to the laboratory view
     * @return LaboratoryView pointer
     */
    LaboratoryView *GetLaboratoryView() {return mLaboratoryView;}
};

#endif //LABORATORYDEMO_LABORATORYDEMOMAINFRAME_H
