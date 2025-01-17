// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#include <djvDesktopApp/EventSystem.h>

#include <djvUI/Style.h>
#include <djvUI/UISystem.h>
#include <djvUI/Widget.h>
#include <djvUI/Window.h>

#include <djvRender2D/Render.h>

#include <djvGL/OffscreenBuffer.h>
#if defined(DJV_GL_ES2)
#include <djvGL/Mesh.h>
#include <djvGL/Shader.h>
#endif // DJV_GL_ES2

#include <djvSystem/Context.h>
#include <djvSystem/Event.h>
#include <djvSystem/IObject.h>
#include <djvSystem/Timer.h>
#if defined(DJV_GL_ES2)
#include <djvSystem/ResourceSystem.h>
#endif // DJV_GL_ES2

#include <djvMath/Vector.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#if defined(DJV_GL_ES2)
#include <glm/gtc/matrix_transform.hpp>
#endif // DJV_GL_ES2

#include <codecvt>
#include <locale>

using namespace djv::Core;

namespace djv
{
    namespace Desktop
    {
        namespace
        {
            const System::Event::PointerID pointerID = 1;

            int fromGLFWPointerButton(int value)
            {
                int out = 0;
                switch (value)
                {
                case GLFW_MOUSE_BUTTON_LEFT:   out = 1; break;
                case GLFW_MOUSE_BUTTON_MIDDLE: out = 2; break;
                case GLFW_MOUSE_BUTTON_RIGHT:  out = 3; break;
                default: break;
                }
                return out;
            }

        } // namespace

        struct EventSystem::Private
        {
            GLFWwindow * glfwWindow = nullptr;
            glm::ivec2 resize = glm::ivec2(0, 0);
            glm::vec2 contentScale = glm::vec2(1.F, 1.F);
            std::shared_ptr<Render2D::Render> render;
            std::shared_ptr<GL::OffscreenBuffer> offscreenBuffer;
#if defined(DJV_GL_ES2)
            std::shared_ptr<GL::Shader> shader;
#endif // DJV_GL_ES2
            std::shared_ptr<System::Timer> statsTimer;
        };

        void EventSystem::_init(GLFWwindow * glfwWindow, const std::shared_ptr<System::Context>& context)
        {
            UI::EventSystem::_init("djv::Desktop::EventSystem", context);

            DJV_PRIVATE_PTR();

            p.glfwWindow = glfwWindow;
            p.render = context->getSystemT<Render2D::Render>();

            glm::vec2 contentScale = glm::vec2(1.F, 1.F);
            glfwGetWindowContentScale(p.glfwWindow, &contentScale.x, &contentScale.y);
            _contentScale(contentScale);

#if defined(DJV_GL_ES2)
            auto resourceSystem = context->getSystemT<System::ResourceSystem>();
            const System::File::Path shaderPath = resourceSystem->getPath(System::File::ResourcePath::Shaders);
            p.shader = GL::Shader::create(
                System::File::Path(shaderPath, "djvRender2DVertex.glsl"),
                System::File::Path(shaderPath, "djvRender2DFragment.glsl"));
#endif // DJV_GL_ES2

            glfwGetFramebufferSize(glfwWindow, &p.resize.x, &p.resize.y);
            {
                std::stringstream ss;
                ss << "Frame buffer size: " << p.resize;
                _log(ss.str());
            }
            glfwSetFramebufferSizeCallback(glfwWindow, _resizeCallback);
            glfwSetWindowContentScaleCallback(glfwWindow, _contentScaleCallback);
            glfwSetWindowRefreshCallback(glfwWindow, _redrawCallback);
            glfwSetWindowFocusCallback(p.glfwWindow, _focusCallback);
            glfwSetCursorEnterCallback(glfwWindow, _pointerEnterCallback);
            glfwSetCursorPosCallback(glfwWindow, _pointerCallback);
            glfwSetMouseButtonCallback(glfwWindow, _buttonCallback);
            glfwSetDropCallback(glfwWindow, _dropCallback);
            glfwSetKeyCallback(glfwWindow, _keyCallback);
            glfwSetCharModsCallback(glfwWindow, _charCallback);
            glfwSetScrollCallback(glfwWindow, _scrollCallback);

            p.statsTimer = System::Timer::create(context);
            p.statsTimer->setRepeating(true);
            auto weak = std::weak_ptr<EventSystem>(std::dynamic_pointer_cast<EventSystem>(shared_from_this()));
            p.statsTimer->start(
                System::getTimerDuration(System::TimerValue::VerySlow),
                [weak](const std::chrono::steady_clock::time_point&, const Time::Duration&)
            {
                if (auto system = weak.lock())
                {
                    std::stringstream ss;
                    ss << "Frame buffer size: " << system->_p->resize;
                    system->_log(ss.str());
                }
            });
        }

