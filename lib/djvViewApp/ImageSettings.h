//------------------------------------------------------------------------------
// Copyright (c) 2004-2019 Darby Johnston
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

#pragma once

#include <djvViewApp/Enum.h>

#include <djvUI/ISettings.h>

#include <djvCore/ValueObserver.h>

namespace djv
{
    namespace ViewApp
    {
        //! This class provides image settings.
        class ImageSettings : public UI::Settings::ISettings
        {
            DJV_NON_COPYABLE(ImageSettings);

        protected:
            void _init(Core::Context * context);

            ImageSettings();

        public:
            static std::shared_ptr<ImageSettings> create(Core::Context *);

            std::shared_ptr<Core::IValueSubject<std::string> > observeColorSpace() const;
            std::shared_ptr<Core::IValueSubject<std::string> > observeColorDisplay() const;
            std::shared_ptr<Core::IValueSubject<std::string> > observeColorView() const;
            std::shared_ptr<Core::IValueSubject<std::string> > observeOutputColorSpace() const;
            void setColorSpace(const std::string&);
            void setColorDisplay(const std::string&);
            void setColorView(const std::string&);

            std::shared_ptr<Core::IValueSubject<ImageRotate> > observeImageRotate() const;
            std::shared_ptr<Core::IValueSubject<ImageAspectRatio> > observeImageAspectRatio() const;
            void setImageRotate(ImageRotate);
            void setImageAspectRatio(ImageAspectRatio);

            void load(const picojson::value &) override;
            picojson::value save() override;

        private:
            std::string _getOutputColorSpace() const;

            DJV_PRIVATE();
        };

    } // namespace ViewApp
} // namespace djv
