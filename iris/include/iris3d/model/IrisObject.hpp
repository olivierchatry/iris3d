/*! \class IrisObject
 *  \brief Object user interface
 *  \author Chatry Olivier
 *  \version 0.8
 *  \date    2003-04-09
 */
#ifndef __IRIS_OBJECT_H__
#define __IRIS_OBJECT_H__

//!	IrisObject class.
/*!
	IrisObject represent object that you wan't to display. It loaded from IMD object.

	Source code : Simple object loading (sample/display/).
	Html source code : Simple object loading (sample/display/).
	Source code : Simple object loading and transforming (sample/matrix/).
	Html source code : Simple object loading and transforming (sample/matrix/).
*/
class	IrisObject
{
protected:
    matrix							_object_matrix __attribute__((aligned(32)));
    int								_current_animation;
    int								_loop_begin;
    int								_loop_end;
    bool							_animation;

    //HT
    int								_current_animation_next;
    int								_fps;
    int								_framecount;

    IrisImportObject	*_object;
public:
	//!	Constructor of the IrisObject class.
	/*!
	*/
    IrisObject()
    {
        _current_animation = 0;
        _current_animation_next = 0;
        _loop_begin = 0;
        _loop_end = 0;
        _framecount = 0;
        _fps = 1;
        _animation = false;
    }

	//!	TODO the doc
	/*!
	\return nothing.
	*/
	void	TransformAndLightAndDrawMesh(int mesh_index, matrix *mat);

	//!	TODO the doc
	/*!
	\return nothing.
	*/
    void	TransformAndLightAndDraw();

	//!	TODO the doc
	/*!
	\return nothing.
	*/
	void	TransformAndLightAndDrawInterpolMesh(int mesh_index, matrix *mat);

	//!	TODO the doc
	/*!
		\return nothing.
	*/
    void	TransformAndLightAndDrawInterpol();
	

	//!	TODO the doc
	/*!
		\return nothing.
	*/
    void	TransformAndLightAndDrawNoClip();
	//!	TODO the doc
	/*!
		\return nothing.
	*/
    void	TransformAndLightAndDrawNoClipInterpol();
	//!	TODO the doc
	/*!
		\return nothing.
	*/    
	void	ClippedAndLightedMesh(int mesh_index, matrix *mat);
	//!	TODO the doc
	/*!
		\return nothing.
	*/    
	void	ClippedAndLighted();
	//!	TODO the doc
	/*!
		\return nothing.
	*/
    void	Transform();
	//!	Draw function.
    /*!
		Render transformed mesh. Normally you do not call this function.
		\return nothing.
	*/
    void	Draw();
	//!	TODO the doc
	/*!
		\return an int.
	*/
    int		GetCurrentAnim(){return _current_animation;}
	//!	TODO the doc
	/*!
		\return an int.
	*/
 	int		GetCurrentAnimNext(){return _current_animation_next;}
	
	//!	TODO the doc
	/*!
		\return a reference on a IrisImportObject.
	*/
    IrisImportObject	&GetObject(){return *_object;}

	//!	GetMatrix function.
	/*!
		\return a reference to object matrix.
	*/
    matrix	&GetMatrix(){return _object_matrix;}

	//!	Render function.
	/*!
	Render object using clipping and lighting.
	\return a IrisFrustrum::e_res that allow you to know if the object was inside, outside or collided with the IrisFrustrum, .
	*/
    IrisFrustrum::e_res	Render()
    {
        ClippedAndLighted();
        if(_animation)
	        TransformAndLightAndDrawInterpol();
        else
		    TransformAndLightAndDraw();
        return _object->GetMesh(0).GetClipped();
    }	

	//!	Render function.
	/*!
	Render a particular mesh in the object using passed matrix.
	\param mesh index in the object, no check are made, be carefull
	\param pointer to a matrix to be applied.
	\return a IrisFrustrum::e_res that allow you to know if the object was inside, outside or collided with the IrisFrustrum, .
	*/
	IrisFrustrum::e_res	Render(uint mesh_index, matrix *mat)
	{
		ClippedAndLightedMesh(mesh_index, mat);
		if(_animation)
			TransformAndLightAndDrawInterpolMesh(mesh_index, mat);
		else
			TransformAndLightAndDrawMesh(mesh_index, mat);
		return _object->GetMesh(mesh_index).GetClipped();
	}	

