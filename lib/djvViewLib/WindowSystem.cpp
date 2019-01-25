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

#include <djvViewLib/WindowSystem.h>

#include <djvDesktop/Application.h>

#include <djvUI/Action.h>
#include <djvUI/Menu.h>

#include <djvAV/AVSystem.h>

#include <djvCore/Context.h>

#include <GLFW/glfw3.h>

using namespace djv::Core;

namespace djv
{
    namespace ViewLib
    {
        struct WindowSystem::Private
        {
            std::map<std::string, std::shared_ptr<UI::Action> > actions;
            std::map<std::string, std::shared_ptr<UI::Menu> > menus;
            std::shared_ptr<ValueSubject<bool> > maximize;
            std::shared_ptr<ValueSubject<bool> > next;
            std::shared_ptr<ValueSubject<bool> > prev;
            std::map<std::string, std::shared_ptr<ValueObserver<bool> > > clickedObservers;
            std::shared_ptr<ValueObserver<bool> > fullScreenObserver;
            glm::ivec2 monitorSize = glm::ivec2(0, 0);
            int monitorRefresh = 0;
            BBox2i windowGeom = BBox2i(0, 0, 0, 0);

            void setFullScreen(bool, Context * context);
        };

        void WindowSystem::_init(Context * context)
        {
            IViewSystem::_init("djv::ViewLib::WindowSystem", context);

            DJV_PRIVATE_PTR();
            p.maximize = ValueSubject<bool>::create();
            p.next = ValueSubject<bool>::create();
            p.prev = ValueSubject<bool>::create();

            p.actions["Maximize"] = UI::Action::create();
            p.actions["Maximize"]->setShortcut(GLFW_KEY_M);

            p.actions["Next"] = UI::Action::create();
            p.actions["Next"]->setShortcut(GLFW_KEY_PAGE_DOWN);

            p.actions["Prev"] = UI::Action::create();
            p.actions["Prev"]->setShortcut(GLFW_KEY_PAGE_UP);

            //! \todo Implement me!
            p.actions["Duplicate"] = UI::Action::create();
            p.actions["Duplicate"]->setIcon("djvIconWindowDuplicate");
            p.actions["Duplicate"]->setShortcut(GLFW_KEY_D, GLFW_MOD_CONTROL);
            p.actions["Duplicate"]->setEnabled(false);

            //! \todo Implement me!
            p.actions["Fit"] = UI::Action::create();
            p.actions["Fit"]->setIcon("djvIconWindowFit");
            p.actions["Fit"]->setShortcut(GLFW_KEY_F);
            p.actions["Fit"]->setEnabled(false);

            p.actions["FullScreen"] = UI::Action::create();
            p.actions["FullScreen"]->setIcon("djvIconWindowFullScreen");
            p.actions["FullScreen"]->setButtonType(UI::ButtonType::Toggle);
            p.actions["FullScreen"]->setShortcut(GLFW_KEY_U);

            auto weak = std::weak_ptr<WindowSystem>(std::dynamic_pointer_cast<WindowSystem>(shared_from_this()));
            p.clickedObservers["Maximize"] = ValueObserver<bool>::create(
                p.actions["Maximize"]->observeClicked(),
                [weak, context](bool value)
            {
                if (value)
                {
                    if (auto system = weak.lock())
                    {
                        system->_p->maximize->setAlways(true);
                    }
                }
            });

            p.clickedObservers["Next"] = ValueObserver<bool>::create(
                p.actions["Next"]->observeClicked(),
                [weak, context](bool value)
            {
                if (value)
                {
                    if (auto system = weak.lock())
                    {
                        system->_p->next->setAlways(true);
                    }
                }
            });

            p.clickedObservers["Prev"] = ValueObserver<bool>::create(
                p.actions["Prev"]->observeClicked(),
                [weak, context](bool value)
            {
                if (value)
                {
                    if (auto system = weak.lock())
                    {
                        system->_p->prev->setAlways(true);
                    }
                }
            });

            p.fullScreenObserver = ValueObserver<bool>::create(
                p.actions["FullScreen"]->observeChecked(),
                [weak, context](bool value)
            {
                if (auto system = weak.lock())
                {
                    system->_p->setFullScreen(value, context);
                }
            });
        }

