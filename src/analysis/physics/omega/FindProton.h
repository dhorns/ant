#pragma once

#include "analysis/physics/Physics.h"
#include "base/std_ext/math.h"
#include "analysis/utils/fitter/TreeFitter.h"
#include "analysis/plot/PromptRandomHist.h"
#include "utils/TriggerSimulation.h"

class TTree;

namespace ant {
namespace analysis {
namespace physics {

class FindProton : public Physics {
protected:

    struct branches_t {
        double chi2dof     = std_ext::NaN;
        double probability = std_ext::NaN;
        double copl_angle  = std_ext::NaN;
        double angle_p_cp  = std_ext::NaN;
        int    matched_p   = -1;
        int    isBest      = -1;
        int    fitstatus   = -1;

        double mangle      = std_ext::NaN;

        double p_theta     = std_ext::NaN;
        double p_phi       = std_ext::NaN;
        double p_PSA_r     = std_ext::NaN;
        double p_PSA_a     = std_ext::NaN;
        double p_veto      = std_ext::NaN;
        int    p_detector  = -1;

        double TagW        = std_ext::NaN;
        double TagE        = std_ext::NaN;
        double TagTime     = std_ext::NaN;
        int    TagCh       = -1;

        branches_t() = default;
        branches_t(const branches_t&) = default;
        branches_t(branches_t&&) = delete;
        branches_t& operator=(const branches_t&) = default;
        branches_t& operator=(branches_t&&) = delete;

        void SetBranchtes(TTree* tree);
        void Reset();

    };

    const unsigned nPhotons;

    TTree* tree = nullptr;

    branches_t tree_branches;
    utils::KinFitter fitter;

    TH1D* steps = nullptr;

    PromptRandom::Switch promptrandom;
    utils::TriggerSimulation triggersimu;

public:
    FindProton(const std::string& name, OptionsPtr opts);
    virtual ~FindProton();

    virtual void ProcessEvent(const TEvent& event, manager_t&) override;
};
}}}
