#ifndef EVENTHUB_WEBSOCKET_HANDLER_HPP
#define EVENTHUB_WEBSOCKET_HANDLER_HPP

#include "Connection.hpp"
#include "ConnectionWorker.hpp"
#include "websocket/StateMachine.hpp"
#include <memory>

namespace eventhub {
namespace websocket {

class Handler {
public:
  static void process(std::shared_ptr<Connection>& conn, Worker* wrk, char* buf, size_t nBytes);

private:
  Handler(){};
  ~Handler(){};

  static void _handleDataFrame(std::shared_ptr<Connection>& conn, Worker* wrk, StateMachine& fsm);
  static void _handleControlFrame(std::shared_ptr<Connection>& conn, Worker* wrk, StateMachine& fsm);
  static void _handleClientCommand(std::shared_ptr<Connection>& conn, Worker* wrk, const std::string& command, const std::string& args);
};

}
} // namespace eventhub

#endif
