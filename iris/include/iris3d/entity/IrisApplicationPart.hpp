/*********************************************************
**  File name : IrisApplicationPart.hpp
**	Iris Engine V0.7 "presque"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/
#ifndef __APPLICATIONPART_HPP__
#define __APPLICATIONPART_HPP__

class IrisApplication;

class IrisApplicationPart
{
public:
	IrisApplicationPart() {_id = 0;}
	bool	operator ==(const IrisApplicationPart& p) const
		{return this == &p;}
	virtual void	Init()													{};
	virtual	uint	Run(const IrisApplication &the_app) {return 0;}; // default 
	virtual void	Desinit()												{};
	virtual void	InitialInit() {};							// called once 
	uint	Id() { return _id;}
protected:
	//Scene		scene;
	uint			_id;
};


#endif
