#include "cmd.h"


void CEncoder::encode(std::shared_ptr<SNode> spRoot, std::string str, std::map<char, std::string>& huffmanCode) const {
  if (spRoot == nullptr)
    return;

  if (!spRoot->m_left && !spRoot->m_right) {
    huffmanCode[spRoot->m_ch] = str;
  }

  encode(spRoot->m_left, str + "0", huffmanCode);
  encode(spRoot->m_right, str + "1", huffmanCode);
}

void CDecoder::decode(std::shared_ptr<SNode> spRoot, int& idx, std::string str) const {
  std::ofstream decodeFile;
  decodeFile.open("Huffman/decode.txt", std::ios::app);
  if (spRoot == nullptr) {
    return;
  }
  if (!spRoot->m_left && !spRoot->m_right)
  {
    decodeFile << spRoot->m_ch;
    return;
  }
  idx++;
  if (str[idx] == '0')
    decode(spRoot->m_left, idx, str);
  else
    decode(spRoot->m_right, idx, str);
  decodeFile.close();
}
