#include "CB.h"
#include <cassert>
#include "base/std_ext.h"

using namespace std;
using namespace ant;
using namespace ant::expconfig::detector;

// this include must be after using namespace std;
#include "detail/CB_elements.h" // defines all_elements

const vector<unsigned> CB::holes = CB::initHoles();

void CB::BuildMappings(vector<UnpackerAcquConfig::hit_mapping_t> &hit_mappings,
                       vector<UnpackerAcquConfig::scaler_mapping_t>&) const {
  // CB has only hit_mappings to add,
  // no scalers
  unsigned true_elements = 0;
  for(const CBElement_t& element : elements)  {
    if(std_ext::contains(holes, element.Channel))
      continue;

    hit_mapping_t m_adc;
    m_adc.LogicalChannel = {Type, Channel_t::Type_t::Integral, element.Channel};
    m_adc.RawChannels.push_back(element.ADC);
    hit_mappings.emplace_back(move(m_adc));

    hit_mapping_t m_tdc;
    m_tdc.LogicalChannel = {Type, Channel_t::Type_t::Timing, element.Channel};
    m_tdc.RawChannels.push_back(element.TDC);
    hit_mappings.emplace_back(move(m_tdc));
    true_elements++;
  }

  assert(true_elements == 672);
}

vector<unsigned> CB::initHoles() {
  // hard-code the hole ranges here
  // we even use insert range for single elements
  // for the sake of code cleanness
  vector<unsigned> holes;
  std_ext::insertRange(holes,  26,  26);
  std_ext::insertRange(holes,  29,  38);
  std_ext::insertRange(holes,  40,  40);
  std_ext::insertRange(holes, 311, 311);
  std_ext::insertRange(holes, 315, 316);
  std_ext::insertRange(holes, 318, 319);
  std_ext::insertRange(holes, 353, 366);
  std_ext::insertRange(holes, 400, 402);
  std_ext::insertRange(holes, 405, 405);
  std_ext::insertRange(holes, 408, 408);
  std_ext::insertRange(holes, 679, 679);
  std_ext::insertRange(holes, 681, 689);
  std_ext::insertRange(holes, 691, 692);
  return holes;
}



