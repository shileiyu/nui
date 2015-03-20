#include <iostream>
#include <windows.h>
#include <map>
#include "optionparser.h"
#include "gtest/gtest.h"

int Encode(const wchar_t * widechar,	/*source*/
    const int widechar_size,	/*source size in character*/
    char * multibyte,			/*destnation*/
    int multibyte_size)
{
    int written = WideCharToMultiByte(CP_UTF8, 0, widechar, widechar_size,
        multibyte, multibyte_size, NULL, NULL);
    if (written > 0 && written < multibyte_size)
        multibyte[written] = 0;
    return written;
}

class ParserOption
{
private:
    static const int kInvalidIndex = -1;
public:
    static char ** UTF8CommandLineFromUnicode(int argc, wchar_t ** argv);
    static void FreeUTF8CommandLine(int argc, char ** argv);
public:
    ParserOption();
    virtual ~ParserOption();

    //分析命令行参数
    bool Parser(int argc, char ** argv);
    bool GetOpt(const char * command,
        char * value,
        const int max_size);
    bool HasOpt(const char * command);
protected:
    int GetIndex(const char * command);
    virtual const option::Descriptor * GetUsage();
private:
    option::Option * options_;
    option::Option * buffer_;
    std::map<int, const char *> param_;
};





char ** ParserOption::UTF8CommandLineFromUnicode(int argc, wchar_t ** argv)
{
    char ** ans_argv = new char *[argc];
    for (int i = 0; i < argc; ++i)
    {
        int tmp_len = wcslen(argv[i]) * 3;
        char * tmp = new char[tmp_len];

        Encode(argv[i], wcslen(argv[i]) + 1,
            tmp, tmp_len);
        ans_argv[i] = tmp;
    }
    return ans_argv;
}

void ParserOption::FreeUTF8CommandLine(int argc, char ** argv)
{
    for (int i = 0; i < argc; ++i)
    {
        delete argv[i];
    }
    delete argv;
}

ParserOption::ParserOption()
:options_(0), buffer_(0)
{
    ;
}


ParserOption::~ParserOption()
{
    if (options_)
        delete[]options_;
    if (buffer_)
        delete[]buffer_;
}

const option::Descriptor * ParserOption::GetUsage()
{
    static const option::Descriptor usage[] =
    {
        { 1, 0, "m", "mode", option::Arg::Optional, "" },
        { 0, 0, 0, 0, 0, 0 }
    };
    return usage;
}

bool ParserOption::Parser(int argc, char ** argv)
{
    param_.clear();
    if (options_)
        delete[] options_;
    if (buffer_)
        delete[] buffer_;

    option::Stats stats(GetUsage(), argc, argv);
    options_ = new option::Option[stats.options_max];
    buffer_ = new option::Option[stats.buffer_max];

    option::Parser parse(GetUsage(), argc, argv, options_, buffer_);

    if (parse.error() == false)
    {
        for (int i = 0; i < parse.optionsCount(); ++i)
        {
            option::Option& opt = buffer_[i];
            param_[opt.index()] = opt.arg;
        }
        return true;
    }
    return false;
}

bool ParserOption::GetOpt(const char * command,
    char * value,
    const int max_size)
{
    auto iter = param_.find(GetIndex(command));
    if (iter != param_.end())
    {
        const char * tmp = iter->second;
        if (max_size > (int)strlen(tmp))
        {
            memcpy(value, tmp, strlen(tmp) + 1);
            return true;
        }
        else
        {
            assert(0);
            return false;
        }
    }
    return false;
}

bool ParserOption::HasOpt(const char * command)
{
    int index = GetIndex(command);
    if (index != kInvalidIndex)
    {
        return param_.find(index) != param_.end();
    }
    return false;
}

int ParserOption::GetIndex(const char * command)
{
    const option::Descriptor * usage = GetUsage();
    int index = kInvalidIndex;
    while (!((usage->shortopt == 0) && (usage->longopt == 0)))
    {
        if (strcmp(command, usage->longopt) == 0 ||
            strcmp(command, usage->shortopt) == 0)
        {
            index = usage->index;
            break;
        }
        usage++;
    }
    return index;
}

bool ParseCommandLine(char * command, int size)
{
    int argc = 0;
    wchar_t **  argv = CommandLineToArgvW(::GetCommandLineW(), &argc);

    bool skip = false;
    wchar_t file_path[MAX_PATH];
    GetModuleFileName(NULL, file_path, MAX_PATH);
    if (wcsstr(file_path, *argv) != 0)
    {//根据不同的启动情况 选择跳过首个命令行参数
        skip = true;
    }
    int utf8_argc = skip ? argc - 1 : argc;
    wchar_t ** tmp_argv = skip ? argv + 1 : argv;

    char ** utf8_argv =
        ParserOption::UTF8CommandLineFromUnicode(
        utf8_argc, tmp_argv);
    ::LocalFree(argv);

    ParserOption parser_option;
    if (parser_option.Parser(utf8_argc, utf8_argv))
    {
        if (parser_option.HasOpt("mode"))
        {
            parser_option.GetOpt("mode", command, size);
            ParserOption::FreeUTF8CommandLine(utf8_argc, utf8_argv);
            return true;
        }
    }
    memset(command, 0, size);
    ParserOption::FreeUTF8CommandLine(utf8_argc, utf8_argv);
    return false;
}

int GUIMain();

GTEST_API_ int main(int argc, char ** argv)
{

    char command[256];

    ParseCommandLine(command, 256);
    int exit_code = 0;

    if (strlen(command) && strcmp(command, "gui") == 0)
    {
        GUIMain();
    }
    else
    {
        std::cout << "Running main() from gtest_main.cc\n";

        testing::InitGoogleTest(&argc, argv);
        exit_code = RUN_ALL_TESTS();
        getchar();
    }

    return exit_code;
}