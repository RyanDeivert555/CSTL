## Outputs of running the benchmark on my machine.

### Compiler: `gcc 16.2.1 20260810`
### Triple Target: `x86_64-pc-linux-gnu`
### Flags: `-O3`
```text
---------------------------------------------------------------------------------
Benchmark                       Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------
BM_std_vector/8               351 ns          354 ns      2009309 items_per_second=22.5803M/s
BM_std_vector/16              386 ns          388 ns      1881577 items_per_second=41.2407M/s
BM_std_vector/64              374 ns          377 ns      1807275 items_per_second=169.602M/s
BM_std_vector/256             539 ns          542 ns      1000000 items_per_second=472.601M/s
BM_std_vector/1024            844 ns          851 ns       815875 items_per_second=1.20281G/s
BM_std_vector/4096           2222 ns         2229 ns       298350 items_per_second=1.83759G/s
BM_std_vector/16384         10499 ns        10520 ns        60091 items_per_second=1.55735G/s
BM_std_vector/65536         37795 ns        37834 ns        16271 items_per_second=1.73218G/s
BM_std_vector/262144       168239 ns       168324 ns         4012 items_per_second=1.55738G/s
BM_std_vector/1048576      676244 ns       676465 ns         1281 items_per_second=1.55008G/s
BM_std_vector/4194304     2639261 ns      2640113 ns          314 items_per_second=1.58868G/s
BM_std_vector/10485760   14783353 ns     14783541 ns           47 items_per_second=709.286M/s
BM_cstl_vec/8                 370 ns          372 ns      1935000 items_per_second=21.4795M/s
BM_cstl_vec/16                370 ns          374 ns      1878145 items_per_second=42.7891M/s
BM_cstl_vec/64                402 ns          405 ns      1728175 items_per_second=157.875M/s
BM_cstl_vec/256               484 ns          488 ns      1449500 items_per_second=524.464M/s
BM_cstl_vec/1024              836 ns          841 ns       829085 items_per_second=1.21802G/s
BM_cstl_vec/4096             2326 ns         2332 ns       296086 items_per_second=1.75622G/s
BM_cstl_vec/16384           11247 ns        11263 ns        61027 items_per_second=1.45466G/s
BM_cstl_vec/65536           40564 ns        40597 ns        17065 items_per_second=1.6143G/s
BM_cstl_vec/262144         177100 ns       177197 ns         3915 items_per_second=1.4794G/s
BM_cstl_vec/1048576        732406 ns       732621 ns         1012 items_per_second=1.43127G/s
BM_cstl_vec/4194304       2708700 ns      2709700 ns          236 items_per_second=1.54788G/s
BM_cstl_vec/10485760     15338212 ns     15338711 ns           44 items_per_second=683.614M/s
```

```text
----------------------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------------
BM_std_unordered_map/8               266 ns          268 ns      2680798 items_per_second=29.8715M/s
BM_std_unordered_map/16              385 ns          387 ns      1831123 items_per_second=41.3622M/s
BM_std_unordered_map/64             1469 ns         1474 ns       478146 items_per_second=43.4217M/s
BM_std_unordered_map/256            4743 ns         4747 ns       148039 items_per_second=53.9271M/s
BM_std_unordered_map/1024          17178 ns        17186 ns        40868 items_per_second=59.5829M/s
BM_std_unordered_map/4096          68239 ns        68256 ns        10407 items_per_second=60.0092M/s
BM_std_unordered_map/16384        275650 ns       275665 ns         2486 items_per_second=59.4344M/s
BM_std_unordered_map/65536       1141816 ns      1141859 ns          620 items_per_second=57.3941M/s
BM_std_unordered_map/262144      4779977 ns      4780005 ns          147 items_per_second=54.8418M/s
BM_std_unordered_map/1048576    20884221 ns     20884539 ns           33 items_per_second=50.2082M/s
BM_std_unordered_map/4194304   103549516 ns    103549708 ns            7 items_per_second=40.5052M/s
BM_std_unordered_map/10485760  249640274 ns    249641251 ns            3 items_per_second=42.0033M/s
BM_std_map/8                         276 ns          274 ns      2482655 items_per_second=29.1903M/s
BM_std_map/16                        419 ns          420 ns      1667762 items_per_second=38.1268M/s
BM_std_map/64                       1697 ns         1701 ns       410021 items_per_second=37.6326M/s
BM_std_map/256                      7484 ns         7487 ns        94909 items_per_second=34.1934M/s
BM_std_map/1024                    30778 ns        30781 ns        22499 items_per_second=33.2669M/s
BM_std_map/4096                   145933 ns       145963 ns         4893 items_per_second=28.0618M/s
BM_std_map/16384                  662441 ns       662467 ns         1066 items_per_second=24.7318M/s
BM_std_map/65536                 4960235 ns      4960279 ns          139 items_per_second=13.2122M/s
BM_std_map/262144               30037910 ns     30037564 ns           23 items_per_second=8.72721M/s
BM_std_map/1048576             152399410 ns    152400002 ns            5 items_per_second=6.88042M/s
BM_std_map/4194304             867505544 ns    867507965 ns            1 items_per_second=4.83489M/s
BM_std_map/10485760           2806415644 ns   2806334162 ns            1 items_per_second=3.73646M/s
BM_cstl_hashmap/8                    251 ns          252 ns      2811807 items_per_second=31.7411M/s
BM_cstl_hashmap/16                   296 ns          299 ns      2392671 items_per_second=53.5669M/s
BM_cstl_hashmap/64                   501 ns          503 ns      1415724 items_per_second=127.134M/s
BM_cstl_hashmap/256                 1166 ns         1168 ns       574394 items_per_second=219.125M/s
BM_cstl_hashmap/1024                3764 ns         3773 ns       185644 items_per_second=271.369M/s
BM_cstl_hashmap/4096               15199 ns        15227 ns        53159 items_per_second=269.004M/s
BM_cstl_hashmap/16384              60087 ns        60127 ns        11674 items_per_second=272.491M/s
BM_cstl_hashmap/65536             240102 ns       240207 ns         2798 items_per_second=272.831M/s
BM_cstl_hashmap/262144           1256934 ns      1257251 ns          567 items_per_second=208.506M/s
BM_cstl_hashmap/1048576          5391356 ns      5393241 ns          134 items_per_second=194.424M/s
BM_cstl_hashmap/4194304         22745922 ns     22748758 ns           33 items_per_second=184.375M/s
BM_cstl_hashmap/10485760        43549016 ns     43549712 ns           15 items_per_second=240.777M/s
```

