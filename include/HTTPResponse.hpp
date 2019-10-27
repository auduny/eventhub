#ifndef EVENTHUB_HTTP_RESPONSE_H
#define EVENTHUB_HTTP_RESPONSE_H

#include <string>
#include <unordered_map>

typedef std::unordered_map<std::string, std::string> HeaderList_t;

class HTTPResponse {
public:
  HTTPResponse(int statusCode = 200, const std::string body = "");
  void setStatus(int status, std::string statusMsg);
  void setStatus(int status);
  void setHeader(const std::string& name, const std::string& value);
  void setBody(const std::string& data);
  void appendBody(const std::string& data);
  const std::string getStatusMsg(int statusCode);
  const std::string get();

private:
  int _statusCode;
  std::string _statusMsg;
  std::string _body;
  HeaderList_t _headers;
};

#endif
