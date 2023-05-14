#pragma once

#include <string>


class IEncoderA {
public:
  virtual void encode(std::string& s) = 0;
  virtual void decode(std::string& s) = 0;
};