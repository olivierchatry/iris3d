/*********************************************************
** File name : IrisHelper.cpp
** Iris Engine V0.8 "aleeeezzz"
** Date of creation: 19/02/2002
** Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/

#ifndef __IRIS_HELPER__
#define __IRIS_HELPER__

#ifdef IRIS_HELPER
#include <lwip/lwip.h>
class IrisHelper
{
public:
	~IrisHelper();

	static IrisHelper &Get()
	{
		static IrisHelper helper;
		return helper;
	}
	
	void	Init(int num_var, int *dreamcast_ip, int *netmask, int *getaway);
	void	Update();
	float	GetValue(int index){return _value[index];}
	void	SetValue(int index, float value){_value[index] = value;}
	bool	Connect(int *pc_ip, u16_t port);
	struct tcp_pcb	*GetPCB() { return _pcb;}
private: 
	static	err_t OnConnect(void *arg, struct tcp_pcb *tpcb, err_t err)
	{
		tcp_recv(IrisHelper::Get().GetPCB(), OnReceive);
		return ERR_OK;
	}

	static err_t OnReceive(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
	{
		if(err == ERR_OK && p != NULL) 
		{
			struct tcp_pcb	*pcb = IrisHelper::Get().GetPCB();
			tcp_recved(pcb, p->tot_len);
			int	count = p->tot_len / 4;
			int	*v = (int *) p->payload;
			for (int i = 0; i < count; ++i)
			{
				int		index = *(v ++);
				float	vf = IrisHelper::Get().GetValue(index);
				tcp_write(pcb, &vf, 4, 1);
			}		
			pbuf_free(p);	
		}
		return ERR_OK;
	}
	void	CloseConnection();
	IrisHelper() {}
	std::vector<float>	_value;
	struct ip_addr		_dc_ip;
	struct ip_addr		_pc_ip;
	struct ip_addr		_getaway;
	struct ip_addr		_netmask;
	struct tcp_pcb		*_pcb;
};


inline void IrisHelper::CloseConnection()
{
	if (_pcb)
	{
		tcp_close(_pcb);
		tcp_recv(_pcb, 0);
	}
	_pcb = 0;
}

inline bool IrisHelper::Connect(int pc_ip[4], u16_t port)
{
	IP4_ADDR(&_pc_ip, pc_ip[0], pc_ip[1], pc_ip[2], pc_ip[3]);
	CloseConnection();
	_pcb = tcp_new();
	return (tcp_connect(_pcb, &_pc_ip, port, OnConnect) == ERR_OK);
}
/*
 *	Initialise connection beetween pc and the dreamcast. Note that you have to
 *	specify dreamcast ip.
 */
inline void IrisHelper::Init(int num_var, int dreamcast_ip[4], int netmask[4], int getaway[4])
{
	IP4_ADDR(&_dc_ip, dreamcast_ip[0], dreamcast_ip[1], dreamcast_ip[2], dreamcast_ip[3]);
	IP4_ADDR(&_netmask, netmask[0], netmask[1], netmask[2], netmask[3]);
	IP4_ADDR(&_getaway, getaway[0], getaway[1], getaway[2], getaway[3]);
	lwip_init_static(&_dc_ip, &_netmask, &_getaway);
}


inline IrisHelper::~IrisHelper()
{

}
#endif
#endif