        EventSystem::EventSystem() :
            _p(new Private)
        {}

        EventSystem::~EventSystem()
        {
            DJV_PRIVATE_PTR();
            glfwSetFramebufferSizeCallback(p.glfwWindow, nullptr);
            glfwSetWindowContentScaleCallback(p.glfwWindow, nullptr);
            glfwSetWindowRefreshCallback(p.glfwWindow, nullptr);
            glfwSetWindowFocusCallback(p.glfwWindow, nullptr);
            glfwSetCursorEnterCallback(p.glfwWindow, nullptr);
            glfwSetCursorPosCallback(p.glfwWindow, nullptr);
            glfwSetMouseButtonCallback(p.glfwWindow, nullptr);
            glfwSetDropCallback(p.glfwWindow, nullptr);
            glfwSetKeyCallback(p.glfwWindow, nullptr);
            glfwSetCharModsCallback(p.glfwWindow, nullptr);
            glfwSetScrollCallback(p.glfwWindow, nullptr);
        }

        std::shared_ptr<EventSystem> EventSystem::create(GLFWwindow * glfwWindow, const std::shared_ptr<System::Context>& context)
        {
            auto out = context->getSystemT<EventSystem>();
            if (!out)
            {
                out = std::shared_ptr<EventSystem>(new EventSystem);
                out->_init(glfwWindow, context);
            }
            return out;
        }

        std::string EventSystem::getClipboard() const
        {
            DJV_PRIVATE_PTR();
            return glfwGetClipboardString(p.glfwWindow);
        }

        void EventSystem::setClipboard(const std::string& value)
        {
            DJV_PRIVATE_PTR();
            glfwSetClipboardString(p.glfwWindow, value.c_str());
        }

        void EventSystem::tick()
        {
            UI::EventSystem::tick();
            DJV_PRIVATE_PTR();

            const bool resizeRequest = _resizeRequestReset();
            if (resizeRequest)
            {
                const Image::Size size(p.resize.x, p.resize.y);
                if (size.isValid())
                {
                    p.offscreenBuffer = GL::OffscreenBuffer::create(
                        size,
                        Image::Type::RGBA_U8,
                        _getTextSystem());
                }
                else
                {
                    p.offscreenBuffer.reset();
                }
            }

            /*std::map<std::string, size_t> objectCounts;
            for (const auto& i : _getWindows())
            {
                if (auto window = i.lock())
                {
                    IObject::getObjectCounts(window, objectCounts);
                }
            }
            size_t totalObjectCount = 0;
            for (const auto& i : objectCounts)
            {
                std::cout << i.first << ": " << i.second << std::endl;
                totalObjectCount += i.second;
            }
            std::cout << "Total: " << totalObjectCount << std::endl;
            std::cout << "Global object count: " << IObject::getGlobalObjectCount() << std::endl;
            std::cout << "Global widget count: " << UI::Widget::getGlobalWidgetCount() << std::endl;
            std::cout << std::endl;*/

            if (p.offscreenBuffer)
            {
                const bool redrawRequest = _redrawRequestReset();
                const auto& size = p.offscreenBuffer->getSize();
                if (resizeRequest)
                {
                    for (const auto& i : _getWindows())
                    {
                        if (auto window = i.lock())
                        {
                            window->resize(glm::vec2(size.w, size.h));

                            System::Event::InitLayout initLayout;
                            _initLayoutRecursive(window, initLayout);

                            System::Event::PreLayout preLayout;
                            _preLayoutRecursive(window, preLayout);

                            if (window->isVisible())
                            {
                                System::Event::Layout layout;
                                _layoutRecursive(window, layout);

                                System::Event::Clip clip(Math::BBox2f(0.F, 0.F, static_cast<float>(size.w), static_cast<float>(size.h)));
                                _clipRecursive(window, clip);
                            }
                        }
                    }
                }

                if (resizeRequest || redrawRequest)
                {
                    p.offscreenBuffer->bind();
                    p.render->beginFrame(size);
                    for (const auto& i : _getWindows())
                    {
                        if (auto window = i.lock())
                        {
                            if (window->isVisible())
                            {
                                System::Event::Paint paintEvent(Math::BBox2f(0.F, 0.F, static_cast<float>(size.w), static_cast<float>(size.h)));
                                System::Event::PaintOverlay paintOverlayEvent(Math::BBox2f(0.F, 0.F, static_cast<float>(size.w), static_cast<float>(size.h)));
                                _paintRecursive(window, paintEvent, paintOverlayEvent);
                            }
                        }
                    }
                    p.render->endFrame();

                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                }
            }

            _redraw();
        }

