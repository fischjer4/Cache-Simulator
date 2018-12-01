#!/./bin/sh
set -e  #exit on first error
make  #execute makefile
mkdir -p ./outputs  #create output folder if not present
mkdir -p ./outputs/32BlockSize  #create output/32BlockSize folder if not present
mkdir -p ./outputs/64BlockSize  #create output/64BlockSize folder if not present

# Simple Test
# ./bin/cache_sim ./resources/configs/testconfig ./resources/tracefiles/simpletracefile

# ================================ 32B Block Size  Tests ================================
./bin/cache_sim ./resources/configs/32BlockSize/directWriteBackLRU32Block ./resources/tracefiles/wgetORSTtracefile
./bin/cache_sim ./resources/configs/32BlockSize/directWriteBackLRU32Block ./resources/tracefiles/lsTracefile
./bin/cache_sim ./resources/configs/32BlockSize/directWriteBackLRU32Block ./resources/tracefiles/catTracefile

./bin/cache_sim ./resources/configs/32BlockSize/twoWayWriteBackLRU32Block ./resources/tracefiles/wgetORSTtracefile
./bin/cache_sim ./resources/configs/32BlockSize/twoWayWriteBackLRU32Block ./resources/tracefiles/lsTracefile
./bin/cache_sim ./resources/configs/32BlockSize/twoWayWriteBackLRU32Block ./resources/tracefiles/catTracefile


./bin/cache_sim ./resources/configs/32BlockSize/directWriteThroughRandom32Block ./resources/tracefiles/wgetORSTtracefile
./bin/cache_sim ./resources/configs/32BlockSize/directWriteThroughRandom32Block ./resources/tracefiles/lsTracefile
./bin/cache_sim ./resources/configs/32BlockSize/directWriteThroughRandom32Block ./resources/tracefiles/catTracefile

./bin/cache_sim ./resources/configs/32BlockSize/twoWayWriteThroughRandom32Block ./resources/tracefiles/wgetORSTtracefile
./bin/cache_sim ./resources/configs/32BlockSize/twoWayWriteThroughRandom32Block ./resources/tracefiles/lsTracefile
./bin/cache_sim ./resources/configs/32BlockSize/twoWayWriteThroughRandom32Block ./resources/tracefiles/catTracefile



# ================================ 64B Block Size  Tests ================================
./bin/cache_sim ./resources/configs/64BlockSize/directWriteBackLRU64Block ./resources/tracefiles/wgetORSTtracefile
./bin/cache_sim ./resources/configs/64BlockSize/directWriteBackLRU64Block ./resources/tracefiles/lsTracefile
./bin/cache_sim ./resources/configs/64BlockSize/directWriteBackLRU64Block ./resources/tracefiles/catTracefile

./bin/cache_sim ./resources/configs/64BlockSize/twoWayWriteBackLRU64Block ./resources/tracefiles/wgetORSTtracefile
./bin/cache_sim ./resources/configs/64BlockSize/twoWayWriteBackLRU64Block ./resources/tracefiles/lsTracefile
./bin/cache_sim ./resources/configs/64BlockSize/twoWayWriteBackLRU64Block ./resources/tracefiles/catTracefile


./bin/cache_sim ./resources/configs/64BlockSize/directWriteThroughRandom64Block ./resources/tracefiles/wgetORSTtracefile
./bin/cache_sim ./resources/configs/64BlockSize/directWriteThroughRandom64Block ./resources/tracefiles/lsTracefile
./bin/cache_sim ./resources/configs/64BlockSize/directWriteThroughRandom64Block ./resources/tracefiles/catTracefile

./bin/cache_sim ./resources/configs/64BlockSize/twoWayWriteThroughRandom64Block ./resources/tracefiles/wgetORSTtracefile
./bin/cache_sim ./resources/configs/64BlockSize/twoWayWriteThroughRandom64Block ./resources/tracefiles/lsTracefile
./bin/cache_sim ./resources/configs/64BlockSize/twoWayWriteThroughRandom64Block ./resources/tracefiles/catTracefile
