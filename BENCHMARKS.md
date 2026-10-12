Outputs of running the benchmark on my machine.

```text
Compiler: gcc 16.1.1 20260725
Target:   x86_64-pc-linux-gnu
Flags:    -O3
```

```text
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
BM_std_vector/8            335 ns          336 ns      2064979 items_per_second=23.7841M/s
BM_std_vector/16           336 ns          338 ns      2086289 items_per_second=47.2954M/s
BM_std_vector/64           346 ns          349 ns      1984136 items_per_second=183.358M/s
BM_std_vector/256          396 ns          398 ns      1741918 items_per_second=643.032M/s
BM_std_vector/1024         565 ns          568 ns      1246499 items_per_second=1.80247G/s
BM_std_vector/4096        1229 ns         1232 ns       569935 items_per_second=3.32486G/s
BM_std_vector/16384       3912 ns         3928 ns       179420 items_per_second=4.17063G/s
BM_std_vector/32768       7485 ns         7506 ns        94010 items_per_second=4.36535G/s
BM_std_vector_BigO        0.23 N          0.23 N    
BM_std_vector_RMS           16 %            16 %    
BM_cstl_vec/8              332 ns          335 ns      2095170 items_per_second=23.901M/s
BM_cstl_vec/16             337 ns          339 ns      2070154 items_per_second=47.1319M/s
BM_cstl_vec/64             346 ns          348 ns      1996539 items_per_second=183.655M/s
BM_cstl_vec/256            397 ns          400 ns      1760700 items_per_second=639.495M/s
BM_cstl_vec/1024           563 ns          565 ns      1228204 items_per_second=1.81318G/s
BM_cstl_vec/4096          1236 ns         1238 ns       568845 items_per_second=3.30779G/s
BM_cstl_vec/16384         3959 ns         3978 ns       177708 items_per_second=4.11903G/s
BM_cstl_vec/32768         7638 ns         7660 ns        93345 items_per_second=4.27759G/s
BM_cstl_vec_BigO          0.24 N          0.24 N    
BM_cstl_vec_RMS             15 %            15 %    
```

```text
-------------------------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------------------
BM_std_unordered_map/8            240 ns          241 ns      2961845 items_per_second=33.1654M/s
BM_std_unordered_map/16           330 ns          331 ns      2091229 items_per_second=48.2816M/s
BM_std_unordered_map/64          1273 ns         1275 ns       550019 items_per_second=50.188M/s
BM_std_unordered_map/256         4247 ns         4250 ns       166701 items_per_second=60.2337M/s
BM_std_unordered_map/1024       16784 ns        16807 ns        40762 items_per_second=60.928M/s
BM_std_unordered_map/4096       64258 ns        64268 ns        10868 items_per_second=63.7327M/s
BM_std_unordered_map/16384     246505 ns       246515 ns         2747 items_per_second=66.4625M/s
BM_std_unordered_map/32768     497743 ns       497856 ns         1422 items_per_second=65.8182M/s
BM_std_unordered_map_BigO       15.17 N         15.17 N    
BM_std_unordered_map_RMS            1 %             1 %    
BM_std_map/8                      257 ns          259 ns      2893026 items_per_second=30.8354M/s
BM_std_map/16                     373 ns          374 ns      1880838 items_per_second=42.7598M/s
BM_std_map/64                    1432 ns         1434 ns       502825 items_per_second=44.6175M/s
BM_std_map/256                   6582 ns         6587 ns       106913 items_per_second=38.865M/s
BM_std_map/1024                 27724 ns        27727 ns        25344 items_per_second=36.9317M/s
BM_std_map/4096                129393 ns       129399 ns         5183 items_per_second=31.6539M/s
BM_std_map/16384               612090 ns       612170 ns         1108 items_per_second=26.7638M/s
BM_std_map/32768              1534121 ns      1534123 ns          462 items_per_second=21.3594M/s
BM_std_map_BigO                  3.04 NlgN       3.04 NlgN 
BM_std_map_RMS                     12 %            12 %    
BM_cstl_hashmap/8                 233 ns          235 ns      3016910 items_per_second=34.1117M/s
BM_cstl_hashmap/16                269 ns          271 ns      2576611 items_per_second=59.0551M/s
BM_cstl_hashmap/64                460 ns          462 ns      1555353 items_per_second=138.458M/s
BM_cstl_hashmap/256              1031 ns         1032 ns       681392 items_per_second=247.979M/s
BM_cstl_hashmap/1024             3100 ns         3102 ns       227700 items_per_second=330.135M/s
BM_cstl_hashmap/4096            13101 ns        13120 ns        57127 items_per_second=312.198M/s
BM_cstl_hashmap/16384           49991 ns        50023 ns        14320 items_per_second=327.532M/s
BM_cstl_hashmap/32768          101210 ns       101257 ns         6800 items_per_second=323.612M/s
BM_cstl_hashmap_BigO             3.08 N          3.08 N    
BM_cstl_hashmap_RMS                 1 %             1 %    
```

