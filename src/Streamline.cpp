#include <Streamline.hpp>

Streamline::Streamline(string vec_filename, double gap, int iter_times, double h, double limit)
{
    this->vec_filename = vec_filename;

    read_vec();

    calc_vertex(gap, iter_times, h, limit);

    setup_vao();
}

Streamline::~Streamline()
{

}

void Streamline::read_vec()
{
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

            glm::dvec2 next_pos = pos + h * glm::normalize(interpolate_data(pos.x, pos.y));

            if(next_pos.x < 0 || next_pos.x > this->width - 2 ||
               next_pos.y < 0 || next_pos.y > this->height - 2)
                break;

            glm::dvec2 result_pos = pos + h * ( glm::normalize(interpolate_data(pos.x, pos.y)) + glm::normalize(interpolate_data(next_pos.x, next_pos.y)) ) / 2.0;

            if(result_pos.x < 0 || result_pos.x > this->width - 2 ||
               result_pos.y < 0 || result_pos.y > this->height - 2)
                break;

            points.insert(make_pair(result_pos.x * limit_gap, result_pos.y * limit_gap));

            vertex.push_back(-pos.x);
            vertex.push_back(pos.y);
            vertex.push_back(0.0);
            vertex.push_back((double)cnt);

            vertex.push_back(-result_pos.x);
            vertex.push_back(result_pos.y);
            vertex.push_back(0.0);
            vertex.push_back((double)cnt);

            cnt++;

            pos = result_pos;
        }

        for(int i = 1 ; i <= cnt * 2 ; i++)
        {
            double origin = vertex[vertex.size() - 1 - (i - 1) * 4];

            vertex[vertex.size() - 1 - (i - 1) * 4] = origin / cnt * 3 + 1;
        }

        for(auto point: points)
            occupy[point.first][point.second] = true;
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

    glVertexAttribPointer(1, 1, GL_DOUBLE, GL_FALSE, 4 * sizeof(double), (GLvoid*)(3 * sizeof(double)));
    glEnableVertexAttribArray(1);

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

    glDrawArrays(GL_LINES, 0, this->vao.count);

    glBindVertexArray(0);
}

void Streamline::reload(double gap, int iter_times, double h, double limit)
{
    calc_vertex(gap, iter_times, h, limit);

    setup_vao();
}