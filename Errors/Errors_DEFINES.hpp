#pragma once

#include <iostream>
#include <tuple>
#include <fstream>
#include <iomanip>

//out files, can be set in compiling

namespace MLib_Error
{
    enum class Logging_Level { trace = 0 , debug , warning , error , fatal };
    constexpr size_t Logging_Level_Size = 5;
}
using lvl = MLib_Error::Logging_Level;


namespace MLib_Error
{
    //Giving to users
    //Printing them results
    class Log_stream final
    {
    private:
        std::ofstream& out_;

    public:
        //when creating - showing level + file + line
        Log_stream (std::ofstream& out);

        template <typename T>
        void Print (const T& value);
    };


    //Owner of file, changing it
    class Logging final
    {
    public:
        Logging ();

        //we have to know level of warning on compiling
        template <Logging_Level level>
        Log_stream& Log (const std::string& file , size_t line);

        // throwing exception std::runtime_error
        void ChangeDirectory (const std::string& file) throw();

        void SetSize_NameFile (size_t new_size) noexcept;
        void SetSize_NameLine (size_t new_size) noexcept;

    private:
        size_t size_file_;  // default: 15
        size_t size_line_;  // default: 4
        std::ofstream out_; // default: "debug.log"
        Log_stream log_;    // default: out_

        void PrintLevel (Logging_Level level);
        void PrintFileLine (const std::string& file , size_t line);
    };


    //Nothing will be printed
    struct Log_stream_Fake {};


    static Log_stream_Fake FAKE_LOGGING;
    static MLib_Error::Logging MAIN_LOG_OBJ;
}

template<class T>
MLib_Error::Log_stream& operator<<(MLib_Error::Log_stream& log , const T& value)
{
    log.Print (value);
    return log;
}
template<class T>
MLib_Error::Log_stream_Fake& operator<<(MLib_Error::Log_stream_Fake& log , const T& value)
{
    return log;
}

//TEMPLATES
namespace MLib_Error
{
    template <typename T>
    void Log_stream::Print (const T& value)
    {
        out_ << value;
    }

    template <Logging_Level level>
    Log_stream& Logging::Log (const std::string& file , size_t line)
    {
        out_ << std::endl; // user shouldn't think about '\n'

        PrintLevel (level);
        PrintFileLine (file , line);
        return log_;
    }
}

//Useful functions to correct directories, size_file, size_line
namespace LOG
{
    inline void ChangeDirectory (const std::string& file) { MLib_Error::MAIN_LOG_OBJ.ChangeDirectory (file); }
    inline void SetSize_NameFile (size_t new_size) { MLib_Error::MAIN_LOG_OBJ.SetSize_NameFile (new_size); }
    inline void SetSize_NameLine (size_t new_size) { MLib_Error::MAIN_LOG_OBJ.SetSize_NameLine (new_size); }
}