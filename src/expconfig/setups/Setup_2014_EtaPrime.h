#include "Setup.h"

namespace ant {
namespace expconfig {
namespace setup {

class Setup_2014_EtaPrime :
        public Setup
{
public:

    Setup_2014_EtaPrime() {
        const auto trigger = std::make_shared<detector::Trigger>();

        AddDetector(trigger);
        AddDetector<detector::EPT_2014>(GetBeamEnergy());
        AddDetector<detector::CB>();
        AddDetector<detector::PID_2014>();
        AddDetector<detector::TAPS_2013>(false, false); // no Cherenkov, don't use sensitive channels

        // the order of the calibrations can be important
        AddCalibration<calibration::TimingCATCH>(Detector_t::Type_t::CB, trigger->Reference_CATCH_CBCrate);
        AddCalibration<calibration::IntegralSADC>(Detector_t::Type_t::CB);
        AddCalibration<calibration::TimingCATCH>(Detector_t::Type_t::PID, trigger->Reference_CATCH_CBCrate);
        AddCalibration<calibration::IntegralCAEN>(Detector_t::Type_t::PID);
        AddCalibration<calibration::TimingTAPS>();
        AddCalibration<calibration::IntegralTAPS>();
    }

    virtual double GetBeamEnergy() const override {
        return 1604.0;
    }

    bool Matches(const THeaderInfo& header) const override {
        if(!Setup::Matches(header))
            return false;
        /// \todo Make beamtime match stricter than just detectors
        return true;

    }

    void BuildMappings(std::vector<hit_mapping_t>& hit_mappings,
                       std::vector<scaler_mapping_t>& scaler_mappings) const
    {
        Setup::BuildMappings(hit_mappings, scaler_mappings);
        // you may tweak the mapping at this location here
        // for example, ignore elements
    }
};

}}} // namespace ant::expconfig::setup
