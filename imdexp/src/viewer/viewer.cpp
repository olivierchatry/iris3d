
#include <stdafx.h>

const int	grid_size = 80;  // draw a 10x10 grid.
const int	grid_primitive_count = grid_size * grid_size * 4; // eache sqaure take 4 lines.
const float grid_increment = 20.0f;

Viewer::Viewer() : _loger(0), _d3d_device(0),_d3d_object(0), 
_d3d_backbuffer(0), _d3d_depthbuffer(0), _wnd(0),
_list_element(0), _list_material(0), _texture(0), _viewer_timer_event(0),
				_capture_movement(false), _angle_x(0), _angle_y(0), _angle_z(0), _zoom(10),
				_at(0.0f, 0.0f, 0.0f), _up(0.0f, 0.0f, 1.0f), _position(0.0f, -2.0f, 0.0f),
				_thread_handle(0), _end_thread(false), _can_free(false)
{
}

//////////////////////////////////////////////////////////////////////////
// generate a vertex buffer containt line list for drawing a grid.
// !!!!!!! not efficient !!!!!!!!!
//////////////////////////////////////////////////////////////////////////
void	Viewer::GenerateGrid(int size, float increment)
{
	float	middle = (float) size / 2.0f;
	float	inc_middle = increment / 2.0f;
	_vb_grid.Allocate(_d3d_device, size * size * 8, D3DPT_LINELIST);
	vertex3d_t	*v = _vb_grid.Lock();
	for (float x = -middle; x < middle; ++x)
	{
		for (float y = - middle; y < middle; ++y)
		{
			v->_color = 0xffffffff;v->_pos = D3DXVECTOR3((x * increment) - inc_middle, (y * increment) - inc_middle, 0.0f);v++;
			v->_color = 0xffffffff;v->_pos = D3DXVECTOR3((x * increment) - inc_middle, (y * increment) + inc_middle, 0.0f);v++;
			v->_color = 0xffffffff;v->_pos = D3DXVECTOR3((x * increment) - inc_middle, (y * increment) + inc_middle, 0.0f);v++;
			v->_color = 0xffffffff;v->_pos = D3DXVECTOR3((x * increment) + inc_middle, (y * increment) + inc_middle, 0.0f);v++;
			v->_color = 0xffffffff;v->_pos = D3DXVECTOR3((x * increment) + inc_middle, (y * increment) + inc_middle, 0.0f);v++;
			v->_color = 0xffffffff;v->_pos = D3DXVECTOR3((x * increment) + inc_middle, (y * increment) - inc_middle, 0.0f);v++;
			v->_color = 0xffffffff;v->_pos = D3DXVECTOR3((x * increment) + inc_middle, (y * increment) - inc_middle, 0.0f);v++;
			v->_color = 0xffffffff;v->_pos = D3DXVECTOR3((x * increment) - inc_middle, (y * increment) - inc_middle, 0.0f);v++;
		}	
	}
	_vb_grid.Unlock();
}

void Viewer::StartD3DThread()
{
	_thread_handle = CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)D3DThread,
		this,
		0,
		(LPDWORD)&_thread_id);
	_can_free = false;
	SetThreadPriority(_thread_handle, THREAD_PRIORITY_IDLE);
}

