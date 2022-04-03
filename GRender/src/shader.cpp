#include <fstream>
#include <sstream>

#include "shader.h"

namespace fs = std::filesystem;

namespace GRender {

static bool CheckShaderError(int32_t shader, int32_t flag, bool isProgram, std::string msg) {
    int success = 0;
    char error[1024] = {0};

    if (isProgram)
        glad_glGetProgramiv(shader, flag, &success);
    else
        glad_glGetShaderiv(shader, flag, &success);

    if (success == GL_FALSE)
    {
        if (isProgram)
            glad_glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glad_glGetShaderInfoLog(shader, sizeof(error), NULL, error);

        ASSERT(false, msg + " :: " + std::string(error));
    } 

    return success;
}

static int32_t CreateShader(const char *shaderSource, GLenum shaderType) {
    int32_t shader = glad_glCreateShader(shaderType);
    ASSERT(shader != 0,"Failed to create shader!");

    int sourceLength = int(strlen(shaderSource));
    glShaderSource(shader, 1, &shaderSource, &sourceLength);

    std::string type = shaderType == GL_VERTEX_SHADER ? "GL_VERTEX_SHADER" : "GL_FRAGMENT_SHADER";
    std::string error = "Error: Shader compilation failed :: " + type + " = > ";

    glCompileShader(shader);

    if (CheckShaderError(shader, GL_COMPILE_STATUS, false, error))
        return shader;
    else
    {
        std::cerr << shaderSource << std::endl;
        return -1;
    }
}

static int32_t genShader(const fs::path &vertex_shader, const fs::path &frag_shader) {

    auto readShader = [](const fs::path &path) -> std::string {
        std::ifstream arq(path);
        std::stringstream shader;
        shader << arq.rdbuf();
        arq.close();
        shader << "\0";
        return shader.str();
    };

    int32_t vtx = CreateShader(readShader(vertex_shader).c_str(), GL_VERTEX_SHADER);
    int32_t frg = CreateShader(readShader(frag_shader).c_str(), GL_FRAGMENT_SHADER);

    if (vtx < 0 || frg < 0)
        return -1;

    // Create program
    int32_t program = glad_glCreateProgram();
    glad_glAttachShader(program, vtx);
    glad_glAttachShader(program, frg);

    // Link shaders to program
    std::string error = "ERROR: Cannot link shader programs => ";

    glad_glLinkProgram(program);
    if (!CheckShaderError(program, GL_LINK_STATUS, true, error))
        return -1;

    glad_glDeleteShader(vtx);
    glad_glDeleteShader(frg);

    return program;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
    
Shader::~Shader(void) {
    for (auto &it : vProgram)
        glad_glDeleteProgram(it.second);
}

bool Shader::loadShader(const std::string &label, const fs::path &vtxPath, const fs::path &frgPath) {
    ASSERT(fs::exists(vtxPath),"Shader not found! => " + vtxPath.string());
    ASSERT(fs::exists(frgPath), "Shader not found! => " + frgPath.string());

    int32_t code = genShader(vtxPath.string(), frgPath.string());
    ASSERT(code > 0, "Shader code was not generated!");

    vProgram[label] = code;
    return true;
}

void Shader::useProgram(const std::string &name) {
    auto it = vProgram.find(name);
    ASSERT(it != vProgram.end(), "Shader program not found! -> " + name);

    program_used = it->second;
    glUseProgram(program_used);
}

void Shader::setInteger(const std::string &name, int val) {
    int32_t loc = glad_glGetUniformLocation(program_used, name.c_str());
    glad_glUniform1i(loc, val);
}

void Shader::setFloat(const std::string &name, float val) {
    int32_t loc = glad_glGetUniformLocation(program_used, name.c_str());
    glad_glUniform1f(loc, val);
}

void Shader::setVec2f(const std::string &name, const float *v) {
    int32_t loc = glad_glGetUniformLocation(program_used, name.c_str());
    glad_glUniform2f(loc, v[0], v[1]);
}

void Shader::setVec3f(const std::string &name, const float *v) {
    int32_t loc = glad_glGetUniformLocation(program_used, name.c_str());
    glad_glUniform3f(loc, v[0], v[1], v[2]);
}

void Shader::setVec4f(const std::string &name, const float *v) {
    int32_t loc = glad_glGetUniformLocation(program_used, name.c_str());
    glad_glUniform4f(loc, v[0], v[1], v[2], v[3]);
}

void Shader::setMatrix3f(const std::string &name, const float *mat) {
    int32_t loc = glad_glGetUniformLocation(program_used, name.c_str());
    glad_glUniformMatrix3fv(loc, 1, GL_FALSE, mat);
}

void Shader::setMatrix4f(const std::string &name, const float *mat) {
    int32_t loc = glad_glGetUniformLocation(program_used, name.c_str());
    glad_glUniformMatrix4fv(loc, 1, GL_FALSE, mat);
}

void Shader::setIntArray(const std::string &name, const int *ptr, int32_t N) {
    int32_t loc = glad_glGetUniformLocation(program_used, name.c_str());
    glad_glUniform1iv(loc, N, ptr);
}

void Shader::setFloatArray(const std::string &name, const float *ptr, int32_t N) {
    int32_t loc = glad_glGetUniformLocation(program_used, name.c_str());
    glad_glUniform1fv(loc, N, ptr);
}

void Shader::setVec2fArray(const std::string &name, const float *ptr, int32_t N) {
    int32_t loc = glad_glGetUniformLocation(program_used, name.c_str());
    glad_glUniform2fv(loc, N, ptr);
}

void Shader::setVec3fArray(const std::string &name, const float *ptr, int32_t N) {
    int32_t loc = glad_glGetUniformLocation(program_used, name.c_str());
    glad_glUniform3fv(loc, N, ptr);
}

void Shader::setMat3Array(const std::string &name, const float *ptr, int32_t N) {
    int32_t loc = glad_glGetUniformLocation(program_used, name.c_str());
    glad_glUniformMatrix3fv(loc, N, true, ptr);
}

} // namespace GRender