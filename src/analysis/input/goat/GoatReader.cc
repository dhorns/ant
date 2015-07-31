#include "GoatReader.h"

#include <string>
#include <iostream>
#include <memory>

#include "TTree.h"

#include "detail/TreeManager.h"

#include "base/ReadTFiles.h"
#include "base/Logger.h"
#include "base/std_ext.h"

using namespace ant;
using namespace ant::input;
using namespace std;

/**
 * @brief map goat apparatus numbers to apparatus_t enum values
 * in case unknown values show up: -> exception and do not sliently ignore
 */
detector_t IntToDetector_t( const int& a ) {
    detector_t d = detector_t::None;
    if(a & TrackInput::DETECTOR_NaI) {
        d |= detector_t::CB;
    }
    if(a & TrackInput::DETECTOR_PID) {
        d |= detector_t::PID;
    }
    if(a & TrackInput::DETECTOR_MWPC) {
        d |= detector_t::MWPC;
    }
    if(a & TrackInput::DETECTOR_BaF2) {
        d |= detector_t::TAPS;
    }
    if(a & TrackInput::DETECTOR_PbWO4) {
        d |= detector_t::TAPS;
    }
    if(a & TrackInput::DETECTOR_Veto) {
        d |= detector_t::TAPSVeto;
    }
    return d;
}

void GoatReader::CopyTagger(std::shared_ptr<Event> &event)
{
    for( Int_t i=0; i<tagger.GetNTagged(); ++i) {
        event->Reconstructed().TaggerHits().emplace_back(
                    TaggerHitPtr(new TaggerHit(
                                     tagger.GetTaggedChannel(i),
                                     tagger.GetTaggedEnergy(i),
                                     tagger.GetTaggedTime(i))
                                 ));
    }
}

void GoatReader::CopyTrigger(std::shared_ptr<Event> &event)
{
    TriggerInfo& ti = event->Reconstructed().TriggerInfos();

    ti.CBEenergySum() = trigger.GetEnergySum();
    ti.Multiplicity() = trigger.GetMultiplicity();

    for( int err=0; err < trigger.GetNErrors(); ++err) {
        ti.Errors().emplace_back(
                    DAQError(
                        trigger.GetErrorModuleID()[err],
                        trigger.GetErrorModuleIndex()[err],
                        trigger.GetErrorCode()[err]));
    }
}

void GoatReader::CopyDetectorHits(std::shared_ptr<Event>&)
{

}

/**
 * @brief map the cluster sizes from goat to unisgend ints
 * negative values mean no hit in the calorimeter
 * map those to 0
 */
clustersize_t GoatReader::MapClusterSize(const int& size) {
    return size < 0 ? 0 : size;
}

void GoatReader::CopyTracks(std::shared_ptr<Event> &event)
{
    for(Int_t i=0; i< tracks.GetNTracks(); ++i) {

        event->Reconstructed().Candidates().emplace_back(
                    CandidatePtr( new Candidate(
                                  tracks.GetClusterEnergy(i),
                                  tracks.GetTheta(i),
                                  tracks.GetPhi(i),
                                  tracks.GetTime(i),
                                  MapClusterSize(tracks.GetClusterSize(i)),
                                  IntToDetector_t(tracks.GetDetectors(i)),
                                  tracks.GetVetoEnergy(i),
                                  tracks.GetMWPC0Energy(i)+tracks.GetMWPC1Energy(i)
                                  )));
    }
}


void GoatReader::CopyParticles(std::shared_ptr<Event> &event, ParticleInput &input_module, const ParticleTypeDatabase::Type &type)
{
    for(Int_t i=0; i < input_module.GetNParticles(); ++i) {

        const auto trackIndex = input_module.GeTCandidateIndex(i);
        if(trackIndex == -1) {
            cerr << "No Track for this particle!!" << endl;
        } else {
            const auto& track = event->Reconstructed().Candidates().at(trackIndex);

            event->Reconstructed().Particles().AddParticle(
                    std::make_shared<Particle>(type,track));
        }

    }
}

class MyTreeRequestMgr: public TreeRequestManager {
protected:
    ReadTFiles& m;
    TreeManager& t;

public:
    MyTreeRequestMgr(ReadTFiles& _m, TreeManager& _t):
        m(_m), t(_t) {}

    TTree *GetTree(const std::string &name) {
        TTree* tree = nullptr;
        if( m.GetObject(name, tree) ) {
            t.AddTree(tree);
            VLOG(6) << "TTree " << name << " opened";
            return tree;
        } else
            VLOG(7) << "Could not find TTree " << name << " in any of the provided files";
            return nullptr;
    }

};

GoatReader::GoatReader(const std::shared_ptr<ReadTFiles>& rootfiles):
    files(rootfiles),
    trees(std_ext::make_unique<TreeManager>())
{
    /// \todo find a smart way to manage trees and modules:
    //   if module does not init or gets removed-> remove also the tree from the list
    //   two modules use same tree?
    //   reset branch addresses ?

    for(auto module = active_modules.begin(); module != active_modules.end(); ) {

        if( (*module)->SetupBranches( MyTreeRequestMgr(*files, *trees))) {
            module++;
        } else {
            module = active_modules.erase(module);
            VLOG(7) << "Not activating GoAT Input module";
        }
    }

    max_entry = std::min(GetNEvents(), max_entry);
}

GoatReader::~GoatReader() {}

Long64_t GoatReader::GetNEvents() const
{
    return trees->GetEntries();
}

bool GoatReader::hasData() const {
    return current_entry+1 < max_entry;
}

long long GoatReader::EventsRead() const
{
    return current_entry;
}

long long GoatReader::TotalEvents() const
{
    return max_entry;
}

void GoatReader::SetMaxEntries(const long long max)
{
    max_entry = std::min(max, GetNEvents());
}

std::shared_ptr<Event> GoatReader::ReadNextEvent()
{
    ++current_entry;
    trees->GetEntry(current_entry);

    active_modules.GetEntry();

    std::shared_ptr<Event> event = make_shared<Event>();

    CopyTrigger(event);
    CopyTagger(event);
    CopyTracks(event);
    CopyDetectorHits(event);

    CopyParticles(event, photons, ParticleTypeDatabase::Photon);
    CopyParticles(event, protons, ParticleTypeDatabase::Proton);
    CopyParticles(event, pichagred, ParticleTypeDatabase::eCharged);
    CopyParticles(event, echarged, ParticleTypeDatabase::PiCharged);
    CopyParticles(event, neutrons, ParticleTypeDatabase::Neutron);


    return event;
}
