/*********************************************************
**  File name : faceclipper.cpp
**	Iris Engine V0.7 "presque"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**					 Maes francis
**           (http://www.epitech.net/labconsole/)
**
**	NEWCLIPPER code by:
**			 Heinrich Tillack (a128.x15.org) 15/11/02
**   based on code by Trilinear, Bero and my own brain :-)
*********************************************************/
/* face clippler */
#include "Iris.hpp"

static inline void *pvr_commit_init(void)
{
    QACR0 = ((((uint32)PVR_TA_INPUT) >> 26) << 2) & 0x1c;
    QACR1 = ((((uint32)PVR_TA_INPUT) >> 26) << 2) & 0x1c;
    return (void*)0xe0000000;
}


inline float lerp(float k, float a, float b)
{
    return a + k * (b - a);
}

#define	ARGB_A(c)	(c>>24)
#define	ARGB_R(c)	((c>>16)&0xff)
#define	ARGB_G(c)	((c>>8)&0xff)
#define	ARGB_B(c)	(c&0xff)

//from bero
inline uint32 lerpcolor(float d, uint32 argb0, uint32 argb1)
{
    //return argb0; /* todo */
    if (argb0 == argb1) {
        return argb0;
    } else {
        int a,r,g,b;
        a = ARGB_A(argb0) + (int)((ARGB_A(argb1) - ARGB_A(argb0))*d);
        r = ARGB_R(argb0) + (int)((ARGB_R(argb1) - ARGB_R(argb0))*d);
        g = ARGB_G(argb0) + (int)((ARGB_G(argb1) - ARGB_G(argb0))*d);
        b = ARGB_B(argb0) + (int)((ARGB_B(argb1) - ARGB_B(argb0))*d);

        return (((a<<8) | r)<<16) | (g<<8) | (b);
    }
}


inline void ClipZ(vertex2dtl *vertnew, vertex2dtl *vert1, vertex2dtl *vert2) {

    float m=(Z_NEAR_VALUE-vert1->z)/(vert2->z - vert1->z);

    vertnew->z = Z_NEAR_VALUE;
    vertnew->x = lerp(m,vert1->x,vert2->x);
    vertnew->y = lerp(m,vert1->y,vert2->y);
    vertnew->u = lerp(m,vert1->u,vert2->u);
    vertnew->v = lerp(m,vert1->v,vert2->v);
    vertnew->oargb = lerpcolor(m,vert1->oargb,vert2->oargb);//FIXME

}

static inline void CopyV(vertex2dtl *vertold, vertex2dtl *vertnew) {

    *vertnew = *vertold;
}


inline uint Check_W(vertex2dtl vertold[],uint16 *indis,uint32 count) 
{
    uint result=0;

	while(count--)
        if (vertold[*indis++].flags) 
			result++; 
    return result;
}


inline int CopyVIndis(vertex2dtl vertold[], vertex2dtl *vertnew,uint16 *indis,uint32 count) 
{
    uint result=0;
    while(count--)
	{
        if (vertold[*indis].flags) 
			result++; 
		CopyV(&vertold[*indis++],vertnew++);
    }
    return result;
}


inline int CopyVIndisF(vertex2dtl vertold[], vertex2dtl *vertnew,uint16 *indis,uint32 count) {

    while(count--)
        CopyV(&vertold[*indis++],vertnew++);
}

#define LESS_Z	1	// <z bad vert clipping outcodes
#define GEQUAL_Z 0  // >= z good vert

int PreComputeClipCodes(vertex2dtl v[],uint16 *indices,uint32 nbindices)
{
    uint result=0;
    while(nbindices--){
        if (v[*indices].z < Z_NEAR_VALUE)
		{ //FIXME?!: < 0
            //HACK: I use v.flags for my clipcode var`s
            //because at the Commit stage I do not use "flags"
            v[*indices].flags=LESS_Z;
            result++;
        }
        else
            v[*indices].flags=GEQUAL_Z;
        indices++;
    }
    return result;
}




