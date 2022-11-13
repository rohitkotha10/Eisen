#include "shader.h"

using namespace std;

namespace Eisen {
    string parse(string shaderPath) {
        ifstream ifs(shaderPath);

        string final;

        int cnt = 0;
        while (!ifs.eof()) {
            string temp;
            getline(ifs, temp);
            final += temp + '\n';
        }
        return final;
    };

    void setInt(GLuint program, string name, int value) {
        glUniform1i(glGetUniformLocation(program, name.c_str()), value);
    }
    void setFloat(GLuint program, string name, float value) {
        glUniform1f(glGetUniformLocation(program, name.c_str()), value);
    }
    void setMat4(GLuint program, string name, const glm::mat4& data) {
        glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &data[0][0]);
    }
    void setVec3(GLuint program, string name, float x, float y, float z) {
        glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);
    }

    void setVec3(GLuint program, string name, const glm::vec3& data) {
        glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &data[0]);
    }
    void setVec4(GLuint program, string name, float x, float y, float z, float a) {
        glUniform4f(glGetUniformLocation(program, name.c_str()), x, y, z, a);
    }

    void setVec4(GLuint program, string name, const glm::vec4& data) {
        glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, &data[0]);
    }

    void setIntArray(GLuint program, string name, int size, int* data) {
        glUniform1iv(glGetUniformLocation(program, name.c_str()), size, data);
    }

}  // namespace Eisen