INT_PTR  CALLBACK Viewer::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static POINT	old_mouse_pos = {0};

	switch (msg) 
	{
	case WM_COMMAND:
		switch (LOWORD(wParam)) 
		{
		case IDOK:
			Viewer::Get().Destroy();
			EndDialog(hWnd, 1);
			break;
		}
		break;
	case WM_KEYDOWN:
		if(wParam == VK_UP)
			Viewer::Get().GoUp();
		if(wParam == VK_DOWN)
			Viewer::Get().GoDown();
		if(wParam == VK_LEFT)
			Viewer::Get().StrafeLeft();
		if(wParam == VK_RIGHT)
			Viewer::Get().StrafeRight();
		break;
	case WM_LBUTTONDOWN:
		if (Viewer::Get()._capture_movement == false)
		{
			RECT rect;
			ShowCursor(FALSE);
			GetClientRect(hWnd, &rect);
			Viewer::Get().SetWindowSize(&rect);
			GetCursorPos(&Viewer::Get()._old_pos);
			SetCursorPos(Viewer::Get()._wnd_middle_x, 
						 Viewer::Get()._wnd_middle_y);
			Viewer::Get()._capture_movement = true;
		}
		else
		{
			Viewer::Get()._capture_movement = false;
			ShowCursor(TRUE);
			SetCursorPos(Viewer::Get()._old_pos.x, 
				Viewer::Get()._old_pos.y);
		}
		break;
	
	case WM_LBUTTONUP:
		break;

	case WM_MOUSEMOVE:
		if (Viewer::Get()._capture_movement)
		{
			POINT new_pos;
			GetCursorPos(&new_pos);
			if( (new_pos.x == Viewer::Get()._wnd_middle_x) && (new_pos.y == Viewer::Get()._wnd_middle_y))
				break;
			int offset_y = Viewer::Get()._wnd_middle_x - new_pos.x;
			int offset_z = Viewer::Get()._wnd_middle_y - new_pos.y;
			SetCursorPos(Viewer::Get()._wnd_middle_x, 
						Viewer::Get()._wnd_middle_y);
			if (offset_y != 0 || offset_z != 0)
			{
				Viewer::Get().SetCameraAngle(offset_y / 1000.0f, offset_z / 1000.0f);
			}
		}
		break;
	case WM_CLOSE:
	case WM_QUIT:
	case WM_DESTROY:
		Viewer::Get().Destroy();
		EndDialog(hWnd, 0);
		break;
	default:
		return FALSE;
	}
	return TRUE;

}


Viewer::~Viewer()
{
	Destroy();
}

void Viewer::DestroyTextures()
{
	if (_texture)
	{
		for (uint i = 0; i < _texture_count; ++i)
			if (_texture[i])
				_texture[i]->Release();
		delete [] _texture;
		_texture = 0;
	}
}

void Viewer::PrintIluError()
{
	ILenum	error;
	while ((error = ilGetError())) 
		Debugf("+ ilu error: %s", iluErrorString(error));
}

typedef LPDIRECT3D8 (__stdcall *DIRECT3DCREATE8) (uint);


bool Viewer::InitDirect3D(HWND hwnd, bool fullscreen, int size_x, int size_y)
{
	// loading d3d8.dll
	_d3d_object = Direct3DCreate8(D3D_SDK_VERSION);
	ASSERT_VIEWER(_d3d_object != 0, "!!! Cannot create d3d object");
	D3DCAPS8	dev_caps;
	HRESULT		hres;		
	// look for hardware card, if no exit.
	hres = _d3d_object->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &dev_caps);
	ASSERT_VIEWER(SUCCEEDED(hres), "No 3d hardware");
	D3DPRESENT_PARAMETERS pp = {0};
	pp.BackBufferWidth = size_x;
	pp.BackBufferHeight = size_y;
	pp.BackBufferCount = 1;
	// no faa
	pp.MultiSampleType = D3DMULTISAMPLE_NONE; // no aa
	pp.hDeviceWindow = hwnd;
	pp.Windowed = !fullscreen;
	// stencil for shadow
	pp.EnableAutoDepthStencil = true;
	pp.AutoDepthStencilFormat = D3DFMT_D16;
	pp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pp.FullScreen_PresentationInterval = fullscreen ? D3DPRESENT_INTERVAL_IMMEDIATE : D3DPRESENT_INTERVAL_DEFAULT;
	if (!fullscreen)
	{
		// FIX ME : don't like voodoo card so ... don't really need this
		ASSERT_VIEWER((dev_caps.Caps2 & D3DCAPS2_CANRENDERWINDOWED), "cannot render on windows");
		D3DDISPLAYMODE current_displaye_mode;
		hres = _d3d_object->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &current_displaye_mode);
		ASSERT_VIEWER(SUCCEEDED(hres), "get display mode");
		pp.BackBufferFormat = current_displaye_mode.Format;
	}
	else
	{
		int mode_count = _d3d_object->GetAdapterModeCount(D3DADAPTER_DEFAULT);
		ASSERT_VIEWER(mode_count != 0, "no display mode");
		D3DDISPLAYMODE	new_mode, best_mode;
		bool			found = false;
		do
		{
			hres = _d3d_object->EnumAdapterModes(D3DADAPTER_DEFAULT, mode_count - 1, &new_mode);
			ASSERT_VIEWER(SUCCEEDED(hres), "adaptator enumeration");
			if (new_mode.Height == size_y && new_mode.Width == size_x)
				if ((found && (new_mode.Format < new_mode.Format)) || !found) 
					best_mode = new_mode, found = true;
		}
		while (--mode_count);
		ASSERT_VIEWER(found, "cannot find a suitable display mode");
		pp.BackBufferFormat = best_mode.Format;
	}

	// look for hardware 	vertex proecesind
	int flags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	if (dev_caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		flags = (dev_caps.DevCaps & D3DDEVCAPS_PUREDEVICE) ? D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_MIXED_VERTEXPROCESSING;
	hres = _d3d_object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, _wnd, flags | D3DCREATE_MULTITHREADED, &pp, &_d3d_device);
	ASSERT_VIEWER(SUCCEEDED(hres), "device creation");
	hres = _d3d_device->GetRenderTarget(&_d3d_backbuffer);
	ASSERT_VIEWER(SUCCEEDED(hres), "set backbuffer");
	hres = _d3d_device->GetDepthStencilSurface(&_d3d_depthbuffer);
	ASSERT_VIEWER(SUCCEEDED(hres), "set stencil buffer");	
	return true;
}

