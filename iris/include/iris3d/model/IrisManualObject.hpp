/*********************************************************
**	File name : IrisManualObject.hpp
**	Iris Engine V0.7 "presque"
**	Date of creation: 01/07/2002
**	Author : Olivier Chatry - Epitech Console Laboratory
**	        (http://www.epitech.net/labconsole/)
*********************************************************/

#ifndef __IRISMANUALOBJECT_HPP__
#define __IRISMANUALOBJECT_HPP__

//!	IrisManual Object.
/*!
	The IrisManualObject class allow user to create an object manually by describing vertex by vertex an object. 
	Note that these object must be stripped.

	Source code : Texture loading with Manual object (sample/manualobject/).
	Html source code : Texture loading with Manual object (sample/manualobject/).
*/
class IrisManualObject
{
protected:
    matrix					_matrix __attribute__((aligned(32)));
    vect3d					*_vertex_buffer;
    vect3d					*_trans_vertex;
    vertex2dtl			*_2d_vertex;
    vect3d					*_normal;
    vect3d					*_trans_normal;
    IrisMaterial		*_material;
    uint16					*_index;
    unsigned				_count;
    bool						_lighted;
    IrisFrustrum::e_res	_clipped;
    vect3d					_center;
    float						_radius;
    uint32					*_color;
public:
    // bug from wattel_g
	//! Constructor for the IrisManualObject class.
	/*!
	*/
    IrisManualObject() :
            _vertex_buffer(NULL),
            _trans_vertex(NULL),
            _2d_vertex(NULL),
            _normal(NULL),
            _trans_normal(NULL),
            _material(NULL),
            _index(NULL),
            _count(0),
            _color(NULL)
    {}
	//! Constructor for the IrisManualObject class.
	/*!
		This constructor allocate vertex buffer.
		\param count : number of vertices in vertex buffer.
	*/
    IrisManualObject(unsigned count) :
            _vertex_buffer(NULL),
            _trans_vertex(NULL),
            _2d_vertex(NULL),
            _normal(NULL),
            _trans_normal(NULL),
            _material(NULL),
            _index(NULL),
            _count(0),
            _color(NULL)
    { Allocate(count);	}
	//! Destructor for the IrisManualObject class.
	/*!
	*/
    ~IrisManualObject()
    {
        Delete();
    }
	//! Delete function.
	/*!
	This function free the vertex buffer.

	Note : This function is called when the IrisManualObject is destroyed.
	\return nothing.
	*/
    void			Delete();
	//! Allocate function.
	/*!
	This function allocate vertex buffer.
	\param count : number of vertices in vertex buffer.
	\return nothing.
	*/
    void	Allocate(unsigned count);
	//!	GetVertex function.
	/*!
	This function return a reference to a vect3d in vertex buffer.

	Note : Allocate must be called before using GetVertex.
	\param index : index of vertex in vertex buffer start from 0 to (number of vertex - 1).
	\return a reference to a vect3d in vertex buffer.
	*/
    vect3d	&GetVertex(unsigned index)
    {	return _vertex_buffer[index];	}
    const vect3d	&GetVertex(unsigned index) const
        {	return _vertex_buffer[index];	}
	//!	SetUV function.
	/*!
		This function define texture coordinate of a vertex in the vertex buffer.
		\param index : index of vertex in vertex buffer start from 0 to (number of vertex - 1).
		\param u : x texture coordinate.
		\param v : y texture coordinate.
		\return nothing.
	*/
    void			SetUV(int index, float u, float v)
    {
        _2d_vertex[index].u = u;
        _2d_vertex[index].v = v;
    }
	//!	SetColor function.
	/*!
		This function define color of a vertex in the vertex buffer.
		\param index : index of vertex in vertex buffer start from 0 to (number of vertex - 1).
		\param color : color of the vertex. (see IrisColor class).
		\return nothing.
	*/
    void			SetColor(int index, const IrisColor &color)
    {
        uint32 col = color.Pack();
        _color[index] = col;
        _2d_vertex[index].argb = col;
        _2d_vertex[index].oargb = col;
    }
	//! SetColor function.
	/*!
		This function define color of all vertices in the vertex buffer.
		\param color : color to apply. (see IrisColor class).
		\return nothing.
	*/
    void			SetColor(const IrisColor &color)
    {
        uint32 col = color.Pack();
        int index = _count;
        while (index --)
        {
            _color[index] = col;
            _2d_vertex[index].argb = col;
            _2d_vertex[index].oargb = col;
        }
    }
	//!	PrecalculateBoundingSphere function.
	/*!
		This function calculate a bounding sphere around the object.
		You must call this function when you change a vertex coordinate. 
		\return nothing.
	*/
    void			PrecalculateBoundingSphere();
	//!	SetMaterial function.
	/*!
		Define IrisManualObject material.
		\param material : the material to set.
		\return nothing.
	*/
    void			SetMaterial(IrisMaterial *material)
    { _material = material;	}
	//!	SetMaterial function.
	/*!
		Define IrisManualObject material using IrisMaterialManager.
		\param file_name : file name with path of material to get from IrisMaterialManager.
		\return nothing.
	*/
    void			SetMaterial(char *file_name)
    { _material = IrisContext::Get().GetMaterialManager().LoadMaterial(file_name);	}
	//!	GetMaterial function.
	/*!
		\return a reference to an IrisMaterial of current material.
	*/
    IrisMaterial &GetMaterial()
    { return *_material;	}
    const IrisMaterial &GetMaterial() const
        { return *_material;	}
    // rendering fct
	//!TODO the doc
	/*!
		\return nothing.
	*/
    void	TransformAndLightAndDraw();
	//!TODO the doc
	/*!
		\return nothing.
	*/
    void	TransformAndLightAndDrawNoClip();
	//!TODO the doc
	/*!
		\return nothing.
	*/
    void	ClippedAndLighted();
	//!TODO the doc
	/*!
		\return nothing.
	*/
    void	Transform();
	//! Draw function.
	/*!
		Render transformed mesh. Normally you do not call this function.
		\return nothing.
	*/
    void	Draw();
    const matrix &GetMatrix() const { return _matrix;}
	//! GetMatrix function.
	/*!
		\return a reference to object matrix.
	*/
    matrix &GetMatrix() { return _matrix;}
	//! Render function.
	/*!
		Render object using clipping and lighting.
		\return nothing.
	*/
    void	Render()
    {
        ClippedAndLighted();
        TransformAndLightAndDraw();
    }
	//! Precalculate2D function.
	/*!
		This function with Render2D permit user to draw a texture with 2D coordinate.
		See sample

		Source code : Texture loading with Manual object (sample/manualobject/).
		Html source code : Texture loading with Manual object (sample/manualobject/).
		\return nothing.
	*/
    void	Precalculate2D();
	//! Render2D function.
	/*!
		This function with Precalculate2D permit user to draw a texture with 2D coordinate.
		See sample 	

		Source code : Texture loading with Manual object (sample/manualobject/).
		Html source code : Texture loading with Manual object (sample/manualobject/).
		\return nothing.
	*/
    void	Render2D()
    {
        IrisContext::Get().SetMaterial(_material);
        //MODIFIED by Heinrich Tillack
        IrisContext::Get().DrawPrimitive(_2d_vertex, _index, _count,true);
    }
	//! RenderNoClip function.
	/*!
		Render object without clipping but with lighting.
		\return nothing.
	*/
    void	RenderNoClip()
    {
        ClippedAndLighted();
        TransformAndLightAndDrawNoClip();
    }
};

#endif //__IRISMANUALOBJECT_HPP__
