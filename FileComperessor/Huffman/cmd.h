#pragma once


#include <string>
#include <memory>
#include <map>
#include <queue>
#include <fstream>
#include <iostream>
#include "ICommand.h"


// Invoker for commands
class CInvoker {
public:
  void invoke(ICommand* cmd) {
    cmd->exec();
  }
};


struct SNode {
  SNode(char ch, std::size_t freq, std::shared_ptr<SNode> left, std::shared_ptr<SNode> right) :
    m_ch(ch),
    m_freq(freq),
    m_left(left),
    m_right(right)
  {}

  // members
  char m_ch;
  std::size_t m_freq;
  std::shared_ptr<SNode> m_left;
  std::shared_ptr<SNode> m_right;
};


// Receiver class
class CEncoder {
public:
  // operations
  void encode(std::shared_ptr<SNode> spRoot, std::string str, std::map<char, std::string>& huffmanCode) const;
};

// Receiver class
class CDecoder {
public:
  // operations
  void decode(std::shared_ptr<SNode> spRoot, int& index, std::string str) const;
};

// Command encode
class CCmdEncode : public ICommand {
public:
  CCmdEncode(std::shared_ptr<CEncoder> spEncoder, std::shared_ptr<SNode> spRoot,
             std::string sSrc, std::map<char, std::string>& mapHuff) :
              m_spEncoder(spEncoder),
              m_spRoot(spRoot),
              m_sSrc(sSrc),
              m_mapHuffmanCode(mapHuff)
              { }

  // ICommand
  void exec() override {
    m_spEncoder->encode(m_spRoot, m_sSrc, m_mapHuffmanCode);
  }

private:
  std::shared_ptr<CEncoder>    m_spEncoder;
  std::shared_ptr<SNode>       m_spRoot;
  std::string                  m_sSrc;
  std::map<char, std::string>& m_mapHuffmanCode;
};

// Command decode
class CCmdDecode : public ICommand {
public:
  CCmdDecode(std::shared_ptr<CDecoder> spDecoder, std::shared_ptr<SNode> spRoot, int& idx,
             std::string str) : 
             m_spDecoder(spDecoder),
             m_spRoot(spRoot),
             m_idx(idx),
             m_sCode(str)
             { }

  // ICommand
  void exec() override {
    m_spDecoder->decode(m_spRoot, m_idx, m_sCode);
  }

private:
  std::shared_ptr<CDecoder> m_spDecoder;
  std::shared_ptr<SNode>    m_spRoot;
  int&                      m_idx;
  std::string               m_sCode;
};
