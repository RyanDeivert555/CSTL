## Outputs of running the benchmark on my machine.

### Compiler: `gcc 16.1.1 20260725`
### Triple Target: `x86_64-pc-linux-gnu`
### Flags: `-O3`
```text
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
BM_std_vector/8            341 ns          343 ns      2079247 items_per_second=23.3211M/s
BM_std_vector/16           338 ns          341 ns      2035236 items_per_second=46.8573M/s
BM_std_vector/64           345 ns          347 ns      2019335 items_per_second=184.25M/s
BM_std_vector/256          395 ns          398 ns      1752715 items_per_second=643.376M/s
BM_std_vector/1024         558 ns          561 ns      1247207 items_per_second=1.82532G/s
BM_std_vector/4096        1236 ns         1240 ns       570000 items_per_second=3.30443G/s
BM_std_vector/16384       3998 ns         4017 ns       173611 items_per_second=4.07886G/s
BM_std_vector/32768       7684 ns         7700 ns        89577 items_per_second=4.25535G/s
BM_std_vector_BigO        0.24 N          0.24 N    
BM_std_vector_RMS           15 %            15 %    
BM_cstl_vec/8              340 ns          342 ns      2016042 items_per_second=23.4178M/s
BM_cstl_vec/16             345 ns          349 ns      1981786 items_per_second=45.8787M/s
BM_cstl_vec/64             349 ns          351 ns      1956190 items_per_second=182.437M/s
BM_cstl_vec/256            406 ns          410 ns      1740293 items_per_second=623.752M/s
BM_cstl_vec/1024           596 ns          600 ns      1187074 items_per_second=1.70739G/s
BM_cstl_vec/4096          1290 ns         1293 ns       536243 items_per_second=3.16661G/s
BM_cstl_vec/16384         4210 ns         4234 ns       168791 items_per_second=3.86992G/s
BM_cstl_vec/32768         8099 ns         8136 ns        88793 items_per_second=4.02758G/s
BM_cstl_vec_BigO          0.25 N          0.25 N    
BM_cstl_vec_RMS             15 %            15 %    
```

```text
-------------------------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------------------
BM_std_unordered_map/8            251 ns          253 ns      2816550 items_per_second=31.5671M/s
BM_std_unordered_map/16           358 ns          359 ns      2037546 items_per_second=44.5341M/s
BM_std_unordered_map/64          1367 ns         1367 ns       538643 items_per_second=46.8188M/s
BM_std_unordered_map/256         4316 ns         4319 ns       156696 items_per_second=59.2706M/s
BM_std_unordered_map/1024       16493 ns        16505 ns        40875 items_per_second=62.0436M/s
BM_std_unordered_map/4096       66657 ns        66665 ns        10166 items_per_second=61.4412M/s
BM_std_unordered_map/16384     262931 ns       262952 ns         2655 items_per_second=62.3079M/s
BM_std_unordered_map/32768     557598 ns       557767 ns         1240 items_per_second=58.7485M/s
BM_std_unordered_map_BigO        1.14 NlgN       1.14 NlgN 
BM_std_unordered_map_RMS            4 %             4 %    
BM_std_map/8                      253 ns          257 ns      2702402 items_per_second=31.1817M/s
BM_std_map/16                     370 ns          369 ns      1917662 items_per_second=43.3881M/s
BM_std_map/64                    1490 ns         1491 ns       478894 items_per_second=42.9278M/s
BM_std_map/256                   6797 ns         6799 ns       106311 items_per_second=37.6505M/s
BM_std_map/1024                 28901 ns        28897 ns        24572 items_per_second=35.4359M/s
BM_std_map/4096                136942 ns       136963 ns         5224 items_per_second=29.9059M/s
BM_std_map/16384               626663 ns       626720 ns         1161 items_per_second=26.1424M/s
BM_std_map/32768              1550520 ns      1550580 ns          473 items_per_second=21.1327M/s
BM_std_map_BigO                  3.08 NlgN       3.08 NlgN 
BM_std_map_RMS                     11 %            11 %    
BM_cstl_hashmap/8                 250 ns          251 ns      2780800 items_per_second=31.8281M/s
BM_cstl_hashmap/16                298 ns          303 ns      2416751 items_per_second=52.832M/s
BM_cstl_hashmap/64                490 ns          493 ns      1340216 items_per_second=129.896M/s
BM_cstl_hashmap/256              1105 ns         1100 ns       637631 items_per_second=232.634M/s
BM_cstl_hashmap/1024             3206 ns         3200 ns       220896 items_per_second=320.03M/s
BM_cstl_hashmap/4096            13294 ns        13314 ns        55054 items_per_second=307.648M/s
BM_cstl_hashmap/16384           50853 ns        50887 ns        13540 items_per_second=321.97M/s
BM_cstl_hashmap/32768           98917 ns        98992 ns         6815 items_per_second=331.016M/s
BM_cstl_hashmap_BigO             3.04 N          3.04 N    
BM_cstl_hashmap_RMS                 3 %             3 %    
```

