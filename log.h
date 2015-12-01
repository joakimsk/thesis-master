#ifndef __LOG_H_INCLUDED__
#define __LOG_H_INCLUDED__
#include <sstream>
#include <boost/format.hpp>
#include <iostream>
#include "globals.h"

using namespace std;

enum log_level_t {
    LOG_NOTHING,
    LOG_CRITICAL,
    LOG_ERROR,
    LOG_WARNING,
    LOG_INFO,
    LOG_DEBUG
};

namespace log_impl {
class formatted_log_t {
public:
    formatted_log_t( log_level_t level, const wchar_t* msg ) : fmt(msg), level(level) {}
    ~formatted_log_t() {
        // GLOBAL_LEVEL is a global variable and could be changed at runtime
        // Any customization could be here
        if ( level <= GLOBAL_LEVEL ) wcout << level << L" " << fmt << endl;
    }        
    template <typename T> 
    formatted_log_t& operator %(T value) {
        fmt % value;
        return *this;
    }    
protected:
    log_level_t		level;
    boost::wformat  	fmt;
};
}//namespace log_impl
// Helper function. Class formatted_log_t will not be used directly.
template <log_level_t level>
log_impl::formatted_log_t log(const wchar_t* msg) {
    return log_impl::formatted_log_t( level, msg );
}
#endif