inline int ZClip(vertex2dtl *vert)
//from KGL based on code by Trilinear?!!!

/* returns 0 for full clip, 3 for triangle output and 4 for quad output
   based on sutherland-hodgeman clip algorithm */
{
    uint i,j;
    vertex2dtl tempvert[4]  __attribute__((aligned(32)));


    //if ( (vert[0].flags||vert[1].flags ||vert[2].flags))
    if(1)
    {/* one or more parts behind camera */
        j=0;
        CopyV(&vert[0],&vert[3]);/* copy first to next after last to make cyclic */

        i=0;
        while(i<3){

            if (vert[i].flags){
                if(!vert[i+1].flags){
                    /* this out, next in */
                    CopyV(&vert[i],&tempvert[j]);
                    ClipZ(&tempvert[j],&vert[i],&vert[i+1]);
                    j++;

                }//else{/* both out, ignore */}
            }else
                if(vert[i+1].flags)
                {
                    /* this in, next out */
                    CopyV(&vert[i],&tempvert[j]);
                    CopyV(&vert[i+1],&tempvert[j+1]);
                    ClipZ(&tempvert[j+1],&vert[i],&vert[i+1]);
                    j+=2;
                }
                else{
                    /* both in */
                    CopyV(&vert[i],&tempvert[j]);
                    j++;
                }
            i++;
        }



        switch (j) {
        case 3:

            CopyV(&tempvert[0],&vert[0]);
            CopyV(&tempvert[1],&vert[1]);
            CopyV(&tempvert[2],&vert[2]);

            return 3;
        case 4:

            CopyV(&tempvert[0],&vert[0]);
            CopyV(&tempvert[1],&vert[1]);
            CopyV(&tempvert[2],&vert[3]);
            CopyV(&tempvert[3],&vert[2]);

            return 4;
        }/* if j==0 we return 0 which we are about to do anyway */

    }else{/* is all in front of camera */
        return 3;
    }

    return 0;/* if it isn't one of the above it is an invalid poly or completely behind camera */
}


void IrisFaceClipper::CommitLerp(volatile pvr_vertex_t *vert,
                             const vect3d& v3dpos, const vect3d& v3dneg,
                             const vertex2dtl& vpos, const vertex2dtl& vneg,
                             bool end)
{
    vertex2dtl i;
#if 1
    // float k = (vpos.z) / (vpos.z - vneg.z);
    // float k = 0;
    // vect3d l;
    i.x = vpos.x + 0.01f;// lerp(k, vpos.x, vneg.x);
    i.y = vpos.y + 0.01f;//  lerp(k, vpos.y, vneg.y);
    i.z = Z_NEAR_VALUE;
    // IrisContext::Get().GetGeometryPipeline().Transform(l, &i);
    i.u = vpos.u;//lerp(k, vpos.u, vneg.u);//LerpMapping(k, vpos.u, vneg.u);
    i.v = vpos.v;//lerp(k, vpos.v, vneg.v);//LerpMapping(k, vpos.v, vneg.v);
    i.oargb = vpos.oargb;//LerpColor(k, vpos.oargb, vneg.oargb);
    //printf("y en a marre: %f %f %f %f\n", vneg.x, k, i.x, i.z );
#else
    //MODIFIES by Heinrich Tillack
    float d= ( vneg.z-vpos.z);

    float k;
    if(d) k= (Z_NEAR_VALUE-vpos.z) /d;
    else k=1;

    i.x = lerp(k, vpos.x, vneg.x);
    i.y = lerp(k, vpos.y, vneg.y);
    i.z = Z_NEAR_VALUE;

    i.u = lerp(k, vpos.u, vneg.u);//LerpMapping(k, vpos.u, vneg.u);
    i.v = lerp(k, vpos.v, vneg.v);//LerpMapping(k, vpos.v, vneg.v);
    i.oargb = vpos.oargb;//LerpColor(k, vpos.oargb, vneg.oargb);

#endif
    Commit(vert, i, end);
}

