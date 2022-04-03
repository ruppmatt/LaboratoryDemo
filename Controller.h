/**
 * @file Controller.h
 * @author Charles Owen
 *
 * This class supports the control of the
 * application using the command line.
 */

#ifndef LABORATORYDEMO_CONTROLLER_H
#define LABORATORYDEMO_CONTROLLER_H

#include <list>

class LaboratoryDemoMainFrame;
class LaboratoryView;
class ControlPanel;

/**
 * This class supports the control of the
 * application using the command line.
 */
class Controller final {
private:
    /// The MainFrame window
    LaboratoryDemoMainFrame* mMainFrame = nullptr;

    /// The Laboratory view
    LaboratoryView* mLaboratoryView = nullptr;

    /// The control panel
    ControlPanel* mControlPanel = nullptr;

    class Task {
    protected:
        Controller* mController;

    public:
        Task(Controller* controller) : mController(controller) {}

        virtual ~Task() = default;

        /// Default constructor (disabled)
        Task() = delete;

        /// Copy constructor (disabled)
        Task(const Task &) = delete;

        /// Assignment operator
        void operator=(const Task &) = delete;

        /**
         * Execute this task,
         * @return
         */
        virtual bool Execute() = 0;
    };

    class TaskFrame : public Task {
    private:
        int mFrame;

    public:
        TaskFrame(Controller* controller, const wxString &arg);
        bool Execute() override;
    };

    class TaskLaboratory : public Task {
    private:
        int mLaboratory;

    public:
        TaskLaboratory(Controller* controller, const wxString &arg);
        bool Execute() override;
    };

    class TaskCapture : public Task {
    private:
        wxString mFilename;

    public:
        TaskCapture(Controller* controller, const wxString &arg);
        bool Execute() override;
    };

    class TaskWriteGIF : public Task {
    private:
        wxString mFilename;
        double mDuration;

    public:
        TaskWriteGIF(Controller* controller, const wxString &arg1, const wxString &arg2);
        bool Execute() override;
    };

    class TaskExit : public Task {
    private:
        wxString mFilename;

    public:
        explicit TaskExit(Controller* controller) : Task(controller) {}
        bool Execute() override;
    };

    std::list<std::shared_ptr<Task>> mTasks;
    std::vector<wxImage> mImages;

public:
    Controller() = default;

    void OnInitCmdLine(wxCmdLineParser& parser);
    bool OnCmdLineParsed(wxCmdLineParser& parser);
    void SetWindows(LaboratoryDemoMainFrame *frame,
            LaboratoryView* laboratoryView, ControlPanel* controlPanel);

    void Execute();
};

#endif //LABORATORYDEMO_CONTROLLER_H
