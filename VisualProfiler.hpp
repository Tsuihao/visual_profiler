
#ifndef VISUALPROFILER_HPP
#define VISUALPROFILER_HPP

#include <iostream>
#include <chrono>
#include <string>
#include <algorithm> // std::replace
#include <thread>    // std::thread::id
#include <time.h>    // strftime
#include <mutex>
#include <fstream>

// The marco 
#define PROFILING 1
#ifdef PROFILING
    #define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
    #define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCTION__) // use the function name or __FUNCSIG__ using the function signature
#else
    #define PROFILE_SCOPE(name) /* empty code */
#endif

struct ProfileResult
{
    const std::string name;
    long long start, end;
    uint32_t threadID;
};

class Instrumentor
{
public:
    // delete copy ctor
    Instrumentor(const Instrumentor&) = delete;
    
    // delete copy assignement operator
    Instrumentor& operator = (const Instrumentor&) = delete;

    ~Instrumentor()
    {
        endSession();
    }

    void beginSession(const std::string& name, const std::string& filepath_prefix = "profiling")
    {
        if(m_hasActiveSession) { endSession(); }
        m_hasActiveSession = true;

        // append date and time in the output file
        const std::string& currTime = getCurrTime();
        std::string filepath = filepath_prefix + "_" + currTime + ".json";
        
        m_outputStream.open(filepath);
        writeHeader();
        m_sessionName = name;
    }

    void endSession()
    {
        if(!m_hasActiveSession) {return;}
        m_hasActiveSession = false;
        writeFooter();
        m_outputStream.close();
        m_profileCount = 0;
    }

    void writeProfile(const ProfileResult& result)
    {
        std::lock_guard<std::mutex> lock(m_lock);

        if(m_profileCount++ > 0)
            m_outputStream << ",";

        std::string name = result.name;
        std::replace(name.begin(), name.end(), '"', '\'');

        m_outputStream << "{";
        m_outputStream << "\"cat\":\"function\",";
        m_outputStream << "\"dur\":" << (result.end - result.start) << ',';
        m_outputStream << "\"name\":\"" << name << "\",";
        m_outputStream << "\"ph\":\"X\",";
        m_outputStream << "\"pid\":0,";
        m_outputStream << "\"tid\":" << result.threadID << ",";
        m_outputStream << "\"ts\":" << result.start;
        m_outputStream << "}";

        m_outputStream.flush();
    }

    void writeHeader()
    {
        m_outputStream << "{\"otherData\": {},\"traceEvents\":[";
        m_outputStream.flush();
    }

    void writeFooter()
    {
        m_outputStream << "]}";
        m_outputStream.flush();
    }

    // singleton pattern
    static Instrumentor& get()
    {
        static Instrumentor instance;
        return instance;
    }

protected:

    // helper function for the time class
    const std::string getCurrTime()
    {
        std::time_t t = time(NULL);
        std::tm *now = localtime(&t);
        char buf[20];
        strftime(buf, sizeof(buf), "%m-%d.%X", now);
        return buf;
    }

private:
    // make ctor private
    Instrumentor():m_sessionName("None"), m_profileCount(0), m_hasActiveSession(false){}

    std::string     m_sessionName;
    std::ofstream   m_outputStream;
    int             m_profileCount;
    std::mutex      m_lock;
    bool            m_hasActiveSession;
};


class InstrumentationTimer
{
public:
    InstrumentationTimer(const std::string name):m_result( {name, 0, 0, 0} ), m_isStopped(false)
    {
        m_startTimestamp = std::chrono::high_resolution_clock::now();
    }

    ~InstrumentationTimer()
    {
        if(!m_isStopped)
            stop();
    }

    void stop()
    {
        auto endTimestamp = std::chrono::high_resolution_clock::now();
        m_result.start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimestamp).time_since_epoch().count();
        m_result.end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimestamp).time_since_epoch().count();
        m_result.threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
        Instrumentor::get().writeProfile(m_result);

        m_isStopped = true;
    }

private:
    ProfileResult m_result;
    bool m_isStopped;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimestamp;
};

#endif