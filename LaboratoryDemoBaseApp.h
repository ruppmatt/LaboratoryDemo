/**
 * @file LaboratoryDemoBaseApp.h
 * @author Charles Owen
 *
 * Base class for the LaboratoryDemo application class
 */

#ifndef LABORATORYDEMO_LABORATORYDEMOBASEAPP_H
#define LABORATORYDEMO_LABORATORYDEMOBASEAPP_H

#include "Controller.h"

class ILaboratoryIsolator;


/**
 * Base class for the LaboratoryDemo application class
 * This ia an abstract class.
 */
class LaboratoryDemoBaseApp : public wxApp {
private:
    /// The application command-line-based controller
    Controller mController;

public:
    bool OnInit() override;

    virtual std::shared_ptr<ILaboratoryIsolator> CreateIsolator(std::wstring resourcesDir) = 0;

    bool OnCmdLineParsed(wxCmdLineParser & 	parser) override;

    void OnInitCmdLine(wxCmdLineParser& parser) override;
};

#endif //LABORATORYDEMO_LABORATORYDEMOBASEAPP_H
