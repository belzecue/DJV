//------------------------------------------------------------------------------
// Copyright (c) 2018 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
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

#include <djvViewLib/Project.h>

#include <djvViewLib/Context.h>

#include <iostream>
#include <sstream>

namespace djv
{
    namespace ViewLib
    {
        namespace
        {
            size_t projectCount = 0;

        } // namespace

        struct Project::Private
        {
            std::string fileName;
            bool changes = false;
        };
        
        Project::Project(const std::shared_ptr<Context> &, QObject * parent) :
            QObject(parent),
            _p(new Private)
        {
            ++projectCount;
            std::stringstream s;
            s << "Untitled " << projectCount;
            _p->fileName = s.str();
        }
        
        Project::~Project()
        {}

        const std::string & Project::getFileName() const
        {
            return _p->fileName;
        }

        bool Project::hasChanges() const
        {
            return _p->changes;
        }
            
        void Project::open(const std::string & fileName)
        {
            _p->fileName = fileName;
            Q_EMIT fileNameChanged(_p->fileName);
        }
        
        void Project::close()
        {
            _p->fileName = std::string();
            Q_EMIT fileNameChanged(_p->fileName);
        }
        
        void Project::save()
        {}
        
        void Project::saveAs(const std::string & fileName)
        {
            _p->fileName = fileName;
            Q_EMIT fileNameChanged(_p->fileName);
        }

    } // namespace ViewLib
} // namespace djv