        void EventSystem::_pushClipRect(const Math::BBox2f& value)
        {
            _p->render->pushClipRect(value);
        }

        void EventSystem::_popClipRect()
        {
            _p->render->popClipRect();
        }

        void EventSystem::_focus(bool value)
        {
            if (!value)
            {
                setTextFocus(nullptr);
            }
        }

        void EventSystem::_resize(const glm::ivec2& size)
        {
            DJV_PRIVATE_PTR();
            p.resize = size;
            resizeRequest();
        }

        void EventSystem::_contentScale(const glm::vec2& value)
        {
            if (auto context = getContext().lock())
            {
                DJV_PRIVATE_PTR();
                p.contentScale = value;
                const glm::vec2 dpi(Render2D::dpiDefault * p.contentScale.x, Render2D::dpiDefault * p.contentScale.y);
                {
                    std::stringstream ss;
                    ss << "Content scale: " << p.contentScale << std::endl;
                    ss << "DPI: " << dpi << std::endl;
                    _log(ss.str());
                }
                auto uiSystem = context->getSystemT<UI::UISystem>();
                uiSystem->getStyle()->setDPI(dpi);
            }
        }

        void EventSystem::_redraw()
        {
            DJV_PRIVATE_PTR();
            if (p.offscreenBuffer)
            {
                glDisable(GL_DEPTH_TEST);
                glDisable(GL_SCISSOR_TEST);
                glDisable(GL_BLEND);
                const auto& size = p.offscreenBuffer->getSize();
                glViewport(
                    0,
                    0,
                    GLsizei(size.w),
                    GLsizei(size.h));
#if defined(DJV_GL_ES2)
                p.shader->bind();
                const auto viewMatrix = glm::ortho(
                    0.F,
                    static_cast<float>(size.w),
                    0.F,
                    static_cast<float>(size.h),
                    -1.F,
                    1.F);
                p.shader->setUniform("transform.mvp", viewMatrix);
                p.shader->setUniform("imageFormat", 3);
                p.shader->setUniform("colorMode", 5);
                const GLfloat color[] = { 1.F, 1.F, 1.F, 1.F };
                p.shader->setUniform("color", color);
                p.shader->setUniform("textureSampler", 0);
                
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, p.offscreenBuffer->getColorID());
                                
                auto vbo = GL::VBO::create(2 * 4, GL::VBOType::Pos2_F32_UV_U16);
                std::vector<uint8_t> vboData(6 * (2 * 4 + 2 * 2));
                struct Data
                {
                    float x;
                    float y;
                    uint16_t u;
                    uint16_t v;
                };
                Data* vboP = reinterpret_cast<Data*>(&vboData[0]);
                vboP->x = 0.F;
                vboP->y = 0.F;
                vboP->u = 0.F;
                vboP->v = 0.F;
                ++vboP;
                vboP->x = size.w;
                vboP->y = 0.F;
                vboP->u = 65535;
                vboP->v = 0;
                ++vboP;
                vboP->x = size.w;
                vboP->y = size.h;
                vboP->u = 65535;
                vboP->v = 65535;
                ++vboP;
                vboP->x = size.w;
                vboP->y = size.h;
                vboP->u = 65535;
                vboP->v = 65535;
                ++vboP;
                vboP->x = 0.F;
                vboP->y = size.h;
                vboP->u = 0;
                vboP->v = 65535;
                ++vboP;
                vboP->x = 0.F;
                vboP->y = 0.F;
                vboP->u = 0;
                vboP->v = 0;
                ++vboP;
                vbo->copy(vboData);
                auto vao = GL::VAO::create(GL::VBOType::Pos2_F32_UV_U16, vbo->getID());
                vao->draw(GL_TRIANGLES, 0, 6);
#else // DJV_GL_ES2
                glBindFramebuffer(GL_READ_FRAMEBUFFER, p.offscreenBuffer->getID());
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
                glBlitFramebuffer(
                    0, 0, size.w, size.h,
                    0, 0, size.w, size.h,
                    GL_COLOR_BUFFER_BIT,
                    GL_NEAREST);
#endif // DJV_GL_ES2
            }
        }

