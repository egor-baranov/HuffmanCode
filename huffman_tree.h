#ifndef HUFFMANCODE_HUFFMAN_TREE_H
#define HUFFMANCODE_HUFFMAN_TREE_H

#include <iostream>
#include <cstring>
#include <map>
#include <set>
#include <cassert>
#include <utility>
#include <vector>
#include "binary_operations.h"

using namespace std;

bitSequence tempGlobal;
int indexGlobal = 0;

struct HNode {
  HNode *left, *right;
  string content;

  explicit HNode(string init) {
    content = std::move(init);
    left = right = nullptr;
  }

  bool isLeaf() {
    return left == nullptr and right == nullptr;
  }
};

bool operator>(const HNode &a, const HNode &b) {
  return a.content > b.content;
}

bool contain(const string &s, char c) {
  for (char i: s) if (i == c) return true;
  return false;
}

struct HuffmanTree {
  private:
  HNode *head;
  public:
  HNode *getHead() { return head; }

  explicit HuffmanTree(HNode *init) {
    assert(init);
    head = init;
  }

  explicit HuffmanTree(const string &init) {
    map<char, int> m;
    for (char c: init) ++m[c];
    set<pair<int, HNode *>> s;
    for (pair<char, int> p: m) {
      auto *newNode = new HNode(string(1, p.first));
      s.insert({p.second, newNode});
    }
    while (s.size() > 1) {
      pair<int, HNode *> a = *s.begin();
      s.erase(s.begin());
      pair<int, HNode *> b = *s.begin();
      s.erase(s.begin());
      if (a.second->content > b.second->content) swap(a.second, b.second);
      auto *newNode = new HNode(a.second->content + b.second->content);
      newNode->left = a.second;
      newNode->right = b.second;
      s.insert({a.first + b.first, newNode});
    }
    head = s.begin()->second;
  }

  bitSequence encode(const string &input) {
    bitSequence output;
    auto curNode = head;
    for (char c: input) {
      if (curNode->isLeaf()) output.pushBack(false);
      while (!curNode->isLeaf()) {
        if (contain(curNode->left->content, c)) {
          output.pushBack(false);
          curNode = curNode->left;
        } else {
          output.pushBack(true);
          curNode = curNode->right;
        }
      }
      curNode = head;
    }
    return output;
  }

  string decode(bitSequence input) {
    string output;
    auto curNode = head;
    for (size_t i = 0; i < input.size(); ++i) {
      if (!curNode->isLeaf())
        curNode = (input[i] ? curNode->right : curNode->left);
      if (curNode->isLeaf()) {
        output += curNode->content;
        curNode = head;
      }
    }
    return output;
  }
};

bitSequence dfsEncode(HNode *v) {
  bitSequence ret;
  if (v->isLeaf()) {
    ret.pushBack(true);
    ret.pushBack(v->content[0]);
    return ret;
  }
  ret.pushBack(false);
  return ret + dfsEncode(v->right) + dfsEncode(v->left);
}

bitSequence encodeTree(HuffmanTree hT) {
  return dfsEncode(hT.getHead());
}

HNode *dfsDecode(HNode *v) {
  if (indexGlobal >= tempGlobal.size()) return nullptr;
  if (tempGlobal[indexGlobal] == 1) {
    ++indexGlobal;
    v->content = tempGlobal.slice(indexGlobal, indexGlobal + 8 - 1).toString();
    indexGlobal += 8;
  } else {
    auto *left = new HNode(""), *right = new HNode("");
    v->left = left;
    v->right = right;
    ++indexGlobal;
    v->content = dfsDecode(left)->content + dfsDecode(right)->content;
  }
  return v;
}

HuffmanTree decodeTree(bitSequence input) {
  tempGlobal = std::move(input);
  indexGlobal = 0;
  auto *head = new HNode("");
  dfsDecode(head);
  return HuffmanTree(head);
}

#endif //HUFFMANCODE_HUFFMAN_TREE_H
