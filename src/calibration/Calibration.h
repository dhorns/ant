#pragma once

#include "reconstruct/Reconstruct_traits.h"
#include "calibration/gui/Manager_traits.h"
#include "base/OptionsList.h"

#include <vector>

namespace ant {

class Calibration {
public:

    enum class AddMode_t
    {
        AsDefault,
        RightOpen,
        StrictRange
    };

    /**
     * @brief The Converter struct handles the transition from raw bytes
     * to somewhat meaningful values (not necessarily with physically meaningful units)
     *
     */
    struct Converter {
        using ptr_t = std::shared_ptr<const Converter>;

        virtual std::vector<double> Convert(const std::vector<uint8_t>& rawData) const = 0;
        virtual ~Converter() = default;
    };

    /**
     * @brief The BaseModule class provides a name for identification
     */
    class BaseModule
    {
    public:
        std::string GetName() const { return name; }
        virtual ~BaseModule() = default;
    protected:
        BaseModule(const std::string& name_) :
            name(name_)
        {}
    private:
        const std::string name;
    };

    /**
     * @brief The PhysicsModule class gives the modules the ability to create
     * histograms from events as well as fitting those histograms in order to
     * obtain the parameters needed for calibration
     */
    class PhysicsModule : public BaseModule
    {
    protected:
        using BaseModule::BaseModule; // constructors from base class
    public:
        // factory methods to request the modules providing physics/GUI functionality
        virtual std::vector<std::string> GetPhysicsModules() const {
            // usually there's a physics class with identical name
            // of the calibration module
            return {GetName()};
        }
        // GetGUIs cannot be const since returned Calib GUI Module might modify contents
        // managed by the module itself
        virtual void GetGUIs(std::list<std::unique_ptr<calibration::gui::CalibModule_traits> >& guis, const ant::OptionsPtr options) =0;
    };

    /**
     * @brief The Module class is typically used for calibrations
     *
     * Note that each module has the freedom to implement ReconstructHook::DetectorReadHits
     * or ReconstructHook::Clusters or both. Or you may define your own Calibration module,
     * as long as it derives at least from Calibration::BaseModule
     */
    class Module :
            public PhysicsModule,
            public Updateable_traits
    {
    protected:
       using PhysicsModule::PhysicsModule; // constructors from base class
    };



};

} // namespace ant