        void EventSystem::_hover(System::Event::PointerMove& event, std::shared_ptr<System::IObject>& hover)
        {
            const auto windows = _getWindows();
            for (auto i = windows.rbegin(); i != windows.rend(); ++i)
            {
                if (auto window = i->lock())
                {
                    if (window->isVisible())
                    {
                        _hover(window, event, hover);
                        if (event.isAccepted())
                        {
                            break;
                        }
                    }
                }
            }
        }

        void EventSystem::_hover(const std::shared_ptr<UI::Widget>& widget, System::Event::PointerMove& event, std::shared_ptr<System::IObject>& hover)
        {
            const auto children = widget->getChildWidgets();
            for (auto i = children.rbegin(); i != children.rend(); ++i)
            {
                if ((*i)->isVisible() &&
                    !(*i)->isClipped() &&
                    (*i)->getClipRect().contains(event.getPointerInfo().projectedPos))
                {
                    _hover(*i, event, hover);
                    if (event.isAccepted())
                    {
                        break;
                    }
                }
            }
            if (!event.isAccepted())
            {
                widget->event(event);
                if (event.isAccepted())
                {
                    hover = widget;
                }
            }
        }

        void EventSystem::_focusCallback(GLFWwindow* window, int value)
        {
            if (auto context = reinterpret_cast<System::Context*>(glfwGetWindowUserPointer(window)))
            {
                if (auto system = context->getSystemT<EventSystem>())
                {
                    system->_focus(GL_TRUE == value);
                }
            }
        }

        void EventSystem::_resizeCallback(GLFWwindow * window, int width, int height)
        {
            if (auto context = reinterpret_cast<System::Context*>(glfwGetWindowUserPointer(window)))
            {
                if (auto system = context->getSystemT<EventSystem>())
                {
                    system->_resize(glm::ivec2(width, height));
                }
            }
        }

        void EventSystem::_contentScaleCallback(GLFWwindow* window, float scaleX, float scaleY)
        {
            if (auto context = reinterpret_cast<System::Context*>(glfwGetWindowUserPointer(window)))
            {
                if (auto system = context->getSystemT<EventSystem>())
                {
                    system->_contentScale(glm::vec2(scaleX, scaleY));
                }
            }
        }

        void EventSystem::_redrawCallback(GLFWwindow * window)
        {
            if (auto context = reinterpret_cast<System::Context*>(glfwGetWindowUserPointer(window)))
            {
                if (auto system = context->getSystemT<EventSystem>())
                {
                    system->_redraw();
                }
            }
        }

