//------------------------------------------------------------------------------
// Copyright (c) 2004-2015 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#include <djvGraphics/OpenGLShader.h>

#include <djvCore/Debug.h>

#include <QCoreApplication>

//------------------------------------------------------------------------------
// djvOpenGLShader
//------------------------------------------------------------------------------

djvOpenGLShader::~djvOpenGLShader()
{
    del();
}

namespace
{
void shaderCompile(GLuint id, const QString & source)
{
    //DJV_DEBUG("shaderCompile");
    //DJV_DEBUG_PRINT("source = " << source);
    auto glFuncs = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();
    std::vector<char> buf(source.length());
    memcpy(buf.data(), source.toLatin1().data(), buf.size());
    const char * sources       [] = { buf.data() };
    const GLint  sourceLengths [] = { static_cast<GLint>(buf.size()) };
    DJV_DEBUG_OPEN_GL(glFuncs->glShaderSource(id, 1, sources, sourceLengths));
    DJV_DEBUG_OPEN_GL(glFuncs->glCompileShader(id));
    GLint error = 0;
    glFuncs->glGetShaderiv(id, GL_COMPILE_STATUS, &error);
    char    log [4096] = "";
    GLsizei logSize    = 0;
    glFuncs->glGetShaderInfoLog(id, 4096, &logSize, log);
    //DJV_DEBUG_PRINT("log = " << QString(log));
    if (error != GL_TRUE)
    {
        throw djvError(
            "djvOpenGLShader",
            qApp->translate("djvOpenGLShader", "Cannot compile shader:\n%1").
            arg(log));
    }
}

} // namespace

void djvOpenGLShader::init(
    const QString & vertexSource,
    const QString & fragmentSource) throw (djvError)
{
    if (vertexSource == _vertexSource && fragmentSource == _fragmentSource)
        return;

    //DJV_DEBUG("djvOpenGLShader::init");
    //DJV_DEBUG_PRINT("vertexSource = " << vertexSource);
    //DJV_DEBUG_PRINT("fragmentSource = " << fragmentSource);

    auto glFuncs = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();

    del();

    _vertexSource   = vertexSource;
    _fragmentSource = fragmentSource;

    _vertexId   = glFuncs->glCreateShader(GL_VERTEX_SHADER);
    _fragmentId = glFuncs->glCreateShader(GL_FRAGMENT_SHADER);
    shaderCompile(_vertexId,   _vertexSource);
    shaderCompile(_fragmentId, _fragmentSource);
    DJV_DEBUG_OPEN_GL(_programId = glFuncs->glCreateProgram());
    if (! _programId)
    {
        throw djvError(
            "djvOpenGLShader",
            qApp->translate("djvOpenGLShader", "Cannot create shader program"));
    }
    DJV_DEBUG_OPEN_GL(glFuncs->glAttachShader(_programId, _vertexId));
    DJV_DEBUG_OPEN_GL(glFuncs->glAttachShader(_programId, _fragmentId));
    DJV_DEBUG_OPEN_GL(glFuncs->glLinkProgram (_programId));
    GLint error = 0;
    glFuncs->glGetProgramiv(_programId, GL_LINK_STATUS, &error);
    char log [4096] = "";
    GLsizei logSize = 0;
    glFuncs->glGetProgramInfoLog(_programId, 4096, &logSize, log);
    //DJV_DEBUG_PRINT("log = " << QString(log));
    if (error != GL_TRUE)
    {
        throw djvError(
            "djvOpenGLShader",
            qApp->translate("djvOpenGLShader", "Cannot link shader:\n%1").
            arg(log));
    }
}

void djvOpenGLShader::bind()
{
    //DJV_DEBUG("djvOpenGLShader::bind");
    auto glFuncs = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();
    DJV_DEBUG_OPEN_GL(glFuncs->glUseProgram(_programId));
}
    
const QString & djvOpenGLShader::vertexSource() const
{
    return _vertexSource;
}

const QString & djvOpenGLShader::fragmentSource() const
{
    return _fragmentSource;
}

GLuint djvOpenGLShader::program() const
{
    return _programId;
}

void djvOpenGLShader::setUniform(const QString& name, int value)
{
    auto glFuncs = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();
    glFuncs->glUniform1i(
        glFuncs->glGetUniformLocation(_programId, name.toLatin1().data()),
        static_cast<GLint>(value));
}

void djvOpenGLShader::setUniform(const QString& name, float value)
{
    auto glFuncs = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();
    glFuncs->glUniform1f(
        glFuncs->glGetUniformLocation(_programId, name.toLatin1().data()),
        static_cast<GLfloat>(value));
}

void djvOpenGLShader::setUniform(const QString& name, const glm::mat4x4& value)
{
    auto glFuncs = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();
    glFuncs->glUniformMatrix4fv(
        glFuncs->glGetUniformLocation(_programId, name.toLatin1().data()),
        1,
        false,
        &value[0][0]);
}

void djvOpenGLShader::del()
{
    auto glFuncs = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_1_Core>();
    if (_vertexId)
    {
        glFuncs->glDeleteShader(_vertexId);
        _vertexId = 0;
    }
    if (_fragmentId)
    {
        glFuncs->glDeleteShader(_fragmentId);
        _fragmentId = 0;
    }
    if (_programId)
    {
        glFuncs->glDeleteProgram(_programId);
        _programId = 0;
    }
}
