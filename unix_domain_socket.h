/*
* EdgeVPNio
* Copyright 2020, University of Florida
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/
#ifndef UNIX_SOCKET_DOMAIN_H_
#define UNIX_SOCKET_DOMAIN_H_
#include "tincan_base.h"

#include "rtc_base/platform_thread.h"
#include "rtc_base/third_party/sigslot/sigslot.h"
#include "rtc_base/logging.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <cstddef>


namespace tincan {
class UnixDomainSocket
{
public:
  static const char* const SOCKET_PATH_NAME;
  UnixDomainSocket();
  virtual ~UnixDomainSocket();
  virtual int Create();
  virtual int Listen();
  virtual int Accept();
  virtual int Receive(int client_fd_, char* buf);
  virtual int Send(int client_fd_, char* buf);
  virtual void Close();
  //sigslot::signal1<string> SignalRecv;
  //sigslot::signal1<string> SignalSend;

  int socket_fd_, client_fd_;
  struct sockaddr_un bind_addr_;
};
} //namespace tincan
#endif  // UNIX_SOCKET_DOMAIN_H_
