#include <Streamline.hpp>

Streamline::Streamline(string vec_filename, double gap, int iter_times, double h, double limit)
{
    this->vec_filename = vec_filename;

    this->value_max = 0;

    read_vec();

    calc_vertex(gap, iter_times, h, limit);

    calc_1dtexture();
    load_texture1d();

    setup_vao();
}

Streamline::~Streamline()
{

}

glm::dvec2 custom_f(double x, double y)
{
    return glm::dvec2( x * y * y , x * x * y );
}

void Streamline::read_vec()
{
    cout << this->vec_filename << endl;

    if(this->vec_filename == "./Data/Vector/custom.vec")
    {
        this->width = 250;
        this->height = 250;

        data.assign(this->width, vector<glm::dvec2>(this->height, glm::dvec2(0, 0)));

        for(int w = 0 ; w < this->width ; w++)
        for(int h = 0 ; h < this->height ; h++)
        {
            data[w][h] = custom_f(w / 25.0 - 5, h / 25.0 - 5);
        }

        cout << "read" << endl;

        return;
    }

    fstream vec;
    string inp;

    stringstream ss;

    vec.open(this->vec_filename, fstream::in);

    getline(vec, inp);

    ss << inp;

    ss >> this->width >> this->height;

    data.assign(this->width, vector<glm::dvec2>(this->height, glm::dvec2(0, 0)));

    for(int w = 0 ; w < this->width ; w++)
    for(int h = 0 ; h < this->height ; h++)
    {
        double x, y;

        ss >> x >> y;

        data[w][h] = glm::dvec2(x, y);
    }
}

glm::dvec2 Streamline::interpolate_data(double x, double y)
{
    double dx = x - (int)x;
    double dy = y - (int)y;

    int tx = x;
    int ty = y;

    double rx = data[tx][ty].x * (1 - dx)*(1 - dy) + data[tx + 1][ty].x * dx*(1 - dy) + data[tx][ty + 1].x * (1 - dx)*dy + data[tx + 1][ty + 1].x * dx*dy;
    double ry = data[tx][ty].y * (1 - dx)*(1 - dy) + data[tx + 1][ty].y * dx*(1 - dy) + data[tx][ty + 1].y * (1 - dx)*dy + data[tx + 1][ty + 1].y * dx*dy;

    return glm::dvec2(rx, ry);
}

void Streamline::calc_vertex(double gap, int iter_times, double h, double limit)
{
    double limit_gap = (1 / limit);

    set<pair<double, double>> points;

    occupy.assign(this->width * limit_gap + 1, vector<bool>(this->height * limit_gap + 1, false));

    vertex.clear();

    for(double x = 0 ; x < this->width ; x+=gap)
    for(double y = 0 ; y < this->height ; y+=gap)
    {
        glm::dvec2 pos(x, y);

        points.clear();

        points.insert(make_pair(pos.x * limit_gap, pos.y * limit_gap));

        int cnt = 0;

        for(int i = 0 ; i < iter_times ; i++)
        {
            if(pos.x < 0 || pos.x > this->width - 2 ||
               pos.y < 0 || pos.y > this->height - 2)
                break;

            if(occupy[pos.x * limit_gap][pos.y * limit_gap])
                break;

            glm::dvec2 k1 = interpolate_data(pos.x, pos.y);

            glm::dvec2 next_pos = pos + h * glm::normalize(k1);

            if(next_pos.x < 0 || next_pos.x > this->width - 2 ||
               next_pos.y < 0 || next_pos.y > this->height - 2)
                break;

            glm::dvec2 k2 = interpolate_data(next_pos.x, next_pos.y);

            glm::dvec2 result_pos = pos + h * ( glm::normalize(k1) + glm::normalize(k2) ) / 2.0;

            if(result_pos.x < 0 || result_pos.x > this->width - 2 ||
               result_pos.y < 0 || result_pos.y > this->height - 2)
                break;

            points.insert(make_pair(result_pos.x * limit_gap, result_pos.y * limit_gap));

            vertex.push_back(-pos.x);
            vertex.push_back(pos.y);
            vertex.push_back(glm::length(k1));
            vertex.push_back((double)cnt);

            vertex.push_back(-result_pos.x);
            vertex.push_back(result_pos.y);
            vertex.push_back(glm::length(k1));
            vertex.push_back((double)cnt);

            value_max = max(glm::length(k1), value_max);

            cnt++;

            pos = result_pos;
        }

        if(cnt > 50)
        {
            for(int i = 1 ; i <= cnt * 2 ; i++)
            {
                double origin = vertex[vertex.size() - 1 - (i - 1) * 4];

                vertex[vertex.size() - 1 - (i - 1) * 4] = origin / cnt * 3 + 1;
            }

            for(auto point: points)
                occupy[point.first][point.second] = true;
        }
        else
        {
            for(int i = 0 ; i < cnt * 2 * 4 ; i++)
                vertex.pop_back();
        }
    }

}

