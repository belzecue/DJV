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

#include <djvViewApp/FileSystem.h>

#include <djvViewApp/Application.h>
#include <djvViewApp/FileSettings.h>
#include <djvViewApp/LayersWidget.h>
#include <djvViewApp/Media.h>
#include <djvViewApp/RecentFilesDialog.h>

#include <djvUIComponents/FileBrowserDialog.h>
#include <djvUIComponents/IOSettings.h>

#include <djvUI/Action.h>
#include <djvUI/EventSystem.h>
#include <djvUI/GroupBox.h>
#include <djvUI/Menu.h>
#include <djvUI/RowLayout.h>
#include <djvUI/SettingsSystem.h>
#include <djvUI/Shortcut.h>
#include <djvUI/Window.h>

#include <djvCore/Context.h>
#include <djvCore/FileInfo.h>
#include <djvCore/RecentFilesModel.h>
#include <djvCore/TextSystem.h>

#include <GLFW/glfw3.h>

using namespace djv::Core;

namespace djv
{
    namespace ViewApp
    {
        struct FileSystem::Private
        {
            std::shared_ptr<FileSettings> settings;
            std::shared_ptr<ValueSubject<std::shared_ptr<Media> > > opened;
            std::shared_ptr<ValueSubject<std::pair<std::shared_ptr<Media>, glm::vec2> > > opened2;
            std::shared_ptr<ValueSubject<std::shared_ptr<Media> > > closed;
            std::shared_ptr<ListSubject<std::shared_ptr<Media> > > media;
            std::shared_ptr<ValueSubject<std::shared_ptr<Media> > > currentMedia;
            bool cacheEnabled = false;
            int cacheMax = 0;
            std::map<std::string, std::shared_ptr<UI::Action> > actions;
            std::shared_ptr<UI::Menu> menu;
            std::shared_ptr<UI::FileBrowser::Dialog> fileBrowserDialog;
            Core::FileSystem::Path fileBrowserPath = Core::FileSystem::Path(".");
            size_t threadCount = 4;
            std::shared_ptr<Core::FileSystem::RecentFilesModel> recentFilesModel;
            std::shared_ptr<RecentFilesDialog> recentFilesDialog;
            std::shared_ptr<ListObserver<Core::FileSystem::FileInfo> > recentFilesObserver;
            std::shared_ptr<ListObserver<Core::FileSystem::FileInfo> > recentFilesObserver2;
            std::shared_ptr<ValueObserver<size_t> > threadCountObserver;
            std::shared_ptr<ValueObserver<bool> > cacheEnabledObserver;
            std::shared_ptr<ValueObserver<int> > cacheMaxObserver;
            std::map<std::string, std::shared_ptr<ValueObserver<bool> > > actionObservers;
            std::shared_ptr<ValueObserver<std::string> > localeObserver;
        };

