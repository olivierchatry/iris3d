/*********************************************************
**  File name : IrisLightingPipeline.hpp
**	Iris Engine V0.7 "presque"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/
#ifndef __LIGHTINGPIPELINE_H__
#define __LIGHTINGPIPELINE_H__



class	IrisLightingPipeline
{
public:
	void	Compute(vect3d *vin, vertex2dtl *vout, vect3d *normal, int num, uint32 *color);
	 //HT
	void	ComputeInterpol(int fps,int count,vect3d *vin, vertex2dtl *vout, vect3d *normal, int num, uint32 *color,uint32 *color2);
	int		AddLight(IrisLight *light);
	void	DelLight(int id);
	void	DelLight(IrisLight *light);

	IrisLight		*GetLight(int id);
	bool		IsInLight(const vect3d &center, float radius);
	void		SetAmbientColor(const IrisColor &_color);
	IrisColor		&GetAmbientColor(void) {return _ambient_color;}
	uint32	GetAmbientColorPacked(void) {return _ambient_color_packed;}
	unsigned	Count(){return _light.size();}
	void	Update();
	/**
	 *	Delete all light in IrisLightingPipeline
	 **/
	void	Purge()
	{
		_light.resize(0);
	}
protected:
	std::vector<IrisLight *>		_light;
	IrisColor						_ambient_color;
	uint32							_ambient_color_packed;
};

typedef std::vector<IrisLight *>::iterator	LightIterator;

#endif


