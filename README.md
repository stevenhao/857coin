# 857coin

Mines [857coin](http://6857coin.csail.mit.edu:8080/).

## Build
- Build with `./build.sh`
- Run `./test.sh` to append `"block": "[hsteven, eunicel, dzaefn]"` to the genesis block.

Works best for difficulty <= 42

## Algorithm
For a difficulty `D`, set `N = 1 << (D * 2/3 + 2)`.

Compute `N` hashes (taking the last `D` bits), then sort them. Search for a 3-collision among these hashes. 

By construction of `N`, the likelihood of such a collision is high.