        void FileSystem::_init(Context * context)
        {
            IViewSystem::_init("djv::ViewApp::FileSystem", context);

            DJV_PRIVATE_PTR();

            p.settings = FileSettings::create(context);
            p.opened = ValueSubject<std::shared_ptr<Media> >::create();
            p.opened2 = ValueSubject<std::pair<std::shared_ptr<Media>, glm::vec2> >::create();
            p.closed = ValueSubject<std::shared_ptr<Media> >::create();
            p.media = ListSubject<std::shared_ptr<Media> >::create();
            p.currentMedia = ValueSubject<std::shared_ptr<Media> >::create();

            p.actions["Open"] = UI::Action::create();
            p.actions["Open"]->setIcon("djvIconFileOpen");
            p.actions["Open"]->setShortcut(GLFW_KEY_O, UI::Shortcut::getSystemModifier());
            p.actions["Recent"] = UI::Action::create();
            p.actions["Recent"]->setIcon("djvIconFileRecent");
            p.actions["Recent"]->setShortcut(GLFW_KEY_T, UI::Shortcut::getSystemModifier());
            p.actions["Reload"] = UI::Action::create();
            p.actions["Reload"]->setShortcut(GLFW_KEY_R, UI::Shortcut::getSystemModifier());
            p.actions["Close"] = UI::Action::create();
            p.actions["Close"]->setIcon("djvIconFileClose");
            p.actions["Close"]->setShortcut(GLFW_KEY_E, UI::Shortcut::getSystemModifier());
            p.actions["CloseAll"] = UI::Action::create();
            p.actions["CloseAll"]->setShortcut(GLFW_KEY_E, GLFW_MOD_SHIFT | UI::Shortcut::getSystemModifier());
            //! \todo Implement me!
            p.actions["Export"] = UI::Action::create();
            p.actions["Export"]->setShortcut(GLFW_KEY_X, UI::Shortcut::getSystemModifier());
            p.actions["Export"]->setEnabled(false);
            p.actions["Next"] = UI::Action::create();
            p.actions["Next"]->setShortcut(GLFW_KEY_PAGE_DOWN);
            p.actions["Prev"] = UI::Action::create();
            p.actions["Prev"]->setShortcut(GLFW_KEY_PAGE_UP);
            p.actions["LayersWidget"] = UI::Action::create();
            p.actions["LayersWidget"]->setButtonType(UI::ButtonType::Toggle);
            p.actions["LayersWidget"]->setShortcut(GLFW_KEY_L, UI::Shortcut::getSystemModifier());
            p.actions["NextLayer"] = UI::Action::create();
            p.actions["NextLayer"]->setShortcut(GLFW_KEY_EQUAL, UI::Shortcut::getSystemModifier());
            p.actions["PrevLayer"] = UI::Action::create();
            p.actions["PrevLayer"]->setShortcut(GLFW_KEY_MINUS, UI::Shortcut::getSystemModifier());
            //! \todo Implement me!
            p.actions["8BitConversion"] = UI::Action::create();
            p.actions["8BitConversion"]->setButtonType(UI::ButtonType::Toggle);
            p.actions["8BitConversion"]->setEnabled(false);
            p.actions["MemoryCache"] = UI::Action::create();
            p.actions["MemoryCache"]->setButtonType(UI::ButtonType::Toggle);
            p.actions["MemoryCache"]->setEnabled(false);
            p.actions["Exit"] = UI::Action::create();
            p.actions["Exit"]->setShortcut(GLFW_KEY_Q, UI::Shortcut::getSystemModifier());

            p.menu = UI::Menu::create(context);
            p.menu->addAction(p.actions["Open"]);
            p.menu->addAction(p.actions["Recent"]);
            p.menu->addAction(p.actions["Reload"]);
            p.menu->addAction(p.actions["Close"]);
            p.menu->addAction(p.actions["CloseAll"]);
            p.menu->addAction(p.actions["Export"]);
            p.menu->addAction(p.actions["Next"]);
            p.menu->addAction(p.actions["Prev"]);
            p.menu->addSeparator();
            p.menu->addAction(p.actions["NextLayer"]);
            p.menu->addAction(p.actions["PrevLayer"]);
            p.menu->addAction(p.actions["LayersWidget"]);
            p.menu->addSeparator();
            p.menu->addAction(p.actions["8BitConversion"]);
            p.menu->addSeparator();
            p.menu->addAction(p.actions["MemoryCache"]);
            p.menu->addSeparator();
            p.menu->addAction(p.actions["Exit"]);

            p.recentFilesModel = Core::FileSystem::RecentFilesModel::create(context);

            _actionsUpdate();

            auto weak = std::weak_ptr<FileSystem>(std::dynamic_pointer_cast<FileSystem>(shared_from_this()));
            _setCloseWidgetCallback(
                [weak](const std::string& name)
                {
                    if (auto system = weak.lock())
                    {
                        const auto i = system->_p->actions.find(name);
                        if (i != system->_p->actions.end())
                        {
                            i->second->setChecked(false);
                        }
                    }
                });

            p.recentFilesObserver = ListObserver<Core::FileSystem::FileInfo>::create(
                p.settings->observeRecentFiles(),
                [weak](const std::vector<Core::FileSystem::FileInfo>& value)
                {
                    if (auto system = weak.lock())
                    {
                        system->_p->recentFilesModel->setFiles(value);
                    }
                });

            p.recentFilesObserver2 = ListObserver<Core::FileSystem::FileInfo>::create(
                p.recentFilesModel->observeFiles(),
                [weak](const std::vector<Core::FileSystem::FileInfo>& value)
                {
                    if (auto system = weak.lock())
                    {
                        system->_p->settings->setRecentFiles(value);
                    }
                });

            p.actionObservers["Open"] = ValueObserver<bool>::create(
                p.actions["Open"]->observeClicked(),
                [weak](bool value)
            {
                if (value)
                {
                    if (auto system = weak.lock())
                    {
                        system->_showFileBrowserDialog();
                    }
                }
            });

            p.actionObservers["Recent"] = ValueObserver<bool>::create(
                p.actions["Recent"]->observeClicked(),
                [weak](bool value)
                {
                    if (value)
                    {
                        if (auto system = weak.lock())
                        {
                            system->_showRecentFilesDialog();
                        }
                    }
                });

            p.actionObservers["Reload"] = ValueObserver<bool>::create(
                p.actions["Reload"]->observeClicked(),
                [weak](bool value)
                {
                    if (value)
                    {
                        if (auto system = weak.lock())
                        {
                            if (auto media = system->_p->currentMedia->get())
                            {
                                media->reload();
                            }
                        }
                    }
                });

            p.actionObservers["Close"] = ValueObserver<bool>::create(
                p.actions["Close"]->observeClicked(),
                [weak](bool value)
            {
                if (value)
                {
                    if (auto system = weak.lock())
                    {
                        if (auto media = system->_p->currentMedia->get())
                        {
                            system->close(media);
                        }
                    }
                }
            });

            p.actionObservers["CloseAll"] = ValueObserver<bool>::create(
                p.actions["CloseAll"]->observeClicked(),
                [weak](bool value)
            {
                if (value)
                {
                    if (auto system = weak.lock())
                    {
                        system->closeAll();
                    }
                }
            });

            p.actionObservers["Next"] = ValueObserver<bool>::create(
                p.actions["Next"]->observeClicked(),
                [weak](bool value)
            {
                if (value)
                {
                    if (auto system = weak.lock())
                    {
                        if (auto media = system->_p->currentMedia->get())
                        {
                            const size_t size = system->_p->media->getSize();
                            if (size > 1)
                            {
                                size_t index = system->_p->media->indexOf(system->_p->currentMedia->get());
                                if (index < size - 1)
                                {
                                    ++index;
                                }
                                else
                                {
                                    index = 0;
                                }
                                system->setCurrentMedia(system->_p->media->getItem(index));
                            }
                        }
                    }
                }
            });

            p.actionObservers["Prev"] = ValueObserver<bool>::create(
                p.actions["Prev"]->observeClicked(),
                [weak](bool value)
            {
                if (value)
                {
                    if (auto system = weak.lock())
                    {
                        const size_t size = system->_p->media->getSize();
                        if (size > 1)
                        {
                            size_t index = system->_p->media->indexOf(system->_p->currentMedia->get());
                            if (index > 0)
                            {
                                --index;
                            }
                            else
                            {
                                index = size - 1;
                            }
                            system->setCurrentMedia(system->_p->media->getItem(index));
                        }
                    }
                }
            });

            p.actionObservers["NextLayer"] = ValueObserver<bool>::create(
                p.actions["NextLayer"]->observeClicked(),
                [weak, context](bool value)
                {
                    if (value)
                    {
                        if (auto system = weak.lock())
                        {
                            if (auto media = system->_p->currentMedia->get())
                            {
                                media->nextLayer();
                            }
                        }
                    }
                });

            p.actionObservers["PrevLayer"] = ValueObserver<bool>::create(
                p.actions["PrevLayer"]->observeClicked(),
                [weak, context](bool value)
                {
                    if (value)
                    {
                        if (auto system = weak.lock())
                        {
                            if (auto media = system->_p->currentMedia->get())
                            {
                                media->prevLayer();
                            }
                        }
                    }
                });

            p.actionObservers["LayersWidget"] = ValueObserver<bool>::create(
                p.actions["LayersWidget"]->observeChecked(),
                [weak, context](bool value)
                {
                    if (auto system = weak.lock())
                    {
                        if (value)
                        {
                            system->_openWidget("LayersWidget", LayersWidget::create(context));
                        }
                        else
                        {
                            system->_closeWidget("LayersWidget");
                        }
                    }
                });

            p.actionObservers["MemoryCache"] = ValueObserver<bool>::create(
                p.actions["MemoryCache"]->observeChecked(),
                [weak](bool value)
                {
                    if (auto system = weak.lock())
                    {
                        if (auto media = system->_p->currentMedia->get())
                        {
                            media->setCacheEnabled(value);
                        }
                    }
                });

            p.actionObservers["Exit"] = ValueObserver<bool>::create(
                p.actions["Exit"]->observeClicked(),
                [weak, context](bool value)
                {
                    if (value)
                    {
                        dynamic_cast<Application *>(context)->exit();
                    }
                });

            p.cacheEnabledObserver = ValueObserver<bool>::create(
                p.settings->observeCacheEnabled(),
                [weak](bool value)
                {
                    if (auto system = weak.lock())
                    {
                        system->_p->cacheEnabled = value;
                        system->_actionsUpdate();
                        system->_cacheUpdate();
                    }
                });

            p.cacheMaxObserver = ValueObserver<int>::create(
                p.settings->observeCacheMax(),
                [weak](int value)
                {
                    if (auto system = weak.lock())
                    {
                        system->_p->cacheMax = value;
                        system->_cacheUpdate();
                    }
                });

            auto settingsSystem = context->getSystemT<UI::Settings::System>();
            auto ioSettings = settingsSystem->getSettingsT<UI::Settings::IO>();
            p.threadCountObserver = ValueObserver<size_t>::create(
                ioSettings->observeThreadCount(),
                [weak](size_t value)
                {
                    if (auto system = weak.lock())
                    {
                        system->_p->threadCount = value;
                        const auto& media = system->_p->media->get();
                        for (const auto& i : media)
                        {
                            i->setThreadCount(value);
                        }
                    }
                });

            p.localeObserver = ValueObserver<std::string>::create(
                context->getSystemT<TextSystem>()->observeCurrentLocale(),
                [weak](const std::string & value)
            {
                if (auto system = weak.lock())
                {
                    system->_textUpdate();
                }
            });
        }

