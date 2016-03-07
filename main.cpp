#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cassert>
#include <cstdio>
#include "sha256.h"
#include <openssl/sha.h>

typedef long long ll;
#define dbg(x) cerr << #x << ":" << pretty(x) << "\n";
#define p(x) cerr << #x << ":" << (x) << "\n"
using namespace std;

// Hash(B, i) = SHA256(HexDecode(B.parentid) + HexDecode(B.root) + Bytes(B.difficulty) + Bytes(B.timestamp) + Bytes(B.nonces[i]) + B.version)

#define mp make_pair
#define hash hashh
ll _sha256(char *s, int len)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, s, len);
    SHA256_Final(hash, &sha256);
    ll ret = 0;
    for(int i = 0; i < 8; ++i) {
      ret |= (unsigned long long)(hash[SHA256_DIGEST_LENGTH - 1 - i]) << (i * 8);
    }
    return ret;
}


#define hex hexx
string hex = "0123456789abcdef";
char buff[100];

string pretty(string s) { // for logging a hex-encoded string
  string ret;
  for(int i = 0; i < s.size(); ++i) {
    int c = s[i];
    ret += hex[(c >> 4) & 15];
    ret += hex[c & 15];
    ret += " ";
  }
  return ret;
}

string pretty2(char * s, int l) { // for logging a hex encoded cstring
  string ret;
  for(int i = 0; i < l; ++i) {
    int c = s[i];
    ret += hex[(c >> 4) & 15];
    ret += hex[c & 15];
    ret += " ";
  }
  return ret;
}


string parid;
string root;
string difficulty;
string timestamp;
string nonce;
string version;

void init() {
  p(pretty(parid));
  p(pretty(root));
  string packed = parid + root + difficulty + timestamp + nonce + version;
  p(pretty(packed));
  for(int i = 0; i < 89; ++i) {
    buff[i] = packed[i];
  }
}

ll hash(int i) {
  char *pt = buff + 84;
  pt[0] = i >> 24;
  pt[1] = (i >> 16) & 255;
  pt[2] = (i >> 8) & 255;
  pt[3] = i & 255;
  ll ret = _sha256(buff, 89);
  return ret;
}

char MP[200][200];
void prec() {
  for(int i = 0; i < 16; ++i) {
    for(int j = 0; j < 16; ++j) {
      MP[hex[i]][hex[j]] = (i << 4) + j;
    }
  }
}

string decode(string s) {
  string ret;
  assert(s.size() % 2 == 0);
  for(int i = 0; i < s.size(); i += 2) {
    char a = s[i], b = s[i + 1];
    char nxt = MP[a][b];
    ret += nxt;
  }
  return ret;
}

string bytestr(ll l) {
  string ret;
  int off = 56;
  for(int i = 0; i < 8; ++i) {
    ret += char((l >> off) & 255);
    off -= 8;
  }
  return ret;
}


ll gettime(int addseconds) {
  time_t timer;
  struct tm y2k = {0};
  double seconds;

  y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
  y2k.tm_year = 0; y2k.tm_mon = 0; y2k.tm_mday = 1;

  time(&timer);  /* get current time; same as: timer = time(NULL)  */

  seconds = difftime(timer,mktime(&y2k));
  return (addseconds + seconds) * 1000000000L;
}

const int MAXN = 1 << 30;
pair<ll, int> hashes[MAXN];

int main()
{
  string _parid;
  int len;
  ll time;
  cin >> _parid >> len >> time;
  p(parid);
  prec();
  parid = decode(_parid);
//  parid = decode("b72a040d0d784dcc2646906bcdaae7d799b6025166a6df7a833732ab99d159d4");
  root = decode("398d08e407af9956d22fdf12512ba175731d2e3892714653a01fd379ecc22b8d");
  difficulty = bytestr(len);
  timestamp = bytestr(time);
  version = string();
  version.resize(1); // == "\0"
  nonce = bytestr(0);
  init();

  int bits = len * 2 / 3 + 2;
  if (bits > 30) bits = 30;
  int N = 1 << (bits); // number of hashes to compute
  long long msk = (1LL << len) - 1;
  p(time);
  cerr << "hashing " << N << "...\n";

  p(len);
  for(int i = 0; i < N; ++i) {
    hashes[i] = mp(hash(i) & msk, i);
  }
  cerr << "sorting...\n";
  sort(hashes, hashes + N);

  int total = 0;
  string ans = "";
  for(int i = 0; i + 2 < N; ++i) {
    if (hashes[i].first == hashes[i + 2].first) {
      cout << hashes[i].second << ",";
      cout << hashes[i+1].second << ",";
      cout << hashes[i+2].second << "\n";
      cerr << "found!\n";
      return 0;
    }
  }

  cerr << "not found :(\n";
//  p(total);
//  p(time);

  return 0;
}

