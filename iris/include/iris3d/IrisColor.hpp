/*********************************************************
**  File name : IrisColor.hpp
**	Iris Engine V0.7 "presque"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/
#ifndef __COLOR_HPP__
#define __COLOR_HPP__

class	IrisColor
{
public:
	union
	{
		struct
		{
			float	_r, _g, _b, _a;
		};
		float	_rgba[4];
	};
	
	IrisColor()
	{
		_r = _g = _b = 0.0f;
	}

	IrisColor(const float r, const float g, const float b, const float a = 0.0f)
	{
		_r = r; _g = g; _b = b; _a = a;
		for (int i = 0; i < 4; i++)
			if (_rgba[i] > 1.0) _rgba[i] = 1.0f;
	}
	IrisColor(uint32 rgba)
	{
		Unpack(rgba);
	}

	void	Print()
	{
		printf("r=%.3f, g=%.3f, b=%.3f a=%.3f\n", (double) _r, (double) _g, (double) _b, (double) _a);
	}
	uint32	Pack() const
	{
		uint32	r, g, b, a;
		r = (uint32) (_r * 255);
		g = (uint32) (_g * 255);
		b = (uint32) (_b * 255);
		a = (uint32) (_a * 255);
		return (a << 24 | r << 16 |  g << 8 | b);
	}

	void	Unpack(uint32 rgba)
	{
		_a = (float) ((rgba >> 24) & 0xff) / 255.0f;
		_r = (float) ((rgba >> 16) & 0xff) / 255.0f;
		_g = (float) ((rgba >> 8) & 0xff) / 255.0f;
		_b = (float) ((rgba) & 0xff) / 255.0f;
	}

	IrisColor& operator += (const IrisColor &col)
	{
		int	i = 4;
		while (i--)
		{
			this->_rgba[i] += col._rgba[i];
			// bound check
			if (this->_rgba[i] > 1.0)
				this->_rgba[i] = 1.0f;
			if (this->_rgba[i] < 0.0)
				this->_rgba[i] = 0.0f;
		}
		return (*this);
	}

	IrisColor operator + (const IrisColor &col)
	{
		IrisColor	ret;
		int	i = 4;
		while (i--)
		{
			ret._rgba[i] = this->_rgba[i] + col._rgba[i];
			// bound check
			if (ret._rgba[i] > 1.0)
				ret._rgba[i] = 1.0f;
			if (ret._rgba[i] < 0.0)
				ret._rgba[i] = 0.0f;
		}
		return (ret);
	}

};
#endif