        FileSystem::FileSystem() :
            _p(new Private)
        {}

        FileSystem::~FileSystem()
        {}

        std::shared_ptr<FileSystem> FileSystem::create(Context * context)
        {
            auto out = std::shared_ptr<FileSystem>(new FileSystem);
            out->_init(context);
            return out;
        }

        std::shared_ptr<IValueSubject<std::shared_ptr<Media> > > FileSystem::observeOpened() const
        {
            return _p->opened;
        }

        std::shared_ptr<IValueSubject<std::pair<std::shared_ptr<Media>, glm::vec2> > > FileSystem::observeOpened2() const
        {
            return _p->opened2;
        }

        std::shared_ptr<IValueSubject<std::shared_ptr<Media>> > FileSystem::observeClosed() const
        {
            return _p->closed;
        }

        std::shared_ptr<IListSubject<std::shared_ptr<Media> > > FileSystem::observeMedia() const
        {
            return _p->media;
        }

        std::shared_ptr<IValueSubject<std::shared_ptr<Media> > > FileSystem::observeCurrentMedia() const
        {
            return _p->currentMedia;
        }

        void FileSystem::open()
        {
            _showFileBrowserDialog();
        }

        void FileSystem::open(const Core::FileSystem::FileInfo& fileInfo)
        {
            DJV_PRIVATE_PTR();
            auto context = getContext();
            auto media = Media::create(fileInfo, context);
            _mediaInit(media);
            p.media->pushBack(media);
            p.opened->setIfChanged(media);
            // Reset the observer so we don't have an extra shared_ptr holding
            // onto the media object.
            p.opened->setIfChanged(nullptr);
            setCurrentMedia(media);
            p.recentFilesModel->addFile(fileInfo);
            _cacheUpdate();
        }

