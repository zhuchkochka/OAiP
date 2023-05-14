#pragma once


#include <iostream>
#include <fstream>


class CFileOperationsWrapper {
public:
  CFileOperationsWrapper(std::ofstream& huff_file, std::ofstream& freq_file) :
    m_ffreq(freq_file),
    m_fHuff(huff_file)
  { }

  ~CFileOperationsWrapper() {
    m_ffreq.close();
    m_fHuff.close();
  }

  template<class MapT>
  void mapToFile(MapT map, bool isHuff) const {
    if (isValid()) {
      if (isHuff) {
        for (auto it = map.begin(); it != map.end(); it++)
          m_fHuff << it->first << "\t" << it->second << std::endl;
      }
      else
        for (auto it = map.begin(); it != map.end(); it++)
          m_ffreq << it->first << "\t" << it->second << std::endl;
    }
  }

  inline bool isValid() const {
    if (!m_ffreq.is_open() || !m_fHuff.is_open()) {
      std::cout << "Не удалось открыть файл\n";
      return false;
    }

    return true;
  }

private:
  std::ofstream& m_ffreq;
  std::ofstream& m_fHuff;
};
