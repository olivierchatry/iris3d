#include <stdafx.h>

const DWORD vertex3d_t::fvf = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);
const DWORD vertex2d_t::fvf = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

/************************************************************************/
/* indexed buffer class													*/
/************************************************************************/
bool IBWrapper::Allocate(IDirect3DDevice8 *dev, uint count)
{
  HRESULT hr;

  hr = dev->CreateIndexBuffer(count * sizeof(WORD),
                              D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
                              D3DPOOL_DEFAULT, &_ib);
  ASSERT_VIEWER(SUCCEEDED(hr), "Device::CreateIndexBuffer");
  _count = count;
  return true;
}

void IBWrapper::Deallocate()
{
  if (_ib)
  {
    _ib->Release();
    _ib = 0;
  }
}

WORD *IBWrapper::Lock()
{
  D3DINDEXBUFFER_DESC desc;
  HRESULT hr;
  WORD *res;
  hr = _ib->GetDesc(&desc);
  ASSERT_VIEWER(SUCCEEDED(hr), "IndexedBuffer::GetDesc");
  hr = _ib->Lock(0, desc.Size, (BYTE **)&res, 0);
  ASSERT_VIEWER(SUCCEEDED(hr), "IndexedBuffer::Lock");
  return res;
}

bool IBWrapper::Unlock()
{
  HRESULT hr;
  hr = _ib->Unlock();
  ASSERT_VIEWER(SUCCEEDED(hr), "IndexedBuffer::Unlock");
  return true;
}

/************************************************************************/
/* texture class                                                        */
/************************************************************************/
void Texture::Deallocate()
{
  _vb.Deallocate();
  if (_data)
    delete[] _data;
  if (_tex)
  {
    _tex->Release();
    _tex = 0;
  }
}

bool Texture::GenerateD3DTexture()
{
  D3DLOCKED_RECT rect;
  HRESULT hr;
  uint *ptr;

  hr = _tex->LockRect(0, &rect, NULL, 0);
  ASSERT_VIEWER(SUCCEEDED(hr), "Texture::LockRect.");
  ptr = (uint *)rect.pBits;
  for (int linear = 0; linear < _size_x * _size_y; linear++)
  {
    char c = _data[linear];
    ptr[linear] = D3DCOLOR_ARGB(0xff, c, c, c);
  }
  hr = _tex->UnlockRect(0);
  ASSERT_VIEWER(SUCCEEDED(hr), "Texture::UnlockRect.");
  return true;
}

bool Texture::GenerateD3DMesh(IDirect3DDevice8 *dev)
{
  vertex2d_t *c;

  _vb.Allocate(dev, 4, D3DPT_TRIANGLESTRIP);
  c = _vb.Lock();

  c[0]._pos.x = 0.0f;
  c[0]._pos.y = (float)_size_y;
  c[0]._pos.z = 0.0f;
  c[0]._u = 0.0f;
  c[0]._v = 1.0f;

  c[1]._pos.x = 0.0f;
  c[1]._pos.y = 0.0f;
  c[1]._pos.z = 0.0f;
  c[1]._u = 0.0f;
  c[1]._v = 0.0f;

  c[2]._pos.x = (float)_size_x;
  c[2]._pos.y = (float)_size_y;
  c[2]._pos.z = 0.0f;
  c[2]._u = 1.0f;
  c[2]._v = 1.0f;

  c[3]._pos.x = (float)_size_x;
  c[3]._pos.y = 0.0f;
  c[3]._pos.z = 0.0f;
  c[3]._u = 1.0f;
  c[3]._v = 0.0f;
  for (int i = 0; i < 4; ++i)
    c[i]._rhw = 1.0f;
  _vb.Unlock();

  HRESULT hr;
  hr = dev->CreateTexture(_size_x, _size_y, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &_tex);
  ASSERT_VIEWER(SUCCEEDED(hr), "Device::CreateTexture.");
  GenerateD3DTexture();
  return true;
}

void Texture::Draw(IDirect3DDevice8 *dev)
{
  dev->SetTexture(0, _tex);
  _vb.Draw(dev, 2);
  dev->SetTexture(0, 0);
}
