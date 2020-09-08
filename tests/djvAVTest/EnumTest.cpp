// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#include <djvAVTest/EnumTest.h>

#include <djvAV/Enum.h>

using namespace djv::Core;
using namespace djv::AV;

namespace djv
{
    namespace AVTest
    {
        EnumTest::EnumTest(
            const FileSystem::Path& tempPath,
            const std::shared_ptr<Context>& context) :
            ITest("djv::AVTest::EnumTest", tempPath, context)
        {}
        
        void EnumTest::run()
        {
            for (auto i : getSideEnums())
            {
                std::stringstream ss;
                ss << i;
                _print("Side: " + _getText(ss.str()));
            }

            for (auto i : getSwapIntervalEnums())
            {
                std::stringstream ss;
                ss << i;
                _print("Swap interval: " + _getText(ss.str()));
            }

            for (auto i : getAlphaBlendEnums())
            {
                std::stringstream ss;
                ss << i;
                _print("Alpha blend: " + _getText(ss.str()));
            }
            
            {
                const AlphaBlend value = AlphaBlend::First;
                rapidjson::Document document;
                auto& allocator = document.GetAllocator();
                auto json = toJSON(value, allocator);
                AlphaBlend value2 = AlphaBlend::Count;
                fromJSON(json, value2);
                DJV_ASSERT(value == value2);
            }

            try
            {
                auto json = rapidjson::Value(rapidjson::kObjectType);
                AlphaBlend value;
                fromJSON(json, value);
                DJV_ASSERT(false);
            }
            catch (const std::exception&)
            {}
            
            {
                const SwapInterval value = SwapInterval::First;
                rapidjson::Document document;
                auto& allocator = document.GetAllocator();
                auto json = toJSON(value, allocator);
                SwapInterval value2 = SwapInterval::Count;
                fromJSON(json, value2);
                DJV_ASSERT(value == value2);
            }

            try
            {
                auto json = rapidjson::Value(rapidjson::kObjectType);
                SwapInterval value;
                fromJSON(json, value);
                DJV_ASSERT(false);
            }
            catch (const std::exception&)
            {}
        }

    } // namespace AVTest
} // namespace djv

