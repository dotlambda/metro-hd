#include "music.h"

const uint16_t splash[] PROGMEM = {
    0, 0, 8778,
    625, 0, 0,
    937, 0, 6576,
    625, 0, 0,
    937, 0, 5219,
    625, 0, 0,
    937, 0, 4926,
    625, 0, 0,
    937, 0, 4389,
    625, 0, 0,
    937, 0, 3288,
    625, 0, 0,
    937, 0, 2609,
    625, 0, 0,
    937, 0, 2194,
    625, 0, 0,
    937, 0, 8285,
    625, 0, 0,
    937, 0, 6967,
    625, 0, 0,
    937, 0, 5219,
    625, 0, 0,
    937, 0, 4142,
    625, 0, 0,
    937, 0, 3483,
    625, 0, 0,
    937, 0, 2609,
    625, 0, 0,
    937, 0, 2071,
    625, 0, 0,
    937, 0, 1741,
    625, 0, 0,
    937, 0, 8778,
    625, 0, 0,
    937, 0, 6576,
    625, 0, 0,
    937, 0, 5219,
    625, 0, 0,
    937, 0, 4926,
    625, 0, 0,
    937, 0, 4389,
    625, 0, 0,
    937, 0, 3288,
    625, 0, 0,
    937, 0, 2609,
    625, 0, 0,
    937, 0, 2194,
    625, 0, 0,
    937, 0, 8285,
    625, 0, 0,
    937, 0, 6967,
    625, 0, 0,
    937, 0, 5219,
    625, 0, 0,
    937, 0, 4142,
    625, 0, 0,
    937, 0, 3483,
    625, 0, 0,
    937, 0, 2609,
    625, 0, 0,
    937, 0, 2071,
    0, 1, 2071,
    625, 0, 205,
    312, 0, 1741,
    312, 1, 1741,
    312, 0, 0,
    625, 0, 8778,
    0, 1, 8778,
    625, 0, 137,
    312, 0, 6576,
    625, 0, 137,
    312, 0, 5219,
    625, 0, 137,
    312, 0, 4389,
    625, 0, 137,
    312, 0, 8778,
    625, 0, 137,
    312, 0, 6576,
    625, 0, 137,
    312, 0, 5219,
    625, 0, 137,
    312, 0, 4389,
    625, 0, 137,
    312, 0, 8778,
    625, 0, 137,
    312, 0, 6576,
    625, 0, 137,
    312, 0, 5219,
    625, 0, 137,
    312, 0, 4389,
    625, 0, 137,
    312, 0, 8778,
    625, 0, 137,
    312, 0, 6576,
    625, 0, 137,
    312, 0, 5219,
    625, 0, 137,
    312, 0, 4389,
    625, 0, 137,
    312, 0, 8778,
    625, 0, 205,
    312, 0, 4389,
    625, 0, 205,
    312, 0, 2194,
    625, 0, 205,
    312, 0, 6576,
    0, 1, 6576,
    625, 0, 411,
    312, 0, 3910,
    625, 0, 411,
    312, 0, 2463,
    625, 0, 411,
    312, 0, 5859,
    0, 1, 5859,
    625, 0, 822,
    312, 0, 3910,
    625, 0, 822,
    312, 0, 8778,
    0, 1, 8778,
    625, 0, 1644,
    312, 0, 4389,
    625, 0, 1644,
    312, 0, 2194,
    625, 0, 1644,
    0, 1, 1644,
    312, 0, 6576,
    0, 1, 6576,
    625, 0, 3288,
    312, 0, 3910,
    625, 0, 3288,
    1250, 0, 5859,
    0, 1, 6576,
    625, 0, 6576,
    1250, 1, 4389,
    7500, 0, 0,
    0, 1, 2194,
    625, 1, 0,
    312, 1, 1097,
    625, 1, 0,
    312, 1, 548,
    625, 1, 0,
    312, 1, 1644,
    625, 1, 0,
    312, 0, 4926,
    0, 1, 4926,
    2500, 0, 1464,
    0, 1, 0,
    312, 1, 977,
    625, 1, 0,
    312, 0, 5219,
    0, 1, 5219,
    1875, 0, 0,
    0, 1, 548,
    625, 1, 0,
    312, 0, 5859,
    0, 1, 5859,
    1875, 0, 0,
    0, 1, 548,
    625, 1, 0,
    312, 0, 6576,
    0, 1, 6576,
    1250, 0, 2609,
    312, 1, 2609,
    312, 0, 0,
    312, 0, 5859,
    0, 1, 5859,
    1875, 0, 0,
    0, 1, 548,
    625, 1, 0,
    312, 0, 4926,
    0, 1, 4926,
    1875, 0, 0,
    0, 1, 615,
    625, 1, 0,
    937, 0, 3910,
    0, 1, 3910,
    1250, 0, 2929,
    312, 1, 2929,
    312, 0, 0,
    312, 0, 4389,
    0, 1, 4389,
    5000, 0, 615,
    312, 1, 0,
    312, 1, 1464,
    625, 1, 0,
    312, 1, 977,
    625, 1, 0,
    312, 0, 3288,
    0, 1, 3288,
    1875, 0, 0,
    1875, 1, 548,
    625, 1, 0,
    312, 0, 3288,
    0, 1, 3288,
    1875, 0, 0,
    1875, 1, 615,
    625, 1, 0,
    312, 0, 3288,
    0, 1, 3288,
    1250, 0, 977,
    312, 1, 0,
    312, 0, 3288,
    0, 1, 3288,
    7500, 0, 0,
    7500, 0, 411,
    0, 1, 1741,
    312, 0, 1741,
    1562, 1, 274,
    312, 0, 0,
    468, 0, 244,
    312, 0, 0,
    468, 0, 217,
    0, 1, 2071,
    312, 0, 2071,
    1562, 1, 153,
    312, 0, 0,
    468, 0, 137,
    312, 0, 0,
    468, 0, 153,
    0, 1, 2463,
    312, 0, 2463,
    937, 1, 0,
    1093, 0, 205,
    312, 0, 0,
    468, 1, 2194,
    1875, 1, 137,
    625, 1, 0,
    937, 1, 2463,
    1875, 1, 153,
    625, 1, 0,
    937, 1, 2609,
    1250, 1, 1644,
    625, 1, 0,
    625, 1, 2463,
    1875, 1, 0,
    1875, 1, 137,
    625, 1, 0,
    937, 1, 2194,
    1875, 1, 0,
    1875, 1, 153,
    625, 1, 0,
    937, 1, 1644,
    1250, 1, 274,
    312, 1, 0,
    625, 1, 1845,
    5000, 1, 274,
    312, 1, 0,
    625, 1, 183,
    625, 1, 0,
    937, 1, 366,
    625, 1, 0,
    937, 1, 2071,
    5000, 1, 517,
    312, 1, 0,
    625, 1, 366,
    625, 1, 0,
    937, 1, 205,
    625, 1, 0,
    937, 1, 2194,
    1875, 1, 137,
    625, 1, 0,
    937, 1, 2463,
    1875, 1, 153,
    625, 1, 0,
    937, 1, 2609,
    1250, 1, 274,
    312, 1, 0,
    625, 1, 2929,
    1875, 1, 0,
    1875, 1, 137,
    625, 1, 0,
    937, 1, 2609,
    1875, 1, 0,
    1875, 1, 153,
    625, 1, 0,
    937, 1, 2194,
    1875, 1, 0,
    1875, 1, 1845,
    1875, 1, 183,
    625, 1, 0,
    937, 1, 2194,
    1875, 1, 0,
    1875, 1, 274,
    625, 1, 0,
    937, 1, 2929,
    1250, 1, 183,
    312, 1, 0,
    625, 0, 6576,
    0, 1, 6576,
    312, 0, 3288,
    156, 0, 6576,
    625, 0, 3288,
    312, 0, 6576,
    937, 0, 205,
    156, 1, 0,
    468, 0, 6576,
    0, 1, 6576,
    1250, 0, 4142,
    312, 1, 4142,
    312, 0, 6576,
    0, 1, 6576,
    1875, 0, 4926,
    0, 1, 4926,
    937, 0, 6576,
    0, 1, 822,
    1875, 0, 822,
    937, 0, 5859,
    1875, 0, 822,
    312, 1, 0,
    937, 0, 5219,
    1250, 0, 0,
    1875, 0, 5859,
    0, 1, 5859,
    1875, 0, 822,
    937, 0, 5219,
    1875, 0, 822,
    312, 1, 0,
    625, 0, 5859,
    1250, 0, 0,
    1875, 0, 6576,
    0, 1, 6576,
    5000, 0, 411,
    312, 1, 0,
    625, 1, 548,
    625, 1, 0,
    937, 1, 652,
    625, 1, 0,
    937, 0, 8778,
    0, 1, 8778,
    5000, 0, 2194,
    312, 1, 0,
    625, 1, 2463,
    625, 1, 0,
    937, 1, 2609,
    625, 1, 0,
    937, 0, 6576,
    0, 1, 6576,
    1875, 0, 0,
    1875, 1, 732,
    625, 1, 0,
    312, 0, 5859,
    0, 1, 5859,
    1875, 0, 0,
    1875, 1, 822,
    625, 1, 0,
    312, 0, 5219,
    0, 1, 5219,
    1250, 0, 1231,
    312, 1, 0,
    312, 0, 5859,
    0, 1, 5859,
    1875, 0, 0,
    1875, 1, 822,
    625, 1, 0,
    312, 0, 5219,
    0, 1, 5219,
    1875, 0, 0,
    1875, 1, 870,
    625, 1, 0,
    312, 0, 5859,
    0, 1, 5859,
    1250, 0, 822,
    312, 1, 0,
    312, 0, 6576,
    0, 1, 6576,
    5000, 0, 1231,
    312, 1, 0,
    312, 1, 2929,
    625, 1, 0,
    312, 1, 1955,
    625, 1, 0,
    312, 0, 8778,
    0, 1, 8778,
    2500, 0, 1097,
    0, 1, 0,
    312, 1, 3288,
    625, 1, 0,
    312, 0, 8778,
    0, 1, 8778,
    625, 0, 1955,
    0, 1, 0,
    312, 0, 7820,
    0, 1, 7820,
    625, 0, 1231,
    0, 1, 0,
    312, 0, 6967,
    0, 1, 6967,
    625, 0, 2929,
    0, 1, 0,
    312, 0, 6576,
    0, 1, 6576,
    625, 0, 1955,
    0, 1, 0,
    312, 0, 5859,
    0, 1, 5859,
    625, 0, 732,
    0, 1, 0,
    312, 0, 4389,
    0, 1, 4389,
    625, 0, 2194,
    0, 1, 0,
    312, 0, 3483,
    0, 1, 3483,
    625, 0, 1304,
    0, 1, 0,
    312, 0, 2929,
    0, 1, 3483,
    625, 0, 3483,
    0, 1, 0,
    312, 0, 6967,
    0, 1, 6967,
    625, 0, 1097,
    0, 1, 0,
    312, 0, 5859,
    0, 1, 5859,
    625, 0, 1304,
    0, 1, 0,
    312, 0, 4389,
    0, 1, 4389,
    625, 0, 1741,
    0, 1, 0,
    312, 0, 3483,
    0, 1, 3483,
    625, 0, 1741,
    0, 1, 0,
    312, 0, 6576,
    0, 1, 6576,
    625, 0, 822,
    0, 1, 0,
    312, 0, 4926,
    0, 1, 4926,
    625, 0, 3910,
    0, 1, 0,
    312, 1, 3910,
    625, 0, 1231,
    0, 1, 0,
    312, 0, 3288,
    0, 1, 3288,
    625, 0, 2929,
    0, 1, 0,
    312, 0, 3910,
    0, 1, 3910,
    625, 0, 1955,
    0, 1, 0,
    312, 0, 4926,
    0, 1, 4926,
    625, 0, 1231,
    0, 1, 0,
    312, 0, 6576,
    0, 1, 6576,
    625, 0, 2929,
    0, 1, 0,
    312, 0, 7820,
    0, 1, 7820,
    625, 0, 1955,
    0, 1, 0,
    312, 0, 6967,
    0, 1, 6967,
    625, 0, 3483,
    0, 1, 0,
    312, 0, 5219,
    0, 1, 5219,
    625, 0, 1741,
    0, 1, 0,
    312, 0, 4389,
    0, 1, 4389,
    625, 0, 870,
    0, 1, 0,
    312, 0, 3483,
    0, 1, 3483,
    625, 0, 2194,
    0, 1, 0,
    312, 0, 2609,
    0, 1, 2609,
    625, 0, 1741,
    0, 1, 0,
    312, 0, 2194,
    0, 1, 2194,
    625, 0, 1304,
    0, 1, 0,
    312, 0, 2609,
    0, 1, 2609,
    625, 0, 1097,
    0, 1, 0,
    312, 0, 3483,
    0, 1, 3483,
    625, 0, 870,
    0, 1, 0,
    312, 0, 3910,
    0, 1, 3910,
    625, 0, 1955,
    0, 1, 0,
    312, 0, 2929,
    0, 1, 2929,
    625, 0, 1464,
    0, 1, 0,
    312, 0, 3910,
    0, 1, 3910,
    625, 0, 1955,
    0, 1, 0,
    312, 0, 4926,
    0, 1, 4926,
    625, 0, 977,
    0, 1, 0,
    312, 0, 5859,
    0, 1, 5859,
    625, 0, 1231,
    0, 1, 0,
    312, 0, 4926,
    0, 1, 4926,
    625, 0, 1464,
    0, 1, 0,
    312, 0, 5859,
    0, 1, 5859,
    625, 0, 1955,
    0, 1, 0,
    312, 0, 7820,
    0, 1, 7820,
    625, 0, 3288,
    0, 1, 0,
    312, 0, 8778,
    0, 1, 8778,
    10000, 0, 4389,
    0, 1, 0,
    312, 1, 977,
    625, 1, 0,
    312, 1, 732,
    625, 1, 0,
    312, 1, 488,
    625, 1, 0,
    312, 1, 1231,
    625, 1, 0,
    937, 1, 1644,
    625, 1, 0,
    937, 1, 2194,
    625, 1, 0,
    312, 1, 4389,
    625, 1, 0,
    312, 1, 2194,
    625, 1, 0,
    312, 1, 1955,
    625, 1, 0,
    312, 1, 3910,
    625, 1, 0,
    312, 1, 1955,
    625, 1, 0,
    312, 1, 1741,
    625, 1, 0,
    312, 1, 3483,
    625, 1, 0,
    312, 1, 1741,
    625, 1, 0,
    312, 1, 1644,
    625, 1, 0,
    312, 1, 3288,
    625, 1, 0,
    312, 1, 1644,
    625, 1, 0,
    312, 1, 1464,
    625, 1, 0,
    312, 1, 2929,
    625, 1, 0,
    312, 1, 1464,
    625, 1, 0,
    312, 1, 3288,
    625, 1, 0,
    312, 0, 2194,
    0, 1, 4389,
    312, 0, 4389,
    312, 1, 3288,
    156, 0, 0,
    312, 0, 2194,
    0, 1, 2194,
    312, 0, 1097,
    156, 0, 3288,
    156, 1, 3288,
    156, 0, 0,
    312, 0, 2194,
    0, 1, 2194,
    312, 0, 1097,
    156, 0, 3288,
    156, 1, 3288,
    156, 0, 0,
    312, 0, 2194,
    312, 0, 3288,
    312, 1, 0,
    156, 0, 0,
    312, 0, 2194,
    0, 1, 2194,
    312, 0, 1955,
    156, 0, 3288,
    156, 1, 3288,
    156, 0, 0,
    312, 0, 2194,
    0, 1, 2194,
    312, 0, 1231,
    156, 0, 3288,
    156, 1, 3288,
    156, 0, 0,
    312, 0, 2194,
    0, 1, 2929,
    312, 0, 2929,
    156, 0, 3288,
    156, 1, 3288,
    156, 0, 0,
    312, 0, 2194,
    0, 1, 2194,
    312, 0, 1955,
    156, 0, 3288,
    156, 1, 3288,
    156, 0, 0,
    312, 0, 4389,
    0, 1, 4389,
    312, 0, 2194,
    156, 0, 5219,
    156, 1, 5219,
    156, 0, 0,
    312, 0, 6576,
    0, 1, 6576,
    312, 0, 2194,
    156, 0, 8778,
    156, 1, 8778,
    156, 0, 0,
    312, 1, 1097,
    625, 1, 0,
    312, 1, 3288,
    625, 1, 0,
    312, 1, 1955,
    625, 1, 0,
    312, 1, 1231,
    625, 1, 0,
    312, 1, 2929,
    625, 1, 0,
    312, 1, 1955,
    625, 1, 0,
    312, 0, 1644,
    0, 1, 1644,
    625, 0, 822,
    1250, 1, 0,
    937, 0, 1644,
    0, 1, 1644,
    625, 0, 822,
    1250, 1, 0,
    937, 0, 1644,
    0, 1, 1644,
    625, 0, 822,
    625, 1, 0,
    625, 1, 822,
    937, 0, 1644,
    625, 0, 822,
    312, 1, 0,
    937, 0, 1644,
    0, 1, 1644,
    625, 0, 822,
    1250, 1, 0,
    937, 0, 1644,
    0, 1, 1644,
    625, 0, 822,
    625, 1, 0,
    625, 0, 1097,
    0, 1, 1097,
    625, 0, 0,
    625, 1, 0,
    937, 0, 822,
    0, 1, 822,
    625, 0, 0,
    312, 0, 822,
    0, 1, 822,
    625, 0, 0,
    312, 0, 1097,
    0, 1, 1097,
    625, 0, 0,
    312, 1, 822,
    625, 0, 0,
    312, 0, 822,
    0, 1, 822,
    625, 0, 0,
    312, 0, 1097,
    0, 1, 1097,
    625, 0, 0,
    312, 0, 822,
    0, 1, 822,
    625, 0, 0,
    312, 0, 977,
    0, 1, 977,
    625, 0, 0,
    625, 1, 0,
    937, 0, 732,
    0, 1, 732,
    625, 0, 0,
    312, 0, 732,
    0, 1, 732,
    625, 0, 0,
    312, 0, 977,
    0, 1, 977,
    625, 0, 0,
    312, 1, 732,
    625, 0, 0,
    312, 0, 732,
    0, 1, 732,
    625, 0, 0,
    312, 0, 977,
    0, 1, 977,
    625, 0, 0,
    312, 0, 732,
    0, 1, 732,
    625, 0, 0,
    312, 0, 870,
    0, 1, 870,
    625, 0, 0,
    625, 1, 0,
    937, 0, 652,
    0, 1, 652,
    625, 0, 0,
    312, 0, 652,
    0, 1, 652,
    625, 0, 0,
    312, 0, 870,
    0, 1, 870,
    625, 0, 0,
    312, 0, 435,
    0, 1, 435,
    625, 0, 0,
    312, 0, 548,
    0, 1, 548,
    625, 0, 0,
    312, 0, 652,
    0, 1, 652,
    625, 0, 0,
    312, 0, 870,
    0, 1, 870,
    625, 0, 0,
    312, 0, 822,
    0, 1, 1035,
    625, 1, 822,
    312, 1, 1035,
    4062, 0, 1035,
    937, 1, 205,
    625, 1, 822,
    1250, 1, 0,
    1406, 1, 870,
    312, 1, 0,
    468, 1, 977,
    312, 1, 0,
    468, 1, 1097,
    312, 1, 0,
    156, 1, 1231,
    312, 1, 0,
    156, 1, 1304,
    312, 1, 0,
    156, 1, 1464,
    312, 1, 0,
    156, 1, 1644,
    312, 1, 0,
    156, 1, 205,
    312, 1, 0,
    468, 1, 822,
    312, 1, 0,
    468, 1, 1644,
    312, 1, 0,
    156, 1, 205,
    312, 1, 0,
    468, 1, 822,
    312, 1, 0,
    468, 1, 1644,
    312, 1, 0,
    468, 1, 205,
    312, 1, 0,
    468, 1, 822,
    312, 1, 0,
    468, 1, 1644,
    312, 1, 0,
    468, 1, 205,
    312, 1, 0,
    468, 1, 822,
    312, 1, 0,
    468, 1, 1644,
    312, 1, 0,
    156, 1, 205,
    312, 1, 0,
    468, 1, 822,
    312, 1, 0,
    156, 1, 1644,
    312, 1, 0,
    156, 1, 205,
    312, 1, 0,
    468, 1, 822,
    312, 1, 205,
    156, 1, 1644,
    312, 1, 0,
    156, 1, 1464,
    625, 1, 822,
    156, 1, 0,
    312, 1, 1644,
    312, 1, 1304,
    312, 1, 205,
    156, 1, 0,
    312, 0, 1644,
    0, 1, 1644,
    625, 0, 1231,
    0, 1, 822,
    156, 1, 0,
    156, 1, 0,
    312, 0, 2194,
    0, 1, 2194,
    625, 0, 548,
    0, 1, 0,
    312, 0, 3288,
    0, 1, 3288,
    625, 0, 2194,
    0, 1, 0,
    312, 0, 1644,
    0, 1, 1644,
    625, 0, 822,
    0, 1, 0,
    312, 0, 3103,
    0, 1, 3103,
    625, 0, 1551,
    0, 1, 0,
    312, 0, 2194,
    0, 1, 2194,
    625, 0, 548,
    0, 1, 0,
    312, 0, 3288,
    0, 1, 3288,
    625, 0, 2194,
    0, 1, 0,
    312, 0, 1644,
    0, 1, 1644,
    625, 0, 822,
    0, 1, 0,
    312, 0, 3103,
    0, 1, 3103,
    625, 0, 1551,
    0, 1, 0,
    312, 0, 2194,
    0, 1, 2194,
    625, 0, 548,
    0, 1, 0,
    312, 0, 3288,
    0, 1, 3288,
    625, 0, 2194,
    0, 1, 0,
    312, 0, 1644,
    0, 1, 1644,
    625, 0, 822,
    0, 1, 0,
    312, 0, 3103,
    0, 1, 3103,
    625, 0, 1551,
    0, 1, 0,
    312, 0, 2194,
    0, 1, 2194,
    625, 0, 548,
    0, 1, 0,
    312, 0, 3288,
    0, 1, 3288,
    625, 0, 2194,
    0, 1, 0,
    312, 0, 1644,
    0, 1, 1644,
    625, 0, 822,
    0, 1, 0,
    312, 0, 3103,
    0, 1, 3103,
    625, 0, 1551,
    0, 1, 0,
    312, 0, 3288,
    0, 1, 3288,
    312, 0, 2194,
    156, 0, 3483,
    0, 1, 3483,
    312, 0, 2609,
    0, 1, 0,
    156, 0, 3910,
    0, 1, 3910,
    312, 0, 3288,
    0, 1, 0,
    156, 0, 4389,
    0, 1, 4389,
    312, 0, 3483,
    0, 1, 0,
    156, 0, 4926,
    0, 1, 4926,
    312, 0, 3910,
    0, 1, 0,
    156, 0, 5219,
    0, 1, 5219,
    312, 0, 4389,
    0, 1, 0,
    156, 0, 5859,
    0, 1, 5859,
    312, 0, 5219,
    0, 1, 0,
    156, 0, 6576,
    0, 1, 6576,
    312, 0, 5859,
    0, 1, 0,
    156, 0, 6967,
    0, 1, 6967,
    312, 0, 6576,
    0, 1, 0,
    156, 0, 7820,
    0, 1, 7820,
    312, 0, 6967,
    0, 1, 0,
    156, 0, 8778,
    0, 1, 8778,
    312, 0, 7820,
    0, 1, 0,
    156, 0, 9853,
    0, 1, 9853,
    312, 0, 8778,
    0, 1, 0,
    156, 0, 10439,
    0, 1, 10439,
    312, 0, 9853,
    0, 1, 0,
    156, 0, 11718,
    0, 1, 11718,
    312, 0, 10439,
    0, 1, 0,
    156, 0, 12414,
    0, 1, 12414,
    312, 0, 11718,
    0, 1, 0,
    156, 0, 13153,
    0, 1, 13153,
    312, 0, 0,
    156, 0, 13153,
    937, 1, 13153,
    625, 1, 0,
    937, 1, 6576,
    625, 1, 0,
    937, 1, 13153,
    625, 1, 0,
    937, 1, 13153,
    625, 1, 0,
    1875, 1, 6576,
    625, 1, 0,
    937, 1, 13153,
    625, 1, 0,
    1562, 0, 0,
    1875, 0, 11718,
    625, 0, 0,
    937, 0, 9853,
    625, 0, 0,
    937, 0, 7820,
    625, 0, 0,
    937, 0, 6576,
    625, 0, 0,
    937, 0, 5859,
    625, 0, 0,
    937, 0, 4926,
    625, 0, 0,
    937, 0, 3910,
    625, 0, 0,
    937, 0, 4389,
    625, 0, 0,
    937, 0, 4926,
    625, 0, 0,
    937, 0, 5859,
    625, 0, 0,
    937, 0, 6576,
    625, 0, 0,
    937, 0, 6967,
    625, 0, 0,
    937, 0, 7820,
    625, 0, 0,
    937, 0, 8778,
    625, 0, 0,
    937, 0, 6576,
    625, 0, 0,
    937, 0, 4389,
    625, 0, 0,
    937, 0, 3288,
    625, 0, 0,
    937, 0, 2609,
    625, 0, 0,
    937, 0, 2463,
    625, 0, 0,
    937, 0, 2194,
    625, 0, 0,
    937, 0, 2609,
    625, 0, 0,
    937, 0, 3288,
    625, 0, 0,
    937, 0, 4389,
    625, 0, 0,
    937, 0, 4142,
    625, 0, 0,
    937, 0, 4926,
    625, 0, 0,
    937, 0, 4389,
    625, 0, 0,
    937, 0, 4142,
    625, 0, 0,
    937, 0, 4389,
    625, 0, 0,
    937, 0, 3288,
    STOP
};

