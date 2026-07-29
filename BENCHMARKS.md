# Benchmarks

Outputs of running the benchmark on my machine.

Compiler: gcc 16.1.1 20260725
Target:   x86_64-pc-linux-gnu
Flags:    -O3

Run on (16 X 2496.01 MHz CPU s)
CPU Caches:
L1 Data 48 KiB (x8)
L1 Instruction 32 KiB (x8)
L2 Unified 1280 KiB (x8)
L3 Unified 18432 KiB (x1)

## std::vector vs cstl_vec (both preallocated as to not test internal allocators)
```
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
BM_std/8             338 ns          342 ns      2041583 items_per_second=23.395M/s
BM_std/16            343 ns          345 ns      1967723 items_per_second=46.3158M/s
BM_std/64            352 ns          355 ns      1950591 items_per_second=180.034M/s
BM_std/256           402 ns          405 ns      1742784 items_per_second=632.285M/s
BM_std/1024          568 ns          570 ns      1235698 items_per_second=1.7978G/s
BM_std/4096         1231 ns         1232 ns       567501 items_per_second=3.3253G/s
BM_std/16384        3928 ns         3946 ns       178349 items_per_second=4.15185G/s
BM_std/32768        7574 ns         7597 ns        92265 items_per_second=4.31313G/s
BM_std_BigO         0.23 N          0.23 N
BM_std_RMS            16 %            16 %
BM_cstl/8            335 ns          338 ns      2065829 items_per_second=23.6419M/s
BM_cstl/16           337 ns          339 ns      2020883 items_per_second=47.1384M/s
BM_cstl/64           349 ns          352 ns      1993378 items_per_second=181.914M/s
BM_cstl/256          398 ns          401 ns      1751574 items_per_second=638.484M/s
BM_cstl/1024         568 ns          570 ns      1242589 items_per_second=1.79695G/s
BM_cstl/4096        1242 ns         1244 ns       556337 items_per_second=3.29305G/s
BM_cstl/16384       4054 ns         4072 ns       173263 items_per_second=4.0238G/s
BM_cstl/32768       7642 ns         7661 ns        93227 items_per_second=4.277G/s
BM_cstl_BigO        0.24 N          0.24 N
BM_cstl_RMS           16 %            16 %
```

## std::queue vs cstl_queue (no prealloc) vs cstl_queue (prealloc)
```
------------------------------------------------------------------------------------
Benchmark                          Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------
BM_std_queue/8                   184 ns          186 ns      3743024 items_per_second=43.054M/s
BM_std_queue/16                  187 ns          189 ns      3696959 items_per_second=84.7879M/s
BM_std_queue/64                  198 ns          200 ns      3482402 items_per_second=319.739M/s
BM_std_queue/256                 276 ns          278 ns      2527952 items_per_second=922.443M/s
BM_std_queue/1024                540 ns          541 ns      1319870 items_per_second=1.89188G/s
BM_std_queue/4096               1849 ns         1851 ns       368241 items_per_second=2.21263G/s
BM_std_queue/16384              7017 ns         7033 ns       100403 items_per_second=2.32951G/s
BM_std_queue/32768             14019 ns        14040 ns        51058 items_per_second=2.33397G/s
BM_std_queue_BigO               0.43 N          0.43 N
BM_std_queue_RMS                   4 %             4 %
BM_cstl_no_prealloc/8            177 ns          178 ns      3914390 items_per_second=44.8204M/s
BM_cstl_no_prealloc/16           193 ns          195 ns      3602215 items_per_second=82.0686M/s
BM_cstl_no_prealloc/64           239 ns          241 ns      2983333 items_per_second=266.034M/s
BM_cstl_no_prealloc/256          361 ns          363 ns      1952167 items_per_second=705.676M/s
BM_cstl_no_prealloc/1024         821 ns          818 ns       862317 items_per_second=1.25145G/s
BM_cstl_no_prealloc/4096        2453 ns         2455 ns       283425 items_per_second=1.66855G/s
BM_cstl_no_prealloc/16384       9651 ns         9667 ns        71050 items_per_second=1.69477G/s
BM_cstl_no_prealloc/32768      40793 ns        40501 ns        17325 items_per_second=809.069M/s
BM_cstl_no_prealloc_BigO        0.00 N^2        0.00 N^2
BM_cstl_no_prealloc_RMS           11 %            11 %
BM_cstl_prealloc/8               181 ns          181 ns      3592529 items_per_second=44.2016M/s
BM_cstl_prealloc/16              186 ns          187 ns      3810434 items_per_second=85.5586M/s
BM_cstl_prealloc/64              211 ns          212 ns      3336832 items_per_second=301.717M/s
BM_cstl_prealloc/256             290 ns          291 ns      2418883 items_per_second=878.318M/s
BM_cstl_prealloc/1024            554 ns          555 ns      1277527 items_per_second=1.84474G/s
BM_cstl_prealloc/4096           1623 ns         1625 ns       441737 items_per_second=2.52033G/s
BM_cstl_prealloc/16384          6020 ns         6037 ns       119145 items_per_second=2.71395G/s
BM_cstl_prealloc/32768         11813 ns        11837 ns        58734 items_per_second=2.76829G/s
BM_cstl_prealloc_BigO           0.36 N          0.36 N
BM_cstl_prealloc_RMS               6 %             6 %
```

