/*********************************************************
**  File name : spline.hpp
**	Iris Engine V0.7 "presque"
**  Date Of Creation: 07/09/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/

#ifndef __SPLINE_HPP__
#define __SPLINE_HPP__

#define SPLINE_MAGIC	"SFspline100>\0"

class	Spline
{
public:
	/*
	** Constructor, does nothing
	*/
	Spline() {};
	/*
	** Destructor, does memory freeing
	*/
	~Spline() {};


	struct curve_t
	{
		typedef		std::vector<vect3d>		storage;
		typedef		storage::iterator		iterator;
		typedef		storage::const_iterator	const_iterator;
		typedef		std::vector<float>		floats_t;
		storage		v;
		floats_t	adjlen;
		bool		closed;
		float		len;
		void		update_len();
	};
	
	/*	
	**	typedef of spline vector
	*/
	typedef std::vector<curve_t>				spline_t;
	
	/*	
	**	typedef of spline iterator
	*/
	typedef spline_t::iterator	spline_it_t;

	/*
	**	Load a spline from a smousse's exported spline
	**	using smousse plugin
	*/
	bool	LoadFromFile(char *file_name);
	
	/*
	**	Return the begin of the spline vector
	*/
	spline_it_t	begin() { return _spline.begin();}
	
	/*
	**	Return the end of the spline vector
	*/
	spline_it_t end() { return _spline.end();}

	/*
	**	Get spline vector
	*/
	spline_t		&get() { return _spline;}
	const spline_t	&get() const { return _spline;}


	curve_t&		operator [](uint i)				{return _spline[i];}
	const curve_t&	operator [](uint i) const	{return _spline[i];}

protected:
	/*
	**	the spline
	*/
	spline_t	_spline;	// the spline
};

inline void	Spline::curve_t::update_len()
{
	static const float epsilon = 0.0001f;

	len = 0;
	if (adjlen.size() != v.size())
		adjlen.resize(v.size());
	uint i = 0;
	for (; i < v.size() - 1; ++i)
	{
		adjlen[i] = (v[i + 1] - v[i]).Magnitude();
		if (!adjlen[i])
			adjlen[i] = epsilon;
		len += adjlen[i];
	}
	if (closed)
	{
		adjlen[i] = (v.back() - v.front()).Magnitude();
		if (!adjlen[i])
			adjlen[i] = epsilon;
		len += adjlen[i];
	}
}


inline bool Spline::LoadFromFile(char *file_name)
{
	FILE	*file;
	
	Spline	the_spline;

	file = fopen(file_name, "rb");
	if (file == NULL)
		return false;
	
	unsigned size = strlen(SPLINE_MAGIC) + 1;
	char	name[size];
	fread(name, 1, size, file);
	if (strcmp(name, SPLINE_MAGIC) == 0)
	{
		unsigned	sc; // spline count
		fread(&sc, sizeof(unsigned), 1, file);
		_spline.resize(sc);		
		printf("[spline] number of curve = %d\n", sc);
		for (spline_it_t it = _spline.begin(); it != _spline.end(); ++it)
		{
			unsigned cc;
			fread(&cc, sizeof(unsigned), 1, file);
			printf("[curve] number of point = %d\n", cc);
			if (cc)
			{
				it->v.resize(cc);
				Spline::curve_t::iterator itc;
				for (itc = it->v.begin(); itc != it->v.end(); itc++)
					fread((*itc)._pos , sizeof(float), 3, file);
				unsigned tmp;
				fread(&tmp, sizeof(unsigned), 1, file);
				it->closed = tmp == 1;
				it->update_len();
			}
			else
			{
				it->v.resize(0);
			}
		}
			
	}
	fclose(file);
	return true;
}
#endif
