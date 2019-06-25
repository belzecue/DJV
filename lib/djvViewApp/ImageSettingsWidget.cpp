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

#include <djvViewApp/ImageSettingsWidget.h>

#include <djvViewApp/ColorSpaceModel.h>
#include <djvViewApp/ImageSettings.h>

#include <djvUI/ComboBox.h>
#include <djvUI/FormLayout.h>
#include <djvUI/SettingsSystem.h>
#include <djvUI/ToggleButton.h>

#include <djvCore/Context.h>
#include <djvCore/TextSystem.h>

using namespace djv::Core;

namespace djv
{
    namespace ViewApp
    {
        struct ImageSettingsWidget::Private
        {
            std::shared_ptr<ColorSpaceModel> colorSpaceModel;
            ImageRotate rotate = ImageRotate::First;
            ImageAspectRatio aspectRatio = ImageAspectRatio::First;

            std::shared_ptr<UI::ComboBox> inputColorSpaceComboBox;
            std::shared_ptr<UI::ComboBox> colorDisplayComboBox;
            std::shared_ptr<UI::ComboBox> colorViewComboBox;
            std::shared_ptr<UI::ComboBox> rotateComboBox;
            std::shared_ptr<UI::ComboBox> aspectRatioComboBox;
            std::shared_ptr<UI::FormLayout> formLayout;

            std::shared_ptr<ListObserver<std::string> > colorSpacesObserver;
            std::shared_ptr<ListObserver<std::string> > colorDisplaysObserver;
            std::shared_ptr<ListObserver<std::string> > colorViewsObserver;
            std::shared_ptr<ValueObserver<std::string> > inputColorSpaceObserver;
            std::shared_ptr<ValueObserver<std::string> > inputColorSpaceObserver2;
            std::shared_ptr<ValueObserver<std::string> > colorDisplayObserver;
            std::shared_ptr<ValueObserver<std::string> > colorDisplayObserver2;
            std::shared_ptr<ValueObserver<std::string> > colorViewObserver;
            std::shared_ptr<ValueObserver<std::string> > colorViewObserver2;
            std::shared_ptr<ValueObserver<ImageRotate> > rotateObserver;
            std::shared_ptr<ValueObserver<ImageAspectRatio> > aspectRatioObserver;
        };

        void ImageSettingsWidget::_init(Context* context)
        {
            ISettingsWidget::_init(context);
            DJV_PRIVATE_PTR();

            setClassName("djv::ViewApp::ImageSettingsWidget");

            p.colorSpaceModel = ColorSpaceModel::create(context);

            p.inputColorSpaceComboBox = UI::ComboBox::create(context);
            p.colorDisplayComboBox = UI::ComboBox::create(context);
            p.colorViewComboBox = UI::ComboBox::create(context);
            p.rotateComboBox = UI::ComboBox::create(context);
            p.aspectRatioComboBox = UI::ComboBox::create(context);

            p.formLayout = UI::FormLayout::create(context);
            p.formLayout->addChild(p.inputColorSpaceComboBox);
            p.formLayout->addChild(p.colorDisplayComboBox);
            p.formLayout->addChild(p.colorViewComboBox);
            p.formLayout->addChild(p.rotateComboBox);
            p.formLayout->addChild(p.aspectRatioComboBox);
            addChild(p.formLayout);

            _widgetUpdate();

            auto weak = std::weak_ptr<ImageSettingsWidget>(std::dynamic_pointer_cast<ImageSettingsWidget>(shared_from_this()));
            p.inputColorSpaceComboBox->setCallback(
                [weak, context](int value)
                {
                    if (auto widget = weak.lock())
                    {
                        auto settingsSystem = context->getSystemT<UI::Settings::System>();
                        if (auto imageSettings = settingsSystem->getSettingsT<ImageSettings>())
                        {
                            imageSettings->setColorSpace(widget->_p->colorSpaceModel->indexToColorSpace(value));
                        }
                    }
                });

            p.colorDisplayComboBox->setCallback(
                [weak, context](int value)
                {
                    if (auto widget = weak.lock())
                    {
                        auto settingsSystem = context->getSystemT<UI::Settings::System>();
                        if (auto imageSettings = settingsSystem->getSettingsT<ImageSettings>())
                        {
                            imageSettings->setColorDisplay(widget->_p->colorSpaceModel->indexToDisplay(value));
                        }
                    }
                });

            p.colorViewComboBox->setCallback(
                [weak, context](int value)
                {
                    if (auto widget = weak.lock())
                    {
                        auto settingsSystem = context->getSystemT<UI::Settings::System>();
                        if (auto imageSettings = settingsSystem->getSettingsT<ImageSettings>())
                        {
                            imageSettings->setColorView(widget->_p->colorSpaceModel->indexToView(value));
                        }
                    }
                });

            p.rotateComboBox->setCallback(
                [weak, context](int value)
                {
                    if (auto widget = weak.lock())
                    {
                        auto settingsSystem = context->getSystemT<UI::Settings::System>();
                        if (auto imageSettings = settingsSystem->getSettingsT<ImageSettings>())
                        {
                            imageSettings->setImageRotate(static_cast<ImageRotate>(value));
                        }
                    }
                });

            p.aspectRatioComboBox->setCallback(
                [weak, context](int value)
                {
                    if (auto widget = weak.lock())
                    {
                        auto settingsSystem = context->getSystemT<UI::Settings::System>();
                        if (auto imageSettings = settingsSystem->getSettingsT<ImageSettings>())
                        {
                            imageSettings->setImageAspectRatio(static_cast<ImageAspectRatio>(value));
                        }
                    }
                });

            p.colorSpacesObserver = ListObserver<std::string>::create(
                p.colorSpaceModel->observeColorSpaces(),
                [weak](const std::vector<std::string>& value)
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_p->inputColorSpaceComboBox->setItems(value);
                    }
                });