        void FileSystem::open(const Core::FileSystem::FileInfo& fileInfo, const glm::vec2& pos)
        {
            DJV_PRIVATE_PTR();
            auto context = getContext();
            auto media = Media::create(fileInfo, context);
            _mediaInit(media);
            p.media->pushBack(media);
            p.opened2->setIfChanged(std::make_pair(media, pos));
            // Reset the observer so we don't have an extra shared_ptr holding
            // onto the media object.
            p.opened2->setIfChanged(std::make_pair(nullptr, glm::ivec2(0, 0)));
            setCurrentMedia(media);
            p.recentFilesModel->addFile(fileInfo);
            _cacheUpdate();
        }

        void FileSystem::close(const std::shared_ptr<Media>& media)
        {
            DJV_PRIVATE_PTR();
            size_t index = p.media->indexOf(media);
            if (index != invalidListIndex)
            {
                p.media->removeItem(index);
                p.closed->setIfChanged(media);
                p.closed->setIfChanged(nullptr);
                const size_t size = p.media->getSize();
                std::shared_ptr<Media> current;
                if (size > 0)
                {
                    if (index == size)
                    {
                        --index;
                    }
                    current = p.media->getItem(index);
                }
                setCurrentMedia(current);
                _cacheUpdate();
            }
        }

