#pragma	   once
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"

#define ASSERT_VIEWER(value, str)  {if (!value) {Loger::Get().Print(str);return false;}}

#include "vb_wrap.h"


class Viewer
{
public:
	~Viewer();
	static INT_PTR  CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool	Create(HINSTANCE hinstance);
	void	Destroy();
	
	void	SetImportedElements(ImdExp::element_list_t *list_element);

	void	SetImportedMaterial(ImportedMaterial *list_material);
//	static Viewer	&Get() 
//	{ 
	//	static Viewer singleton;
	//	return singleton;
//	}
	static void D3DThread(Viewer *viewer);

	void	StartD3DThread();

	void	SetCameraAngle(float y, float z)
	{
		_angle_y = y;
		_angle_z = z;
	}

	void	AddVectorToCamera(D3DXVECTOR3 *vec)
	{
		D3DXVec3Add(&_position, &_position, vec);
		D3DXVec3Add(&_at, &_at, vec);
	}
	
	void	GoUp()
	{
		AddVectorToCamera(&_direction);
	}

	void	SetWindowSize(RECT *rect)
	{
		_wnd_middle_x =  rect->left + _wnd_width / 2;
		_wnd_middle_y =  rect->top + _wnd_height / 2;
	}
	void	GoDown()
	{
		AddVectorToCamera(&_inv_direction);
	}

	void	StrafeLeft()
	{
		AddVectorToCamera(&_strafe_left);
	}

	void	StrafeRight()
	{
		AddVectorToCamera(&_inv_strafe_left);
	}

	void	RotateAt(float angle, D3DXVECTOR3 *axis);
	// view, too lazy for accessor
	POINT					_old_pos;
	uint	_wnd_middle_x;
	uint	_wnd_middle_y;
	bool	_can_free;
protected:
	Viewer();
	void	PrintIluError();
	void	Debug(char *str);
	void	Debugf(char *str, ...);
	//////////////////////////////////////////////////////////////////////////
	// threaded part
	void	UpdateCamera();
	void	GenerateGrid(int size, float increment);
	bool	InitDirect3D(HWND hwnd, bool fullscreen, int size_x, int size_y);
	void	CreateD3dBuffer();
	void	LoadTextures();
	void	DestroyTextures();
	void	Render();
	void	DestroyThread();
	bool	_end_thread;
	DWORD	_thread_id;
	HANDLE	_thread_handle;
	//////////////////////////////////////////////////////////////////////////

	uint	_wnd_width;
	uint	_wnd_height;
	ImdExp::element_list_t	*_list_element;
	ImportedMaterial		*_list_material;
	Loger					*_loger;
	// camera movement
	float					_angle_x;
	float					_angle_y;
	float					_angle_z;
	bool					_capture_movement;
	float					_zoom;
	// windows relative
	HWND					_wnd;	
	size_t					_texture_count;
	UINT_PTR				_viewer_timer_event;
	// direct3d
	D3DXVECTOR3				_position;
	D3DXVECTOR3				_up;
	D3DXVECTOR3				_at;
	std::vector<VBILVect3d>	_vb;
	std::vector<void *>		_texture_list;
	VBVect3d				_vb_grid;
	IDirect3DDevice8		*_d3d_device;
	IDirect3DTexture8		**_texture;
	IDirect3D8				*_d3d_object;
	IDirect3DSurface8		*_d3d_backbuffer;
	IDirect3DSurface8		*_d3d_depthbuffer;
	// mouse camera
	D3DXVECTOR3				_direction;
	D3DXVECTOR3				_inv_direction;
	D3DXVECTOR3				_strafe_left;
	D3DXVECTOR3				_inv_strafe_left;
};