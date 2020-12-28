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
#include "unix_domain_socket.h"
#include "delegate.h"
#include "rtc_base/platform_thread.h"
#include "rtc_base/third_party/sigslot/sigslot.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/types.h>


namespace tincan
{
using namespace rtc;

void
Delegate::startSocketFunction (void* obj)
{
  UnixDomainSocket* inst =  static_cast<UnixDomainSocket*>(obj);
  char* buffer = new char[1492];
  std::unique_lock<std::mutex> lg(uskt_mutex_);
  while (!skt_stop_) {
    inst->Receive(client_fd_, buffer);
    //inst->signalRecv(buffer);
    skt_cond_.wait(lg);
  }

}

int
Delegate::Create()
{
  // Creating socket
  socket_fd_ = socket(AF_UNIX, SOCK_SEQPACKET, 0);
  if (socket_fd_ == -1) {
    RTC_LOG(LS_ERROR) << "Creation of socket failed\n";
     return socket_fd_;
  }

  memset(&bind_addr_, 0, sizeof(&bind_addr_));

  //Bind socket to socket path
  bind_addr_.sun_family = AF_UNIX;
  strncpy(bind_addr_.sun_path, SOCKET_PATH_NAME, sizeof(bind_addr_.sun_path)-1);

  int status = bind(socket_fd_, reinterpret_cast<sockaddr*>(&bind_addr_), sizeof(bind_addr_));
  if (status == -1) {
    RTC_LOG(LS_ERROR) << "Binding of socket failed\n";
    return status;
  }
  return socket_fd_;
}

//Returns listening socket fd on success
int
Delegate::Listen()
{
  int status = listen(socket_fd_, 1);
  if (status == -1) {
    RTC_LOG(LS_ERROR) << "Listen failure\n";
  }
  return status;
}

//Returns accepted client socket fd on success, which should be passed to send, recv calls
int
Delegate::Accept()
{
  client_fd_ = accept4(socket_fd_, nullptr, nullptr, SOCK_NONBLOCK);
  if (client_fd_ == -1) {
    RTC_LOG(LS_ERROR) << "Accept failure\n";
  }
  return client_fd_;
}

//Receives data for the passed client connected fd, within the buffer
int
Delegate::Receive(int cfd, char* buffer)
{
  int status = recv(cfd, &buffer, sizeof(buffer), 0);
  if (status == -1) {
    RTC_LOG(LS_ERROR) << "Receive data failure\n";
  }
  return status;
}

//Sends data to the mentioned client connected fd with passed buffer
int
Delegate::Send(int cfd, char* buffer)
{
  int status = send(cfd,  &buffer, sizeof(buffer), MSG_NOSIGNAL);
  if (status == -1) {
    RTC_LOG(LS_ERROR) << "Send data failure\n";
  }
  return status;
}

void
Delegate::Close(){
  close(client_fd_);
  close(socket_fd_);
}

} //end namespace tincan
