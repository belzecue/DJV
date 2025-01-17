// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2020 Darby Johnston
// All rights reserved.

#include <djvCore/OS.h>

#include <djvCore/Memory.h>
#include <djvCore/String.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif // NOMINMAX
#include <windows.h>
#include <Shlobj.h>
#include <shellapi.h>
#include <stdlib.h>
#include <VersionHelpers.h>

#include <array>
#include <codecvt>
#include <locale>

//#pragma optimize("", off)

namespace djv
{
    namespace Core
    {
        namespace OS
        {
            namespace
            {
                enum class Windows
                {
                    Unknown,
                    _7,
                    _8,
                    _8_1,
                    _10,

                    Count
                };

                std::string getLabel(Windows value)
                {
                    const std::array<std::string, static_cast<size_t>(Windows::Count)> data =
                    {
                        "Unknown",
                        "Windows 7",
                        "Windows 8",
                        "Windows 8.1",
                        "Windows 10"
                    };
                    return data[static_cast<size_t>(value)];
                }

                typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);

                Windows windowsVersion()
                {
                    Windows out = Windows::Unknown;
                    if (IsWindows10OrGreater())
                    {
                        out = Windows::_10;
                    }
                    else if (IsWindows8Point1OrGreater())
                    {
                        out = Windows::_8_1;
                    }
                    else if (IsWindows8OrGreater())
                    {
                        out = Windows::_8;
                    }
                    else if (IsWindows7OrGreater())
                    {
                        out = Windows::_7;
                    }
                    return out;
                }

            } // namespace

            std::string getInformation()
            {
                return getLabel(windowsVersion());
            }

            size_t getRAMSize()
            {
                MEMORYSTATUSEX statex;
                statex.dwLength = sizeof(statex);
                GlobalMemoryStatusEx(&statex);
                return statex.ullTotalPhys;
            }

            std::string getUserName()
            {
                WCHAR tmp[String::cStringLength] = { 0 };
                DWORD size = String::cStringLength;
                GetUserNameW(tmp, &size);
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utf16;
                return std::string(utf16.to_bytes(tmp));
            }

            int getTerminalWidth()
            {
                int out = 80;
                HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
                if (h != INVALID_HANDLE_VALUE)
                {
                    CONSOLE_SCREEN_BUFFER_INFO info;
                    if (GetConsoleScreenBufferInfo(h, &info))
                    {
                        out = info.dwSize.X;
                    }
                }
                return out;
            }

            bool getEnv(const std::string& name, std::string& out)
            {
                size_t size = 0;
                wchar_t * p = 0;
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utf16;
                if (0 == _wdupenv_s(&p, &size, utf16.from_bytes(name).c_str()))
                {
                    if (p)
                    {
                        out = utf16.to_bytes(p);
                        free(p);
                        return true;
                    }
                }
                if (p)
                {
                    free(p);
                }
                return false;
            }

            bool setEnv(const std::string& name, const std::string& value)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utf16;
                return _wputenv_s(utf16.from_bytes(name).c_str(), utf16.from_bytes(value).c_str()) == 0;
            }

            bool clearEnv(const std::string& name)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utf16;
                return _wputenv_s(utf16.from_bytes(name).c_str(), utf16.from_bytes(std::string()).c_str()) == 0;
            }

            void openURL(const std::string& value)
            {
                ShellExecute(0, 0, value.c_str(), 0, 0, SW_SHOW);
            }

        } // namespace OS
    } // namespace Core
} // namespace djv

