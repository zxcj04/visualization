#include "WindowManagement.hpp"

template <typename T>
void WindowManagement::load_image(T &image, string filename, int width, int height)
{
    if(height == 0)
        height = width;

    ifstream file(filename.c_str(), ios::in | ios::binary);

    unsigned char ch;

    // unsigned char ***image = new unsigned char**[256];

    // for(int i = 0; i < 256; i++)
    // {
    //     image[i] = new unsigned char*[256];

    //     for(int j = 0; j < 256; j++)
    //         image[i][j] = new unsigned char[4];
    // }

    for(int i = 0; i < 138; i++)
        file.get();

    for(int i = 0; i < 256*256*4 ; i++)
    {
        ch = file.get();

        int k =  i % 4;

        if(k == 0)
            k = 2;
        else if(k == 2)
            k = 0;

        // cout << (int) ch << " ";

        // if(k == 3)
        //     cout << endl;

        image[i / 4 / 256][i / 4 % 256][k] = ch;
    }

    file.close();
}

void WindowManagement::make_check_pattern()
{
    int i, j, c;

    for (i = 0; i < TSIZE; i++)
        for (j = 0; j < TSIZE; j++)
        {
            c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;
            checkboard[i][j][0] = c;
            checkboard[i][j][1] = c;
            checkboard[i][j][2] = c;
            checkboard[i][j][3] = 255;
        }
}

/*----------------------------------------------------------
 * Procedure to maKe a brick wall texture map
 */
void WindowManagement::make_brick_pattern()
{
    int y, x, i;

    for (y = 0; y < 12; y++)
    { /*----Create the first row of bricks----*/
        for (x = 0; x < 2; x++)
        {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

        for (x = 2; x < 30; x++)
        {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }

        for (x = 30; x < 34; x++)
        {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

        for (x = 34; x < 62; x++)
        {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }

        for (x = 62; x < 64; x++)
        {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }
    }

    for (y = 12; y < 16; y++) /*--Create the cement between two breick layers */
        for (x = 0; x < 64; x++)
        {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

    for (y = 16; y < 28; y++)
    { /*--Create the 2nd layer of bricks---*/
        for (x = 0; x < 14; x++)
        {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }

        for (x = 14; x < 18; x++)
        {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

        for (x = 18; x < 46; x++)
        {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }

        for (x = 46; x < 50; x++)
        {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

        for (x = 50; x < 64; x++)
        {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }
    }

    for (y = 28; y < 32; y++) /*---Add another layer of cement----*/
        for (x = 0; x < 64; x++)
        {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

    for (y = 32; y < 64; y++)
        for (x = 0; x < 64; x++)
            for (i = 0; i < 4; i++)
                brick[y][x][i] = brick[y - 32][x][i];
}

bool WindowManagement::texture_init()
{
    load_image(sand, "./assets/sand.bmp", 256);
    load_image(seaweed, "./assets/seaweed.bmp", 256);
    load_image(bubble, "./assets/bubble.bmp", 256);

    glGenTextures(5, textName);

    make_check_pattern();

    glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
    glBindTexture(GL_TEXTURE_2D, textName[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, checkboard);

    make_brick_pattern();

    glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
    glBindTexture(GL_TEXTURE_2D, textName[1]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, brick);

    glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
    glBindTexture(GL_TEXTURE_2D, textName[2]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, sand);

    glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
    glBindTexture(GL_TEXTURE_2D, textName[3]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, seaweed);

    glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
    glBindTexture(GL_TEXTURE_2D, textName[4]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, bubble);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    return true;
}