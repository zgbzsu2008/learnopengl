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

// 所有汉字 [\u4e00-\u9fa5]
static std::u16string base_chs =
    u"\u7684\u4e00\u4e86\u662f\u6211\u4e0d\u5728\u4eba\u4eec\u6709\u6765\u4ed6\u8fd9\u4e0a\u7740\u4e2a\u5730\u5230"
    u"\u5927\u91cc\u8bf4\u5c31\u53bb\u5b50\u5f97\u4e5f\u548c\u90a3\u8981\u4e0b\u770b\u5929\u65f6\u8fc7\u51fa\u5c0f"
    u"\u4e48\u8d77\u4f60\u90fd\u628a\u597d\u8fd8\u591a\u6ca1\u4e3a\u53c8\u53ef\u5bb6\u5b66\u53ea\u4ee5\u4e3b\u4f1a"
    u"\u6837\u5e74\u60f3\u751f\u540c\u8001\u4e2d\u5341\u4ece\u81ea\u9762\u524d\u5934\u9053\u5b83\u540e\u7136\u8d70"
    u"\u5f88\u50cf\u89c1\u4e24\u7528\u5979\u56fd\u52a8\u8fdb\u6210\u56de\u4ec0\u8fb9\u4f5c\u5bf9\u5f00\u800c\u5df1"
    u"\u4e9b\u73b0\u5c71\u6c11\u5019\u7ecf\u53d1\u5de5\u5411\u4e8b\u547d\u7ed9\u957f\u6c34\u51e0\u4e49\u4e09\u58f0"
    u"\u4e8e\u9ad8\u624b\u77e5\u7406\u773c\u5fd7\u70b9\u5fc3\u6218\u4e8c\u95ee\u4f46\u8eab\u65b9\u5b9e\u5403\u505a"
    u"\u53eb\u5f53\u4f4f\u542c\u9769\u6253\u5462\u771f\u5168\u624d\u56db\u5df2\u6240\u654c\u4e4b\u6700\u5149\u4ea7"
    u"\u60c5\u8def\u5206\u603b\u6761\u767d\u8bdd\u4e1c\u5e2d\u6b21\u4eb2\u5982\u88ab\u82b1\u53e3\u653e\u513f\u5e38"
    u"\u6c14\u4e94\u7b2c\u4f7f\u5199\u519b\u5427\u6587\u8fd0\u518d\u679c\u600e\u5b9a\u8bb8\u5feb\u660e\u884c\u56e0"
    u"\u522b\u98de\u5916\u6811\u7269\u6d3b\u90e8\u95e8\u65e0\u5f80\u8239\u671b\u65b0\u5e26\u961f\u5148\u529b\u5b8c"
    u"\u5374\u7ad9\u4ee3\u5458\u673a\u66f4\u4e5d\u60a8\u6bcf\u98ce\u7ea7\u8ddf\u7b11\u554a\u5b69\u4e07\u5c11\u76f4"
    u"\u610f\u591c\u6bd4\u9636\u8fde\u8f66\u91cd\u4fbf\u6597\u9a6c\u54ea\u5316\u592a\u6307\u53d8\u793e\u4f3c\u58eb"
    u"\u8005\u5e72\u77f3\u6ee1\u65e5\u51b3\u767e\u539f\u62ff\u7fa4\u7a76\u5404\u516d\u672c\u601d\u89e3\u7acb\u6cb3"
    u"\u6751\u516b\u96be\u65e9\u8bba\u5417\u6839\u5171\u8ba9\u76f8\u7814\u4eca\u5176\u4e66\u5750\u63a5\u5e94\u5173"
    u"\u4fe1\u89c9\u6b65\u53cd\u5904\u8bb0\u5c06\u5343\u627e\u4e89\u9886\u6216\u5e08\u7ed3\u5757\u8dd1\u8c01\u8349"
    u"\u8d8a\u5b57\u52a0\u811a\u7d27\u7231\u7b49\u4e60\u9635\u6015\u6708\u9752\u534a\u706b\u6cd5\u9898\u5efa\u8d76"
    u"\u4f4d\u5531\u6d77\u4e03\u5973\u4efb\u4ef6\u611f\u51c6\u5f20\u56e2\u5c4b\u79bb\u8272\u8138\u7247\u79d1\u5012"
    u"\u775b\u5229\u4e16\u521a\u4e14\u7531\u9001\u5207\u661f\u5bfc\u665a\u8868\u591f\u6574\u8ba4\u54cd\u96ea\u6d41"
    u"\u672a\u573a\u8be5\u5e76\u5e95\u6df1\u523b\u5e73\u4f1f\u5fd9\u63d0\u786e\u8fd1\u4eae\u8f7b\u8bb2\u519c\u53e4"
    u"\u9ed1\u544a\u754c\u62c9\u540d\u5440\u571f\u6e05\u9633\u7167\u529e\u53f2\u6539\u5386\u8f6c\u753b\u9020\u5634"
    u"\u6b64\u6cbb\u5317\u5fc5\u670d\u96e8\u7a7f\u5185\u8bc6\u9a8c\u4f20\u4e1a\u83dc\u722c\u7761\u5174\u5f62\u91cf"
    u"\u54b1\u89c2\u82e6\u4f53\u4f17\u901a\u51b2\u5408\u7834\u53cb\u5ea6\u672f\u996d\u516c\u65c1\u623f\u6781\u5357"
    u"\u67aa\u8bfb\u6c99\u5c81\u7ebf\u91ce\u575a\u7a7a\u6536\u7b97\u81f3\u653f\u57ce\u52b3\u843d\u94b1\u7279\u56f4"
    u"\u5f1f\u80dc\u6559\u70ed\u5c55\u5305\u6b4c\u7c7b\u6e10\u5f3a\u6570\u4e61\u547c\u6027\u97f3\u7b54\u54e5\u9645"
    u"\u65e7\u795e\u5ea7\u7ae0\u5e2e\u5566\u53d7\u7cfb\u4ee4\u8df3\u975e\u4f55\u725b\u53d6\u5165\u5cb8\u6562\u6389"
    u"\u5ffd\u79cd\u88c5\u9876\u6025\u6797\u505c\u606f\u53e5\u533a\u8863\u822c\u62a5\u53f6\u538b\u6162\u53d4\u80cc"
    u"\u7ec6";
} // namespace learnopengl

#endif // !LEARNOPENGL_CODECVT_HPP