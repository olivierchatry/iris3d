#include <stdafx.h>
#include <algorithm>
#include "BIPEXP.H"

//////////////////////////////////////////////////////////////////////////
// bones importer
//////////////////////////////////////////////////////////////////////////
int ImdExp::RecursiveGetBoneIndex(INode *root, INode *node, int &index)
{
  int bone_index = -1;
  for (int i = 0; i < root->NumberOfChildren(); i++)
  {
    INode *child_node = root->GetChildNode(i);
    if (IsNodeBone(child_node))
    {
      if (child_node == node)
        return index;
      index++;
    }
    int bone_ret = RecursiveGetBoneIndex(root->GetChildNode(i), node, index);
    if (bone_ret != -1)
      return bone_ret;
  }
  return -1;
}

int ImdExp::GetBoneIndex(INode *node)
{
  int index = 0;
  return RecursiveGetBoneIndex(ip->GetRootNode(), node, index);
}

bool ImdExp::IsNodeBone(INode *node)
{
  Object *object = node->GetObjectRef();
  if (!object)
    return false;
  if (object->ClassID() == BONE_OBJ_CLASSID)
    return true;
  while (object->SuperClassID() == GEN_DERIVOB_CLASS_ID)
  {
    IDerivedObject *derived_object = static_cast<IDerivedObject *>(object);
    if (derived_object->ClassID() == BONE_OBJ_CLASSID)
      return true;
    object = derived_object->GetObjRef();
    if (object->ClassID() == BONE_OBJ_CLASSID)
      return true;
  }
  Control *controller = node->GetTMController();
  return (((controller->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) || (controller->ClassID() == BIPBODY_CONTROL_CLASS_ID)) && (controller->ClassID() != FOOTPRINT_CLASS_ID));
}

bool ImdExp::HaveChildBone(INode *node)
{
  for (int i = 0; i < node->NumberOfChildren(); ++i)
    if (IsNodeBone(node->GetChildNode(i)))
      return true;
  return false;
}

void ImdExp::ImportBoneAnimation(INode *node, BoneData *bone_data)
{
  Matrix3 baseTM = node->GetNodeTM(0);
  baseTM.Invert();
  TimeValue inc = GetTicksPerFrame();
  for (int i = _plugin_config._begin_bone_frame; i <=
                                                 _plugin_config._end_bone_frame;
       ++i)
  {
    TimeValue tv = i * inc;
    bone_data->_animation.push_back(BoneAnim());
    BoneAnim &bone_anim = bone_data->_animation.back();
    Matrix3 nodeTM = node->GetNodeTM(tv);
    bone_anim.matrix = FixCoordSys(baseTM * nodeTM);
    bone_anim.matrix.NoScale();
    bone_anim.pos = Point3(0.0f, 0.0f, 0.0f);
  }
}

void ImdExp::RecursiveImportBoneData(INode *node, ImportedBone *bone, BoneData *bone_data)
{
  // import animation.
  for (int i = 0; i < node->NumberOfChildren(); ++i)
  {
    INode *child_node = node->GetChildNode(i);
    if (IsNodeBone(child_node))
    {
      BoneData *child_data = 0;
      if (bone_data != 0)
      {
        bone_data->_bone_child.push_back(BoneData());
        child_data = &(bone_data->_bone_child.back());
      }
      else
      {
        bone->_bone_data.push_back(BoneData());
        child_data = &(bone->_bone_data.back());
      }
      child_data->_bone_index = GetBoneIndex(child_node);
      child_data->_name = child_node->GetName();
      ImportBoneAnimation(child_node, child_data);
      RecursiveImportBoneData(child_node, bone, child_data);
    }
    else
      RecursiveImportBoneData(child_node, bone, bone_data);
  }
}

// import bone data from root node.
ImportedBone *ImdExp::ImportBoneData()
{
  ImportedBone *bone;
  INode *node = ip->GetRootNode();
  int bone_count = GetNumBones(node);
  if (bone_count == 0)
    return 0;
  _log->Printf("++ [ImdExp::ImportBoneData] number of bones = %d", bone_count);
  bone = new ImportedBone;
  RecursiveImportBoneData(node, bone);
  _log->Print("-- [ImdExp::ImportBoneData]");
  return bone;
}
