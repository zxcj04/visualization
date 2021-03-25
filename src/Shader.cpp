/*  Usage:

    Shader ourShader("path/to/shaders/shader.vs", "path/to/shaders/shader.fs");
    ...
    while(...)
    {
        ourShader.use();
        ourShader.setFloat("someUniform", 1.0f);
        DrawStuff();
    }
*/

#include "Shader.hpp"
#include <string>
#include <iostream>
using namespace std;

Shader::Shader()
{

}

Shader::Shader(string vertexPath, string fragmentPath)
{
    this->_vert = vertexPath;
    this->_frag = fragmentPath;

    this->load(vertexPath, fragmentPath, true);
}

void Shader::load(string vertexPath, string fragmentPath, bool init)
{
    // 1. read file
    string vertexCode;
    string fragmentCode;
    ifstream vShaderFile;
    ifstream fShaderFile;

    vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

    try{
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();

    }
    catch(const ifstream::failure &e){
        cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READED" << endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();


    // 2. compile shaders
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << infoLog << endl;
    }
    else{
        cout << "SUCCESS::SHADER::VERTEX::COMPILATION" << endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << infoLog << endl;
    }
    else{
        cout << "SUCCESS::SHADER::FRAGMENT::COMPILATION" << endl;
    }

    // shader program
    if(init)
    {
        this->ID = glCreateProgram();
    }
    else
    {
        glDetachShader(ID, _vertex);
        glDetachShader(ID, _fragment);
    }

    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    // cc@adkevin3307
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED: " << infoLog << endl;
    }
    else{
        cout << "SUCCESS::SHADER::PROGRAM::LINKING" << endl;
    }

    _vertex = vertex;
    _fragment = fragment;
    // delete the shader as they-re linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::reload()
{
    this->load(_vert, _frag);
}

void Shader::use()
{
    glUseProgram(this->ID);
}

void Shader::set_uniform(const string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
}

void Shader::set_uniform(const string &name, int value) const
{
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::set_uniform(const string &name, float value) const
{
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::set_uniform(const string &name, glm::vec3 value) const
{
    glUniform3fv(glGetUniformLocation(this->ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::set_uniform(const string &name, glm::vec4 value) const
{
    glUniform4fv(glGetUniformLocation(this->ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::set_uniform(const string &name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
