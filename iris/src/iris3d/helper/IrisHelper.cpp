/*********************************************************
** File name : IrisHelper.hpp
**  Iris Engine V0.9 "alllaiii"
** Date of creation: 19/02/2002
** Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/

#ifdef IRIS_HELPER

#include <vector>
#include <helper/IrisHelper.hpp>

void IrisHelper::CloseConnection()
{
	if (_pcb)
	{
		tcp_close(_pcb);
		tcp_recv(_pcb, 0);
	}
	_pcb = 0;
}

void IrisHelper::SendData()
{
	if (_network.size_to_transmit > 0)
	{
		int	max_size = tcp_sndbuf(_pcb);
		int	len = _network.size_to_transmit > max_size ? max_size : _network.size_to_transmit;
		_network.size_to_transmit -= len;
		tcp_write(_pcb, _network.buffer, len, 0);
	}
	if (_network.size_to_transmit == 0)
		_network.state = network_none;
}

err_t IrisHelper::OnPoll(void *arg, struct tcp_pcb *pcb)
{
	IrisHelper *helper = &(IrisHelper::Get());
	if ((helper->_network.state == network_send) 
		&& (helper->_network.size_to_transmit > 0)) 
		helper->SendData();
	if (helper->_network.to_free && helper->_network.size_to_transmit ==  0)
	{
		delete (char*) helper->_network.buffer; 
		helper->_network.buffer = 0;
		helper->_network.to_free = false;
	}
	return ERR_OK;
}

bool IrisHelper::Connect(int pc_ip[4], u16_t port)
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
void IrisHelper::Init(int num_var, int dreamcast_ip[4], int netmask[4], int getaway[4])
{
	IP4_ADDR(&_dc_ip, dreamcast_ip[0], dreamcast_ip[1], dreamcast_ip[2], dreamcast_ip[3]);
	IP4_ADDR(&_netmask, netmask[0], netmask[1], netmask[2], netmask[3]);
	IP4_ADDR(&_getaway, getaway[0], getaway[1], getaway[2], getaway[3]);
	lwip_init_static(&_dc_ip, &_netmask, &_getaway);
	_value.resize(num_var);
	_value_name.resize(num_var);
}


IrisHelper::~IrisHelper()
{
	CloseConnection();
}

err_t IrisHelper::OnConnect(void *arg, struct tcp_pcb *tpcb, err_t err)
{
	IrisHelper	&helper = IrisHelper::Get();
	struct tcp_pcb	*pcb = helper.GetPCB();
	
	size_t	count = helper._value.size();
	// send all value and data name.
	helper._network.transmited = 0;
	helper._network.size_to_transmit = sizeof(count);
	helper._network.state = network_send;
	helper._network.to_free = true;
	for (size_t i = 0; i < count; ++i)
	{
		float	default_value = helper._value[i];
		char	*name = helper._value_name[i];
		char	len  = strlen(name);
		helper._network.size_to_transmit += sizeof(default_value);
		helper._network.size_to_transmit += sizeof(len);
		helper._network.size_to_transmit += len;
	}
	helper._network.buffer = new char[helper._network.size_to_transmit];
	// tcp_write(pcb, &count, sizeof(count), 1);
	// count data to send.
	int	offset = 0;
	char *buffer = (char *) helper._network.buffer;
	memcpy(buffer, &count, sizeof(count));
	offset += sizeof(count);
	for (size_t i = 0; i < count; ++i)
	{
		float	default_value = helper._value[i];
		char	*name = helper._value_name[i];
		char	len  = strlen(name);

		memcpy(buffer + offset, &default_value, sizeof(default_value));
		offset += sizeof(default_value);
		memcpy(buffer + offset, &len, sizeof(len));
		offset += sizeof(len);
		memcpy(buffer + offset, name, len);
		offset += len;
	}
	tcp_poll(pcb, OnPoll, 10);
	tcp_recv(pcb, OnReceive);
	return ERR_OK;
}

err_t IrisHelper::OnReceive(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
	
	if(err == ERR_OK && p != NULL) 
	{
		IrisHelper helper = IrisHelper::Get();
		struct tcp_pcb	*pcb = helper.GetPCB();
		tcp_recved(pcb, p->tot_len);
		int	count = p->tot_len / 8;
		int		*v = (int *) p->payload;
		float	*f = (float *) p->payload;
		for (int i = 0; i < count; ++i)
		{
			int		index	= *(v ++);
			f++;v++;
			helper._value[index] = *(f ++);
		}		
		pbuf_free(p);
	}
	return ERR_OK;
}



#endif
