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

bool WindowManagement::texture_init()
{
    // load_image(sand, "./assets/sand.bmp", 256);
    // load_image(seaweed, "./assets/seaweed.bmp", 256);
    // load_image(bubble, "./assets/bubble.bmp", 256);

    glGenTextures(5, textName);

    // make_check_pattern();

    // glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
    // glBindTexture(GL_TEXTURE_2D, textName[0]);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA,
    //              GL_UNSIGNED_BYTE, checkboard);

    // make_brick_pattern();

    // glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
    // glBindTexture(GL_TEXTURE_2D, textName[1]);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA,
    //              GL_UNSIGNED_BYTE, brick);

    // glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
    // glBindTexture(GL_TEXTURE_2D, textName[2]);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA,
    //              GL_UNSIGNED_BYTE, sand);

    // glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
    // glBindTexture(GL_TEXTURE_2D, textName[3]);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA,
    //              GL_UNSIGNED_BYTE, seaweed);

    // glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
    // glBindTexture(GL_TEXTURE_2D, textName[4]);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA,
    //              GL_UNSIGNED_BYTE, bubble);

    // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    return true;
}