void Viewer::SetImportedElements(ImdExp::element_list_t *list_element)
{
	_list_element = list_element;
}

void Viewer::LoadTextures()
{
	bool	il_error = false;
	DestroyTextures();
	if (_list_material == 0)
		return;
	_texture_count = _list_material->_material_data.size();
	_texture = new IDirect3DTexture8*[_texture_count];
	for (uint i = 0; i < _texture_count; ++i)
	{
		MaterialData *material = _list_material->_material_data[i];
		ILuint	il_id;
		material->_data = 0;
		if (material->_diffuse_map == 0)
			continue;
		ilGenImages(1, &il_id);
		ilBindImage(il_id);
		_texture[i] = 0;
		if (ilLoadImage(material->_diffuse_map)  == IL_TRUE)
		{
			if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE) == IL_TRUE)
			{
				ilutSetInteger(ILUT_D3D_POOL, D3DPOOL_MANAGED);
				_texture[i] = ilutD3D8Texture(_d3d_device);
				material->_data = (void *) (_texture[i]);
			}
			else
				il_error = true;
		}
		else
			il_error = true;
		if (il_error)
		{
			PrintIluError();
		}
		ilDeleteImages(1, &il_id);
	}
}

void Viewer::Debug(char *str)
{
	Loger::Get().Print(str);
}

void Viewer::Debugf(char *str, ...)
{
	va_list args;
	TCHAR buffer[1024]; // FIXMEEE
	va_start(args, str);
	vsprintf(buffer, str, args);
	Debug(buffer);
	va_end(args);
}


void Viewer::UpdateCamera()
{
	D3DXVECTOR3	axis, sub;
	D3DXVec3Subtract(&_direction, &_at, &_position);
	D3DXVec3Cross(&_inv_strafe_left, &_up, &_direction);
	D3DXVec3Normalize(&_inv_strafe_left, &_inv_strafe_left);		
	RotateAt(_angle_z, &_inv_strafe_left);
	RotateAt(_angle_y, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	D3DXVec3Normalize(&_direction, &_direction);
	_inv_direction = -_direction;
	_strafe_left = -_inv_strafe_left;
	D3DXMATRIX	view_matrix;
	D3DXMatrixLookAtLH(&view_matrix,
		&_position,
		&_at, 
		&_up);
	_angle_z = _angle_y = 0;
	_d3d_device->SetTransform(D3DTS_VIEW, &view_matrix);
}

void Viewer::RotateAt(float angle, D3DXVECTOR3 *axis)
{
	D3DXVECTOR3	tmp;
	D3DXVECTOR3 direction;

	D3DXVec3Subtract(&direction, &_at, &_position);
	float c = cosf(angle);
	float s = sinf(angle);
	
	tmp.x =	 (c + ( 1 - c) * axis->x) * direction.x;
	tmp.x += ((1 - c) * axis->x * axis->y - axis->z * s) * direction.y;
	tmp.x += ((1 - c) * axis->x * axis->z + axis->y * s) * direction.z;

	tmp.y =  ((1 - c) * axis->x * axis->y + axis->z * s) * direction.x;
	tmp.y += (c + (1 - c) * axis->y) * direction.y;
	tmp.y += ((1 - c) * axis->y * axis->z - axis->x * s) * direction.z;

	tmp.z  = ((1 - c) * axis->x * axis->z - axis->y * s) * direction.x;
	tmp.z += ((1 - c) * axis->y * axis->z + axis->x * s) * direction.y;
	tmp.z += (c + (1 - c) * axis->z)		* direction.z;
	D3DXVec3Add(&_at, &_position, &tmp);
}

void Viewer::Render()
{
	if (_d3d_device)
	{
		_d3d_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0x00,0x0,0x0,0x0), 1, 0);
		_d3d_device->BeginScene();
		_d3d_device->SetTexture(0, 0);
		_d3d_device->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
		_d3d_device->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
		_d3d_device->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		_d3d_device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		_d3d_device->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		_d3d_device->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
		_vb_grid.Draw(_d3d_device, grid_primitive_count);
		for (uint mesh = 0; mesh < _vb.size(); ++mesh)
		{
			_d3d_device->SetTexture(0, (IDirect3DBaseTexture8 *) _texture_list[mesh]);
			_vb[mesh].DrawIndexed(_d3d_device, D3DPT_TRIANGLESTRIP);
		}
		_d3d_device->EndScene();
		_d3d_device->Present(NULL, NULL, NULL, NULL);
	}
}