        void EventSystem::_pointerEnterCallback(GLFWwindow* window, int value)
        {
            if (auto context = reinterpret_cast<System::Context*>(glfwGetWindowUserPointer(window)))
            {
                if (auto system = context->getSystemT<EventSystem>())
                {
                    if (GLFW_FALSE == value)
                    {
                        System::Event::PointerInfo info;
                        info.id = pointerID;
                        info.pos.x = -1.F;
                        info.pos.y = -1.F;
                        info.pos.z = 0.F;
                        info.dir.x = 0.F;
                        info.dir.y = 0.F;
                        info.dir.z = 1.F;
                        info.projectedPos = info.pos;
                        system->_pointerMove(info);
                    }
                }
            }
        }

        void EventSystem::_pointerCallback(GLFWwindow * window, double x, double y)
        {
            if (auto context = reinterpret_cast<System::Context*>(glfwGetWindowUserPointer(window)))
            {
                if (auto system = context->getSystemT<EventSystem>())
                {
                    System::Event::PointerInfo info;
                    info.id = pointerID;
                    info.pos.x = static_cast<float>(x);
                    info.pos.y = static_cast<float>(y);
#if defined(DJV_PLATFORM_MACOS)
                    info.pos.x *= system->_p->contentScale.x;
                    info.pos.y *= system->_p->contentScale.y;
#endif // DJV_PLATFORM_MACOS
                    info.pos.z = 0.F;
                    info.dir.x = 0.F;
                    info.dir.y = 0.F;
                    info.dir.z = 1.F;
                    info.projectedPos = info.pos;
                    system->_pointerMove(info);
                }
            }
        }

        void EventSystem::_buttonCallback(GLFWwindow * window, int button, int action, int modifiers)
        {
            if (auto context = reinterpret_cast<System::Context*>(glfwGetWindowUserPointer(window)))
            {
                if (auto system = context->getSystemT<EventSystem>())
                {
                    switch (action)
                    {
                    case GLFW_PRESS:   system->_buttonPress  (fromGLFWPointerButton(button)); break;
                    case GLFW_RELEASE: system->_buttonRelease(fromGLFWPointerButton(button)); break;
                    default: break;
                    }
                }
            }
        }

        void EventSystem::_dropCallback(GLFWwindow * window, int count, const char ** paths)
        {
            if (auto context = reinterpret_cast<System::Context*>(glfwGetWindowUserPointer(window)))
            {
                if (auto system = context->getSystemT<EventSystem>())
                {
                    std::vector<std::string> list;
                    for (int i = 0; i < count; ++i)
                    {
                        list.push_back(paths[i]);
                    }
                    system->_drop(list);
                }
            }
        }

        void EventSystem::_keyCallback(GLFWwindow * window, int key, int scancode, int action, int modifiers)
        {
            if (auto context = reinterpret_cast<System::Context*>(glfwGetWindowUserPointer(window)))
            {
                if (auto system = context->getSystemT<EventSystem>())
                {
                    switch (action)
                    {
                    case GLFW_PRESS:   system->_keyPress  (key, modifiers); break;
                    case GLFW_REPEAT:  system->_keyPress  (key, modifiers); break;
                    case GLFW_RELEASE: system->_keyRelease(key, modifiers); break;
                    default: break;
                    }
                }
            }
        }

        void EventSystem::_charCallback(GLFWwindow * window, unsigned int character, int modifiers)
        {
            if (auto context = reinterpret_cast<System::Context*>(glfwGetWindowUserPointer(window)))
            {
                if (auto system = context->getSystemT<EventSystem>())
                {
                    std::basic_string<djv_char_t> utf32;
                    utf32.push_back(character);
                    system->_text(utf32, modifiers);
                }
            }
        }

        void EventSystem::_scrollCallback(GLFWwindow * window, double x, double y)
        {
            if (auto context = reinterpret_cast<System::Context*>(glfwGetWindowUserPointer(window)))
            {
                if (auto system = context->getSystemT<EventSystem>())
                {
                    system->_scroll(x, y);
                }
            }
        }

    } // namespace Desktop
} // namespace djv
