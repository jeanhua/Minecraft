#pragma once
#include "../../core.h"

class Shader {
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	void begin();
	void end();

public:
	GLuint getAttribPos(const GLchar* name);
	GLuint getUniformPos(const GLchar* name) const;

	GLuint getShaderProgram()const;

	void setVec3(const std::string& name, float x, float y, float z);
	void setInt(const std::string& name, int value);
	void setFloat(const std::string& name, float value);
	void setMat4(const std::string& name, glm::mat4 value);

private:
	enum CheckType
	{
		link, compile
	};
	void checkError(GLuint target, CheckType type);
private:
	GLuint mProgram;
};