void Viewer::Destroy()
{
	DestroyThread();
	if (_loger)
		_loger = 0;
	_d3d_device = 0;
	_d3d_object = 0;
	_wnd = 0;
}

void	Viewer::DestroyThread()
{
	if (_thread_handle)
	{
		Viewer::_end_thread = true;
		WaitForSingleObject(_thread_handle, 2000);
		CloseHandle(_thread_handle);
		_thread_handle = 0;
		Viewer::_end_thread = false;
	}
}

bool Viewer::Create(HINSTANCE hinstance)
{
	if (_wnd)
		SendMessage(_wnd, WM_QUIT, 0, 0); // destroy our windows.
	RECT	rect;
	_wnd = CreateDialog(hinstance, MAKEINTRESOURCE(IDD_DIALOGVIEWER), 0, WndProc);
	ASSERT_VIEWER(_wnd != 0, "Cannot create window !!!");
	GetClientRect(_wnd, &rect);
	_wnd_height = rect.bottom - rect.top;
	_wnd_width = rect.right - rect.left;
	_wnd_middle_x = _wnd_width >> 1;
	_wnd_middle_y = _wnd_height >> 1;
	Loger::Get().Printf("Client window is %d %d", _wnd_width, _wnd_height);
	ShowWindow(_wnd, SW_SHOW);
	return true;
}

void Viewer::D3DThread(Viewer *viewer)
{
	ilInit();
	iluInit();
	ilutInit();
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION ||
		iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION) 
		viewer->Debug("DevIL header version is different from lib version !!!!");
	viewer->InitDirect3D(viewer->_wnd,  false, viewer->_wnd_width, viewer->_wnd_height);
	viewer->GenerateGrid(grid_size, grid_increment);
	viewer->LoadTextures();
	viewer->CreateD3dBuffer();
	// default setting.
	D3DXMATRIX	projection_matrix;
	D3DXMatrixPerspectiveFovLH(&projection_matrix, D3DX_PI/3, (float) viewer->_wnd_width/ (float) viewer->_wnd_height, 1.0f, 1000.0f);
	viewer->_d3d_device->SetTransform(D3DTS_PROJECTION, &projection_matrix);
	D3DMATERIAL8	material;
	material.Ambient.a = 0;
	material.Ambient.r = material.Ambient.g = material.Ambient.b = 0.0f;
	material.Diffuse.r = material.Diffuse.g = material.Diffuse.b = 0.1f;
	material.Specular.r = material.Specular.g = material.Specular.b = 0.1f;
	material.Power = 0.0f;
	viewer->_d3d_device->SetMaterial(&material);
	// no lighting 
	viewer->_d3d_device->SetRenderState(D3DRS_LIGHTING, 0);
	// no culling
	viewer->_d3d_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// set default material
	viewer->_can_free = true;
	while (!viewer->_end_thread)
	{
		viewer->UpdateCamera();
		viewer->Render();
	}
	viewer->DestroyTextures();
	for (uint i = 0; i < viewer->_vb.size(); ++i)
		viewer->_vb[i].Deallocate();
	viewer->_vb_grid.Deallocate();
	if (viewer->_d3d_device)
	viewer->_d3d_device->Release(); 
	if (viewer->_d3d_object)
		viewer->_d3d_object->Release();
}