void Streamline::setup_vao()
{
    GLuint vbo;

    glGenVertexArrays(1, &(this->vao.id));
    glGenBuffers(1, &(vbo));

    glBindVertexArray(this->vao.id);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertex.size() * sizeof(this->vertex[0]), this->vertex.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 4 * sizeof(double), (GLvoid*)(0 * sizeof(double)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 1, GL_DOUBLE, GL_FALSE, 4 * sizeof(double), (GLvoid*)(2 * sizeof(double)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 1, GL_DOUBLE, GL_FALSE, 4 * sizeof(double), (GLvoid*)(3 * sizeof(double)));
    glEnableVertexAttribArray(2);

    this->vao.count = vertex.size() / 4;

    vertex.clear();

    glBindVertexArray(0);
}

int Streamline::vao_count()
{
    return this->vao.count;
}

glm::vec3 Streamline::resolution()
{
    return glm::vec3(-this->width, this->height, 0.0f);
}

void Streamline::draw()
{
    glBindVertexArray(vao.id);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_1D, this->texture_1d);

    glDrawArrays(GL_LINES, 0, this->vao.count);

    glBindVertexArray(0);
}

void Streamline::reload(double gap, int iter_times, double h, double limit)
{
    calc_vertex(gap, iter_times, h, limit);

    setup_vao();
}

double Streamline::max_value()
{
    return this->value_max;
}

void Streamline::calc_1dtexture()
{
    this->texture_data_1d = new unsigned char[256][4];

    for(int i = 0; i < 256 ; i++)
    {
        this->texture_data_1d[i][0] = 0;
        this->texture_data_1d[i][1] = 0;
        this->texture_data_1d[i][2] = 0;
        this->texture_data_1d[i][3] = 255;
    }
}

void Streamline::load_texture1d()
{
    glGenTextures(1, &texture_1d);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_1D, texture_1d);
    glTexImage1D(
        GL_TEXTURE_1D,
        0,
        GL_RGBA,
        256,
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

unsigned int retrieve_color_value(ImVector<ImVec2> polyline, int x)
{
    if(x == 255)
        x = x * 2 - 1;
    else if(x == 0)
        x = x * 2 + 1;
    else
        x *= 2;

    if(polyline.size() == 0)
        return 0;

    ImVec2 *target;

    for(target = polyline.begin(); target != polyline.end(); ++target)
        if(target->x > x)
            break;
        else if(target->x == x)
            return target->y;

    if(target == polyline.end())
        return 0;
    else if(target == polyline.begin() && target->x > x)
        return 0;

    float alpha = x - (target - 1)->x;
    float beta = target->x - x;

    return 255 - (((target - 1)->y * beta) + (target->y * alpha)) / (alpha + beta);
}

void Streamline::reload_1d_texture(vector<ImVector<ImVec2>> rgba_polylines)
{
    delete this->texture_data_1d;
    this->texture_data_1d = new unsigned char[256][4];

    for(int i = 0; i < 256 ; i++)
    {
        this->texture_data_1d[i][0] = retrieve_color_value(rgba_polylines[0], i);
        this->texture_data_1d[i][1] = retrieve_color_value(rgba_polylines[1], i);
        this->texture_data_1d[i][2] = retrieve_color_value(rgba_polylines[2], i);
        this->texture_data_1d[i][3] = retrieve_color_value(rgba_polylines[3], i);
    }

    glDeleteTextures(1, &this->texture_1d);

    load_texture1d();
}