        void FileSystem::closeAll()
        {
            DJV_PRIVATE_PTR();
            while (p.media->getSize())
            {
                const size_t i = p.media->getSize() - 1;
                auto media = p.media->getItem(i);
                p.media->removeItem(i);
                p.closed->setIfChanged(media);
                p.closed->setIfChanged(nullptr);
            }
            setCurrentMedia(nullptr);
            _cacheUpdate();
        }

        void FileSystem::setCurrentMedia(const std::shared_ptr<Media> & media)
        {
            DJV_PRIVATE_PTR();
            if (p.currentMedia->setIfChanged(media))
            {
                _actionsUpdate();
            }
        }

        std::map<std::string, std::shared_ptr<UI::Action> > FileSystem::getActions() const
        {
            return _p->actions;
        }

        MenuData FileSystem::getMenu() const
        {
            return
            {
                _p->menu,
                "A"
            };
        }

        void FileSystem::_actionsUpdate()
        {
            DJV_PRIVATE_PTR();
            const size_t size = p.media->getSize();
            p.actions["NextLayer"]->setEnabled(size);
            p.actions["PrevLayer"]->setEnabled(size);
            p.actions["Close"]->setEnabled(size);
            p.actions["CloseAll"]->setEnabled(size);
            p.actions["Export"]->setEnabled(size);
            p.actions["Next"]->setEnabled(size > 1);
            p.actions["Prev"]->setEnabled(size > 1);
            p.actions["MemoryCache"]->setChecked(p.cacheEnabled);
        }

        void FileSystem::_cacheUpdate()
        {
            DJV_PRIVATE_PTR();
            const auto& media = p.media->get();
            const size_t mediaSize = media.size();
            if (mediaSize)
            {
                size_t mediaCacheSize = (p.cacheMax * Memory::gigabyte) / mediaSize;
                for (const auto& i : media)
                {
                    i->setCacheEnabled(p.cacheEnabled);
                    i->setCacheMax(mediaCacheSize);
                }
            }
        }

        void FileSystem::_textUpdate()
        {
            DJV_PRIVATE_PTR();
            p.actions["Open"]->setText(_getText(DJV_TEXT("Open")));
            p.actions["Open"]->setTooltip(_getText(DJV_TEXT("Open tooltip")));
            p.actions["Recent"]->setText(_getText(DJV_TEXT("Recent")));
            p.actions["Recent"]->setTooltip(_getText(DJV_TEXT("Recent tooltip")));
            p.actions["Reload"]->setText(_getText(DJV_TEXT("Reload")));
            p.actions["Reload"]->setTooltip(_getText(DJV_TEXT("Reload tooltip")));
            p.actions["Close"]->setText(_getText(DJV_TEXT("Close")));
            p.actions["Close"]->setTooltip(_getText(DJV_TEXT("Close tooltip")));
            p.actions["CloseAll"]->setText(_getText(DJV_TEXT("Close All")));
            p.actions["CloseAll"]->setTooltip(_getText(DJV_TEXT("Close all tooltip")));
            p.actions["Export"]->setText(_getText(DJV_TEXT("Export")));
            p.actions["Export"]->setTooltip(_getText(DJV_TEXT("Export tooltip")));
            p.actions["Next"]->setText(_getText(DJV_TEXT("Next")));
            p.actions["Next"]->setTooltip(_getText(DJV_TEXT("Next tooltip")));
            p.actions["Prev"]->setText(_getText(DJV_TEXT("Previous")));
            p.actions["Prev"]->setTooltip(_getText(DJV_TEXT("Prev tooltip")));
            p.actions["LayersWidget"]->setText(_getText(DJV_TEXT("Layers Widget")));
            p.actions["LayersWidget"]->setTooltip(_getText(DJV_TEXT("Layers widget tooltip")));
            p.actions["NextLayer"]->setText(_getText(DJV_TEXT("Next layer")));
            p.actions["NextLayer"]->setTooltip(_getText(DJV_TEXT("Next layer tooltip")));
            p.actions["PrevLayer"]->setText(_getText(DJV_TEXT("Previous Layer")));
            p.actions["PrevLayer"]->setTooltip(_getText(DJV_TEXT("Previous layer tooltip")));
            p.actions["8BitConversion"]->setText(_getText(DJV_TEXT("8-Bit Conversion")));
            p.actions["8BitConversion"]->setTooltip(_getText(DJV_TEXT("8-bit conversion tooltip")));
            p.actions["MemoryCache"]->setText(_getText(DJV_TEXT("Memory Cache")));
            p.actions["MemoryCache"]->setTooltip(_getText(DJV_TEXT("Memory cache tooltip")));
            p.actions["Exit"]->setText(_getText(DJV_TEXT("Exit")));
            p.actions["Exit"]->setTooltip(_getText(DJV_TEXT("Exit tooltip")));

            p.menu->setText(_getText(DJV_TEXT("File")));
        }