```text
----------------------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------------
BM_std_unordered_set/8               267 ns          269 ns      2619572 items_per_second=29.7607M/s
BM_std_unordered_set/16              374 ns          375 ns      1756014 items_per_second=42.6588M/s
BM_std_unordered_set/64             1482 ns         1483 ns       488260 items_per_second=43.1533M/s
BM_std_unordered_set/256            4780 ns         4782 ns       150295 items_per_second=53.5326M/s
BM_std_unordered_set/1024          17823 ns        17831 ns        40005 items_per_second=57.4288M/s
BM_std_unordered_set/4096          65611 ns        65617 ns        11185 items_per_second=62.4228M/s
BM_std_unordered_set/16384        270333 ns       270359 ns         2589 items_per_second=60.601M/s
BM_std_unordered_set/65536       1088538 ns      1088629 ns          625 items_per_second=60.2005M/s
BM_std_unordered_set/262144      4657572 ns      4657384 ns          120 items_per_second=56.2857M/s
BM_std_unordered_set/1048576    20342085 ns     20279351 ns           33 items_per_second=51.7066M/s
BM_std_unordered_set/4194304   100375531 ns    100376129 ns            7 items_per_second=41.7859M/s
BM_std_unordered_set/10485760  231312046 ns    231314021 ns            3 items_per_second=45.3313M/s
BM_std_set/8                         270 ns          271 ns      2602149 items_per_second=29.4688M/s
BM_std_set/16                        385 ns          386 ns      1838735 items_per_second=41.4654M/s
BM_std_set/64                       1589 ns         1589 ns       432664 items_per_second=40.2651M/s
BM_std_set/256                      7184 ns         7186 ns       101280 items_per_second=35.6262M/s
BM_std_set/1024                    31486 ns        31492 ns        22777 items_per_second=32.5164M/s
BM_std_set/4096                   147569 ns       147561 ns         4633 items_per_second=27.758M/s
BM_std_set/16384                  733511 ns       733562 ns         1013 items_per_second=22.3349M/s
BM_std_set/65536                 5080557 ns      5080608 ns          100 items_per_second=12.8992M/s
BM_std_set/262144               30813947 ns     30813771 ns           23 items_per_second=8.50737M/s
BM_std_set/1048576             149865294 ns    149863931 ns            5 items_per_second=6.99685M/s
BM_std_set/4194304             889375862 ns    889377166 ns            1 items_per_second=4.716M/s
BM_std_set/10485760           2665721969 ns   2665700663 ns            1 items_per_second=3.93358M/s
BM_cstl_set/8                        245 ns          245 ns      2856763 items_per_second=32.6803M/s
BM_cstl_set/16                       289 ns          291 ns      2460152 items_per_second=54.9553M/s
BM_cstl_set/64                       478 ns          480 ns      1461260 items_per_second=133.461M/s
BM_cstl_set/256                     1083 ns         1085 ns       623289 items_per_second=236.02M/s
BM_cstl_set/1024                    3199 ns         3215 ns       211256 items_per_second=318.515M/s
BM_cstl_set/4096                   11894 ns        11906 ns        60797 items_per_second=344.016M/s
BM_cstl_set/16384                  56922 ns        56957 ns        12021 items_per_second=287.655M/s
BM_cstl_set/65536                 213212 ns       213270 ns         3292 items_per_second=307.292M/s
BM_cstl_set/262144               1080334 ns      1080546 ns          631 items_per_second=242.603M/s
BM_cstl_set/1048576              5042392 ns      5043423 ns          100 items_per_second=207.91M/s
BM_cstl_set/4194304             21151657 ns     21153672 ns           33 items_per_second=198.278M/s
BM_cstl_set/10485760            43371584 ns     43371402 ns           16 items_per_second=241.767M/s
```

