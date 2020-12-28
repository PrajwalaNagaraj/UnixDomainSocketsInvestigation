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
#ifndef DELEGATE_H_
#define DELEGATE_H_
#include "tincan_base.h"
#include "unix_domain_socket.h"

#include "rtc_base/platform_thread.h"
#include "rtc_base/third_party/sigslot/sigslot.h"
#include "rtc_base/logging.h"

namespace tincan {
class Delegate :
  public UnixDomainSocket
{
public:
  // Main implementation logic for unix socket handling.
  void startSocketFunction(void *obj);
  int Create() override;
  int Listen() override;
  int Accept() override;
  int Receive(int client_fd, char* buf) override;
  int Send(int client_fd, char* buf) override;
  void Close() override;
protected:
  std::mutex uskt_mutex_; //mutex used to lock and synchronize conditional variable for exit condition
  std::condition_variable skt_cond_; //Conditional variable to notify the exit condition with Quit() from ctrl_listener
  bool skt_stop_ = false; //condition used to exit startSocketFunction
};
} //namespace tincan
#endif // DELEGATE_H_
