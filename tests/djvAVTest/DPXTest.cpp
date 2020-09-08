// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2020 Darby Johnston
// All rights reserved.

#include <djvAVTest/DPXTest.h>

#include <djvCore/Context.h>
#include <djvCore/Error.h>
#include <djvCore/FileIO.h>
#include <djvCore/Memory.h>
#include <djvCore/TextSystem.h>

using namespace djv::Core;
using namespace djv::AV;
using namespace djv::AV::IO;

namespace djv
{
    namespace AVTest
    {
        DPXTest::DPXTest(
            const FileSystem::Path& tempPath,
            const std::shared_ptr<Context>& context) :
            ITest(
                "djv::AVTest::DPXTest",
                FileSystem::Path(tempPath, "DPXTest"),
                context)
        {}
                
        void DPXTest::run()
        {
            _enum();
            _header();
            _serialize();
        }
        
        void DPXTest::_enum()
        {
            for (const auto& i : DPX::getVersionEnums())
            {
                std::stringstream ss;
                ss << i;
                _print("Version: " + _getText(ss.str()));
            }

            for (const auto& i : DPX::getEndianEnums())
            {
                std::stringstream ss;
                ss << i;
                _print("Endian: " + _getText(ss.str()));
            }
        }
                
        void DPXTest::_header()
        {
            if (auto context = getContext().lock())
            {
                auto textSystem = context->getSystemT<TextSystem>();
                
                try
                {
                    auto io = FileSystem::FileIO::create();
                    io->open(FileSystem::Path(getTempPath(), "empty.dpx").get(), FileSystem::FileIO::Mode::Write);
                    io->open(FileSystem::Path(getTempPath(), "empty.dpx").get(), FileSystem::FileIO::Mode::Read);
                    Info info;
                    DPX::Transfer transfer = DPX::Transfer::First;
                    DPX::read(io, info, transfer, textSystem);
                    DJV_ASSERT(false);
                }
                catch (const std::exception& e)
                {
                    _print(Error::format(e));
                }
                
                try
                {
                    auto io = FileSystem::FileIO::create();
                    io->open(FileSystem::Path(getTempPath(), "test.dpx").get(), FileSystem::FileIO::Mode::Write);
                    DPX::Header header;
                    DPX::zero(header);
                    io->write(&header.file, sizeof(DPX::Header::File));
                    io->write(&header.image, sizeof(DPX::Header::Image));
                    io->write(&header.source, sizeof(DPX::Header::Source));
                    io->write(&header.film, sizeof(DPX::Header::Film));
                    io->write(&header.tv, sizeof(DPX::Header::TV));
                    io->open(FileSystem::Path(getTempPath(), "test.dpx").get(), FileSystem::FileIO::Mode::Read);
                    Info info;
                    DPX::Transfer transfer = DPX::Transfer::First;
                    DPX::read(io, info, transfer, textSystem);
                    DJV_ASSERT(false);
                }
                catch (const std::exception& e)
                {
                    _print(Error::format(e));
                }
                
                try
                {
                    DPX::Header header;
                    DPX::zero(header);
                    header.image.elemSize = 0;
                    Info info;
                    info.video.push_back(Image::Info());
                    _headerIO(header, info);
                    DJV_ASSERT(false);
                }
                catch (const std::exception& e)
                {
                    _print(Error::format(e));
                }
                
                for (const auto& i : DPX::getOrientEnums())
                {
                    try
                    {
                        DPX::Header header;
                        DPX::zero(header);
                        header.image.elemSize = 1;
                        header.image.orient = static_cast<uint16_t>(i);
                        Info info;
                        info.video.push_back(Image::Info());
                        _headerIO(header, info);
                        DJV_ASSERT(false);
                    }
                    catch (const std::exception& e)
                    {
                        _print(Error::format(e));
                    }
                }
                
                for (const auto& i : DPX::getComponentsEnums())
                {
                    try
                    {
                        DPX::Header header;
                        DPX::zero(header);
                        header.image.elemSize = 1;
                        header.image.elem[0].packing = static_cast<uint16_t>(i);
                        Info info;
                        info.video.push_back(Image::Info());
                        _headerIO(header, info);
                        DJV_ASSERT(false);
                    }
                    catch (const std::exception& e)
                    {
                        _print(Error::format(e));
                    }
                }
                
                for (const auto& i : {
                    DPX::Descriptor::L,
                    DPX::Descriptor::RGB,
                    DPX::Descriptor::RGBA })
                {
                    try
                    {
                        DPX::Header header;
                        DPX::zero(header);
                        header.image.elemSize = 1;
                        header.image.elem[0].packing = static_cast<uint16_t>(DPX::Components::Pack);
                        header.image.elem[0].descriptor = static_cast<uint8_t>(i);
                        Info info;
                        info.video.push_back(Image::Info());
                        _headerIO(header, info);
                        DJV_ASSERT(false);
                    }
                    catch (const std::exception& e)
                    {
                        _print(Error::format(e));
                    }
                }

                for (const auto& i : {
                    DPX::Descriptor::L,
                    DPX::Descriptor::RGB,
                    DPX::Descriptor::RGBA })
                {
                    for (auto j : { 10, 16 })
                    {
                        try
                        {
                            DPX::Header header;
                            DPX::zero(header);
                            header.image.elemSize = 1;
                            header.image.elem[0].packing = static_cast<uint16_t>(DPX::Components::TypeA);
                            header.image.elem[0].descriptor = static_cast<uint8_t>(i);
                            header.image.elem[0].bitDepth = j;
                            Info info;
                            info.video.push_back(Image::Info());
                            _headerIO(header, info);
                            DJV_ASSERT(false);
                        }
                        catch (const std::exception& e)
                        {
                            _print(Error::format(e));
                        }
                    }
                }

                try
                {
                    DPX::Header header;
                    DPX::zero(header);
                    header.file.imageOffset = 2048;
                    header.file.headerSize = 2048 - 384;
                    header.file.industryHeaderSize = 384;
                    header.image.size[0] = 1;
                    header.image.size[1] = 1;
                    header.image.elemSize = 1;
                    header.image.elem[0].packing = static_cast<uint16_t>(DPX::Components::TypeA);
                    header.image.elem[0].descriptor = static_cast<uint8_t>(DPX::Descriptor::RGB);
                    header.image.elem[0].bitDepth = 10;
                    Info info;
                    info.video.push_back(Image::Info(1, 1, Image::Type::RGB_U10));
                    _headerIO(header, info);
                    DJV_ASSERT(false);
                }
                catch (const std::exception& e)
                {
                    _print(Error::format(e));
                }

                try
                {
                    DPX::Header header;
                    DPX::zero(header);
                    header.file.imageOffset = 2048;
                    header.file.headerSize = 2048 - 384;
                    header.file.industryHeaderSize = 384;
                    header.image.size[0] = 0;
                    header.image.size[1] = 0;
                    header.image.elemSize = 1;
                    header.image.elem[0].packing = static_cast<uint16_t>(DPX::Components::TypeA);
                    header.image.elem[0].descriptor = static_cast<uint8_t>(DPX::Descriptor::RGB);
                    header.image.elem[0].bitDepth = 10;
                    Info info;
                    info.video.push_back(Image::Info(1, 1, Image::Type::RGB_U10));
                    _headerIO(header, info);
                    DJV_ASSERT(false);
                }
                catch (const std::exception& e)
                {
                    _print(Error::format(e));
                }

                try
                {
                    DPX::Header header;
                    DPX::zero(header);
                    header.file.imageOffset = 2048;
                    header.file.headerSize = 2048 - 384;
                    header.file.industryHeaderSize = 384;
                    header.image.size[0] = 0;
                    header.image.size[1] = 0;
                    header.image.elemSize = 1;
                    header.image.elem[0].packing = static_cast<uint16_t>(DPX::Components::TypeA);
                    header.image.elem[0].descriptor = static_cast<uint8_t>(DPX::Descriptor::RGB);
                    header.image.elem[0].bitDepth = 10;
                    header.image.elem[0].encoding = 0;
                    header.image.elem[0].linePadding = 1;
                    Info info;
                    info.video.push_back(Image::Info(1, 1, Image::Type::RGB_U10));
                    _headerIO(header, info);
                    DJV_ASSERT(false);
                }
                catch (const std::exception& e)
                {
                    _print(Error::format(e));
                }

                for (const auto& i : DPX::getTransferEnums())
                {
                    DPX::Header header;
                    DPX::zero(header);
                    header.file.imageOffset = 2048;
                    header.file.headerSize = 2048 - 384;
                    header.file.industryHeaderSize = 384;
                    header.image.size[0] = 0;
                    header.image.size[1] = 0;
                    header.image.elemSize = 1;
                    header.image.elem[0].packing = static_cast<uint16_t>(DPX::Components::TypeA);
                    header.image.elem[0].descriptor = static_cast<uint8_t>(DPX::Descriptor::RGB);
                    header.image.elem[0].bitDepth = 10;
                    header.image.elem[0].encoding = 0;
                    header.image.elem[0].transfer = static_cast<uint8_t>(i);
                    Info info;
                    info.video.push_back(Image::Info(1, 1, Image::Type::RGB_U10));
                    _headerIO(header, info);
                }
                
                {
                    auto io = FileSystem::FileIO::create();
                    io->open(FileSystem::Path(getTempPath(), "test.dpx").get(), FileSystem::FileIO::Mode::Write);
                    Info info;
                    info.video.push_back(Image::Info(0, 0, Image::Type::RGB_U10));
                    info.tags.setTag("Time", " ");
                    info.tags.setTag("Creator", " ");
                    info.tags.setTag("Project", " ");
                    info.tags.setTag("Copyright", " ");
                    info.tags.setTag("Source Offset", "1 1");
                    info.tags.setTag("Source Center", "1 1");
                    info.tags.setTag("Source Size", "1 1");
                    info.tags.setTag("Source File", " ");
                    info.tags.setTag("Source Time", " ");
                    info.tags.setTag("Source Input Device", " ");
                    info.tags.setTag("Source Input Serial", " ");
                    info.tags.setTag("Source Border", "1 1 1 1");
                    info.tags.setTag("Source Pixel Aspect", "1 1");
                    info.tags.setTag("Source Scan Size", "1 1");
                    info.tags.setTag("Keycode", "1:1:1:1:1");
                    info.tags.setTag("Film Format", " ");
                    info.tags.setTag("Film Frame", "1");
                    info.tags.setTag("Film Sequence", "1");
                    info.tags.setTag("Film Hold", "1");
                    info.tags.setTag("Film Frame Rate", "24");
                    info.tags.setTag("Film Shutter", "180");
                    info.tags.setTag("Film Frame ID", " ");
                    info.tags.setTag("Film Slate", " ");
                    info.tags.setTag("Timecode", "1");
                    info.tags.setTag("TV Interlace", "1");
                    info.tags.setTag("TV Field", "1");
                    info.tags.setTag("TV Video Signal", "1");
                    info.tags.setTag("TV Sample Rate", "1 1");
                    info.tags.setTag("TV Frame Rate", "29.97");
                    info.tags.setTag("TV Time Offset", "1");
                    info.tags.setTag("TV Gamma", "1");
                    info.tags.setTag("TV Black Level", "1");
                    info.tags.setTag("TV Black Gain", "1");
                    info.tags.setTag("TV Breakpoint", "1");
                    info.tags.setTag("TV White Level", "1");
                    info.tags.setTag("TV Integration Times", "1");
                    DPX::write(io, info, DPX::Version::_2_0, DPX::Endian::Auto, DPX::Transfer::First);
                    io->open(FileSystem::Path(getTempPath(), "test.dpx").get(), FileSystem::FileIO::Mode::Read);
                    Info info2;
                    info2.video.push_back(Image::Info());
                    DPX::Transfer transfer = DPX::Transfer::First;
                    DPX::read(io, info2, transfer, textSystem);
                    DJV_ASSERT(info.tags == info2.tags);
                }

                for (const auto& i : Image::getTypeEnums())
                {
                    for (const auto& j : DPX::getVersionEnums())                
                    {
                        for (const auto& k : DPX::getEndianEnums())                
                        {
                            for (const auto& l : DPX::getTransferEnums())                
                            {
                                auto io = FileSystem::FileIO::create();
                                io->open(FileSystem::Path(getTempPath(), "test.dpx").get(), FileSystem::FileIO::Mode::Write);
                                Info info;
                                info.video.push_back(Image::Info(0, 0, i));
                                DPX::write(io, info, j, k, l);
                            }
                        }
                    }
                }
            }
        }