```text
-------------------------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------------------
BM_std_unordered_set/8            245 ns          246 ns      2886636 items_per_second=32.4702M/s
BM_std_unordered_set/16           349 ns          352 ns      1892433 items_per_second=45.4247M/s
BM_std_unordered_set/64          1420 ns         1421 ns       483432 items_per_second=45.0244M/s
BM_std_unordered_set/256         4471 ns         4474 ns       155995 items_per_second=57.215M/s
BM_std_unordered_set/1024       16625 ns        16635 ns        43022 items_per_second=61.5572M/s
BM_std_unordered_set/4096       67358 ns        67364 ns        10651 items_per_second=60.8042M/s
BM_std_unordered_set/16384     254504 ns       254519 ns         2749 items_per_second=64.3724M/s
BM_std_unordered_set/32768     501155 ns       501237 ns         1329 items_per_second=65.3742M/s
BM_std_unordered_set_BigO       15.36 N         15.36 N    
BM_std_unordered_set_RMS            2 %             2 %    
BM_std_set/8                      251 ns          251 ns      2771385 items_per_second=31.8899M/s
BM_std_set/16                     363 ns          364 ns      1928226 items_per_second=43.9626M/s
BM_std_set/64                    1459 ns         1460 ns       472445 items_per_second=43.833M/s
BM_std_set/256                   6689 ns         6690 ns       100981 items_per_second=38.268M/s
BM_std_set/1024                 27654 ns        27657 ns        24900 items_per_second=37.0249M/s
BM_std_set/4096                131017 ns       131030 ns         5393 items_per_second=31.26M/s
BM_std_set/16384               615579 ns       615613 ns         1138 items_per_second=26.6141M/s
BM_std_set/32768              1399888 ns      1399924 ns          534 items_per_second=23.407M/s
BM_std_set_BigO                  2.82 NlgN       2.82 NlgN 
BM_std_set_RMS                      5 %             5 %    
BM_cstl_set/8                     225 ns          227 ns      3050450 items_per_second=35.2118M/s
BM_cstl_set/16                    263 ns          266 ns      2601624 items_per_second=60.2487M/s
BM_cstl_set/64                    421 ns          423 ns      1661334 items_per_second=151.387M/s
BM_cstl_set/256                   965 ns          967 ns       725987 items_per_second=264.61M/s
BM_cstl_set/1024                 2833 ns         2836 ns       246216 items_per_second=361.031M/s
BM_cstl_set/4096                 9827 ns         9839 ns        70863 items_per_second=416.297M/s
BM_cstl_set/16384               52570 ns        52592 ns        13208 items_per_second=311.53M/s
BM_cstl_set/32768               93012 ns        93045 ns         7670 items_per_second=352.172M/s
BM_cstl_set_BigO                 2.91 N          2.91 N    
BM_cstl_set_RMS                    10 %            10 %    
```

```text
------------------------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------------
BM_std_queue/8                         192 ns          193 ns      3645859 items_per_second=41.4384M/s
BM_std_queue/16                        195 ns          196 ns      3567232 items_per_second=81.466M/s
BM_std_queue/64                        207 ns          209 ns      3366016 items_per_second=306.463M/s
BM_std_queue/256                       293 ns          295 ns      2404721 items_per_second=868.57M/s
BM_std_queue/1024                      582 ns          584 ns      1218564 items_per_second=1.75289G/s
BM_std_queue/4096                     1995 ns         1997 ns       349093 items_per_second=2.05155G/s
BM_std_queue/16384                    7485 ns         7503 ns        94035 items_per_second=2.18366G/s
BM_std_queue/32768                   14729 ns        14746 ns        48040 items_per_second=2.22224G/s
BM_std_queue_BigO                     0.45 N          0.45 N    
BM_std_queue_RMS                         5 %             5 %    
BM_cstl_queue_no_prealloc/8            184 ns          185 ns      3745906 items_per_second=43.1721M/s
BM_cstl_queue_no_prealloc/16           203 ns          205 ns      3446405 items_per_second=77.9608M/s
BM_cstl_queue_no_prealloc/64           255 ns          256 ns      2776692 items_per_second=250.128M/s
BM_cstl_queue_no_prealloc/256          396 ns          399 ns      1717111 items_per_second=641.972M/s
BM_cstl_queue_no_prealloc/1024         912 ns          914 ns       765303 items_per_second=1.11988G/s
BM_cstl_queue_no_prealloc/4096        2672 ns         2675 ns       263714 items_per_second=1.53121G/s
BM_cstl_queue_no_prealloc/16384       9253 ns         9271 ns        76085 items_per_second=1.76728G/s
BM_cstl_queue_no_prealloc/32768      41513 ns        41249 ns        16933 items_per_second=794.397M/s
BM_cstl_queue_no_prealloc_BigO        0.00 N^2        0.00 N^2  
BM_cstl_queue_no_prealloc_RMS           13 %            13 %    
BM_cstl_queue_prealloc/8               185 ns          187 ns      3685760 items_per_second=42.8793M/s
BM_cstl_queue_prealloc/16              186 ns          187 ns      3740150 items_per_second=85.3878M/s
BM_cstl_queue_prealloc/64              211 ns          213 ns      3232832 items_per_second=300.847M/s
BM_cstl_queue_prealloc/256             307 ns          308 ns      2278157 items_per_second=831.243M/s
BM_cstl_queue_prealloc/1024            699 ns          701 ns       980747 items_per_second=1.46081G/s
BM_cstl_queue_prealloc/4096           2310 ns         2314 ns       331266 items_per_second=1.77018G/s
BM_cstl_queue_prealloc/16384          7953 ns         7973 ns        84874 items_per_second=2.05493G/s
BM_cstl_queue_prealloc/32768         15339 ns        15361 ns        46951 items_per_second=2.13313G/s
BM_cstl_queue_prealloc_BigO           0.47 N          0.47 N    
BM_cstl_queue_prealloc_RMS               6 %             6 %    
```

