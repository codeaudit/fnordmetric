/**
 * This file is part of the "FnordMetric" project
 *   Copyright (c) 2014 Paul Asmuth, Google Inc.
 *   Copyright (c) 2016 Paul Asmuth, FnordCorp B.V.
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <algorithm>
#include <metricd/util/inspect.h>
#include <metricd/transport/http/httprequest.h>

namespace fnordmetric {
namespace http {

std::string HTTPMessage::kEmptyHeader = "";

const std::string& HTTPMessage::version() const {
  return version_;
}

void HTTPMessage::setVersion(const std::string& version) {
  version_ = version;
}

const std::vector<std::pair<std::string, std::string>>&
    HTTPMessage::headers() const {
  return headers_;
}

const std::string& HTTPMessage::getHeader(const std::string& key) const {
  auto key_low = key;
  std::transform(key_low.begin(), key_low.end(), key_low.begin(), ::tolower);

  for (const auto& header : headers_) {
    if (header.first == key_low) {
      return header.second;
    }
  }

  return kEmptyHeader;
}

bool HTTPMessage::hasHeader(const std::string& key) const {
  auto key_low = key;
  std::transform(key_low.begin(), key_low.end(), key_low.begin(), ::tolower);

  for (const auto& header : headers_) {
    if (header.first == key_low) {
      return true;
    }
  }

  return false;
}

void HTTPMessage::addHeader(const std::string& key, const std::string& value) {
  auto key_low = key;
  std::transform(key_low.begin(), key_low.end(), key_low.begin(), ::tolower);
  headers_.emplace_back(key_low, value);
}

void HTTPMessage::setHeader(const std::string& key, const std::string& value) {
  auto key_low = key;
  std::transform(key_low.begin(), key_low.end(), key_low.begin(), ::tolower);

  for (auto& header : headers_) {
    if (header.first == key_low) {
      header.second = value;
      return;
    }
  }

  headers_.emplace_back(key_low, value);
}

void HTTPMessage::clearHeaders() {
  headers_.clear();
}

const Buffer& HTTPMessage::body() const {
  return body_;
}

void HTTPMessage::addBody(const std::string& body) {
  body_ = Buffer(body.c_str(), body.length());
}

void HTTPMessage::addBody(const void* data, size_t size) {
  body_ = Buffer(data, size);
}

void HTTPMessage::addBody(const Buffer& buf) {
  body_ = buf;
}

void HTTPMessage::appendBody(const void* data, size_t size) {
  body_.append(data, size);
}

void HTTPMessage::clearBody() {
  body_.clear();
}

std::unique_ptr<InputStream> HTTPMessage::getBodyInputStream() const {
  return BufferInputStream::fromBuffer(&body_);
}

std::unique_ptr<OutputStream> HTTPMessage::getBodyOutputStream() {
  return BufferOutputStream::fromBuffer(&body_);
}

}
}

template <>
std::string StringUtil::toString(fnordmetric::http::HTTPMessage::kHTTPMethod method) {
  switch (method) {
    case fnordmetric::http::HTTPMessage::M_CONNECT: return "CONNECT";
    case fnordmetric::http::HTTPMessage::M_DELETE: return "DELETE";
    case fnordmetric::http::HTTPMessage::M_GET: return "GET";
    case fnordmetric::http::HTTPMessage::M_HEAD: return "HEAD";
    case fnordmetric::http::HTTPMessage::M_OPTIONS: return "OPTIONS";
    case fnordmetric::http::HTTPMessage::M_POST: return "POST";
    case fnordmetric::http::HTTPMessage::M_PUT: return "PUT";
    case fnordmetric::http::HTTPMessage::M_TRACE: return "TRACE";
    default: RAISE(kTypeError, "invalid HTTP method");
  }
}

