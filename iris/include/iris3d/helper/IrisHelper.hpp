/*********************************************************
** File name : IrisHelper.hpp
**  Iris Engine V0.9 "alllaiii"
** Date of creation: 19/02/2002
** Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/

#ifndef __IRIS_HELPER__
#define __IRIS_HELPER__

#include <lwip/lwip.h>

enum network_state
{
  network_none,
  network_send,
  network_receive
};

class IrisHelper
{
public:
  ~IrisHelper();

  static IrisHelper &Get()
  {
    static IrisHelper helper;
    return helper;
  }

  void Init(int num_var, int *dreamcast_ip, int *netmask, int *getaway);
  void Update();
  float GetValue(int index) { return _value[index]; }
  void SetValue(int index, float value, char *name)
  {
    _value[index] = value;
    _value_name[index] = name;
  }
  bool Connect(int *pc_ip, u16_t port);
  struct tcp_pcb *GetPCB() { return _pcb; }

private:
  void SendData();
  struct network_t
  {
    int transmited;
    int size_to_transmit;
    void *buffer;
    network_state state;
    bool to_free;
  } _network;
  static err_t OnConnect(void *arg, struct tcp_pcb *tpcb, err_t err);
  static err_t OnReceive(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);
  static err_t OnPoll(void *arg, struct tcp_pcb *pcb);
  void CloseConnection();
  IrisHelper() {}
  std::vector<float> _value;
  std::vector<char *> _value_name;
  struct ip_addr _dc_ip;
  struct ip_addr _pc_ip;
  struct ip_addr _getaway;
  struct ip_addr _netmask;
  struct tcp_pcb *_pcb;
};

#endif