        void DPXTest::_headerIO(
            DPX::Header& header,
            AV::IO::Info& info,
            DPX::Version version,
            DPX::Endian endian,
            DPX::Transfer transfer)
        {
            if (auto context = getContext().lock())
            {
                auto textSystem = context->getSystemT<TextSystem>();
                auto io = FileSystem::FileIO::create();
                io->open(FileSystem::Path(getTempPath(), "test.dpx").get(), FileSystem::FileIO::Mode::Write);
                Memory::Endian fileEndian = Memory::getEndian();
                switch (endian)
                {
                case DPX::Endian::MSB: fileEndian = Memory::Endian::MSB; break;
                case DPX::Endian::LSB: fileEndian = Memory::Endian::LSB; break;
                default: break;
                }
                if (fileEndian != Memory::getEndian())
                {
                    io->setEndianConversion(true);
                    DPX::convertEndian(header);
                }
                memcpy(
                    &header.file.magic,
                    Memory::Endian::MSB == fileEndian ? DPX::magic[0] : DPX::magic[1],
                    4);
                io->write(&header.file, sizeof(DPX::Header::File));
                io->write(&header.image, sizeof(DPX::Header::Image));
                io->write(&header.source, sizeof(DPX::Header::Source));
                io->write(&header.film, sizeof(DPX::Header::Film));
                io->write(&header.tv, sizeof(DPX::Header::TV));
                
                io->open(FileSystem::Path(getTempPath(), "test.dpx").get(), FileSystem::FileIO::Mode::Read);
                DPX::read(io, info, transfer, textSystem);
            }
        }

        void DPXTest::_serialize()
        {
            {
                DPX::Options options;
                rapidjson::Document document;
                auto& allocator = document.GetAllocator();
                auto json = toJSON(options, allocator);
                DPX::Options options2;
                fromJSON(json, options2);
                DJV_ASSERT(options == options2);
            }
        }

    } // namespace AVTest
} // namespace djv


