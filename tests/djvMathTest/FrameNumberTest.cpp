// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#include <djvMathTest/FrameNumberTest.h>

#include <djvMath/FrameNumber.h>

#include <iostream>
#include <sstream>

using namespace djv::Core;
using namespace djv::Math;

namespace djv
{
    namespace MathTest
    {
        FrameNumberTest::FrameNumberTest(
            const System::File::Path& tempPath,
            const std::shared_ptr<System::Context>& context) :
            ITest("djv::MathTest::FrameNumberTest", tempPath, context)
        {}
        
        void FrameNumberTest::run()
        {
            _sequence();
            _operators();
            _conversion();
            _serialize();
        }

        void FrameNumberTest::_sequence()
        {
            {
                const Frame::Sequence sequence;
                DJV_ASSERT(std::vector<Frame::Range>() == sequence.getRanges());
                DJV_ASSERT(0 == sequence.getPad());
                DJV_ASSERT(!sequence.isValid());
                DJV_ASSERT(!sequence.contains(0));
                DJV_ASSERT(0 == sequence.getFrameCount());
                DJV_ASSERT(Frame::invalid == sequence.getFrame(0));
                DJV_ASSERT(Frame::invalid == sequence.getIndex(0));
                DJV_ASSERT(Frame::invalid == sequence.getLastIndex());
            }
            
            {
                const Frame::Sequence sequence(Frame::Range(0, 99), 4);
                DJV_ASSERT(1 == sequence.getRanges().size());
                DJV_ASSERT(4 == sequence.getPad());
                DJV_ASSERT(sequence.isValid());
                DJV_ASSERT(sequence.contains(0));
                DJV_ASSERT(100 == sequence.getFrameCount());
                DJV_ASSERT(0 == sequence.getFrame(0));
                DJV_ASSERT(0 == sequence.getIndex(0));
                DJV_ASSERT(99 == sequence.getLastIndex());
            }
            
            {
                const Frame::Sequence sequence({ Frame::Range(0, 9), Frame::Range(10, 99) }, 4);
                DJV_ASSERT(1 == sequence.getRanges().size());
                DJV_ASSERT(4 == sequence.getPad());
                DJV_ASSERT(sequence.isValid());
                DJV_ASSERT(sequence.contains(0));
                DJV_ASSERT(100 == sequence.getFrameCount());
                DJV_ASSERT(0 == sequence.getFrame(0));
                DJV_ASSERT(0 == sequence.getIndex(0));
                DJV_ASSERT(99 == sequence.getLastIndex());
            }
            
            {
                Frame::Sequence sequence({ Frame::Range(10, 9), Frame::Range(3, 1) });
                const auto& ranges = sequence.getRanges();
                DJV_ASSERT(ranges[0] == Frame::Range(1, 3));
                DJV_ASSERT(ranges[1] == Frame::Range(9, 10));
            }

            {
                Frame::Sequence sequence;
                for (Frame::Number i = -10; i <= 10; ++i)
                {
                    sequence.add(Frame::Range(i));
                }
                DJV_ASSERT(sequence.getRanges()[0] == Frame::Range(-10, 10));
            }

            {
                Frame::Sequence sequence(Frame::Range(1, 3));
                sequence.add(Frame::Range(3, 10));
                DJV_ASSERT(sequence.getRanges()[0] == Frame::Range(1, 10));
                sequence.add(Frame::Range(12, 100));
                DJV_ASSERT(sequence.getRanges()[0] == Frame::Range(1, 10));
            }
        }
                
        void FrameNumberTest::_operators()
        {
            {
                const Frame::Sequence sequence(Frame::Range(1, 3));
                DJV_ASSERT(sequence == sequence);
                DJV_ASSERT(sequence != Frame::Sequence());
            }
        }

