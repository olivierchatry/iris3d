/*********************************************************
**  File name : IrisContext.hpp
**	Iris Engine V0.7 "presque"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/

#ifndef __CONTEXT_HPP__
#define __CONTEXT_HPP__

extern char	romdisk[];

typedef enum shading_type
{
	flat = PVR_SHADE_FLAT,
	gauraud	= PVR_SHADE_GOURAUD
};
typedef enum culling_type
{
	none = PVR_CULLING_NONE,
	small = PVR_CULLING_SMALL,
	ccw = PVR_CULLING_CCW,
	cw = PVR_CULLING_CW
};
typedef enum depth_fct
{
	never = PVR_DEPTHCMP_NEVER,
	lesser = PVR_DEPTHCMP_LESS,
	equal = PVR_DEPTHCMP_EQUAL,
	less_or_equal = PVR_DEPTHCMP_LEQUAL,
	greater = PVR_DEPTHCMP_GREATER,
	not_equal = PVR_DEPTHCMP_NOTEQUAL,
	greater_or_equal = PVR_DEPTHCMP_GEQUAL,
	always = PVR_DEPTHCMP_ALWAYS
};

typedef enum blend_type
{
	zero = PVR_BLEND_ZERO,
	one = PVR_BLEND_ONE,
	dest_color = PVR_BLEND_DESTCOLOR,
	inv_dest_color = PVR_BLEND_INVDESTCOLOR,
	src_alpha = PVR_BLEND_SRCALPHA,
	inv_src_alpha = PVR_BLEND_INVSRCALPHA,
	dest_alpha = PVR_BLEND_DESTALPHA,
	inv_dest_alpha = PVR_BLEND_INVDESTALPHA
};

typedef enum filter_type
{
	point = PVR_FILTER_NONE,
	nearest = PVR_FILTER_NEAREST,
	bilinear = PVR_FILTER_BILINEAR,
	trilinear1 = PVR_FILTER_TRILINEAR1,
	trilinear2 = PVR_FILTER_TRILINEAR2
};

typedef enum texture_env_type
{
	replace = PVR_TXRENV_REPLACE,
	modulate = PVR_TXRENV_MODULATE,
	decal = PVR_TXRENV_DECAL,
	modulate_alpha = PVR_TXRENV_MODULATEALPHA
};

class IrisContext
{
protected:
    pvr_poly_cxt_t		_cxt;
    pvr_poly_hdr_t		_hdr;

	// pipeline
	IrisGeometryPipeline	_geom_pipeline;
    IrisLightingPipeline	_light_pipeline;
	// current material
    IrisMaterial			*_material;
    IrisInputManager		_input_manager;
    IrisMaterialManager		_material_manager;
    IrisObjectManager		_object_manager;
	filter_type				_filter;
	blend_type				_src_blend;
	blend_type				_dst_blend;
	texture_env_type		_env;
	bool					_dirty_header;
	bool					_dirty_material;
public:
	// render state
	void	SetTexEnv(texture_env_type e)
	{
		_env = e;
		if (_material)
		{
			pvr_poly_cxt_t	*cxt = _material->GetCxt();
			_dirty_material |= cxt->txr.env != (int) e;
			cxt->txr.env = (int) e;
		}
	}

	void	SetFilterMode(filter_type f)
	{
		_filter = f;
		if (_material)
		{
			pvr_poly_cxt_t	*cxt = _material->GetCxt();
			_dirty_material |= cxt->txr.filter != (int) f;
			cxt->txr.filter = (int) f;
		}
	}

	void	SetBlendSrc(blend_type b)
	{
		_dirty_header |= _cxt.blend.src != (int) b;
		_src_blend = b;
		_cxt.blend.src = (int) b;
	}

	void	SetBlendDst(blend_type b)
	{
		_dirty_header |= _cxt.blend.dst != (int) b;
		_dst_blend = b;
		_cxt.blend.dst = (int) b;
	}


	void	SetShading(shading_type s)
	{
		_dirty_header |= _cxt.gen.shading != (int) s;
		_cxt.gen.shading = (int) s;
	}

	void	SetCulling(culling_type c)
	{
		_dirty_header |= _cxt.gen.culling != (int) c;
		_cxt.gen.culling = (int) c;
	}

	void	SetDepthWrite(bool enabled)
	{
		int	set_value = enabled ? PVR_DEPTHWRITE_ENABLE : PVR_DEPTHWRITE_DISABLE;
		_dirty_header |= set_value != _cxt.depth.write;
		_cxt.depth.write = set_value;
	}

	void	SetDepthCmpFct(depth_fct f)
	{
		_dirty_header |= _cxt.depth.comparison != (int) f;
		_cxt.depth.comparison = (int) f;
	}

	//! SetScissorTest
	/*!
		Enable scissort test. Ie the powervr will cut
		the image displayed into the screen using 
		coordinate passe to SetScissor.
	*/
	void	SetScissorTest(bool enabled)
	{
		int	set_value = enabled ? PVR_USERCLIP_INSIDE  : PVR_USERCLIP_DISABLE;
		_dirty_header |= _cxt.gen.clip_mode != set_value;		
		_cxt.gen.clip_mode = set_value;
	}

	~IrisContext()
	{
        printf("[INFO] IrisContext destroyed\n");
    }

    static IrisContext &Get()
    {
        static	IrisContext	the_context;
        return	the_context;

    }
    
	void	SendPvrHeader()
	{
		pvr_poly_compile(&_hdr, &_cxt);
		pvr_prim(&_hdr, sizeof(pvr_poly_hdr_t));		
		_dirty_header = _dirty_material = false;
	}
	void				SetMaterial(IrisMaterial *material)
    {	
		_dirty_material |= _material != material;
		_material = material;
	}
    
	IrisMaterial	*GetMaterial()
    {
		return _material;
	}
    void				SetCurrentMaterial()
    {	
		if (_material)
			memcpy(&_cxt.txr, &(_material->GetCxt()->txr), sizeof(_cxt.txr));
	}

    void				UnloadTexture(pvr_poly_cxt_t *cxt)
    {
		pvr_mem_free(cxt->txr.base);
	}
	
	uint32				LoadPNGFile(char *file_name,  pvr_poly_cxt_t &cxt, TextureType type = texture_noalpha);
    void				InitContext(int vertex_buffer_size);

    void				DrawPrimitive(vertex2dtl v[], uint16 *indices, uint32 num_face,const bool mode2d=false);
    void				DrawPrimitiveIntersect(vect3d v3d[], vertex2dtl v[], uint16 *indices, uint32 num_face);

    void				RenderVertex(const vect3d &vin);

    void				ProcessPrimitive(vect3d vin[], vertex2dtl vout[], uint32 num, uint32 *color)
    {
        uint32 col = _light_pipeline.GetAmbientColorPacked();
        while (num --)
        {
            register uint32 value = *color;
            _geom_pipeline.Transform(*(vin++), vout);
			register uint32 vtmp1 = ((value & 0xff) + (col & 0xff));
			register uint32 vtmp2 = ((value & 0xff00) + (col & 0xff00));
			register uint32 vtmp3 = ((value & 0xff0000) + (col & 0xff0000));
			vtmp1 = vtmp1 > 0xff ? 0xff : vtmp1;
			vtmp2 = vtmp2 > 0xff00 ? 0xff00 : vtmp2;
			vtmp3 = vtmp3 > 0xff0000 ? 0xff0000 : vtmp3;
			vout->oargb =	vtmp1 | vtmp2 | vtmp3;							
            vout ++;
            color ++;
        }
    }


    //MODIFIED by Heinrich Tillack
    void				ProcessPrimitiveInterpol(int fps, int count,vect3d vin[],vect3d vin2[], vertex2dtl vout[], uint32 num, uint32 *color)
    {
		uint32 col = _light_pipeline.GetAmbientColorPacked();
		while (num --)
		{
			register uint32 value = *color;
			_geom_pipeline.Transform(*(vin++),*(vin2++), vout,fps,count);
			register uint32 vtmp1 = ((value & 0xff) + (col & 0xff));
			register uint32 vtmp2 = ((value & 0xff00) + (col & 0xff00));
			register uint32 vtmp3 = ((value & 0xff0000) + (col & 0xff0000));
			vtmp1 = vtmp1 > 0xff ? 0xff : vtmp1;
			vtmp2 = vtmp2 > 0xff00 ? 0xff00 : vtmp2;
			vtmp3 = vtmp3 > 0xff0000 ? 0xff0000 : vtmp3;
			vout->oargb =	vtmp1 | vtmp2 | vtmp3;							
			vout ++;
			color ++;
		}
    }

	//MODIFIED by Heinrich Tillack
    void				ProcessPrimitiveInterpol(int fps, int count,vect3d vin[],vect3d vin2[], vertex2dtl vout[], uint32 num)
    {
        while (num --)
            _geom_pipeline.Transform(*(vin++),*(vin2++), vout ++,fps,count);
    }

    void				ProcessPrimitive(vect3d vin[], vertex2dtl vout[], uint32 num)
    {
        while (num --)
            _geom_pipeline.Transform(*(vin++), vout ++);
    }

    void				ProcessPrimitive(vect3d vin[], vect3d vout[], uint32 num)
    {
        while (num --)
            _geom_pipeline.Transform(*(vin++), vout ++);
    }

    //MODIFIED by Heinrich Tillack
     void				ProcessPrimitive(int fps, int count,vect3d vin[],vect3d vin2[], vect3d vout[], uint32 num)
    {
        while (num --)
            _geom_pipeline.Transform(*(vin++),*(vin2++), vout ++,fps,count);
    }

    void				ProcessNormal(vect3d vin[], vect3d vout[], uint32 num)
    {
        while (num --)
            _geom_pipeline.Transform(*(vin++), vout ++);
    }

    //MODIFIED by Heinrich Tillack
    void				ProcessNormal(int fps, int count,vect3d vin[],vect3d vin2[], vect3d vout[], uint32 num)
    {
       while (num --)
            _geom_pipeline.Transform(*(vin++),*(vin2++), vout ++,fps,count);
    }

	IrisGeometryPipeline	&GetGeometryPipeline(){return _geom_pipeline;}
    IrisLightingPipeline	&GetLightingPipeline(){return _light_pipeline;}
    IrisMaterialManager		&GetMaterialManager(){return _material_manager;}
    IrisInputManager			&GetInputManager(){return _input_manager;}
    IrisObjectManager		&GetObjectManager() {return _object_manager;}

	void				BeginScene()
    {
        _input_manager.Update();

		pvr_poly_cxt_col(&_cxt, PVR_LIST_OP_POLY);
		_cxt.gen.culling = PVR_CULLING_NONE;
		pvr_wait_ready();
		pvr_scene_begin();
		pvr_list_begin(PVR_LIST_OP_POLY);
		_dirty_header = _dirty_material = true;
	}

	void				EndScene()
    {
		// send all remaining list to the pvr
		if (_cxt.list_type == PVR_LIST_OP_POLY)
		{
			pvr_list_finish();
			pvr_list_begin(PVR_LIST_TR_POLY);
			pvr_list_finish();
			pvr_list_begin(PVR_LIST_PT_POLY);
		}
		if (_cxt.list_type == PVR_LIST_TR_POLY)
		{
			pvr_list_finish();
			pvr_list_begin(PVR_LIST_PT_POLY);
		}
		pvr_scene_finish();
    }
    
	void				BeginBlending()
    {
		pvr_list_finish();
		pvr_list_begin(PVR_LIST_TR_POLY);
		_cxt.list_type = PVR_LIST_TR_POLY;
		_cxt.gen.alpha = PVR_ALPHA_ENABLE;
		_cxt.txr.alpha = PVR_TXRALPHA_ENABLE;
		_cxt.blend.src = _src_blend;
		_cxt.blend.dst = _dst_blend;
		_dirty_header = _dirty_material = true;
	}
    
	void				SetScissor(int x, int y, int w, int h)
	{
		
	}

	/*
     *	to do ...
     */
    void				BeginPunch()
    {
		pvr_list_finish();
		pvr_list_begin(PVR_LIST_PT_POLY);
		_cxt.list_type = PVR_LIST_PT_POLY;
		_cxt.gen.alpha = PVR_ALPHA_ENABLE;
		_cxt.txr.alpha = PVR_TXRALPHA_ENABLE;
		_cxt.blend.src = _src_blend;
		_cxt.blend.dst = _dst_blend;
		_dirty_header = _dirty_header = true;
    }

private:
    IrisContext()
	{
		// by default
		_filter = bilinear;
		_src_blend = src_alpha;
		_dst_blend = inv_src_alpha;
	}
};
#endif //__CONTEXT_HPP__
