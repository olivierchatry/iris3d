/*********************************************************
**  File name : IrisContext.hpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/

#ifndef __CONTEXT_HPP__
#define __CONTEXT_HPP__

extern char	romdisk[];

/*! Shading type enum
 *	Provide an enumeration of different shading type supported by the dreamcast. 
 */
enum shading_type
{
    ST_FLAT = PVR_SHADE_FLAT, /*!< Flat shading. Polygon are filled with a united color.*/  
    ST_GAURAUD = PVR_SHADE_GOURAUD/*!< Gouraud shading. Polygon are filled by interpolating beetwen vertice color.*/  
};

/*! Culling type enum
 *  Provide en enumeration of different culling type supported by the dreamcast. 
 *  Culling allow backface removing when drawing elements. It's based
 *  on vertice order of the face. ie : 1-2-3 Not culled, 1-3-2 Culled.
 */
enum culling_type
{
	CT_NONE = PVR_CULLING_NONE, /*!< No culling (default). There is no culling test for rendering, each face are drawed.*/  
	CT_SMALL = PVR_CULLING_SMALL, /*!< Don't know what it does*/ 
	CT_CCW = PVR_CULLING_CCW,  /*!< Counter Clock Wise. Face are rendered when vertices order are clock wise (ie: 1-2-3)*/ 
	CT_CW = PVR_CULLING_CW /*!< Clock Wise. Face are rendered when vertices order are counter clock wise (ie: 1-3-2)*/ 
};

//! Depth compare enum
/*! Provide an enumeration of different depth comparaison 
 * used by the dreamcast to reject pxiel drawing using
 * the zbuffer.
 */
enum depth_fct
{
	DF_NEVER = PVR_DEPTHCMP_NEVER,/*!< ZBuffer test always reject pixel drawing.*/  
	DF_LESSER = PVR_DEPTHCMP_LESS,/*!< Draw pixel if it z value is smaller than the zbuffer corresponding value.*/  
	DF_EQUAL = PVR_DEPTHCMP_EQUAL,/*!< Draw pixel if it z value is equal to the zbuffer corresponding value.*/  
	DF_LESS_OR_EQUAL = PVR_DEPTHCMP_LEQUAL, /*!< Draw pixel if it z value is smaller or equal than the zbuffer corresponding value.*/  
	DF_GREATER = PVR_DEPTHCMP_GREATER,/*!< Draw pixel if it z value is greatre than the zbuffer corresponding value.*/  
	DF_NOT_EQUAL = PVR_DEPTHCMP_NOTEQUAL,/*!< Draw pixel if it z value is not equal to the zbuffer corresponding value.*/  
	DF_GREATER_OR_EQUAL = PVR_DEPTHCMP_GEQUAL, /*!< Draw pixel if it z value is greater or equal than the zbuffer corresponding value.*/  
	DF_ALWAYS = PVR_DEPTHCMP_ALWAYS /*!< Always draw pixel.*/  
};

/*! Blending type enum
 * Provide an enumeration of different blending 
 * factor applyed to source or destination pixel.
 * Used for alpha blending.
 * The formula is : 
 *  FinalColor = SourceCololor (Texel) x SourceBlendFactor + DestinationColor (Pixel) x DestBlendFactor
 *  Decomposed :
 *   FinalColor.R = SourceCololor.Rs x SourceBlendFactor.R + DestinationColor.Rd x DestBlendFactor.R
 *   FinalColor.G = SourceCololor.Gs x SourceBlendFactor.G + DestinationColor.Gd x DestBlendFactor.G
 *   FinalColor.B = SourceCololor.Bs x SourceBlendFactor.B + DestinationColor.Bd x DestBlendFactor.B
 *   FinalColor.A = SourceCololor.As x SourceBlendFactor.A + DestinationColor.Ad x DestBlendFactor.A
 */
