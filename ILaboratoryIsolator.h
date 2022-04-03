/**
 * @file ILaboratoryIsolator.h
 * @author Charles Owen
 *
 * Interface for a laboratory isolator.
 */

#ifndef LABORATORYDEMO_ILABORATORYISOLATOR_H
#define LABORATORYDEMO_ILABORATORYISOLATOR_H

class ILaboratoryIsolator {
public:
    /**
     * Set the position for the root of the laboratory
     * @param location The x,y location to place the laboratory
     */
    virtual void SetLocation(wxPoint location) = 0;

    /**
    * Draw the laboratory at the currently specified location
    * @param graphics Graphics object to render to
    */
    virtual void DrawLaboratory(std::shared_ptr<wxGraphicsContext> graphics) = 0;

    /**
    * Set the current laboratory animation frame
    * @param frame Frame number
    */
    virtual void SetLaboratoryFrame(int frame) = 0;

    /**
     * Set the expected frame rate in frames per second
     */
    virtual void SetFrameRate(double rate) = 0;

    /**
    * Set the laboratory number
    * @param laboratory An integer number. Each number makes a different laboratory
    */
    virtual void SetLaboratoryNumber(int laboratory) = 0;

    /**
     * Get the current laboratory number
     * @return Laboratory number integer
     */
    virtual int GetLaboratoryNumber() = 0;

    /**
     * Get the current laboratory time.
     * @return Laboratory time in seconds
     */
    virtual double GetLaboratoryTime() = 0;

    /**
     * Display the laboratory selection dialog box
     * @param parent Parent window for the dialog box
     * @return True if OK pressed
     */
    virtual bool DoDialog(wxWindow* parent) = 0;
};

#endif //LABORATORYDEMO_ILABORATORYISOLATOR_H
