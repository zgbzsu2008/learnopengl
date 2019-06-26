#ifndef LEARNOPENGL_CODECVT_HPP
#define LEARNOPENGL_CODECVT_HPP

#pragma warning(disable : 4996)

#include <iostream>
#include <iomanip>
#include <string>
#include <clocale>
#include <cuchar>
#include <climits>
#include <codecvt>
#include <memory>

namespace learnopengl {

inline std::string u16srtombs(const std::u16string str)
{
    std::string ret;
    std::mbstate_t state{};
    char out[MB_LEN_MAX]{};
    for (char16_t c : str) {
        std::size_t rc = std::c16rtomb(out, c, &state);
        if (rc != std::size_t(-1)) {
            ret += std::string{ out, rc };
        }
    }
    return ret;
}

inline std::u16string mbsrtou16s(const std::string str)
{
    std::u16string ret;
    std::mbstate_t state{};
    char16_t c16;
    const char* begin = str.data();
    const char* end = begin + str.size();
    while (std::size_t rc = std::mbrtoc16(&c16, begin, end - begin + 1, &state)) {
        if (rc == std::size_t(-3)) {
            std::cout << "Earlier surrogate pair" << std::endl;
        } else if (rc <= std::size_t(-1) / 2) {
            begin += rc;
            ret += c16;
        } else {
            break;
        }
    }
    return ret;
}

inline std::string u32srtombs(const std::u32string str)
{
    std::string ret;
    std::mbstate_t state{};
    char out[MB_LEN_MAX]{};
    for (char32_t c : str) {
        std::size_t rc = std::c32rtomb(out, c, &state);
        if (rc != std::size_t(-1)) {
            ret += std::string{ out, rc };
        }
    }
    return ret;
}

inline std::u32string mbsrtou32s(const std::string str)
{
    std::u32string ret;
    std::mbstate_t state{};
    char32_t c;
    const char* begin = str.data();
    const char* end = begin + str.size();
    while (std::size_t rc = std::mbrtoc32(&c, begin, end - begin + 1, &state)) {
        if (rc == std::size_t(-3)) {
            std::cout << "Earlier surrogate pair" << std::endl;
        } else if (rc <= std::size_t(-1) / 2) {
            begin += rc;
            ret += c;
        } else {
            break;
        }
    }
    return ret;
}

inline std::wstring mbsrtowcs(const std::string& str, int)
{
    std::setlocale(LC_ALL, "zh_CN.UTF-8");
    std::wstring ret;
    std::mbstate_t state = {};
    const char* src = str.data();
    size_t len = std::mbsrtowcs(nullptr, &src, 0, &state);
    if (len != std::size_t(-1)) {
        std::unique_ptr<wchar_t[]> buff(new wchar_t[len + 1]);
        len = std::mbsrtowcs(buff.get(), &src, len, &state);
        if (len != std::size_t(-1)) {
            ret.assign(buff.get(), len);
        }
    }
    return ret;
}

inline std::string wcsrtombs(const std::wstring& str, int)
{
    std::setlocale(LC_ALL, "zh_CN.UTF-8");
    std::string ret;
    std::mbstate_t state = {};
    const wchar_t* src = str.data();
    size_t len = std::wcsrtombs(nullptr, &src, 0, &state);
    if (len != std::size_t(-1)) {
        std::unique_ptr<char[]> buff(new char[len + 1]);
        len = std::wcsrtombs(buff.get(), &src, len, &state);
        if (len != std::size_t(-1)) {
            ret.assign(buff.get(), len);
        }
    }
    return ret;
}

inline std::wstring mbsrtowcs(const std::string& str)
{
    // std::wstring_convert<std::codecvt<wchar_t, char, mbstate_t>> cvt(
    //    new std::codecvt<wchar_t, char, mbstate_t>("CHS")); // mbs为gbk.utf-8
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> cvt; // mbs为标准utf-8
    return cvt.from_bytes(str);
}

inline std::string wcsrtombs(const std::wstring& str)
{
    // std::wstring_convert<std::codecvt<wchar_t, char, mbstate_t>> cvt(
    //    new std::codecvt<wchar_t, char, mbstate_t>("CHS")); // mbs为gbk.utf-8
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> cvt; // mbs为标准utf-8
    return cvt.to_bytes(str);
}

inline std::string wcsrtoutf8(const std::wstring& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
    return cvt.to_bytes(str);
}

inline std::wstring utf8rtowcs(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
    return cvt.from_bytes(str);
}

#if _MSC_VER >= 1900

inline std::string u16srtoutf8(const std::u16string str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t> cvt;
    auto p = reinterpret_cast<const int16_t*>(str.data());
    return cvt.to_bytes(p, p + str.size());
}

inline std::u16string utf8rtou16s(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t> cvt;
    auto p = reinterpret_cast<const char*>(str.data());
    auto s = cvt.from_bytes(p, p + str.size());
    return std::u16string(s.begin(), s.end());
}

inline std::u32string utf8rtou32s(const std::string str)
{
    std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t> cvt;
    auto p = reinterpret_cast<const char*>(str.data());
    auto s = cvt.from_bytes(p, p + str.size());
    return std::u32string(s.begin(), s.end());
}

inline std::string u32srtoutf8(const std::u32string str)
{
    std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t> cvt;
    auto p = reinterpret_cast<const int32_t*>(str.data());
    return cvt.to_bytes(p, p + str.size());
}

#else

inline std::string u16srtoutf8(const std::u16string str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> cvt;
    return cvt.to_bytes(str);
}

inline std::u16string utf8rtou16s(const std::string str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> cvt;
    return cvt.from_bytes(str);
}

inline std::u32string utf8rtou32s(const std::string str)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cvt;
    return cvt.from_bytes(str);
}

inline std::string u32srtoutf8(const std::u32string str)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cvt;
    return cvt.to_bytes(str);
}

#endif // _MSC_VER >= 1900

} // namespace learnopengl

#endif // !LEARNOPENGL_CODECVT_HPP