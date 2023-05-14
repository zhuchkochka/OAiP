#include "CRLEEncoder.h"


void CRLEEncoder::encode(std::string& s) {
  std::string res = "";
  char buf = s[0];
  std::uint32_t count = 0;

  for (int i = 0; i < s.length() + 1; i++) {
    if (buf == s[i]) {
      count++;
    }
    else {
      res += std::to_string(count);
      res += buf;
      count = 1;
      buf = s[i];
    }

    if (count == 9) {
      res += '9';
      res += buf;
      count = 0;
    }
  }

  s = res;
}


void CRLEEncoder::decode(std::string& s) {
  std::string res = "";
  for (int i = 0; i < s.length() + 1; i += 2) {
    for (int j = 0; j < s[i] - '0'; j++) {
      res += s[i + 1];
    }
  }

  s = res;
}