enum blend_type
{
	BT_ZERO = PVR_BLEND_ZERO, /*!< Blend factor is zero.*/  
	BT_ONE = PVR_BLEND_ONE,/*!< Blend factor is one.*/  
	BT_DEST_COLOR = PVR_BLEND_DESTCOLOR, /*!< Blend factor is destination color (Rd, Gd, Bd, Ad).*/  
	BT_INV_DEST_COLOR = PVR_BLEND_INVDESTCOLOR, /*!< Blend factor is one minus destination color (1 - Rd, 1 - Gd, 1 - Bd, 1 - Ad).*/  
	BT_SRC_ALPHA = PVR_BLEND_SRCALPHA, /*!< Blend factor is source alpha (As, As, As, As).*/  
	BT_INV_SRC_ALPHA = PVR_BLEND_INVSRCALPHA, /*!< Blend factor is one minus source alpha (1 - As, 1 - As, 1 - As, 1 - As).*/  
	BT_DEST_ALPHA = PVR_BLEND_DESTALPHA,/*!< Blend factor is destination alpha (Ad, Ad, Ad, Ad).*/  
	BT_INV_DEST_ALPHA = PVR_BLEND_INVDESTALPHA /*!< Blend factor is one minus destination alpha (1 - Ad, 1 - Ad, 1 - Ad, 1 - Ad).*/  
};

/*! Texture filter type enum
 * Provide an enumeration of different texture filtering technique.
 * \note 
 * Trilinear filtering is a mixing beetween to bilinear filtered MipMap.
 */
enum filter_type
{
	FT_POINT = PVR_FILTER_NONE, /*!< No filtering applyed.*/
	FT_NEAREST = PVR_FILTER_NEAREST,/*!< Apply bilinear filtering to nearest pixel (Using z value) .*/
	FT_BILINEAR = PVR_FILTER_BILINEAR,/*!< Apply bilinear filtering  to all pixel.*/
	FT_TRILINEAR1 = PVR_FILTER_TRILINEAR1, /*!< Apply trilinear filtering to all pixel.*/
	FT_TRILINEAR2 = PVR_FILTER_TRILINEAR2/*!< Apply trilinear filtering to all pixel.*/
};

/*! Texture environment enum
 * Provide an enumeration of different method for mixing texture
 * with polygon color.
 */
enum texture_env_type
{
	TE_REPLACE = PVR_TXRENV_REPLACE, /*!< The texture color replace the current polygon color.*/
	TE_MODULATE = PVR_TXRENV_MODULATE, /*!< The texture color is mixed with the  current polygon color. (Polygon Color * Texel Color)*/
	TE_DECAL = PVR_TXRENV_DECAL,/*!< Don't know :(.*/
	TE_MODULATE_ALPHA = PVR_TXRENV_MODULATEALPHA /*!< The texture color is mixed with the  current polygon color using the alpha component. (Polygon Color * (1 - texel_alpha) + Texel Color* (texel_alpha) */
};

/*! Texture parameter enum
 * Provide an enumeration of different method to
 * tile texture on face.
 */
enum texture_parameter_type
{
	TP_REPEAT,/*!< Repeat texture if u or v > 1.0f */
	TP_CLAMP/*!< Clamp (do not iterate) texture if u or v > 1.0f */
};


//!  Iris context.
/*!
	IrisContext class encapsulate all the iris data manager and graphics 
	pipeline. This class represent the dreamcast itself.
*/
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
	IrisSkeletalManager		_skeletal_manager;
	filter_type				_filter;
	blend_type				_src_blend;
	blend_type				_dst_blend;
	texture_env_type		_env;
	texture_parameter_type	_param_u;
	texture_parameter_type	_param_v;
	bool					_dirty_header;
	bool					_dirty_material;
