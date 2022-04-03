/**
 * @file Controller.cpp
 * @author Charles Owen
 */

#include "pch.h"
#include <iostream>
#include <wx/cmdline.h>
#include <wx/wfstream.h>
#include <wx/imaggif.h>
#include "wx/anidecod.h" // wxImageArray
#include <wx/quantize.h>

#include "ControlPanel.h"
#include "LaboratoryView.h"
#include "LaboratoryDemoMainFrame.h"

#include "Controller.h"


using namespace std;



/**
 * Handle the command line events. This is a sequence of keywords with optional
 * arguments. An example command line might be:
 *
 * frame 5 capture-png ../../temp/image5.png frame 10 capture-png ../../temp/image10.png exit
 *
 * The moves to frame 5, captures a PNG image to a file, moves to frame 10, captures
 * another PNG image, then exit the program.
 *
 * Commands:
 * frame N - moves the slider to frame N
 * capture-png filename - Captures the laboratory image as a file
 * capture - Captures the screen as an image, multiple images can be captured
 * laboratory N - Selection laboratory N
 * write-gif filename duration - Write captured images as an animated GIF file, frame duration in seconds
 * exit - Exit the program
 *
 * @param parser Command line parser object
 * @return true
 */
bool Controller::OnCmdLineParsed(wxCmdLineParser& parser)
{
    auto argc = parser.GetParamCount();
    for(int argnum=0; argnum<argc; argnum++)
    {
        auto arg = parser.GetParam(argnum);
        if(arg == "frame")
        {
            if(argnum >= argc-1)
            {
                cerr <<"The frame command requires a frame number" << endl;
                return false;
            }

            argnum++;
            mTasks.push_back(std::make_shared<TaskFrame>(this, parser.GetParam(argnum)));
        }
        else if(arg == "laboratory")
        {
            if(argnum >= argc-1)
            {
                cerr <<"The laboratory command requires a laboratory number" << endl;
                return false;
            }

            argnum++;
            mTasks.push_back(std::make_shared<TaskLaboratory>(this, parser.GetParam(argnum)));
        }
        else if(arg == "capture-png")
        {
            if(argnum >= argc-1)
            {
                cerr <<"The capture-png command requires a file name" << endl;
                return false;
            }

            argnum++;
            mTasks.push_back(std::make_shared<TaskCapture>(this, parser.GetParam(argnum)));
        }
        else if(arg == "write-gif")
        {
            if(argnum >= argc-2)
            {
                cerr <<"The write-gif command requires a file name and frame duration" << endl;
                return false;
            }

            mTasks.push_back(std::make_shared<TaskWriteGIF>(this,
                    parser.GetParam(argnum+1), parser.GetParam(argnum+2)));
            argnum += 2;
        }
        else if(arg == "capture")
        {
            mTasks.push_back(std::make_shared<TaskCapture>(this, L""));
        }
        else if(arg == "exit")
        {
            mTasks.push_back(std::make_shared<TaskExit>(this));
        }
    }

    return true;
}

/**
 * Initialize the command line parser.
 * @param parser Parser to initialize
 */
void Controller::OnInitCmdLine(wxCmdLineParser& parser)
{
    // Just initialize it to a variable list of optional parameters.
    parser.AddParam(L"", wxCMD_LINE_VAL_STRING,
            wxCMD_LINE_PARAM_MULTIPLE | wxCMD_LINE_PARAM_OPTIONAL);
}

/**
 * Set the windows that the controller will use.
 * @param frame MainFrame
 * @param laboratoryView The laboratory view
 * @param controlPanel The control panel
 */
void Controller::SetWindows(LaboratoryDemoMainFrame* frame, LaboratoryView* laboratoryView, ControlPanel* controlPanel)
{
    mMainFrame = frame;
    mLaboratoryView = laboratoryView;
    mControlPanel = controlPanel;
}

/**
 * Execute the contents of the task queue
 */
void Controller::Execute()
{
    if(mTasks.empty())
    {
        return;
    }

    std::shared_ptr<Task> task;
    do {
        task = mTasks.front();
        mTasks.pop_front();
    } while(!task->Execute() && !mTasks.empty());
}

/**
 * Constructor
 * @param controller The controller
 * @param arg Frame argument (integer value)
 */
Controller::TaskFrame::TaskFrame(Controller* controller, const wxString &arg) : Task(controller)
{
    mFrame = wxAtoi(arg);
}

bool Controller::TaskFrame::Execute()
{
    mController->mControlPanel->SetFrame(mFrame);
    mController->mLaboratoryView->Update();
    return false;
}

Controller::TaskCapture::TaskCapture(Controller* controller, const wxString& arg) : Task(controller)
{
    mFilename = arg;
}

bool Controller::TaskCapture::Execute()
{
    wxWindowDC windowDC(mController->mLaboratoryView);

    //Get the size of the screen/DC
    wxCoord screenWidth, screenHeight;
    windowDC.GetSize(&screenWidth, &screenHeight);

    //Create a Bitmap that will later on hold the screenshot image
    //Note that the Bitmap must have a size big enough to hold the screenshot
    //-1 means using the current default colour depth
    wxBitmap screenshot(screenWidth, screenHeight, -1);

    //Create a memory DC that will be used for actually taking the screenshot
    wxMemoryDC memDC;

    //Tell the memory DC to use our Bitmap
    //all drawing action on the memory DC will go to the Bitmap now
    memDC.SelectObject(screenshot);
    //Blit (in this case copy) the actual screen on the memory DC
    //and thus the Bitmap
    memDC.Blit(0, //Copy to this X coordinate
            0, //Copy to this Y coordinate
            screenWidth, //Copy this width
            screenHeight, //Copy this height
            &windowDC, //From where do we copy?
            0, //What's the X offset in the original DC?
            0  //What's the Y offset in the original DC?
    );
    //Select the Bitmap out of the memory DC by selecting a new
    //uninitialized Bitmap
    memDC.SelectObject(wxNullBitmap);

    if(!mFilename.IsEmpty())
    {
        //Our Bitmap now has the screenshot, so let's save it :-)
        screenshot.SaveFile(mFilename, wxBITMAP_TYPE_PNG);
    }
    else
    {
        // Create an image from the bitmap
        auto image = screenshot.ConvertToImage();

        // Add to the list
        mController->mImages.push_back(image);
    }

    return false;
}

bool Controller::TaskExit::Execute()
{
    mController->mMainFrame->Close();
    return true;
}

Controller::TaskWriteGIF::TaskWriteGIF(Controller* controller, const wxString& arg1, const wxString& arg2) : Task(controller)
{
    mFilename = arg1;
    mDuration = wxAtof(arg2);
}

bool Controller::TaskWriteGIF::Execute()
{
    wxFileOutputStream stream(mFilename);
    wxGIFHandler gif;
    wxImageArray images;
    for(auto image: mController->mImages)
    {
        wxQuantize::Quantize(image, image);
        images.Add(image);
    }
    mController->mImages.clear();

    auto ret = gif.SaveAnimation(images,
            &stream, true,
            int(mDuration * 1000));

    stream.Close();
    return false;
}

Controller::TaskLaboratory::TaskLaboratory(Controller* controller, const wxString& arg) :Task(controller)
{
    mLaboratory = wxAtoi(arg);
}

bool Controller::TaskLaboratory::Execute()
{
    mController->mControlPanel->SetLaboratoryNumber(mLaboratory);
    return false;
}
