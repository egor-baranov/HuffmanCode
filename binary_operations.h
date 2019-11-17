#ifndef HUFFMANCODE_BINARY_OPERATIONS_H
#define HUFFMANCODE_BINARY_OPERATIONS_H

#include <iostream>
#include <cstring>
#include <utility>
#include <vector>
#include <bitset>
#include <fstream>
#include <cmath>

using namespace std;

struct bitSequence {
  unsigned char end = 0;
  vector<bitset<8>> content = vector<bitset<8>>();

  bitSequence() = default;

  bitSequence(bool init, unsigned int count) {
    for (unsigned int i = 0; i < count; ++i)
      pushBack(init);
  }

  explicit bitSequence(char init) {
    pushBack(init);
  }

  explicit bitSequence(string init) {
    for (char c: init) pushBack(c);
  }

  void pushBack(bitset<8> add) {
    for (int i = 0; i < 8; ++i) pushBack(add[i]);
  }

  void pushBack(char add) {
    pushBack((bitset<8>) add);
  }

  void pushBack(bool b) {
    if (end == 0) {
      end = 7;
      content.push_back((bitset<8>) b);
      return;
    }
    content.back()[8 - end] = b;
    --end;
  }

  size_t size() {
    return content.size() * 8 - end;
  }

  string toString() {
    string output;
    for (auto i: content) output += (char) i.to_ulong();
    return output;
  }

  bool index(const int i) {
    if (size() <= i or i < 0)
      throw std::runtime_error("Invalid index.");
    return content[i / 8].test(i % 8);
  }

  bool operator[](const int i) {
    return index(i);
  }

  bitSequence slice(unsigned int s = 0, int f = 0) {
    if (f <= 0) f += size();
    bitSequence ret;
    for (size_t i = s; i < f; ++i) ret.pushBack(index(i));
    return ret;
  }
};

bitSequence operator+(bitSequence b1, bitSequence b2) {
  bitSequence output = std::move(b1);
  int index = 0;
  for (auto b: b2.content)
    for (int i = 0; i < 8; ++i) {
      if (index >= b2.size()) break;
      output.pushBack(b[i]);
      ++index;
    }
  return output;
}

ostream &operator<<(ostream &out, bitSequence bs) {
  for (size_t i = 0; i < bs.size(); ++i) cout << bs[i];
  return out;
}

string format(string s) {
  string ret = "[";
  for (size_t i = 0; i < s.size(); ++i) {
    if (i + 1 == s.size()) ret += to_string((int) s[i]) + "]";
    else ret += to_string((int) s[i]) + ", ";
  }
  return ret;
}

bitSequence readBinary(const string &path) {
  ifstream ifst;
  ifst.open(path);
  string s;
  s.assign((istreambuf_iterator<char>(ifst.rdbuf())), istreambuf_iterator<char>());
  bitSequence output(s);
  int skip = output[0] * 4 + output[1] * 2 + output[2];
  ifst.close();
  return output.slice(3, -skip);
}

void writeBinary(bitSequence input, const string &path) {
  ofstream ofst;
  ofst.open(path);
  bitSequence use;
  auto n = ((input.size() + 3) % 8);
  if (n != 0) n = 8 - n;
  use.pushBack((bool) (n / 4));
  use.pushBack((bool) (n % 4 / 2));
  use.pushBack((bool) (n % 2));
  use = use + input;
  ofst << use.toString();
  ofst.close();
}

#endif //HUFFMANCODE_BINARY_OPERATIONS_H