public:
	bool					_soft_clip;
	bool					_lighting;
	
	//! Define texture parameter (tilling)
	void	SetTexParam(texture_parameter_type u, texture_parameter_type v)
	{
		_dirty_material = true;
		_param_u = u;
		_param_v = v;
		if (_material)
		{
			pvr_poly_cxt_t	*cxt = _material->GetCxt();
			if (u == TP_REPEAT)
			{
				if (cxt->txr.uv_clamp == PVR_UVCLAMP_UV)
					cxt->txr.uv_clamp = PVR_UVCLAMP_V;
				else if (cxt->txr.uv_clamp == PVR_UVCLAMP_U)
					cxt->txr.uv_clamp = PVR_UVCLAMP_NONE;
				else 
					_dirty_material = false;
			}
			else
			{
				if (cxt->txr.uv_clamp == PVR_UVCLAMP_NONE)
					cxt->txr.uv_clamp = PVR_UVCLAMP_U;
				else if (cxt->txr.uv_clamp == PVR_UVCLAMP_V)
					cxt->txr.uv_clamp = PVR_UVCLAMP_UV;
				else 
					_dirty_material = false;
			}
			if (v == TP_REPEAT)
			{
				if (cxt->txr.uv_clamp == PVR_UVCLAMP_UV)
					cxt->txr.uv_clamp = PVR_UVCLAMP_U;
				else if (cxt->txr.uv_clamp == PVR_UVCLAMP_V)
					cxt->txr.uv_clamp = PVR_UVCLAMP_NONE;
				else 
					_dirty_material = false;
			}
			else
			{
				if (cxt->txr.uv_clamp == PVR_UVCLAMP_NONE)
					cxt->txr.uv_clamp = PVR_UVCLAMP_V;
				else if (cxt->txr.uv_clamp == PVR_UVCLAMP_U)
					cxt->txr.uv_clamp = PVR_UVCLAMP_UV;
				else 
					_dirty_material = false;
			}
		}
		else 
			_dirty_material = false;
	}


	//! Tell iris to do frustrum culling using precomputed bounding sphere and frustrum plane.
	
	void	SetSoftClip(bool enable)
	{
		_soft_clip = enable;
	}

	//! Tell iris to enable dynamic lighting.
	void	SetLighting(bool enable)
	{
		_lighting = enable;
	}
	//! Define texture environment.
	/*!
		Define the environment for the current texture.
		\param e Texture environment.
		\sa SetMaterial()
	*/
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

	//! Define filter mode.
	/*!
	Define the filtering mode for the current texture.
	\param f Filtering type.
	\sa SetMaterial()
	*/
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

	//! Define blending type for the source color.
	/*!
	Define blending type for the source color. The source is defined
	as the texture we wan't to map on polygon 
	\param b Blending type.
	\sa SetBlendDst()
	*/
	void	SetBlendSrc(blend_type b)
	{
		_dirty_header |= _cxt.blend.src != (int) b;
		_src_blend = b;
		_cxt.blend.src = (int) b;
	}

	//! Define blending type for the destination color.
	/*!
	Define blending type for the destination color. The destination is defined
	as the framebuffer color.
	\param b Blending type.
	\sa SetBlendDst()
	*/
	void	SetBlendDst(blend_type b)
	{
		_dirty_header |= _cxt.blend.dst != (int) b;
		_dst_blend = b;
		_cxt.blend.dst = (int) b;
	}


	//! Define shading method.
	/*!
	Define the shading that will be used fo rendering face color.
	\param s Shading type.
	*/
	void	SetShading(shading_type s)
	{
		_dirty_header |= _cxt.gen.shading != (int) s;
		_cxt.gen.shading = (int) s;
	}

	//! Define culling method.
	/*!
	Define the method that will be used for backface removing.
	\param c Culling type.
	*/
	void	SetCulling(culling_type c)
	{
		_dirty_header |= _cxt.gen.culling != (int) c;
		_cxt.gen.culling = (int) c;
	}

	//! Enable / disable depth buffer writting.
	/*!
	Define if the PowerVR will write to the zbuffer when rendering a pixel.
	\param enabled true if PowerVR can write, false if not.
	*/
	void	SetDepthWrite(bool enabled)
	{
		int	set_value = enabled ? PVR_DEPTHWRITE_ENABLE : PVR_DEPTHWRITE_DISABLE;
		_dirty_header |= set_value != _cxt.depth.write;
		_cxt.depth.write = set_value;
	}
	//! Define Zbuffer test operator.
	/*!
	Define the operator (==, !=, >, etc.) that will be used by the PowerVR for rejecting pixel
	using the the zbuffer.
	\param f Depth test operator.
	*/
	void	SetDepthCmpFct(depth_fct f)
	{
		_dirty_header |= _cxt.depth.comparison != (int) f;
		_cxt.depth.comparison = (int) f;
	}

	//! SetScissorTest
	/*!
		Enable scissort test. Ie the PowerVR will cut
		the image displayed into the screen using 
		coordinate passe to SetScissor.
	*/
	void	SetScissorTest(bool enabled)
	{
		int	set_value = enabled ? PVR_USERCLIP_INSIDE  : PVR_USERCLIP_DISABLE;
		_dirty_header |= _cxt.gen.clip_mode != set_value;		
		_cxt.gen.clip_mode = set_value;
	}

	//! Default destructor.
	~IrisContext()
	{
        printf("[INFO] IrisContext destroyed\n");
    }
	
	//! Return the only instance of the IrisContext.
    static IrisContext &Get()
    {
        static	IrisContext	the_context;
        return	the_context;

    }
    //! Send polygone header to the PowerVR.
	/*!
	Polygone header are handled by Iris. It represent 
	render state like shading type, texture that will
	be used for rendering the next vertices, etc.
	*/
	void	SendPvrHeader()
	{
		pvr_poly_compile(&_hdr, &_cxt);
		pvr_prim(&_hdr, sizeof(pvr_poly_hdr_t));		
		_dirty_header = _dirty_material = false;
	}
	
	//! Define the current material.
	/*!
	\param material The material that will be use as the current material.
	*/
	void				SetMaterial(IrisMaterial *material)
    {	
		_dirty_material |= _material != material;
		_material = material;
	}
    
	//! Return the current material.
	/*!
	\return The current material.
	*/
	IrisMaterial	*GetMaterial()
    {
		return _material;
	}
	
	//! Define the current material in the polygon header.
	/*!
	Internal function. You do not have to use it.
	*/
    void				SetCurrentMaterial()
    {	
		if (_material)
			memcpy(&_cxt.txr, &(_material->GetCxt()->txr), sizeof(_cxt.txr));
		else
		{
			_cxt.txr.enable = PVR_TEXTURE_DISABLE;
		}
	}

	//! Free a texture from the pvr memory.
	/*!
	Internal function. You do not have to use it.
	*/
    void				UnloadTexture(pvr_poly_cxt_t *cxt)
    {
		pvr_mem_free(cxt->txr.base);
	}
	
	//! Load a PNG file to the pvr memory
	/*!
	Internal function. You do not have to use it.
	*/
	uint32				LoadPNGFile(char *file_name,  pvr_poly_cxt_t &cxt, TextureType type = texture_noalpha);
	//! Initialise the dreamcast, KOS and iris. 
	/*!
	\param vertex_buffer_size Size in byte of the vertext buffer that the dreamcast wiil use for storing vertices to render.
	Note that this buffer will be splited for storing the different PVR rendering list(Opaque, Tranlucent, etc.).
	\note Iris only use 3 rendering list, Opaque, Translucent and PunchTru.
	*/
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
			vout->oargb =	vtmp1 | vtmp2 | vtmp3 | (value & 0xff000000);
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
			vout->oargb =	vtmp1 | vtmp2 | vtmp3 | (value & 0xff000000);
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
    IrisInputManager		&GetInputManager(){return _input_manager;}
    IrisObjectManager		&GetObjectManager() {return _object_manager;}
	IrisSkeletalManager		&GetSkeletalManager() {return _skeletal_manager;}
	//! Tell the powervr that we will begin to transmit a new frame.
	/*!
		By default, we begin to render in the opaque polygon list.
		\sa EndScene(), BeginBlending(), BeginPunch()
	*/
	void				BeginScene()
    {
        _input_manager.Update();
		_poly_count	= 0;
		_strip_count = 0;
		pvr_poly_cxt_col(&_cxt, PVR_LIST_OP_POLY);
		_cxt.gen.culling = PVR_CULLING_NONE;
		_cxt.gen.alpha = PVR_ALPHA_DISABLE;
		pvr_wait_ready();
		pvr_scene_begin();
		pvr_list_begin(PVR_LIST_OP_POLY);
		_dirty_header = _dirty_material = true;
	}

	//! Tell the powervr to that it can render the current frame.
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
    
	//! Tell the powervr that we will begin to transmit translucent polygon.
	/*!
	Must be called beetwen BeginScene() and EndScene();
	\sa BeginScene(), EndScene(), BeginPunch()
	*/
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

	//! Tell the powervr that we will begin to transmit punchtru polygon.
	/*!
	Must be called beetwen BeginScene() and EndScene() and after BeginBlending() if used.
	\sa BeginScene(), EndScene(), BeginBlending()
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

	int	_poly_count;
	int	_strip_count;
private:
    IrisContext()
	{
		// by default
		_filter = FT_BILINEAR;
		_src_blend = BT_SRC_ALPHA;
		_dst_blend = BT_INV_SRC_ALPHA;
	}
};
#endif //__CONTEXT_HPP__
