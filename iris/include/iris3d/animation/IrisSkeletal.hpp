/*********************************************************
**  File name : IrisSkeletal.hpp
**	Iris Engine V0.9 "alaiii"
**  Date Of Creation: 25/05/2003
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/

struct IrisBoneAnim
{
  matrix _matrix;
};

class IrisBone
{
  IrisBone() : _index(0), _bone_anim(0), _parent(0) {}
  ~IrisBone();
  void Destroy();
  friend class IrisSkeletal;

protected:
  int _index;
  IrisBoneAnim *_bone_anim;
  IrisBone *_parent;
};

class IrisContext;
//! This class represent a skeleton that can be applyed to a mesh.
class IrisSkeletal
{
public:
  IrisSkeletal();
  ~IrisSkeletal()
  {
    Destroy();
  }
  //! Update mesh vertices.
  /*!
	\param iobject object that will be deformed.
	\todo Skin normal.
	*/
  void Update(IrisImportObject *iobject);
  //! Load a skeleton fronm a file.
  /*!
	\param file Path to the file that contain the skeleton.
	\return true if succefully loaded, false if not.
	*/
  bool LoadFromFile(char *file);
  //! Delete all internal data.
  void Destroy();
  char *GetFileName();

  //! Change animation one step forward, looping if bounding value.
  void NextAnim()
  {
    _current_anim++;
    if (_current_anim >= _loop_end)
      _current_anim = _loop_begin;
  }

  //! Change animation one step backward, looping if bounding value.
  void PrevAnim()
  {
    _current_anim--;
    if (_current_anim <= _loop_begin)
      _current_anim = _loop_end;
  }

  //!	SetCurrentAnim function
  /*!
	Define current frame of animtation.
	\return nothing.
	*/
  void SetCurrentAnim(int i)
  {
    if (i > _loop_end)
      i = i % _loop_end;
    _current_anim = i;
  }

  //!	SetLoopEnd function
  /*!
	Define last frame of animation loop.
	\return nothing.
	*/
  void SetLoopEnd(int i)
  {
    _loop_end = i;
  }

  //!	SetLoopBegin function
  /*!
	Define first frame of animation loop.
	\return nothing.
	*/
  void SetLoopBegin(int i)
  {
    _loop_begin = i;
  }

  IrisBoneAnim *GetBone(int index);

  IrisBoneAnim *GetBone(int index, int anim_index);

protected:
  char *_file_name;
  int _num_anim;
  int _current_anim;
  int _loop_begin;
  int _loop_end;
  IrisBone *_bones;
  int _num_bones;
  IrisContext *_context;
};
