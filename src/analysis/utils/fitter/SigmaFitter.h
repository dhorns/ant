#pragma once

#include "Fitter.h"

namespace ant {
namespace analysis {
namespace utils {

class SigmaFitter : public Fitter
{
public:

    /**
     * @brief SigmaFitter applies energy-momentum constraint to proton/photons using incoming beam
     * @param uncertainty_model model to obtain uncertainties
     * @param settings tune the underlying APLCON fitter
     */
    explicit SigmaFitter(UncertaintyModelPtr uncertainty_model = nullptr,
                       bool fit_Z_vertex = false,
                       const APLCON::Fit_Settings_t& settings = DefaultSettings
             );

    void SetZVertexSigma(double sigma);
    bool IsZVertexFitEnabled() const noexcept;
    bool IsZVertexUnmeasured() const;
    void SetTarget(double length, double center = 0.);

    TParticlePtr  GetFittedProton() const;
    TParticleList GetFittedPhotons() const;
    double GetFittedBeamE() const;
    TParticlePtr GetFittedBeamParticle() const;
    double GetFittedZVertex() const;

    double GetBeamEPull() const;
    double GetZVertexPull() const;

    std::vector<FitParticle> GetFitParticles() const;

    APLCON::Result_t DoFit(double ebeam, const TParticlePtr& proton, const TParticleList& photons, const size_t photon1, const size_t photon2);


    void SetUncertaintyModel(const UncertaintyModelPtr& uncertainty_model) {
        Model = uncertainty_model;
    }

protected:

    void PrepareFit(double ebeam,
                    const TParticlePtr& proton,
                    const TParticleList& photons);


    struct BeamE_t : V_S_P_t {
        double Value_before = std_ext::NaN;
        LorentzVec GetLorentzVec() const noexcept;
        void SetValueSigma(double value, double sigma) {
            V_S_P_t::SetValueSigma(value, sigma);
            Value_before = Value;
        }
    };

    struct Target_t {
        double length = std_ext::NaN;
        double center = std_ext::NaN;

        double start() const { return center - length/2.; }
        double end() const { return center + length/2.; }
    };

    using Proton_t = FitParticle;
    using Photons_t = std::vector<FitParticle>;

    BeamE_t    BeamE;
    Proton_t   Proton;
    Photons_t  Photons;
    Z_Vertex_t Z_Vertex;
    Target_t   Target;

    APLCON::Fitter<BeamE_t, Proton_t, Photons_t, Z_Vertex_t> aplcon;

    // make constraint a static function, then we can use the typedefs
    static std::array<double, 5> constraint(const BeamE_t& beam, const Proton_t& proton,
                                            const Photons_t& photons, const Z_Vertex_t& z_vertex,
                                            const size_t photon1, const size_t photon2);

    double CalcZVertexStartingPoint() const;


private:
    UncertaintyModelPtr Model;
};

}}} // namespace ant::analysis::utils
