#include <stdafx.h>

//////////////////////////////////////////////////////////////////////////
// material
//////////////////////////////////////////////////////////////////////////
MaterialData	*ImdExp::ImportMaterialData(Mtl *max_material, ObjectState &os)
{
	_log->Print("  + Importing material");

	if (max_material->NumSubMtls() > 0)
	{
		// exporting only first sub material
		return ImportMaterialData(max_material->GetSubMtl(0), os);
	}
	else
		if ( max_material->ClassID( ) == Class_ID(DMTL_CLASS_ID, 0) )
		{

			StdMat* std_material = (StdMat*) max_material;
			// Export diffuse map
			ImportedMaterial	*material;
			if (_material_list != 0)
				material = _material_list;
			else
				material = new ImportedMaterial;
			// look if material already in our list
			ImportedMaterial::material_data_list_it_t it;
			for (it = material->_material_data.begin(); it != material->_material_data.end(); ++it)
			{
				MaterialData	*tmp = *it;
				if (strcmp(tmp->_name, max_material->GetName()) == 0)
				{
					_log->Print("    ! material already in list");
					return tmp;
				}
			}
			MaterialData	*material_data = new MaterialData;
			memset(material_data, 0, sizeof(MaterialData));
			material_data->_name = max_material->GetName();
			_log->Printf("    + Name [%s]", material_data->_name);
			Texmap *texture = max_material->GetSubTexmap(ID_DI);
			if ( texture )
			{
				if ( texture->ClassID( ) == Class_ID(BMTEX_CLASS_ID, 0) )
				{
					// This is a bitmap texture
					BitmapTex *Bitmap = ( BitmapTex* )texture;
					material_data->_diffuse_map = Bitmap->GetMapName();
					_log->Printf("    + Diffuse map [%s]", material_data->_diffuse_map);
				}
			}
			texture = 0;
			texture = max_material->GetSubTexmap( ID_RL );
			if( texture )
			{
				if ( texture->ClassID( ) == Class_ID(BMTEX_CLASS_ID, 0) )
				{
					BitmapTex *Bitmap = ( BitmapTex* )texture;
					material_data->_env_map = Bitmap->GetMapName();
					_log->Printf("    + Env map [%s]", material_data->_env_map);
				}
			}
			// if we can get diffuse texture or env texture push it to our material
			// list, else just drop it.
			_material_list = material;
			if (material_data->_diffuse_map || material_data->_env_map)
			{
				material->_material_data.push_back(material_data);	
				return material_data;
			}
			else
				delete material_data;
		}
		return 0;
}

