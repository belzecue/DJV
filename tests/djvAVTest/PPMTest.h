// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2020 Darby Johnston
// All rights reserved.

#include <djvTestLib/Test.h>

namespace djv
{
    namespace AVTest
    {
        class PPMTest : public Test::ITest
        {
        public:
            PPMTest(
                const System::File::Path& tempPath,
                const std::shared_ptr<System::Context>&);
            
            void run() override;

        private:
            void _serialize();
        };
        
    } // namespace AVTest
} // namespace djv

