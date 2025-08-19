//
// Created by jeanhua on 2025/8/16.
//

#ifndef MINECRAFT_SHADER_H
#define MINECRAFT_SHADER_H
#include "../../core.h"

class Shader {
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	void begin() const;
	static void end();

public:
	GLuint getAttribPos(const GLchar* name) const;
	GLuint getUniformPos(const GLchar* name) const;

	[[nodiscard]] GLuint getShaderProgram()const;

	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec3(const std::string& name,const glm::vec3& value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4 value) const;

private:
	enum CheckType
	{
		link, compile
	};
	void checkError(GLuint target, CheckType type) const;
private:
	GLuint mProgram;
};

#endif