## std::unordered_map vs std::map vs cstl_hashmap
```
BM_std_unordered_map/8            241 ns          243 ns      2916245 items_per_second=32.9464M/s
BM_std_unordered_map/16           352 ns          353 ns      1975535 items_per_second=45.2779M/s
BM_std_unordered_map/64          1316 ns         1317 ns       515531 items_per_second=48.6M/s
BM_std_unordered_map/256         4410 ns         4414 ns       158727 items_per_second=58.0036M/s
BM_std_unordered_map/1024       15925 ns        15938 ns        42207 items_per_second=64.2509M/s
BM_std_unordered_map/4096       61839 ns        61849 ns        11255 items_per_second=66.2256M/s
BM_std_unordered_map/16384     249906 ns       249927 ns         2824 items_per_second=65.5552M/s
BM_std_unordered_map/32768     503188 ns       503260 ns         1294 items_per_second=65.1115M/s
BM_std_unordered_map_BigO       15.33 N         15.33 N
BM_std_unordered_map_RMS            1 %             1 %
BM_std_map/8                      255 ns          256 ns      2749733 items_per_second=31.1982M/s
BM_std_map/16                     387 ns          388 ns      1845476 items_per_second=41.2864M/s
BM_std_map/64                    1486 ns         1489 ns       470112 items_per_second=42.9898M/s
BM_std_map/256                   6888 ns         6891 ns       102651 items_per_second=37.1509M/s
BM_std_map/1024                 29415 ns        29420 ns        23702 items_per_second=34.8058M/s
BM_std_map/4096                136271 ns       136292 ns         5114 items_per_second=30.0531M/s
BM_std_map/16384               633111 ns       633194 ns         1104 items_per_second=25.8752M/s
BM_std_map/32768              1553803 ns      1553884 ns          448 items_per_second=21.0878M/s
BM_std_map_BigO                  3.09 NlgN       3.09 NlgN
BM_std_map_RMS                     10 %            10 %
BM_cstl/8                         236 ns          238 ns      2991484 items_per_second=33.6696M/s
BM_cstl/16                        274 ns          276 ns      2590047 items_per_second=58.0361M/s
BM_cstl/64                        442 ns          445 ns      1562746 items_per_second=143.871M/s
BM_cstl/256                      1002 ns         1005 ns       707600 items_per_second=254.815M/s
BM_cstl/1024                     2981 ns         2984 ns       240942 items_per_second=343.208M/s
BM_cstl/4096                    11786 ns        11815 ns        60421 items_per_second=346.688M/s
BM_cstl/16384                   43757 ns        43789 ns        15727 items_per_second=374.156M/s
BM_cstl/32768                   88627 ns        88692 ns         7960 items_per_second=369.457M/s
BM_cstl_BigO                     2.70 N          2.70 N
BM_cstl_RMS                         2 %             2 %
```

## std::unordered_set vs std::set vs cstl_set
```
-------------------------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------------------
BM_std_unordered_set/8            241 ns          242 ns      2936683 items_per_second=33.0796M/s
BM_std_unordered_set/16           339 ns          340 ns      2028653 items_per_second=47.1086M/s
BM_std_unordered_set/64          1314 ns         1317 ns       522510 items_per_second=48.6002M/s
BM_std_unordered_set/256         4305 ns         4307 ns       159594 items_per_second=59.4402M/s
BM_std_unordered_set/1024       16094 ns        16100 ns        43532 items_per_second=63.6034M/s
BM_std_unordered_set/4096       66858 ns        66865 ns        10119 items_per_second=61.2575M/s
BM_std_unordered_set/16384     253197 ns       253213 ns         2746 items_per_second=64.7045M/s
BM_std_unordered_set/32768     509893 ns       509913 ns         1290 items_per_second=64.262M/s
BM_std_unordered_set_BigO       15.55 N         15.55 N
BM_std_unordered_set_RMS            1 %             1 %
BM_std_set/8                      251 ns          253 ns      2783831 items_per_second=31.657M/s
BM_std_set/16                     365 ns          366 ns      1957648 items_per_second=43.7016M/s
BM_std_set/64                    1483 ns         1484 ns       477689 items_per_second=43.1257M/s
BM_std_set/256                   6729 ns         6728 ns       108432 items_per_second=38.0528M/s
BM_std_set/1024                 28304 ns        28312 ns        24618 items_per_second=36.1683M/s
BM_std_set/4096                133904 ns       133915 ns         5144 items_per_second=30.5866M/s
BM_std_set/16384               612244 ns       612310 ns         1172 items_per_second=26.7577M/s
BM_std_set/32768              1374883 ns      1374941 ns          528 items_per_second=23.8323M/s
BM_std_set_BigO                  2.77 NlgN       2.77 NlgN
BM_std_set_RMS                      4 %             4 %
BM_cstl/8                         224 ns          225 ns      3073494 items_per_second=35.4805M/s
BM_cstl/16                        262 ns          264 ns      2755508 items_per_second=60.6843M/s
BM_cstl/64                        411 ns          414 ns      1697093 items_per_second=154.692M/s
BM_cstl/256                       941 ns          943 ns       757050 items_per_second=271.539M/s
BM_cstl/1024                     2708 ns         2711 ns       254692 items_per_second=377.732M/s
BM_cstl/4096                     9296 ns         9307 ns        77282 items_per_second=440.114M/s
BM_cstl/16384                   39773 ns        39799 ns        17772 items_per_second=411.664M/s
BM_cstl/32768                   80355 ns        80393 ns         8917 items_per_second=407.599M/s
BM_cstl_BigO                     2.45 N          2.45 N
BM_cstl_RMS                         2 %             2 %
```
