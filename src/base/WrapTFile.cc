#include "WrapTFile.h"
#include "TFile.h"
#include "std_ext.h"
#include "Logger.h"

#include <stdexcept>

using namespace std;
using namespace ant;

WrapTFile::WrapTFile(const string &filename)
{
    file = std_ext::make_unique<TFile>(filename.c_str(), "RECREATE");
    if(!IsOpen())
        throw std::runtime_error(string("Could not open TFile for writing at ")+filename);
    VLOG(5) << "Opened output file " << filename;
}

bool WrapTFile::IsOpen() const
{
    return file->IsOpen();
}

void WrapTFile::cd()
{
    if(!IsOpen())
        return;
    file->cd();
}

WrapTFile::~WrapTFile()
{
    if(!IsOpen())
        return;

    VLOG(5) << "Syncing output file " << file->GetName();
    file->Write();
    file->Close();
    LOG(INFO) << "Closed output file " << file->GetName();

}