```text
-------------------------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------------------
BM_std_unordered_set/8            252 ns          254 ns      2772323 items_per_second=31.4645M/s
BM_std_unordered_set/16           359 ns          360 ns      1899116 items_per_second=44.4195M/s
BM_std_unordered_set/64          1506 ns         1510 ns       457743 items_per_second=42.389M/s
BM_std_unordered_set/256         4822 ns         4824 ns       145311 items_per_second=53.0732M/s
BM_std_unordered_set/1024       18852 ns        18854 ns        37418 items_per_second=54.3135M/s
BM_std_unordered_set/4096       73468 ns        73487 ns         9737 items_per_second=55.7377M/s
BM_std_unordered_set/16384     271714 ns       271716 ns         2634 items_per_second=60.2982M/s
BM_std_unordered_set/32768     545643 ns       545572 ns         1294 items_per_second=60.0617M/s
BM_std_unordered_set_BigO       16.66 N         16.65 N    
BM_std_unordered_set_RMS            2 %             2 %    
BM_std_set/8                      271 ns          271 ns      2593491 items_per_second=29.5267M/s
BM_std_set/16                     406 ns          410 ns      1770171 items_per_second=39.0537M/s
BM_std_set/64                    1751 ns         1754 ns       427716 items_per_second=36.4862M/s
BM_std_set/256                   7791 ns         7796 ns        88812 items_per_second=32.8355M/s
BM_std_set/1024                 37305 ns        37336 ns        22063 items_per_second=27.4268M/s
BM_std_set/4096                159090 ns       159114 ns         3668 items_per_second=25.7426M/s
BM_std_set/16384               698968 ns       698996 ns         1018 items_per_second=23.4393M/s
BM_std_set/32768              1795911 ns      1795904 ns          396 items_per_second=18.246M/s
BM_std_set_BigO                  3.54 NlgN       3.54 NlgN 
BM_std_set_RMS                     13 %            13 %    
BM_cstl_set/8                     297 ns          299 ns      2191339 items_per_second=26.7259M/s
BM_cstl_set/16                    310 ns          312 ns      2326983 items_per_second=51.2706M/s
BM_cstl_set/64                    528 ns          533 ns      1265507 items_per_second=120.09M/s
BM_cstl_set/256                  1165 ns         1172 ns       595767 items_per_second=218.356M/s
BM_cstl_set/1024                 3465 ns         3470 ns       220384 items_per_second=295.135M/s
BM_cstl_set/4096                11922 ns        11936 ns        56551 items_per_second=343.159M/s
BM_cstl_set/16384               57873 ns        57968 ns        10000 items_per_second=282.641M/s
BM_cstl_set/32768              113622 ns       113792 ns         5193 items_per_second=287.963M/s
BM_cstl_set_BigO                 3.47 N          3.48 N    
BM_cstl_set_RMS                     4 %             4 %    
```

```text
------------------------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------------
BM_std_queue/8                         263 ns          264 ns      2859366 items_per_second=30.2851M/s
BM_std_queue/16                        230 ns          231 ns      2524632 items_per_second=69.1424M/s
BM_std_queue/64                        248 ns          250 ns      2944219 items_per_second=256.398M/s
BM_std_queue/256                       325 ns          327 ns      1965103 items_per_second=783.245M/s
BM_std_queue/1024                      607 ns          607 ns      1037977 items_per_second=1.68578G/s
BM_std_queue/4096                     2348 ns         2352 ns       330617 items_per_second=1.74165G/s
BM_std_queue/16384                    7596 ns         7613 ns        90737 items_per_second=2.15209G/s
BM_std_queue/32768                   15061 ns        14950 ns        46576 items_per_second=2.19181G/s
BM_std_queue_BigO                     0.46 N          0.46 N    
BM_std_queue_RMS                         7 %             7 %    
BM_cstl_queue_no_prealloc/8            208 ns          208 ns      3506120 items_per_second=38.3785M/s
BM_cstl_queue_no_prealloc/16           216 ns          216 ns      3251505 items_per_second=74.1998M/s
BM_cstl_queue_no_prealloc/64           263 ns          265 ns      2620606 items_per_second=241.366M/s
BM_cstl_queue_no_prealloc/256          410 ns          412 ns      1681754 items_per_second=621.652M/s
BM_cstl_queue_no_prealloc/1024         944 ns          947 ns       762435 items_per_second=1.08177G/s
BM_cstl_queue_no_prealloc/4096        2782 ns         2785 ns       250701 items_per_second=1.47057G/s
BM_cstl_queue_no_prealloc/16384       9846 ns         9863 ns        70490 items_per_second=1.66109G/s
BM_cstl_queue_no_prealloc/32768      43473 ns        43200 ns        16508 items_per_second=758.51M/s
BM_cstl_queue_no_prealloc_BigO        0.00 N^2        0.00 N^2  
BM_cstl_queue_no_prealloc_RMS           12 %            12 %    
BM_cstl_queue_prealloc/8               194 ns          196 ns      3549125 items_per_second=40.9059M/s
BM_cstl_queue_prealloc/16              215 ns          216 ns      3403582 items_per_second=74.0101M/s
BM_cstl_queue_prealloc/64              237 ns          238 ns      2909012 items_per_second=268.673M/s
BM_cstl_queue_prealloc/256             347 ns          347 ns      1957739 items_per_second=737.262M/s
BM_cstl_queue_prealloc/1024            749 ns          749 ns       897585 items_per_second=1.36666G/s
BM_cstl_queue_prealloc/4096           2527 ns         2532 ns       291914 items_per_second=1.61776G/s
BM_cstl_queue_prealloc/16384          8662 ns         8689 ns        86052 items_per_second=1.88566G/s
BM_cstl_queue_prealloc/32768         16772 ns        16805 ns        40186 items_per_second=1.94992G/s
BM_cstl_queue_prealloc_BigO           0.52 N          0.52 N    
BM_cstl_queue_prealloc_RMS               6 %             6 %    
```

