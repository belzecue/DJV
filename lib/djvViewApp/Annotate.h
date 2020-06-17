// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#pragma once

#include <djvAV/Color.h>

#include <djvCore/Enum.h>
#include <djvCore/ICommand.h>
#include <djvCore/ValueObserver.h>

#include <glm/vec2.hpp>

#include <memory>

namespace djv
{
    namespace Core
    {
        class Context;
        
    } // namespace Core
    
    namespace AV
    {
        namespace Render2D
        {
            class Render;
            
        } // namespace Render
    } // namespace AV
    
    namespace ViewApp
    {
        class Media;

        enum class AnnotateTool
        {
            Polyline,
            Line,
            Rectangle,
            Ellipse,

            Count,
            First = Polyline
        };
        DJV_ENUM_HELPERS(AnnotateTool);

        enum class AnnotateLineSize
        {
            Small,
            Medium,
            Large,

            Count,
            First = Small
        };
        DJV_ENUM_HELPERS(AnnotateLineSize);
        float getAnnotateLineSize(AnnotateLineSize);

        struct AnnotateOptions
        {
            AnnotateTool tool = AnnotateTool::First;
            AV::Image::Color color = AV::Image::Color(1.F, 1.F, 1.F);
            float lineSize = 0.F;
        };
        
        class AnnotatePrimitive : public std::enable_shared_from_this<AnnotatePrimitive>
        {
            DJV_NON_COPYABLE(AnnotatePrimitive);
            
        protected:
            void _init(const AnnotateOptions&, const std::shared_ptr<Core::Context>&);
            AnnotatePrimitive();
            
        public:
            virtual ~AnnotatePrimitive() = 0;
            
            const AnnotateOptions& getOptions() const;
            
            virtual void draw(const std::shared_ptr<AV::Render2D::Render>&) = 0;
            
            virtual void addPoint(const glm::vec2&) = 0;
            
        protected:
            const std::weak_ptr<Core::Context>& getContext() const;
                        
        private:
            DJV_PRIVATE();
        };

        class AnnotatePolyline : public AnnotatePrimitive
        {
            DJV_NON_COPYABLE(AnnotatePolyline);

        protected:
            void _init(const AnnotateOptions&, const std::shared_ptr<Core::Context>&);
            AnnotatePolyline();

        public:
            ~AnnotatePolyline() override;

            static std::shared_ptr<AnnotatePolyline> create(const AnnotateOptions&, const std::shared_ptr<Core::Context>&);

            void draw(const std::shared_ptr<AV::Render2D::Render>&) override;
            void addPoint(const glm::vec2&) override;

        private:
            DJV_PRIVATE();
        };

        class AnnotateLine : public AnnotatePrimitive
        {
            DJV_NON_COPYABLE(AnnotateLine);

        protected:
            void _init(const AnnotateOptions&, const std::shared_ptr<Core::Context>&);
            AnnotateLine();

        public:
            ~AnnotateLine() override;

            static std::shared_ptr<AnnotateLine> create(const AnnotateOptions&, const std::shared_ptr<Core::Context>&);

            void draw(const std::shared_ptr<AV::Render2D::Render>&) override;
            void addPoint(const glm::vec2&) override;

        private:
            DJV_PRIVATE();
        };

        class AnnotateRectangle : public AnnotatePrimitive
        {
            DJV_NON_COPYABLE(AnnotateRectangle);

        protected:
            void _init(const AnnotateOptions&, const std::shared_ptr<Core::Context>&);
            AnnotateRectangle();

        public:
            ~AnnotateRectangle() override;

            static std::shared_ptr<AnnotateRectangle> create(const AnnotateOptions&, const std::shared_ptr<Core::Context>&);

            void draw(const std::shared_ptr<AV::Render2D::Render>&) override;
            void addPoint(const glm::vec2&) override;

        private:
            DJV_PRIVATE();
        };

        class AnnotateEllipse : public AnnotatePrimitive
        {
            DJV_NON_COPYABLE(AnnotateEllipse);

        protected:
            void _init(const AnnotateOptions&, const std::shared_ptr<Core::Context>&);
            AnnotateEllipse();

        public:
            ~AnnotateEllipse() override;

            static std::shared_ptr<AnnotateEllipse> create(const AnnotateOptions&, const std::shared_ptr<Core::Context>&);

            void draw(const std::shared_ptr<AV::Render2D::Render>&) override;
            void addPoint(const glm::vec2&) override;

        private:
            DJV_PRIVATE();
        };

        class AnnotateCommand : public Core::ICommand
        {
            DJV_NON_COPYABLE(AnnotateCommand);

        protected:
            void _init(
                const std::shared_ptr<AnnotatePrimitive>&,
                const std::shared_ptr<Media>&);
            AnnotateCommand();

        public:
            static std::shared_ptr<AnnotateCommand> create(
                const std::shared_ptr<AnnotatePrimitive>&,
                const std::shared_ptr<Media>&);

            std::shared_ptr<Core::IValueSubject<bool> > observeUndo() const;
            const std::shared_ptr<AnnotatePrimitive>& getPrimitive() const;
            const std::weak_ptr<Media>& getMedia() const;

            void exec() override;
            void undo() override;

        private:
            DJV_PRIVATE();
        };

    } // namespace ViewApp

    DJV_ENUM_SERIALIZE_HELPERS(ViewApp::AnnotateTool);
    DJV_ENUM_SERIALIZE_HELPERS(ViewApp::AnnotateLineSize);

    rapidjson::Value toJSON(ViewApp::AnnotateTool, rapidjson::Document::AllocatorType&);
    rapidjson::Value toJSON(ViewApp::AnnotateLineSize, rapidjson::Document::AllocatorType&);

    //! Throws:
    //! - std::exception
    void fromJSON(const rapidjson::Value&, ViewApp::AnnotateTool&);
    void fromJSON(const rapidjson::Value&, ViewApp::AnnotateLineSize&);

} // namespace djv

