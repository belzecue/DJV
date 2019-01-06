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

#include <djvUI/FileBrowser.h>

#include <djvUI/FileBrowserPrivate.h>
#include <djvUI/FlowLayout.h>
#include <djvUI/RowLayout.h>
#include <djvUI/StackLayout.h>
#include <djvUI/ScrollWidget.h>
#include <djvUI/Splitter.h>
#include <djvUI/ToolButton.h>

#include <djvAV/IO.h>
#include <djvAV/IconSystem.h>

#include <djvCore/DirectoryModel.h>
#include <djvCore/FileInfo.h>

using namespace djv::Core;

namespace djv
{
    namespace UI
    {
        namespace FileBrowser
        {
            struct Widget::Private
            {
                std::shared_ptr<FileSystem::DirectoryModel> directoryModel;
                std::vector<FileSystem::FileInfo> fileInfoList;
                std::map<std::shared_ptr<ItemButton>, FileSystem::FileInfo> buttonToFileInfo;
                std::shared_ptr<Layout::Flow> itemLayout;
                std::shared_ptr<Layout::Vertical> layout;
                std::shared_ptr<ValueObserver<FileSystem::Path> > pathObserver;
                std::shared_ptr<ListObserver<FileSystem::FileInfo> > fileInfoObserver;
                std::shared_ptr<ValueObserver<bool> > hasUpObserver;
                std::shared_ptr<ListObserver<FileSystem::Path> > historyObserver;
                std::shared_ptr<ValueObserver<size_t> > historyIndexObserver;
                std::shared_ptr<ValueObserver<bool> > hasBackObserver;
                std::shared_ptr<ValueObserver<bool> > hasForwardObserver;
                struct IconFuture
                {
                    std::future<std::shared_ptr<AV::Image::Image> > future;
                    std::weak_ptr<ItemButton> widget;
                };
                std::vector<IconFuture> iconFutures;
            };

