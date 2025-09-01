//
// Created by jeanhua on 2025/8/16.
//

#include "shader.h"
#include <fstream>
#include <sstream>
#include <filesystem>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
	if (!std::filesystem::exists(vertexPath)) {
		std::cerr << "File does not exist: " << vertexPath << std::endl;
	}
	if (!std::filesystem::exists(fragmentPath)) {
		std::cerr << "File does not exist: " << fragmentPath << std::endl;
	}
	this->mProgram = 0;
	std::string vertexCode, fragmentCode;
	std::ifstream vShaderFile, fShaderFile;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e) {
		std::cout << "ERROR: Shader File Error: " << e.what() << std::endl;
	}

	GLuint vertexShader, fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
	glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);

	glCompileShader(vertexShader);
	checkError(vertexShader, CheckType::compile);

	glCompileShader(fragmentShader);
	checkError(fragmentShader, CheckType::compile);

	this->mProgram = glCreateProgram();
	glAttachShader(this->mProgram, vertexShader);
	glAttachShader(this->mProgram, fragmentShader);

	glLinkProgram(this->mProgram);
	checkError(this->mProgram, CheckType::link);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}
Shader::~Shader() {
	if (mProgram != 0)glDeleteProgram(mProgram);
}

void Shader::begin() const {
	glUseProgram(this->mProgram);
}

void Shader::end() {
	glUseProgram(0);
}

GLuint Shader::getAttribPos(const GLchar* name) const
{
	return glGetAttribLocation(this->mProgram, name);
}

GLuint Shader::getUniformPos(const GLchar* name) const
{
	GLuint loc = glGetUniformLocation(this->mProgram, name);
	if (loc == -1) {
		std::cout<<"can not get uniform location: "<<std::string(name)<<std::endl;
	}
	return loc;
}

GLuint Shader::getShaderProgram() const
{
	return mProgram;
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	const GLfloat v[] = { x,y,z };
	glUseProgram(mProgram);
	glUniform3fv(static_cast<int>(getUniformPos(name.c_str())),1,v);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
	const GLfloat v[] = { value.x,value.y,value.z };
	glUseProgram(mProgram);
	glUniform3fv(static_cast<int>(getUniformPos(name.c_str())),1,v);
}


void Shader::setInt(const std::string& name, int value) const
{
	glUseProgram(mProgram);
	glUniform1i(static_cast<int>(getUniformPos(name.c_str())), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUseProgram(mProgram);
	glUniform1f(static_cast<int>(getUniformPos(name.c_str())), value);
}

void Shader::setMat4(const std::string& name, glm::mat4 value) const
{
	glUseProgram(mProgram);
	glUniformMatrix4fv(static_cast<int>(getUniformPos(name.c_str())), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setBool(const std::string &name, bool value) const {
	setInt(name,value?1:0);
}


void Shader::checkError(GLuint target, CheckType type) const
{
	int  success;
	char infoLog[512];
	if (type == CheckType::link) {
		glGetProgramiv(this->mProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(this->mProgram, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::LINK_FAILED\n" << infoLog << std::endl;
			return;
		}
	}else if (type == CheckType::compile) {
		glGetShaderiv(target, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(target, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			return;
		}
	}
}