	//!	RenderNoClip function.
	/*!
		Render object without clipping but with lighting.
		\return nothing.
	*/
    void	RenderNoClip()
    {
        ClippedAndLighted();
         if(_animation)
			TransformAndLightAndDrawNoClipInterpol();
        else
			TransformAndLightAndDrawNoClip();
    }

	//! SetMatrix function.
	/*!
		Define object matrix. 
		\param a reference on object matrix.
		\return nothing.
	*/
    void	SetMatrix(const matrix &object_matrix)
    {
        _object_matrix = object_matrix;
    }
	
	//! PrecalculateBoundingSphere function.
	/*!
		Calculate sphere around object. Used by clipping and lighting. 
		Automatically called when object is loaded. 
		You hace to call it if you modify mesh vertex data. 
		But be aware that it take many CPU times.
		\return nothing.
	*/
    void	PrecalculateBoundingSphere()
    {
        for (int i = 0; i < _object->GetNumMesh(); ++i)
        {
            _object->GetMesh(i).ComputeCenter();
            _object->GetMesh(i).ComputeRadius();
        }
    }


	//MODIFIED by Heinrich Tillack
	
	//! TODO the doc
	/*!
		\return nothing.
	*/
    void	NextAnim()
    {
        if(++ _framecount % _fps == 0)
        {
            _framecount=0;
            if(++_current_animation >= _loop_end)
                _current_animation = _loop_begin;

        }

        _current_animation_next = _current_animation+1;
        if(_current_animation_next >= _loop_end)
            _current_animation_next = _loop_begin;
    }

	//! TODO the doc
	/*!
		\return nothing.
	*/
    void	PrevAnim()
    {
        if(++_framecount % _fps == 0)
        { 
			_framecount=0;
            if(--_current_animation < _loop_begin){
                _current_animation = _loop_end;
                _current_animation_next=_loop_end;
            }
        }
        _current_animation_next=_current_animation-1;
        if(_current_animation_next >=_loop_end)
            _current_animation_next =_loop_begin;
    }

	//!	SetCurrentAnim function
	/*!
		Define current frame of animtation.
		\return nothing.
	*/
    void	SetCurrentAnim(int i)
    {
        if (i > _loop_end)
            i = i % _loop_end;
        _current_animation = i;
    }
	
	//!	SetLoopEnd function
	/*!
		Define last frame of animation loop.
		\return nothing.
	*/
    void	SetLoopEnd(int i)
    {
        _loop_end= i;
        _animation= (_loop_begin!=_loop_end);
    }

	//!	SetFPS function
	/*!
		Define frame per seconde of animation. Used for interpollation.

		The formula is : _fps = 60 / fps;
	\return nothing.
	*/
    void	SetFPS(int fps)
    {
        _fps = fps;
    }
	//!	SetLoopBegin function
	/*!
		Define first frame of animation loop.
		\return nothing.
	*/
    void	SetLoopBegin(int i)
    {
        _loop_begin = i;
        _framecount=0;
        _animation= (_loop_begin!=_loop_end);
    }
	//!	GetFPS function
	/*!
		\return an int : frame per seconde of animation.
	*/
    int	GetFPS()
    {
        return _fps;
    }

	//! TODO the doc
	/*!
		\return an int : the frame count.
	*/
    int	GetFrameCount()
    {
        return _framecount;
    }
	//!	LoadFromFile function;
	/*!
		Load an IMD file, initialise animation and bounding sphere.
		\param file_path : directory where the file is located. IMD file and textures must be located in the same directory.
		\param file_name : file name of IMD file.
		\param scale : scale object by value.
		\return nothing.
	*/
    void	LoadFromFile(char *file_path, char *file_name,float scale=1)
    {
        _object = IrisContext::Get().GetObjectManager().LoadObject(file_path, file_name,scale);
        SetCurrentAnim(0);
        PrecalculateBoundingSphere();
    }

};

#endif
