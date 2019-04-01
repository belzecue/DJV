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

#include <djvViewLib/Enum.h>
#include <djvViewLib/ViewLib.h>

#include <djvCore/Sequence.h>
#include <djvCore/Vector.h>

#include <map>
#include <memory>

class QPainter;

namespace djv
{
    namespace ViewLib
    {
        namespace Annotations
        {
            //! This class provides the base functionality for annotation primitives.
            class AbstractPrimitive : public std::enable_shared_from_this<AbstractPrimitive>
            {
            protected:
                void _init(Enum::ANNOTATIONS_COLOR, size_t lineWidth);

            public:
                virtual ~AbstractPrimitive() = 0;

                virtual void draw(QPainter &, const glm::ivec2 & size, const glm::ivec2 & viewPos, float viewZoom) = 0;
                virtual void mouse(const glm::ivec2 &) = 0;

            protected:
                Enum::ANNOTATIONS_COLOR _color;
                size_t _lineWidth;
            };

            //! This class provides a factory to create annotation primitives.
            class PrimitiveFactory
            {
            public:
                static std::shared_ptr<AbstractPrimitive> create(Enum::ANNOTATIONS_PRIMITIVE, Enum::ANNOTATIONS_COLOR, size_t lineWidth);
            };

            //! This class provides a polyline annotation primitive.
            class PolylinePrimitive : public AbstractPrimitive
            {
            public:
                ~PolylinePrimitive() override;

                static std::shared_ptr<PolylinePrimitive> create(Enum::ANNOTATIONS_COLOR, size_t lineWidth);

                void draw(QPainter &, const glm::ivec2 & size, const glm::ivec2 & viewPos, float viewZoom) override;
                void mouse(const glm::ivec2 &) override;

            private:
                std::vector<glm::ivec2> _points;
            };

            //! This class provides a rectangle annotation primitive.
            class RectPrimitive : public AbstractPrimitive
            {
            public:
                ~RectPrimitive() override;

                static std::shared_ptr<RectPrimitive> create(Enum::ANNOTATIONS_COLOR, size_t lineWidth);

                void draw(QPainter &, const glm::ivec2 & size, const glm::ivec2 & viewPos, float viewZoom) override;
                void mouse(const glm::ivec2 &) override;

            private:
                std::vector<glm::ivec2> _points;
            };

            //! This class provides an ellipse annotation primitive.
            class EllipsePrimitive : public AbstractPrimitive
            {
            public:
                ~EllipsePrimitive() override;

                static std::shared_ptr<EllipsePrimitive> create(Enum::ANNOTATIONS_COLOR, size_t lineWidth);

                void draw(QPainter &, const glm::ivec2 & size, const glm::ivec2 & viewPos, float viewZoom) override;
                void mouse(const glm::ivec2 &) override;

            private:
                std::vector<glm::ivec2> _points;
            };

            //! This struct provides the data for an annotation.
            struct Data
            {
                QString text;
                std::vector<std::shared_ptr<AbstractPrimitive> > primitives;
            };

            //! This typedef provides a map of frame numbers and their corresponding
            //! annotations.
            typedef std::map<qint64, std::vector<Data> > Frames;

        } // namespace Annotations
    } // namespace ViewLib
} // namespace djv