        WindowSystem::WindowSystem() :
            _p(new Private)
        {}

        WindowSystem::~WindowSystem()
        {}

        std::shared_ptr<WindowSystem> WindowSystem::create(Context * context)
        {
            auto out = std::shared_ptr<WindowSystem>(new WindowSystem);
            out->_init(context);
            return out;
        }

        std::shared_ptr<Core::IValueSubject<bool> > WindowSystem::observeMaximize() const
        {
            return _p->maximize;
        }

        std::shared_ptr<Core::IValueSubject<bool> > WindowSystem::observeNext() const
        {
            return _p->next;
        }

        std::shared_ptr<Core::IValueSubject<bool> > WindowSystem::observePrev() const
        {
            return _p->prev;
        }

        std::map<std::string, std::shared_ptr<UI::Action> > WindowSystem::getActions()
        {
            return _p->actions;
        }

        NewMenu WindowSystem::createMenu()
        {
            DJV_PRIVATE_PTR();
            p.menus["Window"] = UI::Menu::create(_getText(DJV_TEXT("djv::ViewLib", "Window")), getContext());
            p.menus["Window"]->addAction(p.actions["Maximize"]);
            p.menus["Window"]->addAction(p.actions["Next"]);
            p.menus["Window"]->addAction(p.actions["Prev"]);
            p.menus["Window"]->addAction(p.actions["Duplicate"]);
            p.menus["Window"]->addAction(p.actions["Fit"]);
            p.menus["Window"]->addAction(p.actions["FullScreen"]);
            return { p.menus["Window"], "B" };
        }

        void WindowSystem::_localeEvent(Event::Locale &)
        {
            DJV_PRIVATE_PTR();
            p.actions["Maximize"]->setText(_getText(DJV_TEXT("djv::ViewLib", "Maximize")));
            p.actions["Next"]->setText(_getText(DJV_TEXT("djv::ViewLib", "Next")));
            p.actions["Prev"]->setText(_getText(DJV_TEXT("djv::ViewLib", "Prev")));
            p.actions["Duplicate"]->setText(_getText(DJV_TEXT("djv::ViewLib", "Duplicate")));
            p.actions["Fit"]->setText(_getText(DJV_TEXT("djv::ViewLib", "Fit")));
            p.actions["FullScreen"]->setText(_getText(DJV_TEXT("djv::ViewLib", "Full Screen")));

            p.menus["Window"]->setMenuName(_getText(DJV_TEXT("djv::ViewLib", "Window")));
        }

        void WindowSystem::Private::setFullScreen(bool value, Context * context)
        {
            if (auto app = dynamic_cast<Desktop::Application *>(context))
            {
                auto glfwWindow = app->getGLFWWindow();
                auto glfwMonitor = glfwGetWindowMonitor(glfwWindow);
                if (value && !glfwMonitor)
                {
                    glfwMonitor = glfwGetPrimaryMonitor();
                    auto glfwMonitorMode = glfwGetVideoMode(glfwMonitor);
                    monitorSize.x = glfwMonitorMode->width;
                    monitorSize.y = glfwMonitorMode->height;
                    monitorRefresh = glfwMonitorMode->refreshRate;

                    int x = 0;
                    int y = 0;
                    int w = 0;
                    int h = 0;
                    glfwGetWindowPos(glfwWindow, &x, &y);
                    glfwGetWindowSize(glfwWindow, &w, &h);
                    windowGeom = BBox2i(x, y, w, h);

                    glfwSetWindowMonitor(
                        glfwWindow,
                        glfwMonitor,
                        0,
                        0,
                        glfwMonitorMode->width,
                        glfwMonitorMode->height,
                        glfwMonitorMode->refreshRate);
                }
                else if (!value && glfwMonitor)
                {
                    glfwSetWindowMonitor(
                        glfwWindow,
                        NULL,
                        windowGeom.x(),
                        windowGeom.y(),
                        windowGeom.w(),
                        windowGeom.h(),
                        0);
                }
            }
        }

    } // namespace ViewLib
} // namespace djv
