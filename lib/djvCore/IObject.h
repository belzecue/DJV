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

#include <djvCore/Event.h>

namespace djv
{
    namespace Core
    {
        class Context;
        class LogSystem;
        class ResourceSystem;
        class TextSystem;

        namespace Event
        {
            class IEventSystem;

        } // namespace Event

        //! This class provides the base functionality for objects.
        class IObject : public std::enable_shared_from_this<IObject>
        {
            DJV_NON_COPYABLE(IObject);

        protected:
            void _init(Context *);

            inline IObject();

        public:
            virtual ~IObject() = 0;

            inline Context * getContext() const;

            inline const std::string & getClassName() const;
            inline void setClassName(const std::string &);

            inline const std::string & getObjectName() const;
            inline void setObjectName(const std::string &);

            //! \name Object Hierarchy
            ///@{

            //! Get the parent object.
            inline const std::weak_ptr<IObject> & getParent() const;

            //! Walk up the object hierarchy looking for a parent of the given type.
            template<typename T>
            inline std::shared_ptr<T> getParentRecursiveT() const;

            //! Get the list of child objects.
            inline const std::vector<std::shared_ptr<IObject> > & getChildren() const;

            //! Get the list of child objects of the given type.
            template<typename T>
            inline std::vector<std::shared_ptr<T> > getChildrenT() const;

            //! Recursively find all child objects of the given type.
            template<typename T>
            inline std::vector<std::shared_ptr<T> > getChildrenRecursiveT() const;

            //! Get the first child object of the given type.
            template <typename T>
            inline std::shared_ptr<T> getFirstChildT() const;

            //! Recursively find the first child object of the given type.
            template <typename T>
            inline std::shared_ptr<T> getFirstChildRecursiveT() const;

            //! Add a child object.
            virtual void addChild(const std::shared_ptr<IObject> &);

            //! Remove a child object.
            virtual void removeChild(const std::shared_ptr<IObject> &);

            //! Remove all child objects. The child objects are removed in order
            //! from last to first.
            void clearChildren();

            //! Move this object to the front of the child list.
            virtual void moveToFront();

            //! Move this object to the back of the child list.
            virtual void moveToBack();

            ///@}

            //! \name Enabled State
            ///@{

            inline bool isEnabled(bool parents = false) const;
            inline void setEnabled(bool);

            ///@}

            //! \name Events
            ///@{

            //! Install an event filter.
            void installEventFilter(const std::weak_ptr<IObject> &);

            //! Remove an event filter.
            void removeEventFilter(const std::weak_ptr<IObject> &);

            //! This function receives events for the object.
            virtual bool event(Event::IEvent &);

            ///@}

            //! Get the number of objects that currently exist.
            static size_t getGlobalObjectCount();

        protected:
            //! \name Events
            ///@{

            virtual void _parentChangedEvent(Event::ParentChanged &) {}
            virtual void _childAddedEvent(Event::ChildAdded &) {}
            virtual void _childRemovedEvent(Event::ChildRemoved &) {}
            virtual void _childOrderEvent(Event::ChildOrder &) {}
            virtual void _localeEvent(Event::Locale &) {}
            virtual void _updateEvent(Event::Update &) {}

            //! Over-ride this function to filter events for other objects.
            virtual bool _eventFilter(const std::shared_ptr<IObject> &, Event::IEvent &) { return false; }

            ///@}

            //! \name Convenience Functions
            ///@{

            std::string _getText(const std::string & id) const;

            void _log(const std::string & message, Core::LogLevel = Core::LogLevel::Information);

            inline std::shared_ptr<ResourceSystem> _getResourceSystem() const;
            inline std::shared_ptr<LogSystem> _getLogSystem() const;
            inline std::shared_ptr<TextSystem> _getTextSystem() const;

            ///@}

        private:
            bool _eventFilter(Event::IEvent &);

            template<typename T>
            inline static void _getChildrenRecursiveT(const std::shared_ptr<IObject> &, std::vector<std::shared_ptr<T> > &);
            template<typename T>
            inline static void _getFirstChildRecursiveT(const std::shared_ptr<IObject> &, std::shared_ptr<T> &);

            static Context * _context;

            std::string _className;
            std::string _objectName;

            std::weak_ptr<IObject>                 _parent;
            std::vector<std::shared_ptr<IObject> > _children;

            bool _enabled = true;
            bool _parentsEnabled = true;

            std::vector<std::weak_ptr<IObject> > _filters;

            bool _localeInit = false;

            static std::shared_ptr<ResourceSystem>      _resourceSystem;
            static std::shared_ptr<LogSystem>           _logSystem;
            static std::shared_ptr<TextSystem>          _textSystem;
            static std::shared_ptr<Event::IEventSystem> _eventSystem;

            friend class Event::IEventSystem;
        };

    } // namespace Core
} // namespace djv

#include <djvCore/IObjectInline.h>