```text
---------------------------------------------------------------------------------------------
Benchmark                                   Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------------
BM_std_queue/8                            208 ns          209 ns      3346911 items_per_second=38.1944M/s
BM_std_queue/16                           215 ns          217 ns      3251685 items_per_second=73.8255M/s
BM_std_queue/64                           238 ns          239 ns      2946402 items_per_second=267.741M/s
BM_std_queue/256                          330 ns          332 ns      2129286 items_per_second=770.581M/s
BM_std_queue/1024                         790 ns          792 ns       870926 items_per_second=1.29322G/s
BM_std_queue/4096                        2482 ns         2485 ns       283188 items_per_second=1.64858G/s
BM_std_queue/16384                      11681 ns        11699 ns        55638 items_per_second=1.4005G/s
BM_std_queue/65536                      83486 ns        83361 ns         8466 items_per_second=786.17M/s
BM_std_queue/262144                    376652 ns       376682 ns         1919 items_per_second=695.929M/s
BM_std_queue/1048576                  1606699 ns      1606800 ns          438 items_per_second=652.587M/s
BM_std_queue/4194304                  7029587 ns      7029612 ns           96 items_per_second=596.662M/s
BM_std_queue/10485760                18487925 ns     18487928 ns           38 items_per_second=567.168M/s
BM_cstl_queue_no_prealloc/8               206 ns          208 ns      3332437 items_per_second=38.4633M/s
BM_cstl_queue_no_prealloc/16              234 ns          235 ns      2985361 items_per_second=68.1319M/s
BM_cstl_queue_no_prealloc/64              309 ns          310 ns      2212114 items_per_second=206.187M/s
BM_cstl_queue_no_prealloc/256             525 ns          527 ns      1356082 items_per_second=485.624M/s
BM_cstl_queue_no_prealloc/1024           1338 ns         1340 ns       554460 items_per_second=764.012M/s
BM_cstl_queue_no_prealloc/4096           4298 ns         4305 ns       162285 items_per_second=951.372M/s
BM_cstl_queue_no_prealloc/16384         16192 ns        16213 ns        42737 items_per_second=1.01056G/s
BM_cstl_queue_no_prealloc/65536         63509 ns        63546 ns        12063 items_per_second=1.03132G/s
BM_cstl_queue_no_prealloc/262144       267136 ns       267277 ns         2666 items_per_second=980.795M/s
BM_cstl_queue_no_prealloc/1048576     2831192 ns      2831545 ns          257 items_per_second=370.319M/s
BM_cstl_queue_no_prealloc/4194304    12325744 ns     12325776 ns           58 items_per_second=340.287M/s
BM_cstl_queue_no_prealloc/10485760   41696833 ns     41697582 ns           16 items_per_second=251.472M/s
BM_cstl_queue_prealloc/8                  204 ns          205 ns      3292506 items_per_second=38.9839M/s
BM_cstl_queue_prealloc/16                 212 ns          214 ns      3303613 items_per_second=74.7685M/s
BM_cstl_queue_prealloc/64                 262 ns          263 ns      2638520 items_per_second=242.897M/s
BM_cstl_queue_prealloc/256                428 ns          428 ns      1619760 items_per_second=598.575M/s
BM_cstl_queue_prealloc/1024               940 ns          943 ns       777886 items_per_second=1.08606G/s
BM_cstl_queue_prealloc/4096              2992 ns         2995 ns       229192 items_per_second=1.36764G/s
BM_cstl_queue_prealloc/16384            12578 ns        12614 ns        55809 items_per_second=1.29892G/s
BM_cstl_queue_prealloc/65536            49757 ns        49795 ns        14586 items_per_second=1.31611G/s
BM_cstl_queue_prealloc/262144          194603 ns       194680 ns         3380 items_per_second=1.34654G/s
BM_cstl_queue_prealloc/1048576         831694 ns       831910 ns          870 items_per_second=1.26044G/s
BM_cstl_queue_prealloc/4194304        3463021 ns      3463862 ns          209 items_per_second=1.21087G/s
BM_cstl_queue_prealloc/10485760      19217050 ns     19216575 ns           36 items_per_second=545.662M/s
```

