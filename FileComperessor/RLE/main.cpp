#include <iostream>
#include <vector>
#include <sstream>
#include <memory.h>
#include <fstream>
#include "CRLEEncoder.h"
#include "CBWTEncoder.h"


using namespace std;

typedef std::string CharT;


#define $Ensure(P) if (!P) return -1;


template<typename I, typename C>
std::shared_ptr<I> create_iface() {
  std::shared_ptr<I> sp_iface(new C());

  return sp_iface;
}

bool check_files(ifstream& fin, ofstream& fencodeBWT, ofstream& fdecodeBWT, ofstream& fencodeRLE, ofstream& fdecodeRLE) {
  if (!fin.is_open() || !fencodeBWT.is_open() || !fdecodeBWT.is_open() || !fdecodeRLE.is_open() || !fencodeRLE.is_open())
  {
    cout << "Can't open files!" << std::endl;
    return false;
  }

  return true;
}

int main() {
  ifstream ifs_src("input.txt");
  ofstream ofs_encodeBWT("encodeBWT.txt");
  ofstream ofs_decodeBWT("decodeBWT.txt");
  ofstream ofs_encodeRLE("encodeRLE.txt");
  ofstream ofs_decodeRLE("decodeRLE.txt");

  string text;

  $Ensure(check_files(ifs_src, ofs_encodeBWT, ofs_decodeBWT, ofs_encodeRLE, ofs_decodeRLE));

  std::stringstream ss;
  ss << ifs_src.rdbuf();
  text = ss.str();

  auto sp_iface_ptr = create_iface<IEncoderA, CBWTEncoder>();
  auto sp_iface_RLE_ptr = create_iface<IEncoderA, CRLEEncoder>();

  try
  {
    sp_iface_ptr->encode(text);
    ofs_encodeBWT << text;
  }
  catch (runtime_error& e)
  {
    cout << "ERROR: " << e.what() << "\n";
  }

  sp_iface_RLE_ptr->encode(text);
  ofs_encodeRLE << text;

  sp_iface_RLE_ptr->decode(text);
  ofs_decodeRLE << text;

  sp_iface_ptr->decode(text);
  ofs_decodeBWT << text;

  return 0;
}
