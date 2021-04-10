#pragma once

typedef unsigned char uchar;
typedef unsigned int uint;

struct vertex3d_t
{
  D3DXVECTOR3 _pos;
  D3DXVECTOR3 _norm;
  D3DCOLOR _color;
  float _u, _v;
  static const DWORD fvf;
};

struct vertex2d_t
{
  D3DXVECTOR3 _pos;
  float _rhw;
  D3DCOLOR _color;
  float _u, _v;
  static const DWORD fvf;
};

/************************************************************************/
/* indexed buffer class													*/
/************************************************************************/
class IBWrapper
{
public:
  IBWrapper() : _ib(0) {}
  ~IBWrapper() { Deallocate(); }
  bool Allocate(IDirect3DDevice8 *dev, uint count);
  void Deallocate();
  WORD *Lock();
  bool Unlock();
  uint _count;
  IDirect3DIndexBuffer8 *_ib;
};

/************************************************************************/
/* vb class																*/
/************************************************************************/
template <typename VBTYPE>
class VBWrapper
{
public:
  VBWrapper() : _vb(0) {}
  ~VBWrapper() { Deallocate(); }
  bool Allocate(IDirect3DDevice8 *dev, uint count,
                D3DPRIMITIVETYPE type = D3DPT_TRIANGLELIST,
                D3DPOOL pool = D3DPOOL_MANAGED);

  void Deallocate();
  VBTYPE *Lock();
  bool Unlock();
  bool Draw(IDirect3DDevice8 *dev, uint count);
  bool Draw(IDirect3DDevice8 *dev, D3DPRIMITIVETYPE type, uint count);
  uint _count;

protected:
  IDirect3DVertexBuffer8 *_vb;
  D3DPRIMITIVETYPE _type;
};

template <typename VBTYPE>
bool VBWrapper<VBTYPE>::Allocate(IDirect3DDevice8 *dev, uint count,
                                 D3DPRIMITIVETYPE type,
                                 D3DPOOL pool)
{
  HRESULT hr;

  hr = dev->CreateVertexBuffer(sizeof(VBTYPE) * count,
                               0, VBTYPE::fvf, pool, &_vb);
  _type = type;
  ASSERT_VIEWER(SUCCEEDED(hr), "Device::CreateVertexBuffer.");
  _count = count;
  return true;
}

template <typename VBTYPE>
VBTYPE *VBWrapper<VBTYPE>::Lock()
{
  HRESULT hr;
  D3DVERTEXBUFFER_DESC vb_desc;
  VBTYPE *c;

  hr = _vb->GetDesc(&vb_desc);
  hr = _vb->Lock(0, vb_desc.Size, (LPBYTE *)&c, 0);
  ASSERT_VIEWER(SUCCEEDED(hr), "VertexBuffer::Lock.");
  return c;
}

template <typename VBTYPE>
void VBWrapper<VBTYPE>::Deallocate()
{
  if (_vb)
  {
    _vb->Release();
    _vb = 0;
  }
}

template <typename VBTYPE>
bool VBWrapper<VBTYPE>::Unlock()
{
  HRESULT hr;

  hr = _vb->Unlock();
  ASSERT_VIEWER(SUCCEEDED(hr), "VertexBuffer::Unlock.");
  return true;
}

template <typename VBTYPE>
bool VBWrapper<VBTYPE>::Draw(IDirect3DDevice8 *dev, uint count)
{
  return Draw(dev, _type, count);
}

template <typename VBTYPE>
bool VBWrapper<VBTYPE>::Draw(IDirect3DDevice8 *dev, D3DPRIMITIVETYPE type, uint count)
{
  HRESULT hr;

  hr = dev->SetStreamSource(0, _vb, sizeof(VBTYPE));
  ASSERT_VIEWER(SUCCEEDED(hr), "Device::SetStreamSource.");
  hr = dev->SetVertexShader(VBTYPE::fvf);
  ASSERT_VIEWER(SUCCEEDED(hr), "Device::SetVertexShader.");
  hr = dev->DrawPrimitive(type, 0, count);
  ASSERT_VIEWER(SUCCEEDED(hr), "Device::DrawPrimitive.");
  return true;
}

typedef VBWrapper<vertex2d_t> VBVect2d;
typedef VBWrapper<vertex3d_t> VBVect3d;

/************************************************************************/
/* Indexed VB class                                                     */
/************************************************************************/
template <typename VBTYPE>
class VBIWrapper : public VBWrapper<VBTYPE>
{
public:
  IBWrapper _ib;
  bool DrawIndexed(IDirect3DDevice8 *dev, D3DPRIMITIVETYPE type, uint count);
  bool DrawIndexed(IDirect3DDevice8 *dev, D3DPRIMITIVETYPE type, uint primitive_count, uint index);
};

template <typename VBTYPE>
bool VBIWrapper<VBTYPE>::DrawIndexed(IDirect3DDevice8 *dev, D3DPRIMITIVETYPE type, uint count)
{
  HRESULT hr;

  hr = dev->SetIndices(_ib._ib, index);
  ASSERT_VIEWER(SUCCEEDED(hr), "Device::SetIndices.");
  hr = dev->SetStreamSource(0, _vb, sizeof(VBTYPE));
  ASSERT_VIEWER(SUCCEEDED(hr), "Device::SetStreamSource.");
  hr = dev->SetVertexShader(VBTYPE::fvf);
  ASSERT_VIEWER(SUCCEEDED(hr), "Device::SetVertexShader.");
  hr = dev->DrawIndexedPrimitive(type, 0, _count, 0, count);
  ASSERT_VIEWER(SUCCEEDED(hr), "Device::DrawIndexedPrimitive.");
  return true;
}

