/*********************************************************
**  File name : IrisMaterial.cpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/
#include	"Iris.hpp"

IrisTexture::~IrisTexture()
{
	if (_file_name)
		delete [] _file_name;
}
/*
void	IrisMaterial::Generate(void (*fct_generate)(unsigned short *buffer, int size_x, int size_y), int size_x, int size_y, TextureType type)
{
	pvr_ptr_t txaddr;
	img.w = size_x;
	img.h = size_y;
	txaddr = pvr_mem_malloc(img.w * img.h * 2);
	fct_generate(txaddr, img.w, img.h);
	pvr_txr_load_kimg(&img, txaddr, PVR_TXRLOAD_INVERT_Y);
	cxt.txr.base = txaddr;
	cxt.txr.width = img.w;
	cxt.txr.height = img.h;	
	kos_img_free(&img, 0);
	switch(tex)
	{
	case texture_alpha:
		printf("loading alpha texture : %s\n", file_name);
		cxt.txr.format = PVR_TXRFMT_ARGB4444;
		cxt.txr.alpha = PVR_TXRALPHA_ENABLE;
		break;
	case texture_noalpha:
		printf("loading texture : %s\n", file_name);
		cxt.txr.format = PVR_TXRFMT_RGB565;
		cxt.txr.alpha = PVR_TXRALPHA_DISABLE;
		break;
	case texture_punchtru:
		printf("loading punchtru texture : %s\n", file_name);
		cxt.txr.format = PVR_TXRFMT_ARGB1555;
		cxt.txr.alpha = PVR_TXRALPHA_ENABLE;
		break;
	}
	cxt.txr.env = PVR_TXRENV_MODULATE;
	cxt.txr.enable = PVR_TEXTURE_ENABLE;
	cxt.txr.filter = PVR_FILTER_BILINEAR;
	cxt.txr.mipmap = PVR_MIPMAP_DISABLE;
	cxt.txr.mipmap_bias = PVR_MIPBIAS_NORMAL;
	cxt.txr.uv_flip = PVR_UVFLIP_NONE;
	cxt.txr.uv_clamp = PVR_UVCLAMP_NONE;
}
*/

bool	IrisMaterial::LoadFromFile(char *file_name, TextureType type = texture_noalpha)
{
	uint32			tmp;
	
	tmp = IrisContext::Get().LoadPNGFile(file_name, _cxt, type);
	_texture = new IrisTexture[1];
	IrisTexture * new_one = &(_texture[0]);
	new_one->SetId(tmp);
	new_one->SetFileName(file_name);
	new_one->SetType(type);
	_num_texture = 1;
	return (tmp != 0);
}

void	IrisMaterial::Destroy()
{
	if (_num_texture)
		delete [] _texture;
	IrisContext::Get().UnloadTexture(&_cxt);
	_num_texture = 0;
}

