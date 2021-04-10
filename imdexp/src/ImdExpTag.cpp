#include <stdafx.h>

//////////////////////////////////////////////////////////////////////////
// tag importer
//////////////////////////////////////////////////////////////////////////
Point3 ImdExp::GetCenter(Mesh &mesh)
{
  Point3 p;
  int vertex_count = mesh.getNumVerts();

  for (int i = 0; i < vertex_count; ++i)
    p += mesh.verts[i];
  return p / ((float)i);
}

ImportedTag *ImdExp::ImportTagObject(INode *node, ObjectState &os)
{
  _log->Print("  + Importing Tag Object");
  ImportedTag *imported_tag = new ImportedTag;
  Matrix3 offset_matrix = GetNodeOffsetTM(node);
  Mesh &mesh = GetTriObjectFromNode(node, 0)->GetMesh();
  Interval anim_range(_plugin_config._begin_frame, _plugin_config._end_frame);
  int tag_index = 0;
  if (imported_tag == 0)
    return 0;
  imported_tag->_name = node->GetName();
  imported_tag->_tag_data.resize(_plugin_config._end_frame - _plugin_config._begin_frame + 1);
  TimeValue inc = GetTicksPerFrame();
  for (TimeValue i = _plugin_config._begin_frame; i <= _plugin_config._end_frame; i++)
  {
    AffineParts ap;
    TimeValue tv = i * inc;
    Matrix3 m = FixCoordSys(offset_matrix * node->GetNodeTM(tv) * Inverse(node->GetParentTM(tv)));
    TagData *tag_data = new TagData;
    imported_tag->_tag_data[tag_index++] = tag_data;
    tag_data->_pos = m * GetCenter(mesh);
  }
  TSTR buffer;
  node->GetUserPropBuffer(buffer);
  imported_tag->_user_properties = buffer.data();
  imported_tag->_name = node->GetName();
  _log->Printf("Properties = %s", imported_tag->_user_properties.c_str());

  return imported_tag;
}
