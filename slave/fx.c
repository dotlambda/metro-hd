#include "music.h"

const uint16_t shoot[] PROGMEM = {
    10, 4000,
    10, 3980,
    10, 3960,
    10, 3940,
    10, 3920,
    10, 3900,
    10, 3880,
    10, 3860,
    10, 3840,
    10, 3820,
    10, 3800,
    10, 3780,
    10, 3760,
    10, 3740,
    10, 3720,
    10, 3700,
    10, 3680,
    10, 3660,
    10, 3640,
    10, 3620,
    10, 3600,
    10, 3580,
    10, 3560,
    10, 3540,
    10, 3520,
    10, 3500,
    10, 3480,
    10, 3460,
    10, 3440,
    10, 3420,
    10, 3400,
    10, 3380,
    10, 3360,
    10, 3340,
    10, 3320,
    10, 3300,
    10, 3280,
    10, 3260,
    10, 3240,
    10, 3220,
    10, 3200,
    10, 3180,
    10, 3160,
    10, 3140,
    10, 3120,
    10, 3100,
    10, 3080,
    10, 3060,
    10, 3040,
    10, 3020,
    10, 3000,
    10, 2980,
    10, 2960,
    10, 2940,
    10, 2920,
    10, 2900,
    10, 2880,
    10, 2860,
    10, 2840,
    10, 2820,
    10, 2800,
    10, 2780,
    10, 2760,
    10, 2740,
    10, 2720,
    10, 2700,
    10, 2680,
    10, 2660,
    10, 2640,
    10, 2620,
    10, 2600,
    10, 2580,
    10, 2560,
    10, 2540,
    10, 2520,
    10, 2500,
    10, 2480,
    10, 2460,
    10, 2440,
    10, 2420,
    10, 2400,
    10, 2380,
    10, 2360,
    10, 2340,
    10, 2320,
    10, 2300,
    10, 2280,
    10, 2260,
    10, 2240,
    10, 2220,
    10, 2200,
    10, 2180,
    10, 2160,
    10, 2140,
    10, 2120,
    10, 2100,
    10, 2080,
    10, 2060,
    10, 2040,
    10, 2020,
    10, 2000,
    10, 1980,
    10, 1960,
    10, 1940,
    10, 1920,
    10, 1900,
    10, 1880,
    10, 1860,
    10, 1840,
    10, 1820,
    10, 1800,
    10, 1780,
    10, 1760,
    10, 1740,
    10, 1720,
    10, 1700,
    10, 1680,
    10, 1660,
    10, 1640,
    10, 1620,
    10, 1600,
    10, 1580,
    10, 1560,
    10, 1540,
    10, 1520,
    10, 1500,
    10, 1480,
    10, 1460,
    10, 1440,
    10, 1420,
    10, 1400,
    10, 1380,
    10, 1360,
    10, 1340,
    10, 1320,
    10, 1300,
    10, 1280,
    10, 1260,
    10, 1240,
    10, 1220,
    10, 1200,
    10, 1180,
    10, 1160,
    10, 1140,
    10, 1120,
    10, 1100,
    10, 1080,
    10, 1060,
    10, 1040,
    10, 1020,
    STOP
};

const uint16_t explosion[] PROGMEM = {
    100, 0,
    100, 100,
    100, 200,
    100, 300,
    100, 400,
    100, 500,
    100, 600,
    100, 700,
    100, 800,
    100, 900,
    100, 992.25,
    100, 976.5625,
    100, 961.0,
    100, 945.5625,
    100, 930.25,
    100, 915.0625,
    100, 900.0,
    100, 885.0625,
    100, 870.25,
    100, 855.5625,
    100, 841.0,
    100, 826.5625,
    100, 812.25,
    100, 798.0625,
    100, 784.0,
    100, 770.0625,
    100, 756.25,
    100, 742.5625,
    100, 729.0,
    100, 715.5625,
    100, 702.25,
    100, 689.0625,
    100, 676.0,
    100, 663.0625,
    100, 650.25,
    100, 637.5625,
    100, 625.0,
    100, 612.5625,
    100, 600.25,
    100, 588.0625,
    100, 576.0,
    100, 564.0625,
    100, 552.25,
    100, 540.5625,
    100, 529.0,
    100, 517.5625,
    100, 506.25,
    100, 495.0625,
    100, 484.0,
    100, 473.0625,
    100, 462.25,
    100, 451.5625,
    100, 441.0,
    100, 430.5625,
    100, 420.25,
    100, 410.0625,
    100, 400.0,
    100, 390.0625,
    100, 380.25,
    100, 370.5625,
    100, 361.0,
    100, 351.5625,
    100, 342.25,
    100, 333.0625,
    100, 324.0,
    100, 315.0625,
    100, 306.25,
    100, 297.5625,
    100, 289.0,
    100, 280.5625,
    100, 272.25,
    100, 264.0625,
    100, 256.0,
    100, 248.0625,
    100, 240.25,
    100, 232.5625,
    100, 225.0,
    100, 217.5625,
    100, 210.25,
    100, 203.0625,
    100, 196.0,
    100, 189.0625,
    100, 182.25,
    100, 175.5625,
    100, 169.0,
    100, 162.5625,
    100, 156.25,
    100, 150.0625,
    100, 144.0,
    100, 138.0625,
    100, 132.25,
    100, 126.5625,
    100, 121.0,
    100, 115.5625,
    100, 110.25,
    100, 105.0625,
    100, 100.0,
    100, 95.0625,
    100, 90.25,
    100, 85.5625,
    100, 81.0,
    100, 76.5625,
    100, 72.25,
    100, 68.0625,
    100, 64.0,
    100, 60.0625,
    100, 56.25,
    100, 52.5625,
    100, 49.0,
    100, 45.5625,
    100, 42.25,
    100, 39.0625,
    100, 36.0,
    100, 33.0625,
    100, 30.25,
    100, 27.5625,
    100, 25.0,
    100, 22.5625,
    100, 20.25,
    100, 18.0625,
    100, 16.0,
    100, 14.0625,
    100, 12.25,
    100, 10.5625,
    100, 9.0,
    100, 7.5625,
    100, 6.25,
    100, 5.0625,
    100, 4.0,
    100, 3.0625,
    100, 2.25,
    100, 1.5625,
    100, 1.0,
    100, 0.5625,
    100, 0.25,
    100, 0.0625,
    STOP
};