template <typename VBTYPE>
bool VBIWrapper<VBTYPE>::DrawIndexed(IDirect3DDevice8 *dev, D3DPRIMITIVETYPE type, uint primitive_count, uint index)
{
  HRESULT hr;

  hr = dev->SetIndices(_ib._ib, 0);
  ASSERT_VIEWER(SUCCEEDED(hr), "Device::SetIndices.");
  hr = dev->SetStreamSource(0, _vb, sizeof(VBTYPE));
  ASSERT_VIEWER(SUCCEEDED(hr), "Device::SetStreamSource.");
  hr = dev->SetVertexShader(VBTYPE::fvf);

  ASSERT_VIEWER(SUCCEEDED(hr), "Device::SetVertexShader.");
  hr = dev->DrawIndexedPrimitive(type, 0, _count, index, primitive_count);
  ASSERT_VIEWER(SUCCEEDED(hr), "Device::DrawIndexedPrimitive.");
  return true;
}

typedef VBIWrapper<vertex3d_t> VBIVect3d;

/************************************************************************/
/* Multi - Indexed  VB class                                            */
/************************************************************************/
template <typename VBTYPE>
class VBILWrapper : public VBWrapper<VBTYPE>
{
public:
  std::vector<IBWrapper> _ib;
  std::vector<uint> _primitive_count;
  void Deallocate()
  {
    VBWrapper<VBTYPE>::Deallocate();
    for (size_t i = 0; i < _ib.size(); ++i)
      _ib[i].Deallocate();
  }

  void PrealocateIBList(uint count)
  {
    _ib.resize(count);
    _primitive_count.resize(count);
  }
  bool CreateIB(IDirect3DDevice8 *dev, int index, uint index_count, uint primitive_count);
  bool DrawIndexed(IDirect3DDevice8 *dev, D3DPRIMITIVETYPE type);
};

template <typename VBTYPE>
bool VBILWrapper<VBTYPE>::CreateIB(IDirect3DDevice8 *dev, int index, uint index_count, uint primitive_count)
{
  _ib[index].Allocate(dev, index_count);
  _primitive_count[index] = primitive_count;
  return true;
}

template <typename VBTYPE>
bool VBILWrapper<VBTYPE>::DrawIndexed(IDirect3DDevice8 *dev, D3DPRIMITIVETYPE type)
{
  HRESULT hr;
  hr = dev->SetStreamSource(0, _vb, sizeof(VBTYPE));
  ASSERT_VIEWER(SUCCEEDED(hr), "Device::SetStreamSource.");
  hr = dev->SetVertexShader(VBTYPE::fvf);
  ASSERT_VIEWER(SUCCEEDED(hr), "Device::SetVertexShader.");
  for (uint i = 0; i < _primitive_count.size(); ++i)
  {
    hr = dev->SetIndices(_ib[i]._ib, 0);
    ASSERT_VIEWER(SUCCEEDED(hr), "Device::SetIndices.");
    hr = dev->DrawIndexedPrimitive(type, 0, _count, 0, _primitive_count[i]);
    ASSERT_VIEWER(SUCCEEDED(hr), "Device::DrawIndexedPrimitive.");
  }
  return true;
}

typedef VBILWrapper<vertex3d_t> VBILVect3d;

/************************************************************************/
/* Texture class                                                        */
/************************************************************************/
class Texture
{
public:
  Texture() : _data(0), _tex(0) {}
  ~Texture() { Deallocate(); }
  int _size_x;
  int _size_y;
  uchar *_data;
  void Draw(IDirect3DDevice8 *dev);
  bool GenerateD3DMesh(IDirect3DDevice8 *dev);

protected:
  void Deallocate();
  bool GenerateD3DTexture();
  VBVect2d _vb;
  LPDIRECT3DTEXTURE8 _tex;
};

/************************************************************************/
/* stage class                                                          */
/************************************************************************/
class Stage
{
public:
  Stage() : _texture(0){};
  ~Stage(){};

  Texture *_texture;
  bool _invert;
  bool _link;
  bool _loop;
  //	plane definition
  int _last_plane;
  int _first_plane;
  int _nb_plane;
};

/************************************************************************/
/* Funky code		                                                    */
/************************************************************************/
template <typename Ttype>
inline Ttype sqr(Ttype f)
{
  return f * f;
}

template <typename Ttype>
inline void swap(Ttype *f1, Ttype *f2)
{
  Ttype tmp;

  tmp = *f1;
  *f1 = *f2;
  *f2 = tmp;
}

inline Texture *GenerateSimpleBillboard(uint size_x, uint size_y)
{
  Texture *ret;

  ret = new Texture;
  ret->_size_x = size_x;
  ret->_size_y = size_y;
  ret->_data = new uchar[size_x * size_y];
  for (uint y = 0; y < size_y; ++y)
    for (uint x = 0; x < size_x; ++x)
    {
      uint color = 0xFF - (((uint)sqr(x - (size_x / 2)) + (uint)sqr(y - (size_y / 2))) << 1);
      if (color > 0xFF)
        color = 0;
      ret->_data[x + y * size_x] = color;
    }
  return ret;
}

const float const_max_depth = 255.0f;