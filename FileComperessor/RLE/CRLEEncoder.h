#pragma once


#include <limits>
#include <sstream>
#include "IEncoder.h"


// Run Length Encoding class
class CRLEEncoder : public IEncoderA {
public:
  //CRLEEncoder() { };

  void encode(std::string& s) override;
  void decode(std::string& s) override;
};