#include <iostream>
#include <bitset>
#include "huffman_tree.h"

using namespace std;

int main() {
//  ifstream ifs;
//  ifs.open("textinput");
//  string source;
//  source.assign((istreambuf_iterator<char>(ifs.rdbuf())), istreambuf_iterator<char>());
  HuffmanTree h("abacaba");
  writeBinary(encodeTree(h), "tree");
  h = decodeTree(readBinary("tree"));
  string s;
  while (cin >> s) {
    cout << h.encode(s) << endl;
    cout << h.decode(h.encode(s)) << endl;
  }
  return 0;
}