/*
******************************************************************
*/



void	IrisFaceClipper::Draw(vect3d v3d[], vertex2dtl v[], uint16 *indices, uint32 nbindices)
{




    volatile pvr_vertex_t	*state = (pvr_vertex_t*)pvr_commit_init();

	#ifndef NEWCLIPPER

    uint32		last_pts[2];
    bool		lastpos = false;
    int			sp = 0;
    uint32 i = 0;

    while (i < nbindices)
    {
        bool newpos = v[*indices].z > Z_NEAR_VALUE;
        bool is_end = i == nbindices - 1;
        if (newpos)
        {
            if (lastpos)	/* pos -> pos: just commit */
                Commit(state, v[*indices], is_end);
            else			/* neg -> pos: empty stack and clip */
            {
                for (int c = 0; c < sp; ++c)
                    CommitLerp(state, v3d[*indices], v3d[last_pts[c]], v[*indices], v[last_pts[c]],false);
                Commit(state, v[*indices], is_end);
            }
            sp = 0;
        }
        else
        {
            if (lastpos)	/* pos -> neg: move neg to clip(pos, neg)*/
            {
                //is_end = is_end || (i < nbindices - 2 && v[*(indices + 1)].z < 0 && v[*(indices + 2)].z < 0);
                is_end = true;
                CommitLerp(state, v3d[*(indices - 1)], v3d[*indices], v[*(indices - 1)], v[*indices], is_end);
            }
            else			/* neg -> neg: push vertex in stack, and ignore */
            {
                if (sp < 2)
                    last_pts[sp++] = *indices;
                else
                {
                    last_pts[0] = last_pts[1];
                    last_pts[1] = *indices;
                }
            }
        }
        lastpos = newpos;
        i++;
        indices++;
    }

	#else

    // new BUGFIXED IrisFaceClipper by Heinrich Tillack (http://a128.x15.org).
    // based on code by Trilinear, Bero and my own brain :-)


    uint32		n;

    vertex2dtl vtmp[4],clipbuffer[4] __attribute__((aligned(32)));


    //precompute clipping codes
    n=PreComputeClipCodes(v,indices,nbindices);


    #if 0

    //quick hack of tristrip clipper
    //breaks strips into tris
    //do the first triangle strip

    CopyVIndisF(v,clipbuffer,indices,3);

    Commit_n(state,clipbuffer, ZClip(clipbuffer));

    end=3;
    indices++;

    //now do the rest
    while(end<nbindices){

        CopyVIndisF(v,clipbuffer,indices,3);

        Commit_n(state,clipbuffer,ZClip(clipbuffer));

        end++;
        indices++;
    }

    #else

    //optimized  tristrip clipper
    //commits quads

    if(n==0){

        Commit_n(state,v,indices,nbindices);


    }else if(n!=nbindices)
    {

        uint i=nbindices-2;

        while(i>1){
            n=Check_W(v,indices,4);

            if (n==0) {

                Commit_n(state,v,indices,4);

            } else if (n!=4) {

                CopyVIndisF(v,clipbuffer,indices,4);

                CopyV(&clipbuffer[1],&vtmp[0]);
                CopyV(&clipbuffer[2],&vtmp[1]);
                CopyV(&clipbuffer[3],&vtmp[2]);

                Commit_n(state,clipbuffer, ZClip(clipbuffer));
                Commit_n(state,vtmp, ZClip(vtmp));

            }

            i-=2;
            indices+=2;
        }

        if(i){
            n=Check_W(v,indices,3);

            if (n==0) {
                Commit_n(state,v,indices,3);

            } else if (n!=3) {
                CopyVIndisF(v,clipbuffer,indices,3);
                Commit_n(state,clipbuffer, ZClip(clipbuffer));
            }

        }

	#endif
}
	#endif
}
