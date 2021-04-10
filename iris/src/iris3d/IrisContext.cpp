/*********************************************************
**  File name : IrisContext.cpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/

#include "Iris.hpp"
KOS_INIT_ROMDISK(romdisk);

void IrisContext::InitContext(int vertex_buffer_size)
{
  pvr_init_params_t params = {{PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_16}, vertex_buffer_size};
  pvr_init(&params);
  pvr_poly_cxt_col(&_cxt, PVR_LIST_OP_POLY);
  _cxt.gen.culling = PVR_CULLING_NONE;
  _cxt.gen.shading = PVR_SHADE_GOURAUD;
  _dirty_header = true;
  _input_manager.Init();
  _input_manager.Update();
  _soft_clip = false;
  _lighting = false;
}

// -- optimized
static inline void *pvr_commit_init(void)
{
  QACR0 = ((((uint32)PVR_TA_INPUT) >> 26) << 2) & 0x1c;
  QACR1 = ((((uint32)PVR_TA_INPUT) >> 26) << 2) & 0x1c;
  return (void *)0xe0000000;
}

void IrisContext::DrawPrimitive(vertex2dtl v[], uint16 *indices, uint32 num_face, bool mode2d = false)
{
  if (_dirty_material)
    SetCurrentMaterial();
  if (_dirty_material || _dirty_header)
    SendPvrHeader();
  volatile pvr_vertex_t *vert = (pvr_vertex_t *)pvr_commit_init();
  if (mode2d)
  {
    while (num_face--)
    {
      vertex2dtl *the_vertex = &v[*indices++];
      // asm("pref @%0" : : "r" (&v[*indices]));
      vert->flags = (num_face == 0) ? PVR_CMD_VERTEX_EOL : PVR_CMD_VERTEX;
      vert->x = the_vertex->x;
      vert->y = the_vertex->y;
      vert->z = the_vertex->z;
      vert->u = the_vertex->u;
      vert->v = the_vertex->v;
      vert->argb = the_vertex->oargb;
      vert->oargb = the_vertex->oargb;
      pvr_dr_commit(vert);
      vert = (pvr_vertex_t *)((uint32)vert ^ 32);
    }
  }
  else
  {
    while (num_face--)
    {
      vertex2dtl *the_vertex = &v[*indices++];
      // asm("pref @%0" : : "r" (&v[*indices]));
      float rhw = 1.0f / the_vertex->z;
      vert->flags = (num_face == 0) ? PVR_CMD_VERTEX_EOL : PVR_CMD_VERTEX;
      vert->x = the_vertex->x * rhw;
      vert->y = the_vertex->y * rhw;
      vert->z = rhw;
      vert->u = the_vertex->u;
      vert->v = the_vertex->v;
      vert->argb = the_vertex->oargb;
      vert->oargb = the_vertex->oargb;
      pvr_dr_commit(vert);
      vert = (pvr_vertex_t *)((uint32)vert ^ 32);
    }
  }
}

void IrisContext::DrawPrimitiveIntersect(vect3d v3d[], vertex2dtl v[], uint16 indices[], uint32 num_face)
{
  if (_dirty_material)
    SetCurrentMaterial();
  if (_dirty_material || _dirty_header)
    SendPvrHeader();
  IrisFaceClipper::Draw(v3d, v, indices, num_face);
}

// new version : to do TEXTURE ALPHA for punchtru etc ...

uint32 IrisContext::LoadPNGFile(char *file_name, pvr_poly_cxt_t &cxt, TextureType tex /* TextureType::texture_noalpha*/)
{
  kos_img_t img;
  pvr_ptr_t txaddr;

  if (png_to_img(file_name, tex, &img) < 0)
  {
    printf("[IrisContext::LoadPNGFile] can't load %s\n", file_name);
    return 0;
  }
  txaddr = pvr_mem_malloc(img.w * img.h * 2);
  pvr_txr_load_kimg(&img, txaddr, 0);
  cxt.txr.base = txaddr;
  cxt.txr.width = img.w;
  cxt.txr.height = img.h;
  cxt.txr.env = PVR_TXRENV_MODULATE;
  cxt.txr.enable = PVR_TEXTURE_ENABLE;
  cxt.txr.filter = PVR_FILTER_BILINEAR;
  cxt.txr.mipmap = PVR_MIPMAP_DISABLE;
  cxt.txr.mipmap_bias = PVR_MIPBIAS_NORMAL;
  cxt.txr.uv_flip = PVR_UVFLIP_NONE;
  cxt.txr.uv_clamp = PVR_UVCLAMP_NONE;
  kos_img_free(&img, 0);
  switch (tex)
  {
  case texture_alpha:
    printf("[IrisContext::LoadPNGFile] loading alpha texture : %s\n", file_name);
    pvr_poly_cxt_txr(&cxt, PVR_LIST_TR_POLY, PVR_TXRFMT_ARGB4444, img.w, img.h, txaddr, PVR_FILTER_BILINEAR);
    cxt.txr.env = PVR_TXRENV_MODULATEALPHA;
    cxt.txr.alpha = PVR_TXRALPHA_ENABLE;
    break;
  case texture_noalpha:
    printf("[IrisContext::LoadPNGFile] loading texture : %s\n", file_name);
    pvr_poly_cxt_txr(&cxt, PVR_LIST_OP_POLY, PVR_TXRFMT_RGB565, img.w, img.h, txaddr, PVR_FILTER_BILINEAR);
    break;
  case texture_punchtru:
    printf("[IrisContext::LoadPNGFile] loading punchtru texture : %s\n", file_name);
    pvr_poly_cxt_txr(&cxt, PVR_LIST_PT_POLY, PVR_TXRFMT_ARGB1555, img.w, img.h, txaddr, PVR_FILTER_BILINEAR);
    cxt.txr.env = PVR_TXRENV_MODULATEALPHA;
    cxt.txr.alpha = PVR_TXRALPHA_ENABLE;
    break;
  }
  return (uint32)txaddr;
}
