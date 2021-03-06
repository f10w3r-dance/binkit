#pragma warning(disable:4996)
#pragma warning(disable:4200)
#include <ios>
#include <sstream>
#include <iomanip>
#include "Utility.h"

#include <exception>
#include <iostream>
#include <fstream>

#include <boost/log/trivial.hpp>
#include <boost/log/common.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/utility/setup/from_stream.hpp>
#include <boost/format.hpp> 
#include <boost/log/trivial.hpp>

namespace logging = boost::log;
namespace attrs = boost::log::attributes;

unsigned char HexToByte(char * hexString)
{
    unsigned char returnValue = 0;
    for (int i = 0; hexString[i] && i < 2; i++)
    {
        unsigned char currentByte = 0;
        char c = hexString[i];
        if ('0' <= c && c <= '9')
        {
            currentByte = c - '0';
        }
        else if ('a' <= c && c <= 'f')
        {
            currentByte = c - 'a' + 10;
        }
        else if ('A' <= c && c <= 'F')
        {
            currentByte = c - 'A' + 10;
        }
    
        returnValue = returnValue *16 + currentByte;
    }
    return returnValue;
}

vector<unsigned char> HexToBytes(char *hexString)
{
    vector<unsigned char> bytes;
    for (int i = 0; i < strlen(hexString); i += 2)
    {
        bytes.push_back(HexToByte(hexString + i));
    }

    return bytes;
}

string BytesToHexString(vector<unsigned char> bytes)
{
    std::stringstream stringStream;
    for (unsigned char ch : bytes)
    {
        stringStream << std::setfill('0') << std::setw(sizeof(unsigned char) * 2) << std::hex << (int)(ch);
    }

    return stringStream.str();
}

string BytesToHexString(unsigned char *bytes, int length)
{
    std::stringstream stringStream;
    for (int i = 0; i < length; i++)
    {
        stringStream << std::setfill('0') << std::setw(sizeof(unsigned char) * 2) << std::hex << (int)(bytes[i]);
    }

    return stringStream.str();
}

bool LoadLogSettings(string filename)
{
    BOOST_LOG_TRIVIAL(debug) << boost::format("log setting filename: %s") % filename.c_str();

    try
    {
        std::ifstream settings(filename);
        if (!settings.is_open())
        {
            std::cout << "Could not open " << filename << " file" << std::endl;
            logging::core::get()->set_logging_enabled(false);
            return false;
        }
        logging::init_from_stream(settings);
        logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());
        return true;
    }
    catch (std::exception& e)
    {
        logging::core::get()->set_logging_enabled(false);
        std::cout << "Failure: " << e.what() << std::endl;
        return false;
    }
}
