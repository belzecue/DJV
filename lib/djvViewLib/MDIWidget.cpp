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

#include <djvViewLib/MDIWidget.h>

#include <djvViewLib/ImageView.h>
#include <djvViewLib/Media.h>
#include <djvViewLib/PlaybackWidget.h>
#include <djvViewLib/TimelineSlider.h>

#include <djvUI/Icon.h>
#include <djvUI/Label.h>
#include <djvUI/RowLayout.h>
#include <djvUI/StackLayout.h>
#include <djvUI/ToolButton.h>

#include <djvCore/Memory.h>
#include <djvCore/UID.h>

using namespace djv::Core;

namespace djv
{
    namespace ViewLib
    {
        struct MDIWidget::Private
        {
            std::shared_ptr<ImageView> imageView;
            std::shared_ptr<UI::Label> titleLabel;
            std::shared_ptr<UI::Layout::Horizontal> titleBar;
            std::shared_ptr<UI::Icon> resizeHandle;
            std::shared_ptr<UI::Layout::Horizontal> bottomBar;
            std::shared_ptr<UI::Layout::Stack> layout;
            std::function<void(void)> maximizeCallback;
            std::function<void(void)> closedCallback;
            std::shared_ptr<ValueObserver<std::shared_ptr<AV::Image::Image> > > imageObserver;
        };
        
        void MDIWidget::_init(Context * context)
        {
            IWidget::_init(context);

            DJV_PRIVATE_PTR();
            p.imageView = ImageView::create(context);

            p.titleLabel = UI::Label::create(context);
            p.titleLabel->setMargin(UI::Style::MetricsRole::Margin);

            auto maximizeButton = UI::Button::Tool::create(context);
            maximizeButton->setIcon("djvIconMaximize");

            auto closeButton = UI::Button::Tool::create(context);
            closeButton->setIcon("djvIconClose");

            p.titleBar = UI::Layout::Horizontal::create(context);
            p.titleBar->setClassName("djv::UI::MDI::TitleBar");
            p.titleBar->setBackgroundRole(UI::Style::ColorRole::Overlay);
            p.titleBar->addWidget(p.titleLabel, UI::Layout::RowStretch::Expand);
            auto hLayout = UI::Layout::Horizontal::create(context);
            hLayout->setSpacing(UI::Style::MetricsRole::None);
            hLayout->addWidget(maximizeButton);
            hLayout->addWidget(closeButton);
            p.titleBar->addWidget(hLayout);

            p.resizeHandle = UI::Icon::create(context);
            p.resizeHandle->setPointerEnabled(true);
            p.resizeHandle->setIcon("djvIconWindowResizeHandle");
            p.resizeHandle->setHAlign(UI::HAlign::Right);
            p.resizeHandle->setVAlign(UI::VAlign::Bottom);

            p.bottomBar = UI::Layout::Horizontal::create(context);
            p.bottomBar->setBackgroundRole(UI::Style::ColorRole::Overlay);
            p.bottomBar->addExpander();
            p.bottomBar->addWidget(p.resizeHandle);

            p.layout = UI::Layout::Stack::create(context);
            p.layout->addWidget(p.imageView);

            auto vLayout = UI::Layout::Vertical::create(context);
            vLayout->setSpacing(UI::Style::MetricsRole::None);
            vLayout->addWidget(p.titleBar);
            vLayout->addExpander();
            vLayout->addWidget(p.bottomBar);
            p.layout->addWidget(vLayout);
            IContainer::addWidget(p.layout);

            auto weak = std::weak_ptr<MDIWidget>(std::dynamic_pointer_cast<MDIWidget>(shared_from_this()));
            maximizeButton->setClickedCallback(
                [weak]
            {
                if (auto widget = weak.lock())
                {
                    if (widget->_p->maximizeCallback)
                    {
                        widget->_p->maximizeCallback();
                    }
                }
            });
            closeButton->setClickedCallback(
                [weak]
            {
                if (auto widget = weak.lock())
                {
                    if (widget->_p->closedCallback)
                    {
                        widget->_p->closedCallback();
                    }
                }
            });
        }

        MDIWidget::MDIWidget() :
            _p(new Private)
        {}

        MDIWidget::~MDIWidget()
        {}

        std::shared_ptr<MDIWidget> MDIWidget::create(Context * context)
        {
            auto out = std::shared_ptr<MDIWidget>(new MDIWidget);
            out->_init(context);
            return out;
        }

        const std::string & MDIWidget::getTitle() const
        {
            return _p->titleLabel->getText();
        }

        void MDIWidget::setTitle(const std::string & text)
        {
            _p->titleLabel->setText(text);
        }

        const std::shared_ptr<Media> & MDIWidget::getMedia() const
        {
            return _p->imageView->getMedia();
        }

        void MDIWidget::setMedia(const std::shared_ptr<Media> & value)
        {
            _p->imageView->setMedia(value);
        }

        void MDIWidget::setUIVisible(bool value)
        {
            DJV_PRIVATE_PTR();
            p.titleBar->setVisible(value);
            p.bottomBar->setVisible(value);
        }

        void MDIWidget::setClosedCallback(const std::function<void(void)> & value)
        {
            _p->closedCallback = value;
        }

        void MDIWidget::setMaximizeCallback(const std::function<void(void)> & value)
        {
            _p->maximizeCallback = value;
        }

        std::shared_ptr<UI::Widget> MDIWidget::getMoveHandle()
        {
            return std::dynamic_pointer_cast<Widget>(shared_from_this());
        }

        std::shared_ptr<UI::Widget> MDIWidget::getResizeHandle()
        {
            return _p->resizeHandle;
        }

        void MDIWidget::_preLayoutEvent(Event::PreLayout& event)
        {
            _setMinimumSize(_p->layout->getMinimumSize());
        }

        void MDIWidget::_layoutEvent(Event::Layout&)
        {
            _p->layout->setGeometry(getGeometry());
        }
        
    } // namespace ViewLib
} // namespace djv
