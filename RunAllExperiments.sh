#!/./bin/sh
set -e  #exit on first error
make  #execute makefile
mkdir -p ./outputs  #create output folder if not present

# Simple Test
# ./bin/cache_sim ./resources/configs/testconfig ./resources/tracefiles/simpletracefile


./bin/cache_sim ./resources/configs/directWriteBackLRU32Block ./resources/tracefiles/wgetORSTtracefile
./bin/cache_sim ./resources/configs/directWriteBackLRU32Block ./resources/tracefiles/lsTracefile
./bin/cache_sim ./resources/configs/directWriteBackLRU32Block ./resources/tracefiles/catTracefile

./bin/cache_sim ./resources/configs/twoWayWriteBackLRU32Block ./resources/tracefiles/wgetORSTtracefile
./bin/cache_sim ./resources/configs/twoWayWriteBackLRU32Block ./resources/tracefiles/lsTracefile
./bin/cache_sim ./resources/configs/twoWayWriteBackLRU32Block ./resources/tracefiles/catTracefile


./bin/cache_sim ./resources/configs/directWriteThroughRandom32Block ./resources/tracefiles/wgetORSTtracefile
./bin/cache_sim ./resources/configs/directWriteThroughRandom32Block ./resources/tracefiles/lsTracefile
./bin/cache_sim ./resources/configs/directWriteThroughRandom32Block ./resources/tracefiles/catTracefile

./bin/cache_sim ./resources/configs/twoWayWriteThroughRandom32Block ./resources/tracefiles/wgetORSTtracefile
./bin/cache_sim ./resources/configs/twoWayWriteThroughRandom32Block ./resources/tracefiles/lsTracefile
./bin/cache_sim ./resources/configs/twoWayWriteThroughRandom32Block ./resources/tracefiles/catTracefile