//////////////////////////////////////////////////////////////////////////
//	Create Vertex Buffer from imported element
//////////////////////////////////////////////////////////////////////////
void Viewer::CreateD3dBuffer()
{
	// get number of vertex in totla mesh.
	ImdExp::element_list_it_t	it;
	size_t	num_indices_total = 0;
	size_t	num_vertex_total = 0;
	size_t	num_mesh = 0;
	size_t	num_strip = 0;
	for (it = _list_element->begin(); it != _list_element->end(); ++it)
	{
		if ((*it)->_type == mesh_element)
		{
			ImportedMesh		*mesh = (ImportedMesh*) (*it);
			for (uint i = 0; i < mesh->_strip.size(); ++i)
				num_indices_total += mesh->_strip[i]._face_index.size();
			num_vertex_total += mesh->_mesh_data[0]._vertex.size();
			num_strip += mesh->_strip.size();
			num_mesh ++;
		}
	}
	// create our vertex buffer.
	_vb.resize(num_mesh);	
	_texture_list.resize(num_mesh);
	num_mesh = 0;
	// fill our vertex buffer
	for (it = _list_element->begin(); it != _list_element->end(); ++it)
	{
		if ((*it)->_type == mesh_element)
		{
			ImportedMesh		*mesh = (ImportedMesh*) (*it);
			MeshData		*data = &(mesh->_mesh_data[0]);
			MeshColorMapping	*mapping_color = &(mesh->_mesh_color_mapping);
			bool				have_color = mapping_color->_color.size() > 0;

			if (mesh->_material)
				_texture_list[num_mesh] = mesh->_material->_data;
			else
				_texture_list[num_mesh] = 0;
			_vb[num_mesh].Allocate(_d3d_device, (uint) data->_vertex.size(), D3DPT_TRIANGLESTRIP);
			//////////////////////////////////////////////////////////////////////////
			// fill our vertex buffer
			vertex3d_t *v = _vb[num_mesh].Lock();
			for (uint i = 0; i < data->_vertex.size(); ++i)
			{
				v->_pos.x = data->_vertex[i].x;
				v->_pos.y = data->_vertex[i].y;
				v->_pos.z = data->_vertex[i].z;
				v->_norm.x = data->_normal[i].x;
				v->_norm.y = data->_normal[i].y;
				v->_norm.z = data->_normal[i].z;
				v->_u = mapping_color->_mapping[i]._uv.x;
				v->_v = 1.0f -mapping_color->_mapping[i]._uv.y;
				uint r = (uint) (mapping_color->_color[i].x * 255) & 0xff;
				uint g = (uint) (mapping_color->_color[i].y * 255) & 0xff;
				uint b = (uint) (mapping_color->_color[i].z * 255) & 0xff;
				v->_color =  have_color ?  (r << 16 | g << 8 | b ) : 0;
				v ++;
			}
			_vb[num_mesh].Unlock();
			//////////////////////////////////////////////////////////////////////////
			
			//////////////////////////////////////////////////////////////////////////
			// fill our index buffer;
			_vb[num_mesh].PrealocateIBList((uint) mesh->_strip.size());
			for (i = 0; i < mesh->_strip.size(); ++i)
			{
				uword_list_t &indices = mesh->_strip[i]._face_index;
				_vb[num_mesh].CreateIB(_d3d_device, i, (uint) indices.size(), (uint) indices.size() - 2);
				WORD *ib = _vb[num_mesh]._ib[i].Lock();
				for (uint f = 0; f < indices.size(); ++f)
				{
					*ib = indices[f];
					ib++;
				}
				_vb[num_mesh]._ib[i].Unlock();
			}
			//////////////////////////////////////////////////////////////////////////			
			num_mesh ++;
		}
	}
}

void Viewer::SetImportedMaterial(ImportedMaterial *list_material)
{
	_list_material = list_material;
}



