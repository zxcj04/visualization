#include "Volume.hpp"

// #include "constants.hpp"

int edgeTable[256] = {
    0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
    0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
    0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
    0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
    0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
    0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
    0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
    0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
    0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
    0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
    0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
    0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
    0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
    0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
    0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
    0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
    0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
    0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
    0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
    0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
    0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
    0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
    0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
    0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
    0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
    0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
    0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
    0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
    0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
    0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
    0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
    0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0
};

int triTable[256][16] = {
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
    {3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
    {3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
    {3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
    {9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
    {9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
    {2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
    {8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
    {9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
    {4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
    {3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
    {1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
    {4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
    {4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
    {9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
    {5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
    {2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
    {9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
    {0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
    {2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
    {10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
    {4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
    {5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
    {5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
    {9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
    {0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
    {1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
    {10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
    {8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
    {2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
    {7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
    {9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
    {2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
    {11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
    {9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
    {5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
    {11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
    {11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
    {1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
    {9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
    {5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
    {2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
    {0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
    {5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
    {6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
    {3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
    {6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
    {5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
    {1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
    {10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
    {6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
    {8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
    {7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
    {3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
    {5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
    {0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
    {9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
    {8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
    {5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
    {0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
    {6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
    {10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
    {10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
    {8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
    {1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
    {3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
    {0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
    {10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
    {3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
    {6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
    {9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
    {8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
    {3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
    {6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
    {0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
    {10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
    {10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
    {2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
    {7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
    {7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
    {2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
    {1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
    {11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
    {8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
    {0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
    {7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
    {10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
    {2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
    {6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
    {7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
    {2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
    {1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
    {10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
    {10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
    {0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
    {7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
    {6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
    {8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
    {9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
    {6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
    {4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
    {10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
    {8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
    {0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
    {1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
    {8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
    {10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
    {4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
    {10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
    {5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
    {11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
    {9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
    {6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
    {7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
    {3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
    {7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
    {9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
    {3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
    {6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
    {9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
    {1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
    {4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
    {7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
    {6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
    {3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
    {0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
    {6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
    {0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
    {11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
    {6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
    {5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
    {9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
    {1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
    {1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
    {10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
    {0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
    {5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
    {10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
    {11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
    {9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
    {7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
    {2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
    {8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
    {9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
    {9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
    {1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
    {9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
    {9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
    {5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
    {0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
    {10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
    {2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
    {0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
    {0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
    {9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
    {5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
    {3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
    {5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
    {8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
    {0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
    {9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
    {1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
    {3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
    {4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
    {9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
    {11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
    {11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
    {2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
    {9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
    {3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
    {1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
    {4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
    {4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
    {0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
    {3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
    {3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
    {0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
    {9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
    {1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
};

Volume::Volume(string inf_filename, string raw_filename, METHODS method, int iso_value, float g_max)
{
    this->inf_filename = inf_filename;
    this->raw_filename = raw_filename;

    this->method = method;

    this->resolution = glm::ivec3(0, 0, 0);
    this->voxelsize  = glm::vec3(1.0f, 1.0f, 1.0f);
    this->valuetype  = TYPE::UNSIGNED_CHAR;
    this->endian     = ENDIAN::LITTLE;

    this->min_value = numeric_limits<float>::max();
    this->max_value = numeric_limits<float>::min();

    this->max_gradient_len = numeric_limits<float>::min();

    read_inf(this->inf_filename);

    this->voxel_count = this->resolution.x * this->resolution.y * this->resolution.z;

    this->data.assign(this->resolution[0], vector<vector<float> >(this->resolution[1], vector<float>(this->resolution[2], 1)));
    this->gradient.assign(this->resolution[0], vector<vector<glm::vec3> >(this->resolution[1], vector<glm::vec3>(this->resolution[2], glm::vec3())));

    if(this->valuetype == TYPE::UNSIGNED_CHAR)
        read_data<unsigned char>(this->raw_filename);

    else if(this->valuetype == TYPE::CHAR)
        read_data<char>(this->raw_filename);

    else if(this->valuetype == TYPE::UNSIGNED_SHORT)
        read_data<unsigned short>(this->raw_filename);

    else if(this->valuetype == TYPE::SHORT)
        read_data<short>(this->raw_filename);

    else if(this->valuetype == TYPE::UNSIGNED_INT)
        read_data<unsigned int>(this->raw_filename);

    else if(this->valuetype == TYPE::INT)
        read_data<int>(this->raw_filename);

    else if(this->valuetype == TYPE::FLOAT)
        read_data<float>(this->raw_filename);

    calc_gradient();

    if(this->method == METHODS::ISO_SURFACE)
    {
        calc_vertex(iso_value);

        setup_vao();
    }
    else if(this->method == METHODS::VOLUME_RENDERING)
    {
        calc_3dtexture();
        calc_1dtexture();

        load_texture3d();
        load_texture1d();

        setup_volume_vao();
    }

    calc_histogram();

    calc_mk_table(g_max);
}

Volume::~Volume()
{

}

void Volume::read_inf(string inf_filename)
{
    fstream inf;
    string line;

    inf.open(inf_filename, fstream::in);

    regex resolution("resolution\\s(\\d+)(:|x)?(\\d+)(:|x)?(\\d+)");
    regex voxelsize("(voxelsize|ratio)\\s((\\d|\\.)+)(:|x)((\\d|\\.)+)(:|x)((\\d|\\.)+)");
    regex sampletype("sampletype\\s([a-z]*(\\s[a-z]+)?)");
    regex valuetype("valuetype\\s(ub|b|us|s|ui|i|f)");
    regex endian("(endian|indiantype)\\s(little|big|l|b)");

    smatch matched;

    while(getline(inf, line))
    {
        string new_line;

        for(auto &c: line)
        {
            if(c == ' ' || c == '-')
                continue;
            else if(c == '=')
                new_line += ' ';
            else
                new_line += tolower(c);
        }

        if(regex_search(new_line, matched, resolution))
        {
            this->resolution.x = stoi(matched[5]);
            this->resolution.y = stoi(matched[3]);
            this->resolution.z = stoi(matched[1]);
        }

        if(regex_search(new_line, matched, voxelsize))
        {
            this->voxelsize.x = stof(matched[8]);
            this->voxelsize.y = stof(matched[5]);
            this->voxelsize.z = stof(matched[2]);
        }

        if(regex_search(new_line, matched, sampletype) || regex_search(new_line, matched, valuetype))
        {
            if(matched[1] == "unsignedchar" || matched[1] == "ub")
            {
                this->valuetype = TYPE::UNSIGNED_CHAR;
                this->bytesize = 1;
            }
            if(matched[1] == "char" || matched[1] == "b")
            {
                this->valuetype = TYPE::CHAR;
                this->bytesize = 1;
            }
            if(matched[1] == "unsignedshort" || matched[1] == "us")
            {
                this->valuetype = TYPE::UNSIGNED_SHORT;
                this->bytesize = 2;
            }
            if(matched[1] == "short" || matched[1] == "s")
            {
                this->valuetype = TYPE::SHORT;
                this->bytesize = 2;
            }
            if(matched[1] == "unsignedint" || matched[1] == "ui")
            {
                this->valuetype = TYPE::UNSIGNED_INT;
                this->bytesize = 4;
            }
            if(matched[1] == "int" || matched[1] == "i")
            {
                this->valuetype = TYPE::INT;
                this->bytesize = 4;
            }
            if(matched[1] == "float" || matched[1] == "f")
            {
                this->valuetype = TYPE::FLOAT;
                this->bytesize = 4;
            }
        }

        if(regex_search(new_line, matched, endian))
        {
            if(matched[2] == "little" || matched[2] == "l")
            {
                this->endian = ENDIAN::LITTLE;
            }
            else
            {
                this->endian = ENDIAN::BIG;
            }
        }
    }

    string TYPE_map[] =
    {
        "UNSIGNED_CHAR", "CHAR", "UNSIGNED_SHORT", "SHORT", "UNSIGNED_INT", "INT", "FLOAT"
    };
    string ENDIAN_map[] =
    {
        "LITTLE", "BIG"
    };


    cout << "---------" << this->inf_filename << "---------" << endl;
    cout << "\tresolution: " << this->resolution.x << "x" << this->resolution.y << "x" << this->resolution.z << endl;
    cout << "\tvoxelsize: " << this->voxelsize.x << ":" << this->voxelsize.y << ":" << this->voxelsize.z << endl;
    cout << "\tsampletype: " << TYPE_map[this->valuetype] << endl;
    cout << "\tendian: " << ENDIAN_map[this->endian] << endl;
    cout << "\tbytesize: " << this->bytesize << endl;
    cout << "----------------------------------------------" << endl;

    inf.close();
}

typedef union EndianTest
{
    uint32_t l;
    unsigned char c[4];
} EndianTest;

bool is_big_endian()
{
    uint32_t x = 0x12345678;
    EndianTest et;
    et.l = x;

    if((int) et.c[0] == 0x12 &&
       (int) et.c[1] == 0x34 &&
       (int) et.c[2] == 0x56 &&
       (int) et.c[3] == 0x78
    )
        return true;

    return false;
}

template<typename T>
void Volume::read_data(string raw_filename)
{
    fstream raw;
    raw.open(raw_filename, ios::in | ios::binary);
    char *buffer = new char[this->bytesize * this->voxel_count];

    raw.read(buffer, this->bytesize * this->voxel_count);
    raw.close();

    ENDIAN endian = ENDIAN::LITTLE;

    if(is_big_endian())
    {
        endian = ENDIAN::BIG;
    }

    for(int x = 0; x < this->resolution.x; x++)
    {
        for(int y = 0; y < this->resolution.y; y++)
        {
            for(int z = 0; z < this->resolution.z; z++)
            {
                int index = x * this->resolution.y * this->resolution.z + y * this->resolution.z + z;

                index *= this->bytesize;

                if(this->endian != endian)
                    reverse(buffer + index, buffer + index + this->bytesize);

                T raw_data;

                memcpy(&raw_data, buffer + index, this->bytesize);

                this->data[x][y][z] = (float) raw_data;

                if(this->data[x][y][z] > this->max_value)
                    max_value = (int) this->data[x][y][z];
                else if(this->data[x][y][z] < this->min_value)
                    min_value = (int) this->data[x][y][z];
            }
        }
    }

    delete [] buffer;
}

void Volume::calc_gradient()
{
    for(int x = 0; x < this->resolution.x; x++)
    {
        for(int y = 0; y < this->resolution.y; y++)
        {
            for(int z = 0; z < this->resolution.z; z++)
            {
                glm::vec3 tmp;

                if( x == this->resolution.x - 1 )
                {
                    tmp.x  = this->data[x][y][z] - this->data[x - 1][y][z];
                    tmp.x /= this->voxelsize.x;
                }
                else if( x == 0 )
                {
                    tmp.x  = this->data[x + 1][y][z] - this->data[x][y][z];
                    tmp.x /= this->voxelsize.x;
                }
                else
                {
                    tmp.x  = this->data[x + 1][y][z] - this->data[x - 1][y][z];
                    tmp.x /= this->voxelsize.x * 2;
                }

                if( y == this->resolution.y - 1 )
                {
                    tmp.y  = this->data[x][y][z] - this->data[x][y - 1][z];
                    tmp.y /= this->voxelsize.y;
                }
                else if( y == 0 )
                {
                    tmp.y  = this->data[x][y + 1][z] - this->data[x][y][z];
                    tmp.y /= this->voxelsize.y;
                }
                else
                {
                    tmp.y  = this->data[x][y + 1][z] - this->data[x][y - 1][z];
                    tmp.y /= this->voxelsize.y * 2;
                }

                if( z == this->resolution.z - 1 )
                {
                    tmp.z  = this->data[x][y][z] - this->data[x][y][z - 1];
                    tmp.z /= this->voxelsize.z;
                }
                else if( z == 0 )
                {
                    tmp.z  = this->data[x][y][z + 1] - this->data[x][y][z];
                    tmp.z /= this->voxelsize.z;
                }
                else
                {
                    tmp.z  = this->data[x][y][z + 1] - this->data[x][y][z - 1];
                    tmp.z /= this->voxelsize.z * 2;
                }


                // tmp.y  = this->data[x][y][z] - this->data[x][y - 1][z];
                // tmp.y /= this->voxelsize.y;

                // tmp.z  = this->data[x][y][z] - this->data[x][y][z - 1];
                // tmp.z /= this->voxelsize.z;

                this->gradient[x][y][z] = tmp;

                this->max_gradient_len = max(max_gradient_len, glm::length(tmp));
            }
        }
    }
}

void calc_interpolation(glm::vec3 &vertex, glm::vec3 &vertex_gradient, glm::vec3 p1, glm::vec3 p2, float v1, float v2, glm::vec3 g1, glm::vec3 g2, glm::vec3 voxelsize, int iso_value)
{
    float ratio = (iso_value - v1) / (v2 - v1);

    p1 *= voxelsize;
    p2 *= voxelsize;
    g1 *= voxelsize;
    g2 *= voxelsize;

    vertex          = p1 + ratio * (p2 - p1);
    vertex_gradient = g1 + ratio * (g2 - g1);
}

void calc_intersection(glm::vec3 *vertex, glm::vec3 *vertex_gradient, glm::vec3 *grid, float *grid_value, glm::vec3 *grid_gradient, int edge, glm::vec3 voxelsize, int iso_value)
{
    int p1_table[] = { 0,1,2,3,4,5,6,7,0,1,2,3 };
    int p2_table[] = { 1,2,3,0,5,6,7,4,4,5,6,7 };

    for(int i = 0, now = 1 ; i < 12 ; i++, now <<= 1)
    {
        if(edge & now)
        {
            calc_interpolation(
                vertex[i],
                vertex_gradient[i],
                grid[p1_table[i]],
                grid[p2_table[i]],
                grid_value[p1_table[i]],
                grid_value[p2_table[i]],
                grid_gradient[p1_table[i]],
                grid_gradient[p2_table[i]],
                voxelsize,
                iso_value
            );
        }
    }
}

void Volume::calc_vertex(int iso_value)
{
    glm::vec3 grid[8];
    float grid_value[8];
    glm::vec3 grid_gradient[8];

    for(int x = 0; x < this->resolution.x - 1; x++)
    {
        for(int y = 0; y < this->resolution.y - 1; y++)
        {
            for(int z = 0; z < this->resolution.z - 1; z++)
            {
                grid[0] = glm::vec3(x    , y    , z    );
                grid[1] = glm::vec3(x    , y    , z + 1);
                grid[2] = glm::vec3(x    , y + 1, z + 1);
                grid[3] = glm::vec3(x    , y + 1, z    );
                grid[4] = glm::vec3(x + 1, y    , z    );
                grid[5] = glm::vec3(x + 1, y    , z + 1);
                grid[6] = glm::vec3(x + 1, y + 1, z + 1);
                grid[7] = glm::vec3(x + 1, y + 1, z    );

                int table_index = 0;

                for(int i = 0, tmp = 1; i < 8; ++i, tmp <<= 1)
                {
                    grid_value[i] = this->data[grid[i].x][grid[i].y][grid[i].z];
                    grid_gradient[i] = this->gradient[grid[i].x][grid[i].y][grid[i].z]; //

                    if(grid_value[i] < iso_value)
                        table_index |= tmp;
                }

                if(table_index == 0x0 || table_index == 0xff)
                    continue;

                int edge = edgeTable[table_index];

                glm::vec3 vertex[12];
                glm::vec3 vertex_gradient[12];

                calc_intersection(vertex, vertex_gradient, grid, grid_value, grid_gradient, edge, this->voxelsize, iso_value);

                for(int i = 0; triTable[table_index][i] != -1; i++)
                {
                    this->vertex.push_back(vertex[triTable[table_index][i]][0]);
                    this->vertex.push_back(vertex[triTable[table_index][i]][1]);
                    this->vertex.push_back(vertex[triTable[table_index][i]][2]);

                    this->vertex.push_back(vertex_gradient[triTable[table_index][i]][0]);
                    this->vertex.push_back(vertex_gradient[triTable[table_index][i]][1]);
                    this->vertex.push_back(vertex_gradient[triTable[table_index][i]][2]);
                }
            }
        }
    }

}

void Volume::calc_3dtexture()
{
    this->texture_data_3d = new unsigned char[this->resolution.x * this->resolution.y * this->resolution.z][4];

    for(int x = 0; x < this->resolution.x; x++)
    for(int y = 0; y < this->resolution.y; y++)
    for(int z = 0; z < this->resolution.z; z++)
    {
        glm::vec3 tmp = this->gradient[x][y][z];

        // this->texture_data_3d.push_back(data);

        this->texture_data_3d[x * this->resolution.y * this->resolution.z + y * this->resolution.z + z][0] = (tmp.r + 1) / 2 * 255;
        this->texture_data_3d[x * this->resolution.y * this->resolution.z + y * this->resolution.z + z][1] = (tmp.g + 1) / 2 * 255;
        this->texture_data_3d[x * this->resolution.y * this->resolution.z + y * this->resolution.z + z][2] = (tmp.b + 1) / 2 * 255;

        this->texture_data_3d[x * this->resolution.y * this->resolution.z + y * this->resolution.z + z][3] = (this->data[x][y][z] - this->min_value) / (this->max_value - this->min_value) * 255;
    }
}

void Volume::load_texture3d()
{
    glGenTextures(1, &texture_3d);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_3D, texture_3d);
    glTexImage3D(
        GL_TEXTURE_3D,
        0,
        GL_RGBA,
        this->resolution.x,
        this->resolution.y,
        this->resolution.z,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        this->texture_data_3d
    );
    // set texture options
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Volume::calc_1dtexture()
{
    int range = this->max_value - this->min_value;
    this->texture_data_1d = new unsigned char[range][4];

    for(int i = 0; i < range ; i++)
    {
        this->texture_data_1d[i][0] = 255;
        this->texture_data_1d[i][1] = 0;
        this->texture_data_1d[i][2] = 0;

        this->texture_data_1d[i][3] = 0.005 * 255;
    }
}

void Volume::load_texture1d()
{
    glGenTextures(1, &texture_1d);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_1D, texture_1d);
    glTexImage1D(
        GL_TEXTURE_1D,
        0,
        GL_RGBA,
        this->max_value - this->min_value,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        this->texture_data_1d
    );
    // set texture options
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Volume::calc_histogram()
{
    cout << "min_value: " << this->min_value << " max_value: " << this->max_value << endl;

    float ratio = 255.0;
    if (this->max_value - this->min_value >= 1e-6) ratio /= this->max_value - this->min_value;

    this->histogram.assign((int)(this->max_value - this->min_value) * ratio + 1, 0.0f);

    for(int x = 0; x < this->resolution.x; x++)
    {
        for(int y = 0; y < this->resolution.y; y++)
        {
            for(int z = 0; z < this->resolution.z; z++)
            {
                histogram[(int)(this->data[x][y][z] - this->min_value) * ratio]++;
            }
        }
    }

    this->histogram_max_value = *max_element(this->histogram.begin(), this->histogram.end());
}

void Volume::calc_mk_table(float g_max)
{
    g_max = this->max_gradient_len;

    this->mk_table.assign(256, vector<float>(20 * log2(g_max), 0.0f));

    // float value_range = this->max_value - this->min_value;
    float ratio = 255 / (this->max_value - this->min_value);
    int max_mk_table = 0;
    int m = 0, k = 0;

    for(size_t x = 0; x < this->data.size(); x++)
    {
        for(size_t y = 0; y < this->data[0].size(); y++)
        {
            for(size_t z = 0; z < this->data[0][0].size(); z++)
            {
                m = (this->data[x][y][z]-this->min_value) * ratio;

                float gradient_len = glm::length(this->gradient[x][y][z]);

                k = 20 * log2(gradient_len);

                k = glm::clamp(k, 0, (int) (20 * log2(g_max)) - 1);

                // if (m > 255)
                //     m = 255;
                // if (k > 159)
                //     k = 159;

                mk_table[m][k] += 1.0f;

                if(mk_table[m][k] > max_mk_table)
                    max_mk_table = mk_table[m][k];
            }
        }
    }

    for(size_t m = 0; m < mk_table.size(); m++)
    {
        for(size_t k = 0; k < mk_table[0].size(); k++)
        {
            mk_table[m][k] = 20 * log2(mk_table[m][k]);
        }
    }
}

void Volume::setup_volume_vao()
{
    float x = this->resolution.x;
    float y = this->resolution.y;
    float z = this->resolution.z;

        // // a
        // 0, 0, 0,
        // // b
        // x, 0, 0,
        // // c
        // x, 0, z,
        // // d
        // 0, 0, z,

        // // e
        // 0, y, 0,
        // // f
        // x, y, 0,
        // // g
        // x, y, z,
        // // h
        // 0, y, z,

    vector<float> volume_vertex = {
        // coord: x y z, texCoord: x, y, z
        0, 0, 0,   0, 0, 0,        0, 0, z,   0, 0, 1,        x, 0, z,   1, 0, 1,
        0, 0, 0,   0, 0, 0,        x, 0, z,   1, 0, 1,        x, 0, 0,   1, 0, 0,

        0, y, 0,   0, 1, 0,        x, y, 0,   1, 1, 0,        x, y, z,   1, 1, 1,
        0, y, 0,   0, 1, 0,        x, y, z,   1, 1, 1,        0, y, z,   0, 1, 1,

        0, 0, 0,   0, 0, 0,        x, 0, 0,   1, 0, 0,        x, y, 0,   1, 1, 0,
        0, 0, 0,   0, 0, 0,        x, y, 0,   1, 1, 0,        0, y, 0,   0, 1, 0,

        x, 0, 0,   1, 0, 0,        x, 0, z,   1, 0, 1,        x, y, z,   1, 1, 1,
        x, 0, 0,   1, 0, 0,        x, y, z,   1, 1, 1,        x, y, 0,   1, 1, 0,

        x, 0, z,   1, 0, 1,        0, 0, z,   0, 0, 1,        0, y, z,   0, 1, 1,
        x, 0, z,   1, 0, 1,        0, y, z,   0, 1, 1,        x, y, z,   1, 1, 1,

        0, 0, 0,   0, 0, 0,        0, y, 0,   0, 1, 0,        0, y, z,   0, 1, 1,
        0, 0, 0,   0, 0, 0,        0, y, z,   0, 1, 1,        0, 0, z,   0, 0, 1,
    };

    GLuint vbo;

    glGenVertexArrays(1, &(this->vao.id));
    glGenBuffers(1, &(vbo));

    glBindVertexArray(this->vao.id);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, volume_vertex.size() * sizeof(float), volume_vertex.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    this->vao.count = volume_vertex.size() / 6;

    volume_vertex.clear();

    glBindVertexArray(0);
}

void Volume::setup_vao()
{
    GLuint vbo;

    glGenVertexArrays(1, &(this->vao.id));
    glGenBuffers(1, &(vbo));

    glBindVertexArray(this->vao.id);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertex.size() * sizeof(this->vertex[0]), this->vertex.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    this->vao.count = vertex.size() / 3;

    vertex.clear();
    // gradient.clear();

    glBindVertexArray(0);
}

void Volume::draw()
{
    if(this->method == METHODS::ISO_SURFACE)
    {
        glDisable(GL_CULL_FACE);
    }
    else if(this->method == METHODS::VOLUME_RENDERING)
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_3D, this->texture_3d);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_1D, this->texture_1d);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);
    }

    glBindVertexArray(vao.id);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // glPolygonMode(GL_BACK, GL_LINE);

    glDrawArrays(GL_TRIANGLES, 0, this->vao.count);

    glBindVertexArray(0);
}