        void FrameNumberTest::_conversion()
        {
            {
                std::vector<Frame::Number> frames = {};
                const auto sequence = Frame::fromFrames(frames);
                {
                    std::stringstream ss;
                    ss << sequence;
                    _print(ss.str());
                }
                DJV_ASSERT(0 == sequence.isValid());
            }
            
            {
                std::vector<Frame::Number> frames = { 1 };
                const auto sequence = Frame::fromFrames(frames);
                {
                    std::stringstream ss;
                    ss << sequence;
                    _print(ss.str());
                }
                DJV_ASSERT(1 == sequence.isValid());
                DJV_ASSERT(1 == sequence.getRanges()[0].getMin());
                DJV_ASSERT(1 == sequence.getRanges()[0].getMax());
            }
            
            {
                std::vector<Frame::Number> frames = { 1, 2, 3 };
                const auto sequence = Frame::fromFrames(frames);
                {
                    std::stringstream ss;
                    ss << sequence;
                    _print(ss.str());
                }
                DJV_ASSERT(1 == sequence.isValid());
                DJV_ASSERT(1 == sequence.getRanges()[0].getMin());
                DJV_ASSERT(3 == sequence.getRanges()[0].getMax());
            }
            
            {
                std::vector<Frame::Number> frames = { 1, 3 };
                const auto sequence = Frame::fromFrames(frames);
                {
                    std::stringstream ss;
                    ss << sequence;
                    _print(ss.str());
                }
                const auto& ranges = sequence.getRanges();
                DJV_ASSERT(2 == ranges.size());
                DJV_ASSERT(1 == ranges[0].getMin());
                DJV_ASSERT(1 == ranges[0].getMax());
                DJV_ASSERT(3 == ranges[1].getMin());
                DJV_ASSERT(3 == ranges[1].getMax());
            }
            
            {
                std::vector<Frame::Number> frames = { 1, 2, 3, 5, 6, 8 };
                const auto sequence = Frame::fromFrames(frames);
                {
                    std::stringstream ss;
                    ss << sequence;
                    _print(ss.str());
                }
                const auto& ranges = sequence.getRanges();
                DJV_ASSERT(3 == ranges.size());
                DJV_ASSERT(1 == ranges[0].getMin());
                DJV_ASSERT(3 == ranges[0].getMax());
                DJV_ASSERT(5 == ranges[1].getMin());
                DJV_ASSERT(6 == ranges[1].getMax());
                DJV_ASSERT(8 == ranges[2].getMin());
                DJV_ASSERT(8 == ranges[2].getMax());
            }
            
            {
                const Frame::Sequence sequence(
                {
                    Frame::Range(1, 3),
                    Frame::Range(5, 6),
                    Frame::Range(8)
                });
                const size_t size = sequence.getFrameCount();
                DJV_ASSERT(6 == size);
                const std::vector<Frame::Number> frames = Frame::toFrames(sequence);
                for (size_t i = 0; i < size; ++i)
                {
                    const Frame::Number frame = sequence.getFrame(i);
                    DJV_ASSERT(frame == frames[i]);
                }
            }
            
            {
                const Frame::Sequence sequence(
                {
                    Frame::Range(1, 3),
                    Frame::Range(5, 6),
                    Frame::Range(8)
                });
                DJV_ASSERT(Frame::invalidIndex == sequence.getIndex(0));
                DJV_ASSERT(0 == sequence.getIndex(1));
                DJV_ASSERT(2 == sequence.getIndex(3));
                DJV_ASSERT(5 == sequence.getIndex(8));
            }
            
            {
                DJV_ASSERT("10" == Frame::toString(10));
                DJV_ASSERT("-10" == Frame::toString(-10));
                DJV_ASSERT("0010" == Frame::toString(10, 4));
            }
            
            {
                const Frame::Range r(1, 10);
                DJV_ASSERT("1-10" == Frame::toString(r));
                Frame::Range r2;
                size_t pad = 0;
                Frame::fromString("1-10", r2, pad);
                DJV_ASSERT(r == r2);
                DJV_ASSERT(0 == pad);
                DJV_ASSERT("0001-0010" == Frame::toString(r, 4));
                Frame::fromString("0001-0010", r2, pad);
                DJV_ASSERT(r == r2);
                DJV_ASSERT(4 == pad);
            }
                        
            {
                const Frame::Sequence s(Frame::Range(1, 10));
                DJV_ASSERT("1-10" == Frame::toString(s));
                Frame::Sequence s2;
                Frame::fromString("1-10", s2);
                DJV_ASSERT(s == s2);
            }
                        
            {
                const Frame::Sequence s(Frame::Range(1, 10), 4);
                DJV_ASSERT("0001-0010" == Frame::toString(s));
                Frame::Sequence s2;
                Frame::fromString("0001-0010", s2);
                DJV_ASSERT(s == s2);
            }
            
            {
                Frame::Range range;
                size_t pad = 0;
                Frame::fromString("01-0010", range, pad);
                DJV_ASSERT(range == Frame::Range(1, 10));
                DJV_ASSERT(4 == pad);
            }
        }
        
        void FrameNumberTest::_serialize()
        {
            {
                const Frame::Sequence s(Frame::Range(1, 3));
                std::stringstream ss;
                ss << s;
                Frame::Sequence s2;
                ss >> s2;
                DJV_ASSERT(s == s2);
            }
            
            try
            {
                Frame::Sequence s;
                std::stringstream ss;
                ss >> s;
                DJV_ASSERT(false);
            }
            catch (const std::exception&)
            {}

            {
                const Frame::Sequence s(Frame::Range(1, 10), 4);
                rapidjson::Document document;
                auto& allocator = document.GetAllocator();
                auto json = toJSON(s, allocator);
                Frame::Sequence s2;
                fromJSON(json, s2);
                DJV_ASSERT(s == s2);
            }
            
            try
            {
                auto json = rapidjson::Value();
                Frame::Sequence s;
                fromJSON(json, s);
                DJV_ASSERT(false);
            }
            catch (const std::exception&)
            {}
        }

    } // namespace MathTest
} // namespace djv

