/*********************************************************
**  File name : faceclipper.hpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**					 Maes francis
**           (http://www.epitech.net/labconsole/)
*********************************************************/
#ifndef __FACECLIPPER_HPP__
#define  __FACECLIPPER_HPP__

//comment this to use the old Faceclipper code from Oliver
#ifdef NEWCLIPPER
	#undef NEWCLIPPER
#endif

const float Z_NEAR_VALUE = 0.001f;


class IrisFaceClipper
{
public:
	static void Draw( vect3d v3d[], vertex2dtl v[], uint16 *indices, uint32 nbindices);
private:

	static inline void Commit(volatile pvr_vertex_t *vert, const vertex2dtl& v, bool end = false)
	{
		vert->flags = end ? PVR_CMD_VERTEX_EOL : PVR_CMD_VERTEX;
		float rhw=1/v.z;
		vert->x = v.x*rhw;
		vert->y = v.y*rhw;
		vert->z = rhw;
		vert->u = v.u;
		vert->v = v.v;
		vert->argb = v.oargb;
		vert->oargb = 0xFF000000;
		pvr_dr_commit(vert);
		vert = (pvr_vertex_t*) ((uint32) vert ^ 32);
	}

	static inline void Commit_n(volatile pvr_vertex_t *vert,vertex2dtl vpos[],uint16 *indis,uint32 n)
	{
		while (n--)
		{
			vertex2dtl		*the_vertex = vpos + *indis;
			indis++;
			vert->flags = (n==0) ? PVR_CMD_VERTEX_EOL : PVR_CMD_VERTEX;
			float rhw=1/the_vertex->z;
			vert->x = the_vertex->x*rhw;
			vert->y = the_vertex->y*rhw;
			vert->z = rhw;
			vert->u = the_vertex->u;
			vert->v = the_vertex->v;
			vert->argb =  the_vertex->oargb;
			vert->oargb = 0xff000000;
			pvr_dr_commit(vert);
			vert = (pvr_vertex_t*) ((uint32) vert ^ 32);
		}
	}

	static inline void Commit_n(volatile pvr_vertex_t *vert,vertex2dtl vpos[],uint32 n)
	{
		while (n--)
		{
			vertex2dtl		*the_vertex = vpos ++;
			vert->flags = (n==0) ? PVR_CMD_VERTEX_EOL : PVR_CMD_VERTEX;
			float rhw=1/the_vertex->z;
			vert->x = the_vertex->x*rhw;
			vert->y = the_vertex->y*rhw;
			vert->z = rhw;
			vert->u = the_vertex->u;
			vert->v = the_vertex->v;
			vert->argb =  the_vertex->oargb;
			vert->oargb = 0xff000000;
			pvr_dr_commit(vert);
			vert = (pvr_vertex_t*) ((uint32) vert ^ 32);
		}
	}


};

#endif /* not __FACECLIPPER_HPP__ */