            p.colorDisplaysObserver = ListObserver<std::string>::create(
                p.colorSpaceModel->observeDisplays(),
                [weak](const std::vector<std::string>& value)
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_p->colorDisplayComboBox->setItems(value);
                    }
                });

            p.colorViewsObserver = ListObserver<std::string>::create(
                p.colorSpaceModel->observeViews(),
                [weak](const std::vector<std::string>& value)
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_p->colorViewComboBox->setItems(value);
                    }
                });

            p.inputColorSpaceObserver = ValueObserver<std::string>::create(
                p.colorSpaceModel->observeColorSpace(),
                [weak](const std::string& value)
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_p->inputColorSpaceComboBox->setCurrentItem(widget->_p->colorSpaceModel->colorSpaceToIndex(value));
                    }
                });

            p.colorDisplayObserver = ValueObserver<std::string>::create(
                p.colorSpaceModel->observeDisplay(),
                [weak](const std::string& value)
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_p->colorDisplayComboBox->setCurrentItem(widget->_p->colorSpaceModel->displayToIndex(value));
                    }
                });

            p.colorViewObserver = ValueObserver<std::string>::create(
                p.colorSpaceModel->observeView(),
                [weak](const std::string& value)
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_p->colorViewComboBox->setCurrentItem(widget->_p->colorSpaceModel->viewToIndex(value));
                    }
                });

            auto settingsSystem = context->getSystemT<UI::Settings::System>();
            if (auto imageSettings = settingsSystem->getSettingsT<ImageSettings>())
            {
                p.inputColorSpaceObserver2 = ValueObserver<std::string>::create(
                    imageSettings->observeColorSpace(),
                    [weak](const std::string& value)
                    {
                        if (auto widget = weak.lock())
                        {
                            widget->_p->colorSpaceModel->setColorSpace(value);
                        }
                    });

                p.colorDisplayObserver2 = ValueObserver<std::string>::create(
                    imageSettings->observeColorDisplay(),
                    [weak](const std::string& value)
                    {
                        if (auto widget = weak.lock())
                        {
                            widget->_p->colorSpaceModel->setDisplay(value);
                        }
                    });

                p.colorViewObserver2 = ValueObserver<std::string>::create(
                    imageSettings->observeColorView(),
                    [weak](const std::string& value)
                    {
                        if (auto widget = weak.lock())
                        {
                            widget->_p->colorSpaceModel->setView(value);
                        }
                    });

                p.rotateObserver = ValueObserver<ImageRotate>::create(
                    imageSettings->observeImageRotate(),
                    [weak](ImageRotate value)
                    {
                        if (auto widget = weak.lock())
                        {
                            widget->_p->rotate = value;
                            widget->_p->rotateComboBox->setCurrentItem(static_cast<int>(value));
                        }
                    });

                p.aspectRatioObserver = ValueObserver<ImageAspectRatio>::create(
                    imageSettings->observeImageAspectRatio(),
                    [weak](ImageAspectRatio value)
                    {
                        if (auto widget = weak.lock())
                        {
                            widget->_p->aspectRatio = value;
                            widget->_p->aspectRatioComboBox->setCurrentItem(static_cast<int>(value));
                        }
                    });
            }
        }

        ImageSettingsWidget::ImageSettingsWidget() :
            _p(new Private)
        {}

        std::shared_ptr<ImageSettingsWidget> ImageSettingsWidget::create(Context* context)
        {
            auto out = std::shared_ptr<ImageSettingsWidget>(new ImageSettingsWidget);
            out->_init(context);
            return out;
        }

        std::string ImageSettingsWidget::getSettingsName() const
        {
            return DJV_TEXT("Image");
        }

        std::string ImageSettingsWidget::getSettingsGroup() const
        {
            return DJV_TEXT("DJV");
        }

        std::string ImageSettingsWidget::getSettingsSortKey() const
        {
            return "B";
        }

        void ImageSettingsWidget::_localeEvent(Event::Locale& event)
        {
            ISettingsWidget::_localeEvent(event);
            _widgetUpdate();
        }

        void ImageSettingsWidget::_widgetUpdate()
        {
            DJV_PRIVATE_PTR();

            p.formLayout->setText(p.inputColorSpaceComboBox, _getText(DJV_TEXT("Input color space")) + ":");
            p.formLayout->setText(p.colorDisplayComboBox, _getText(DJV_TEXT("Color display")) + ":");
            p.formLayout->setText(p.colorViewComboBox, _getText(DJV_TEXT("Color view")) + ":");

            p.formLayout->setText(p.rotateComboBox, _getText(DJV_TEXT("Rotate")) + ":");
            p.rotateComboBox->clearItems();
            for (auto i : getImageRotateEnums())
            {
                std::stringstream ss;
                ss << i;
                p.rotateComboBox->addItem(_getText(ss.str()));
            }
            p.rotateComboBox->setCurrentItem(static_cast<int>(p.rotate));
            
            p.formLayout->setText(p.aspectRatioComboBox, _getText(DJV_TEXT("Aspect ratio")) + ":");
            p.aspectRatioComboBox->clearItems();
            for (auto i : getImageAspectRatioEnums())
            {
                std::stringstream ss;
                ss << i;
                p.aspectRatioComboBox->addItem(_getText(ss.str()));
            }
            p.aspectRatioComboBox->setCurrentItem(static_cast<int>(p.aspectRatio));
        }

    } // namespace ViewApp
} // namespace djv