            void Widget::_init(Context * context)
            {
                UI::Widget::_init(context);

                setClassName("djv::UI::FileBrowser::Widget");

                _p->directoryModel = FileSystem::DirectoryModel::create(context);

                auto upButton = Button::Tool::create(context);
                upButton->setIcon(context->getPath(Core::FileSystem::ResourcePath::IconsDirectory, "djvIconArrowUp90DPI.png"));
                upButton->setTooltip(DJV_TEXT("Go up a directory."));
                auto backButton = Button::Tool::create(context);
                backButton->setIcon(context->getPath(Core::FileSystem::ResourcePath::IconsDirectory, "djvIconArrowLeft90DPI.png"));
                backButton->setTooltip(DJV_TEXT("Go back a directory."));
                auto forwardButton = Button::Tool::create(context);
                forwardButton->setIcon(context->getPath(Core::FileSystem::ResourcePath::IconsDirectory, "djvIconArrowRight90DPI.png"));
                forwardButton->setTooltip(DJV_TEXT("Go forward a directory."));

                auto pathWidget = PathWidget::create(context);

                auto topToolLayout = Layout::Horizontal::create(context);
                topToolLayout->setSpacing(Style::MetricsRole::None);
                topToolLayout->addWidget(upButton);
                topToolLayout->addWidget(backButton);
                topToolLayout->addWidget(forwardButton);
                topToolLayout->addWidget(pathWidget, Layout::RowStretch::Expand);

                auto shortcutsWidget = ShorcutsWidget::create(context);

                _p->itemLayout = Layout::Flow::create(context);
                auto scrollWidget = ScrollWidget::create(ScrollType::Vertical, context);
                scrollWidget->addWidget(_p->itemLayout);

                _p->layout = Layout::Vertical::create(context);
                _p->layout->setSpacing(Style::MetricsRole::None);
                _p->layout->addWidget(topToolLayout);
                auto splitter = Layout::Splitter::create(Orientation::Horizontal, context);
                splitter->addWidget(shortcutsWidget);
                splitter->addWidget(scrollWidget);
                splitter->setSplit(.15f);
                _p->layout->addWidget(splitter, Layout::RowStretch::Expand);
                _p->layout->setParent(shared_from_this());

                auto weak = std::weak_ptr<Widget>(std::dynamic_pointer_cast<Widget>(shared_from_this()));
                upButton->setClickedCallback(
                    [weak]
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_p->directoryModel->cdUp();
                    }
                });
                backButton->setClickedCallback(
                    [weak]
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_p->directoryModel->goBack();
                    }
                });
                forwardButton->setClickedCallback(
                    [weak]
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_p->directoryModel->goForward();
                    }
                });

                pathWidget->setPathCallback(
                    [weak](const FileSystem::Path & value)
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_p->directoryModel->setPath(value);
                    }
                });
                pathWidget->setHistoryIndexCallback(
                    [weak](size_t value)
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_p->directoryModel->setHistoryIndex(value);
                    }
                });

                shortcutsWidget->setShortcutCallback(
                    [weak](const FileSystem::Path & value)
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_p->directoryModel->setPath(value);
                    }
                });
                
                _p->pathObserver = ValueObserver<FileSystem::Path>::create(
                    _p->directoryModel->observePath(),
                    [pathWidget](const FileSystem::Path & value)
                {
                    pathWidget->setPath(value);
                });

                _p->fileInfoObserver = ListObserver<FileSystem::FileInfo>::create(
                    _p->directoryModel->observeFileInfoList(),
                    [weak](const std::vector<FileSystem::FileInfo> & value)
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_p->fileInfoList = value;
                        widget->_updateItems();
                    }
                });

                _p->hasUpObserver = ValueObserver<bool>::create(
                    _p->directoryModel->observeHasUp(),
                    [upButton](bool value)
                {
                    upButton->setEnabled(value);
                });

                _p->historyObserver = ListObserver<FileSystem::Path>::create(
                    _p->directoryModel->observeHistory(),
                    [pathWidget](const std::vector<FileSystem::Path> & value)
                {
                    pathWidget->setHistory(value);
                });

                _p->historyIndexObserver = ValueObserver<size_t>::create(
                    _p->directoryModel->observeHistoryIndex(),
                    [pathWidget](size_t value)
                {
                    pathWidget->setHistoryIndex(value);
                });

                _p->hasBackObserver = ValueObserver<bool>::create(
                    _p->directoryModel->observeHasBack(),
                    [backButton](bool value)
                {
                    backButton->setEnabled(value);
                });

                _p->hasForwardObserver = ValueObserver<bool>::create(
                    _p->directoryModel->observeHasForward(),
                    [forwardButton](bool value)
                {
                    forwardButton->setEnabled(value);
                });
            }

            Widget::Widget() :
                _p(new Private)
            {}

            Widget::~Widget()
            {}

            std::shared_ptr<Widget> Widget::create(Context * context)
            {
                auto out = std::shared_ptr<Widget>(new Widget);
                out->_init(context);
                return out;
            }

            void Widget::setPath(const FileSystem::Path & value)
            {
                _p->directoryModel->setPath(value);
            }

            void Widget::_updateEvent(Event::Update& event)
            {
                auto i = _p->iconFutures.begin();
                while (i != _p->iconFutures.end())
                {
                    std::shared_ptr<AV::Image::Image> icon;
                    if (i->future.valid() &&
                        i->future.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
                    {
                        try
                        {
                            icon = i->future.get();
                            if (auto widget = i->widget.lock())
                            {
                                widget->setThumbnail(icon, createUID());
                            }
                        }
                        catch (const std::exception & e)
                        {
                            _log(e.what(), LogLevel::Error);
                        }
                    }
                    if (icon)
                    {
                        i = _p->iconFutures.erase(i);
                    }
                    else
                    {
                        ++i;
                    }
                }
            }

            void Widget::_preLayoutEvent(Event::PreLayout& event)
            {
                _setMinimumSize(_p->layout->getMinimumSize());
            }

            void Widget::_layoutEvent(Event::Layout& event)
            {
                _p->layout->setGeometry(getGeometry());
            }

            bool Widget::_eventFilter(const std::shared_ptr<IObject>& object, Event::IEvent& event)
            {
                switch (event.getEventType())
                {
                case Event::Type::Clip:
                {
                    if (auto button = std::dynamic_pointer_cast<ItemButton>(object))
                    {
                        if (!button->isClipped())
                        {
                            const auto i = _p->buttonToFileInfo.find(button);
                            if (i != _p->buttonToFileInfo.end())
                            {
                                auto context = getContext();
                                if (auto iconSystem = context->getSystemT<AV::Image::IconSystem>().lock())
                                {
                                    if (auto ioSystem = context->getSystemT<AV::IO::System>().lock())
                                    {
                                        if (ioSystem->canRead(i->second))
                                        {
                                            if (auto style = _getStyle().lock())
                                            {
                                                Private::IconFuture future;
                                                future.future = iconSystem->getImage(
                                                    i->second.getPath(),
                                                    AV::Image::Info(static_cast<int>(style->getMetric(UI::Style::MetricsRole::Thumbnail)), 0, AV::Image::Type::RGBA_U8));
                                                future.widget = button;
                                                _p->iconFutures.push_back(std::move(future));
                                            }
                                        }
                                        else
                                        {
                                            switch (i->second.getType())
                                            {
                                            case FileSystem::FileType::Directory:
                                                button->setIcon(context->getPath(FileSystem::ResourcePath::IconsDirectory, "djvIconFileBrowserDirectory90DPI.png"));
                                                break;
                                            default:
                                                button->setIcon(context->getPath(FileSystem::ResourcePath::IconsDirectory, "djvIconFileBrowserFile90DPI.png"));
                                                break;
                                            }
                                        }
                                    }
                                }
                                button->removeEventFilter(shared_from_this());
                                _p->buttonToFileInfo.erase(i);
                            }
                        }
                    }
                    break;
                }
                default: break;
                }
                return false;
            }

            void Widget::_updateItems()
            {
                auto context = getContext();
                _p->buttonToFileInfo.clear();
                _p->itemLayout->clearWidgets();
                auto weak = std::weak_ptr<Widget>(std::dynamic_pointer_cast<Widget>(shared_from_this()));
                for (const auto & fileInfo : _p->fileInfoList)
                {
                    auto button = ItemButton::create(context);
                    button->setText(fileInfo.getFileName(Frame::Invalid, false));
                    _p->itemLayout->addWidget(button);
                    _p->buttonToFileInfo[button] = fileInfo;
                    button->installEventFilter(shared_from_this());
                    button->setClickedCallback(
                        [weak, fileInfo]
                    {
                        if (auto widget = weak.lock())
                        {
                            if (FileSystem::FileType::Directory == fileInfo.getType())
                            {
                                widget->_p->directoryModel->setPath(fileInfo.getPath());
                            }
                        }
                    });
                }
                _resize();
            }

        } // namespace FileBrowser
    } // namespace UI
} // namespace djv
