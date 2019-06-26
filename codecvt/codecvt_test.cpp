#include "codecvt.hpp"

namespace learnopengl {

template<typename T>
void print(const T& str)
{
    std::cout << "[";
    for (auto c : str) {
        std::cout << std::showbase << std::hex << c << ' ';
    }
    std::cout << "]\n";
}

template<>
void print(const std::string& str)
{
    std::cout << "[";
    for (unsigned char c8 : str) {
        std::cout << std::showbase << std::hex << +c8 << ' ';
    }
    std::cout << "]\n";
}

} // namespace learnopengl

// "zß水🍌" 或者 z\u00df\u6c34\U0001F34C
// utf-8 [0x7a 0xc3 0x9f 0xe6 0xb0 0xb4 0xf0 0x9f 0x8d 0x8c]
// utf-16[0x7a 0xdf 0x6c34 0xd83c 0xdf4c]
// utf-32[0x7a 0xdf 0x6c34 0x1f34c]

// "你好吗"
// utf-8  [0xe4 0xbd 0xa0 0xe5 0xa5 0xbd 0xe5 0x90 0x97]
// wcs    [0x4f60 0x597d 0x5417]
// mbs/gbk[0xc4 0xe3 0xba 0xc3 0xc2 0xf0]

int main()
{
    std::cout << "u16srtombs: " << std::endl;
    {
        std::u16string str = u"zß水🍌";
        learnopengl::print(str);
        std::string s = learnopengl::u16srtombs(str);
        learnopengl::print(s);
    }

    std::cout << "mbsrtoc16s: " << std::endl;
    {
        std::string str = u8"z\u00df\u6c34\U0001F34C";
        learnopengl::print(str);
        std::u16string s = learnopengl::mbsrtou16s(str);
        learnopengl::print(s);
    }

    std::cout << "utf8rtou16s: " << std::endl;
    {
        std::string str = u8"z\u00df\u6c34\U0001F34C";
        learnopengl::print(str);
        std::u16string s = learnopengl::utf8rtou16s(str);
        learnopengl::print(s);
    }

    std::cout << "u16srtoutf8: " << std::endl;
    {
        std::u16string str = u"zß水🍌";
        learnopengl::print(str);
        std::string s = learnopengl::u16srtoutf8(str);
        learnopengl::print(s);
    }

    std::cout << "u32srtombs: " << std::endl;
    {
        std::u32string str = U"zß水🍌";
        learnopengl::print(str);
        std::string s = learnopengl::u32srtombs(str);
        learnopengl::print(s);
    }

    std::cout << "mbsrtoc32s: " << std::endl;
    {
        std::string str = u8"z\u00df\u6c34\U0001F34C";
        learnopengl::print(str);
        std::u32string s = learnopengl::mbsrtou32s(str);
        learnopengl::print(s);
    }

    std::cout << "u32srtoutf8: " << std::endl;
    {
        std::u32string str = U"zß水🍌";
        learnopengl::print(str);
        std::string s = learnopengl::u32srtoutf8(str);
        learnopengl::print(s);
    }

    std::cout << "utf8rtou32s: " << std::endl;
    {
        std::string str = u8"zß水🍌";
        learnopengl::print(str);
        std::u32string s = learnopengl::utf8rtou32s(str);
        learnopengl::print(s);
    }

    std::cout << "wcsrtombs: " << std::endl;
    {
        std::wstring str = L"你好";
        learnopengl::print(str);
        std::string s = learnopengl::wcsrtombs(str);
        learnopengl::print(s);
    }

    std::cout << "mbsrtowcs: " << std::endl;
    {
        std::string str = u8"你好";
        learnopengl::print(str);
        std::wstring s = learnopengl::mbsrtowcs(str);
        learnopengl::print(s);
    }

    std::cout << "wcsrtombs-2: " << std::endl;
    {
        std::wstring str = L"你好";
        learnopengl::print(str);
        std::string s = learnopengl::wcsrtombs(str, 0);
        learnopengl::print(s);
    }

    std::cout << "mbsrtowcs-2: " << std::endl;
    {
        std::string str = u8"你好";
        learnopengl::print(str);
        std::wstring s = learnopengl::mbsrtowcs(str, 0);
        learnopengl::print(s);
    }

    std::cout << "utf8rtowcs: " << std::endl;
    {
        std::string str = u8"你好";
        learnopengl::print(str);
        std::wstring s = learnopengl::utf8rtowcs(str);
        learnopengl::print(s);
    }

    std::cout << "wcsrtoutf8: " << std::endl;
    {
        std::wstring str = L"你好";
        learnopengl::print(str);
        std::string s = learnopengl::wcsrtoutf8(str);
        learnopengl::print(s);
    }

    return 0;
}