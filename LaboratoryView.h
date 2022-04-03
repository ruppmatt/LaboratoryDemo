/**
 * @file LaboratoryView.h
 * @author Charles Owen
 *
 * The view for our laboratory
 */

#ifndef LABORATORYDEMO_LABORATORYVIEW_H
#define LABORATORYDEMO_LABORATORYVIEW_H

class LaboratoryDemoMainFrame;
class ILaboratoryIsolator;

/**
 * The view for our laboratory
 */
class LaboratoryView final : public wxScrolledCanvas {
private:
    void OnPaint(wxPaintEvent& event);
    void DrawBottomCorner(std::shared_ptr<wxGraphicsContext> graphics);
    void OnViewWhite(wxCommandEvent& event);


    /// First time we draw
    bool mFirstDraw = true;

    /// The laboratory we are displaying
    std::shared_ptr<ILaboratoryIsolator> mLaboratoryIsolator;

    /// Current laboratory frame
    int mFrame = 0;

    /// Zoom factor
    double mZoom = 1.0;

    /// Frame rate
    int mFrameRate = 30;

    /// White background option
    bool mWhite = false;

public:
    explicit LaboratoryView(LaboratoryDemoMainFrame *mainFrame, std::shared_ptr<ILaboratoryIsolator> isolator, std::wstring resourcesDir);

    void DrawAxis(std::shared_ptr<wxGraphicsContext> graphics);

    double SetFrame(int frame);

    void SetZoom(double zoom);

    /**
     * Get the laboratory we are viewing, as
     * protected by the isolator.
     * @return Laboratory in an isolator object
     */
    auto GetLaboratoryIsolator() {return mLaboratoryIsolator;}

    double GetLaboratoryTime();

    void SetupLaboratory();

    /**
     * Get the frame rate
     * @return Frame rate in frames per second
     */
    auto GetFrameRate() const {return mFrameRate;}

    void OnUpdateUIViewWhite(wxUpdateUIEvent& event);
};

#endif //LABORATORYDEMO_LABORATORYVIEW_H
