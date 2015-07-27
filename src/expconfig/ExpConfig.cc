#include "ExpConfig.h"

//#include "setups/Setup_2014_EtaPrime.h"

#include <Setup.h>

#include "tree/THeaderInfo.h"
#include "unpacker/UnpackerAcqu.h"

#include "base/Logger.h"

#include <type_traits>
#include <list>
#include <iostream>

using namespace std;

namespace ant { // templates need explicit namespace

template<typename T>
shared_ptr<T> ExpConfig::Get_(const THeaderInfo& header) {
    VLOG(9) << "Searching for config of type "
            << std_ext::getTypeAsString<T>();

    static_assert(is_base_of<Base, T>::value, "T must be a base of ExpConfig::Base");

    // make a copy of the list of registered configs
    auto& registry = expconfig::SetupRegistry::get();
    std::list< std::shared_ptr<Base> > modules(registry.begin(), registry.end());

    // remove the config if the config says it does not match
    modules.remove_if([&header] (const shared_ptr<Base>& m) {
        return !m->Matches(header);
    });

    // check if something reasonable is left
    if(modules.empty()) {
        throw ExpConfig::Exception(std_ext::formatter()
                                   << "No config found for header "
                                   << header);
    }
    if(modules.size()>1) {
        throw ExpConfig::Exception(std_ext::formatter()
                                   << "More than one config found for header "
                                   << header);
    }

    // only one instance found, now try to cast it to the
    // requested type unique_ptr<T>
    // this only works if the found module is a derived class of the requested type

    const auto& ptr = dynamic_pointer_cast<T, Base>(modules.back());


    if(ptr==nullptr) {
        throw ExpConfig::Exception("Matched config does not fit to requested type");
    }

    // hand over the unique ptr
    return ptr;
}


shared_ptr<ExpConfig::Module> ExpConfig::Module::Get(const THeaderInfo& header)
{
    return Get_<ExpConfig::Module>(header);
}

list< shared_ptr<ExpConfig::Module> > ExpConfig::Module::GetAll() {
    // make a copy of the list of registered configs
    auto& registry = expconfig::SetupRegistry::get();
    std::list< std::shared_ptr<ExpConfig::Module> > modules(registry.begin(), registry.end());
    return modules;
}


shared_ptr<ExpConfig::Module> ExpConfig::Module::Get(const std::string& name)
{
    for(const auto& module : GetAll()) {
        if(module->GetName() == name)
            return module;
    }
    // nothing found
    return nullptr;
}



shared_ptr<ExpConfig::Reconstruct> ExpConfig::Reconstruct::Get(const THeaderInfo& header)
{
    return Get_<ExpConfig::Reconstruct>(header);
}

template<>
shared_ptr< UnpackerAcquConfig >
ExpConfig::Unpacker<UnpackerAcquConfig>::Get(const THeaderInfo& header) {
    return Get_< UnpackerAcquConfig >(header);
}

} // namespace ant