        void FileSystem::_mediaInit(const std::shared_ptr<Media>& value)
        {
            DJV_PRIVATE_PTR();
            value->setThreadCount(p.threadCount);
        }

        void FileSystem::_showFileBrowserDialog()
        {
            DJV_PRIVATE_PTR();
            auto context = getContext();
            auto eventSystem = context->getSystemT<UI::EventSystem>();
            if (auto window = eventSystem->getCurrentWindow().lock())
            {
                p.fileBrowserDialog = UI::FileBrowser::Dialog::create(context);
                p.fileBrowserDialog->setPath(p.fileBrowserPath);
                auto weak = std::weak_ptr<FileSystem>(std::dynamic_pointer_cast<FileSystem>(shared_from_this()));
                p.fileBrowserDialog->setCallback(
                    [weak](const Core::FileSystem::FileInfo & value)
                {
                    if (auto system = weak.lock())
                    {
                        if (auto parent = system->_p->fileBrowserDialog->getParent().lock())
                        {
                            parent->removeChild(system->_p->fileBrowserDialog);
                        }
                        system->_p->fileBrowserPath = system->_p->fileBrowserDialog->getPath();
                        system->_p->fileBrowserDialog.reset();
                        system->open(value);
                    }
                });
                p.fileBrowserDialog->setCloseCallback(
                    [weak]
                {
                    if (auto system = weak.lock())
                    {
                        if (auto parent = system->_p->fileBrowserDialog->getParent().lock())
                        {
                            parent->removeChild(system->_p->fileBrowserDialog);
                        }
                        system->_p->fileBrowserPath = system->_p->fileBrowserDialog->getPath();
                        system->_p->fileBrowserDialog.reset();
                    }
                });
                window->addChild(p.fileBrowserDialog);
                p.fileBrowserDialog->show();
            }
        }

        void FileSystem::_showRecentFilesDialog()
        {
            DJV_PRIVATE_PTR();
            auto context = getContext();
            if (auto eventSystem = context->getSystemT<UI::EventSystem>())
            {
                if (auto window = eventSystem->getCurrentWindow().lock())
                {
                    if (!p.recentFilesDialog)
                    {
                        p.recentFilesDialog = RecentFilesDialog::create(context);
                        auto weak = std::weak_ptr<FileSystem>(std::dynamic_pointer_cast<FileSystem>(shared_from_this()));
                        p.recentFilesDialog->setCallback(
                            [weak](const Core::FileSystem::FileInfo & value)
                        {
                            if (auto system = weak.lock())
                            {
                                if (auto parent = system->_p->recentFilesDialog->getParent().lock())
                                {
                                    parent->removeChild(system->_p->recentFilesDialog);
                                }
                                system->_p->recentFilesDialog.reset();
                                system->open(value);
                            }
                        });
                        p.recentFilesDialog->setCloseCallback(
                            [weak]
                        {
                            if (auto system = weak.lock())
                            {
                                if (auto parent = system->_p->recentFilesDialog->getParent().lock())
                                {
                                    parent->removeChild(system->_p->recentFilesDialog);
                                }
                                system->_p->recentFilesDialog.reset();
                            }
                        });
                    }
                    window->addChild(p.recentFilesDialog);
                    p.recentFilesDialog->show();
                }
            }
        }

    } // namespace ViewApp
} // namespace djv

