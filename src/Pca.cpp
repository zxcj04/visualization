#include <Pca.hpp>

MatrixXd generate_test_data()
{
    MatrixXd ret(30, 9);

    ret << 90, 50, 55, 90, 90, 72, 56, 57, 62,
           87, 77, 90, 87, 90, 43, 37, 42, 52,
           90, 50, 0, 0, 0, 49, 47, 63, 13,
           90, 94, 90, 100, 87, 58, 75, 53, 50,
           90, 67, 45, 77, 0, 45, 64, 32, 32,
           70, 50, 0, 0, 0, 62, 57, 22, 38,
           95, 87, 90, 90, 92, 49, 50, 66, 55,
           95, 72, 85, 87, 90, 64, 73, 60, 68,
           95, 90, 60, 92, 87, 0, 0, 79, 38,
           88, 69, 90, 90, 87, 88, 96, 75, 64,
           10, 30, 50, 85, 90, 18, 49, 35, 28,
           0, 0, 0, 0, 0, 19, 27, 31, 18,
           94, 100, 90, 90, 90, 88, 60, 60, 70,
           92, 78, 90, 87, 87, 79, 70, 35, 71,
           96, 99, 90, 90, 90, 76, 64, 71, 76,
           100, 100, 90, 95, 100, 88, 90, 81, 90,
           8, 0, 0, 0, 0, 22, 0, 14, 12,
           87, 64, 60, 77, 90, 75, 95, 63, 78,
           87, 30, 40, 65, 85, 81, 60, 38, 77,
           0, 0, 0, 0, 0, 31, 31, 9, 0,
           93, 72, 90, 90, 90, 64, 44, 42, 29,
           87, 70, 60, 77, 87, 69, 49, 47, 58,
           88, 67, 70, 0, 90, 51, 37, 36, 35,
           88, 80, 85, 87, 87, 66, 46, 36, 38,
           70, 35, 0, 0, 45, 53, 39, 45, 29,
           10, 0, 35, 50, 67, 34, 37, 25, 56,
           60, 50, 55, 45, 60, 55, 50, 60, 60,
           61, 45, 50, 50, 55, 60, 50, 58, 61,
           10, 10, 0, 0, 0, 34, 45, 22, 40,
           30, 40, 40, 10, 70, 70, 60, 60, 70 ;

    return ret;
}

Pca::Pca(float threshold)
{
    this->data = generate_test_data();
    normalize_data();

    calc_vertex(threshold);

    setup_vao();

    calc_1dtexture();

    load_texture1d();
}

void Pca::reload(float threshold)
{
    calc_vertex(threshold);

    setup_vao();
}

void Pca::normalize_data()
{
    double data_max = this->data.maxCoeff();
    double data_min = this->data.minCoeff();

    for(int i = 0 ; i < this->data.cols() ; i++)
    {
        double col_max = this->data.col(i).maxCoeff();
        double col_min = this->data.col(i).minCoeff();

        this->data.col(i) = (this->data.col(i) - Eigen::VectorXd::Ones(this->data.rows()) * col_min) / (col_max - col_min);
        this->data.col(i) = this->data.col(i) * (data_max - data_min) + Eigen::VectorXd::Ones(this->data.rows()) * data_min;

        double mean = this->data.col(i).mean();

        this->data.col(i) -= Eigen::VectorXd::Ones(this->data.rows()) * mean;
    }
}

void Pca::calc_vertex(float threshold)
{
    vertex.clear();

    vector<int> group;
    vector<int> exist_groups;

    vector<float> result_class;

    for(int i = 0 ; i < this->data.rows() ; i++)
        group.push_back(i);

    for(int i = 0 ; i < this->data.rows() ; i++)
        for(int j = i + 1 ; j < this->data.rows() ; j++)
        {
            auto tmp = this->data.row(i) - this->data.row(j);

            if(tmp.squaredNorm() < threshold)
                for(auto &g: group)
                    if(g == j)
                        g = i;
        }

    for(auto now: group)
    {
        int index;

        for(index = 0 ; index < exist_groups.size() ; index++)
        {
            if(now == exist_groups[index])
                break;
            else if(now < exist_groups[index])
            {
                exist_groups.insert(exist_groups.begin() + index, now);

                break;
            }
        }

        if(index == exist_groups.size())
            exist_groups.push_back(now);
    }

    for(auto c: group)
        cout << c << " ";
    cout << endl;

    for(auto g: group)
        for(int i = 0 ; i < exist_groups.size() ; i++)
            if(exist_groups[i] == g)
                result_class.push_back((float) i / exist_groups.size());

    MatrixXd A = this->data.transpose() * this->data;

    A /= this->data.rows();

    Eigen::EigenSolver<Eigen::MatrixXd> eigensolver;
    eigensolver.compute(A);
    Eigen::VectorXd eigen_values = eigensolver.eigenvalues().real();
    Eigen::MatrixXd eigen_vectors = eigensolver.eigenvectors().real();

    double value1 = 0, value2 = 0;

    Eigen::VectorXd V1, V2;

    for(int i = 0 ; i < eigen_values.size() ; i++)
    {
        if(fabs(eigen_values[i]) > fabs(value1))
        {
            value2 = value1;
            V2 = V1;

            value1 = eigen_values[i];
            V1 = eigen_vectors.row(i);
        }
        else if(fabs(eigen_values[i]) > fabs(value2))
        {
            value2 = eigen_values[i];
            V2 = eigen_vectors.row(i);
        }
    }

    for(int i = 0 ; i < this->data.rows() ; i++)
    {
        double x = this->data.row(i) * V1;
        double y = this->data.row(i) * V2;

        this->vertex.push_back(x);
        this->vertex.push_back(y);
        this->vertex.push_back(0.0);
        this->vertex.push_back(result_class[i]);
    }
}

void Pca::setup_vao()
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

void Pca::calc_1dtexture()
{
    this->texture_data_1d = new unsigned char[256][4];

    for(int i = 0; i < 256; i++)
    {
        if(i > 245)
        {
            this->texture_data_1d[i][0] = 0;
            this->texture_data_1d[i][1] = 0;
            this->texture_data_1d[i][2] = 0;
            this->texture_data_1d[i][3] = 255;
        }
        else if(i < 96)
        {
            this->texture_data_1d[i][0] = 255;
            this->texture_data_1d[i][1] = i * 2.6;
            this->texture_data_1d[i][2] = 0;
            this->texture_data_1d[i][3] = 255;
        }
        else if(i < 128)
        {
            this->texture_data_1d[i][0] = 255 - (i-96) * 8;
            this->texture_data_1d[i][1] = 255;
            this->texture_data_1d[i][2] = 0;
            this->texture_data_1d[i][3] = 255;
        }
        else if(i < 192)
        {
            this->texture_data_1d[i][0] = 0;
            this->texture_data_1d[i][1] = 255 - (i-128) * 4;
            this->texture_data_1d[i][2] = (i-128) * 4;
            this->texture_data_1d[i][3] = 255;
        }
        else
        {
            this->texture_data_1d[i][0] = (i-192) * 4;
            this->texture_data_1d[i][1] = 0;
            this->texture_data_1d[i][2] = 255;
            this->texture_data_1d[i][3] = 255;
        }
    }
}

void Pca::load_texture1d()
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

void Pca::draw()
{
    glBindVertexArray(vao.id);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_1D, this->texture_1d);

    glDrawArrays(GL_POINTS, 0, this->vao.count);

    glBindVertexArray(0);
}

int Pca::vao_count()
{
    return this->vao.count;
}

Pca::~Pca()
{

}