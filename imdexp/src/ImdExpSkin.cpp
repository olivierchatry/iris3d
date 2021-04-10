#include <stdafx.h>

//////////////////////////////////////////////////////////////////////////
//	skin importer
//////////////////////////////////////////////////////////////////////////

ImportedSkin *ImdExp::ImportSkinData(INode *node, ImportedMesh *imesh)
{
  // first we get how many bones are interfering with
  // our vertices.
  imesh->_skin = 0;
  // the user wan't to export skin data ?
  if (_plugin_config._export_skin == false)
    return 0;
  int num_bones = GetNumBones(ip->GetRootNode());
  Modifier *skin_modifier = FindModifier(node, SKIN_CLASSID);
  ImportedSkin *iskin = 0;
  int num_real_points = 0;
  if (skin_modifier)
  {
    iskin = new ImportedSkin;
    ISkin *skin = (ISkin *)skin_modifier->GetInterface(I_SKIN);
    if (skin == 0)
      return 0;
    ISkinContextData *skin_context = skin->GetContextInterface(node);
    if (skin_context == 0)
      return 0;
    const int num_points = skin_context->GetNumPoints();
    num_real_points = num_points;
    for (int i = 0; i < num_points; i++)
    {
      // get all bones for a given control point.
      num_bones = skin_context->GetNumAssignedBones(i);
      for (int j = 0; j < num_bones; j++)
      {
        const int assigned_bone = skin_context->GetAssignedBone(i, j);
        if (assigned_bone < 0)
          continue;
        INode *bone = skin->GetBone(assigned_bone);
        PointWeight pw;
        pw._weight = skin_context->GetBoneWeight(i, j);
        // vertex -> texture coord correspondance.
        for (size_t vi = 0; vi < imesh->_map_vertices.size(); ++vi)
        {
          MapVertice &mv = imesh->_map_vertices[vi];
          if (mv._original_indice == i)
          {
            pw._point_index = mv._indice;
            iskin->_skin_data[bone]._point_weight.push_back(pw);
          }
        }
        pw._point_index = i;
        iskin->_skin_data[bone]._point_weight.push_back(pw);
        iskin->_skin_data[bone]._bone_index = GetBoneIndex(bone);
        if (pw._weight == 1.0f)
          break; // go next point.
      }
    }
    skin_modifier->ReleaseInterface(I_SKIN, skin);
  }
  imesh->_skin = iskin;
  return iskin;
}
