#include "websocket/StateMachine.hpp"

#include <string>
#include "Common.hpp"
#include "websocket/ws_parser.h"

namespace eventhub {
namespace websocket {
static int parserOnDataBegin(void* userData, uint8_t frameType) {
  //DLOG(INFO) << "on_data_begin";
  auto obj = static_cast<StateMachine*>(userData);

  obj->clearPayload();
  return 0;
}

static int parserOnDataPayload(void* userData, const char* buff, size_t len) {
  //DLOG(INFO) << "on_data_payload: " << buff;
  auto obj = static_cast<StateMachine*>(userData);
  obj->appendPayload(buff);
  return 0;
}

static int parserOnDataEnd(void* userData) {
  //DLOG(INFO) << "on_data_end";
  auto obj = static_cast<StateMachine*>(userData);
  obj->setState(StateMachine::WS_DATA_READY);
  return 0;
}

static int parserOnControlBegin(void* userData, uint8_t frameType) {
  auto obj = static_cast<StateMachine*>(userData);
  //DLOG(INFO) << "on_control_begin";
  obj->clearControlPayload();
  obj->setControlFrameType(frameType);
  return 0;
}

static int parserOnControlPayload(void* userData, const char* buff, size_t len) {
  //DLOG(INFO) << "on_control_payload";
  auto obj = static_cast<StateMachine*>(userData);
  obj->appendControlPayload(buff);
  return 0;
}

static int parserOnControlEnd(void* userData) {
  auto obj = static_cast<StateMachine*>(userData);
  obj->setState(StateMachine::state::WS_CONTROL_READY);
  //DLOG(INFO) << "on_control_end";
  return 0;
}

StateMachine::StateMachine() {
  ws_parser_init(&_ws_parser);

  _ws_parser_callbacks = {
      .on_data_begin      = parserOnDataBegin,
      .on_data_payload    = parserOnDataPayload,
      .on_data_end        = parserOnDataEnd,
      .on_control_begin   = parserOnControlBegin,
      .on_control_payload = parserOnControlPayload,
      .on_control_end     = parserOnControlEnd,
  };

  setState(state::WS_PARSE);
}

const StateMachine::state StateMachine::getState() {
  return _state;
}

StateMachine::state StateMachine::setState(state newState) {
  return _state = newState;
}

void StateMachine::clearPayload() {
  _payload_buf.clear();
  setState(state::WS_PARSE);
}

void StateMachine::clearControlPayload() {
  _control_payload_buf.clear();
  setState(state::WS_PARSE);
}

void StateMachine::appendPayload(const char* data) {
  _payload_buf.append(data);
}

void StateMachine::appendControlPayload(const char* data) {
  _control_payload_buf.append(data);
}

void StateMachine::setControlFrameType(uint8_t frameType) {
  _control_frame_type = frameType;
}

const std::string& StateMachine::getPayload() {
  return _payload_buf;
}

const std::string& StateMachine::getControlPayload() {
  return _control_payload_buf;
}

uint8_t StateMachine::getControlFrameType() {
  return _control_frame_type;
}

StateMachine::state StateMachine::process(char* buf, ssize_t len) {
  ws_parser_execute(&_ws_parser, &_ws_parser_callbacks, this, buf, len);
  return _state;
}

}
} // namespace eventhub
