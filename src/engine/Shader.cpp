#include "Shader.hpp"

#include <cstdio>
#include <format>
#include <string>

#include "utils/clrp.hpp"
#include "glm/gtc/type_ptr.hpp"

fspath Shader::directory = "shaders";

Shader::Shader() {}

Shader::Shader(const fspath& vsPath, const fspath& fsPath, const fspath& gsPath) {
  program = glCreateProgram();
  GLuint shaders[3];
  u8 idx = 0;

  shaders[idx++] = compile(vsPath, GL_VERTEX_SHADER);
  shaders[idx++] = compile(fsPath, GL_FRAGMENT_SHADER);

  if (!gsPath.empty()) shaders[idx++] = compile(gsPath, GL_GEOMETRY_SHADER);

  for (int i = 0; i < idx; i++)
    glAttachShader(program, shaders[i]);

  if (!link(program))
    printLinkError(vsPath, fsPath, gsPath);

  for (int i = 0; i < idx; i++)
    glDeleteShader(shaders[i]);
}

Shader::Shader(const fspath& compPath) {
  program = glCreateProgram();
  GLuint shader = compile(compPath, GL_COMPUTE_SHADER);
  glAttachShader(program, shader);

  if (!link(program))
    printLinkError(compPath, "", "");

  glDeleteShader(shader);
}

void Shader::clear() {
  glDeleteProgram(program);
}

void Shader::setDirectoryLocation(const fspath& path) { Shader::directory = path; }

GLint Shader::getUniformLoc(const std::string& name) const {
  use();
  return glGetUniformLocation(program, name.c_str());
}

void Shader::use() const { glUseProgram(program); }

void Shader::setUniform1f(const GLint& loc, const GLfloat& n)    const { use(); glUniform1f(loc, n); }
void Shader::setUniform2f(const GLint& loc, const vec2& v)       const { use(); glUniform2f(loc, v.x, v.y); }
void Shader::setUniform3f(const GLint& loc, const vec3& v)       const { use(); glUniform3f(loc, v.x, v.y, v.z); }
void Shader::setUniform4f(const GLint& loc, const vec4& v)       const { use(); glUniform4f(loc, v.x, v.y, v.z, v.w); }
void Shader::setUniform1i(const GLint& loc, const GLint& v)      const { use(); glUniform1i(loc, v); }
void Shader::setUniform1ui(const GLint& loc, const GLuint& v)    const { use(); glUniform1ui(loc, v); }
void Shader::setUniform2i(const GLint& loc, const ivec2& v)      const { use(); glUniform2i(loc, v.x, v.y); }
void Shader::setUniformMatrix4f(const GLint& loc, const mat4& m) const { use(); glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(m)); }

void Shader::setUniform1f(const std::string& name, const GLfloat& n)    const { setUniform1f(getUniformLoc(name), n); }
void Shader::setUniform2f(const std::string& name, const vec2& v)       const { setUniform2f(getUniformLoc(name), v); }
void Shader::setUniform3f(const std::string& name, const vec3& v)       const { setUniform3f(getUniformLoc(name), v); }
void Shader::setUniform4f(const std::string& name, const vec4& v)       const { setUniform4f(getUniformLoc(name), v); }
void Shader::setUniform1i(const std::string& name, const GLint& v)      const { setUniform1i(getUniformLoc(name), v); }
void Shader::setUniform1ui(const std::string& name, const GLuint& v)    const { setUniform1ui(getUniformLoc(name), v); }
void Shader::setUniform2i(const std::string& name, const ivec2& v)      const { setUniform2i(getUniformLoc(name), v); }
void Shader::setUniformMatrix4f(const std::string& name, const mat4& m) const { setUniformMatrix4f(getUniformLoc(name), m); }

void Shader::setUniformTexture(const std::string& name, GLuint unit) const {
  use();
  const GLint loc = getUniformLoc(name);
  glUniform1i(loc, unit);
}

void Shader::setUniformTexture(const GLint& loc, const Texture& texture) const {
  use();
  glUniform1i(loc, texture.getUnit());
}

void Shader::setUniformTexture(const Texture& texture) const {
  const GLint loc = getUniformLoc(texture.getUniformName());
  setUniformTexture(loc, texture);
}

GLuint Shader::load(fspath path, int type) {
  path = directory.empty() ? path : directory / path;
  std::string shaderStr = readFile(path);
  const char* shaderStrPtr = shaderStr.c_str();
  GLuint shaderId = glCreateShader(type);
  glShaderSource(shaderId, 1, &shaderStrPtr, NULL);

  return shaderId;
}

GLuint Shader::compile(const fspath& path, int type) {
  GLuint shaderId = load(path, type);
  GLint hasCompiled;
  char infoLog[1'024];

  glCompileShader(shaderId);
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &hasCompiled);

  // if GL_FALSE
  if (!hasCompiled) {
    glGetShaderInfoLog(shaderId, 1'024, NULL, infoLog);
    std::string fmt = clrp::prepare(clrp::ATTRIBUTE::BOLD, clrp::FG::RED);
    std::string head = std::format("\n===== Shader compilation error ({}) =====\n\n", path.string().c_str());
    printf(fmt.c_str(), head.c_str());
    puts(infoLog);
    for (size_t i = 0; i < head.length() - 3; i++)
      printf(fmt.c_str(), "=");
    puts("");
    exit(1);
  }

  return shaderId;
}

bool Shader::link(GLuint program) {
  GLint hasLinked;

  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &hasLinked);

  return hasLinked;
}

void Shader::printLinkError(const fspath& vsPath, const fspath& fsPath, const fspath& gsPath) {
  char infoLog[1'024];
  glGetProgramInfoLog(program, 1'024, NULL, infoLog);
  std::string fmt = clrp::prepare(clrp::ATTRIBUTE::BOLD, clrp::FG::RED);
  printf(fmt.c_str(), "\n===== Shader link error =====\n\n");
  printf("vs (or comp): [%s]\nfs: [%s]\nfs: [%s]\n\n", vsPath.string().c_str(), fsPath.string().c_str(), gsPath.string().c_str());
  puts(infoLog);
  printf(fmt.c_str(), "=============================\n\n");
  exit(1);
}

