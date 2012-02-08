#ifndef TID_MESSAGE_H
#define TID_MESSAGE_H

#include <string>

namespace TiD
{

//-----------------------------------------------------------------------------
class TiDMessage
{
public:
    TiDMessage (std::string const& version, std::string const& type,
                std::string const& parameters, std::string const& content)
      : version_ (version), type_ (type), parameters_ (parameters),
        content_ (content)
      {}

    virtual ~TiDMessage () {}

    std::string getVersion () const {return version_;}
    std::string getType () const {return type_;}
    std::string getParameters () const {return parameters_;}
    std::string getContent () const {return content_;}

private:
    std::string version_;
    std::string type_;
    std::string parameters_;
    std::string content_;
};

}

#endif // TID_MESSAGE_H
