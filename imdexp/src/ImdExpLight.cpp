#include <stdafx.h>
//////////////////////////////////////////////////////////////////////////
// light importer
//////////////////////////////////////////////////////////////////////////
void ImdExp::ImportLightPos(INode *node, TimeValue t, LightData &la)
{
  AffineParts ap;
  Matrix3 m = node->GetNodeTM(t) * Inverse(node->GetParentTM(t));
  decomp_affine(m, &ap);
  la._pos = ap.t;
}

void ImdExp::ImportLightData(INode *node, ObjectState &os)
{
  TimeValue t = _plugin_config._begin_frame;
  GenLight *light = (GenLight *)os.obj;
  struct LightState ls;
  Interval valid = FOREVER;
  Interval animRange(_plugin_config._begin_frame, _plugin_config._end_frame);
  ImportedLight *imported_light = new ImportedLight;
  light->EvalLightState(t, valid, &ls);
  if (ls.on == FALSE)
  {
    delete imported_light;
    return;
  }
  _log->Print("Importing Light");
  switch (ls.type)
  {
  case TSPOT_LIGHT:
  case FSPOT_LIGHT:
  case OMNI_LIGHT:
    imported_light->_light_type = ImportedLight::light_omni;
    _log->Print("Omnidirectional light");
    break;
  case DIR_LIGHT:
    imported_light->_light_type = ImportedLight::light_directionnal;
    _log->Print("Directional light");
    break;
  }
  INode *target = node->GetTarget();
  imported_light->_have_target = (target != 0);
  imported_light->_color = (DWORD)ls.color;
  // import light animation. All animation are sampled.
  // time are not frame relative ...
  TimeValue start = ip->GetAnimRange().Start();
  TimeValue end = ip->GetAnimRange().End();
  TimeValue inc = GetTicksPerFrame();
  for (TimeValue i = _plugin_config._begin_frame; i <= _plugin_config._end_frame; i++)
  {
    LightData la;
    ImportLightPos(node, i * inc, la);
    if (target)
    {
      LightData target_la;
      ImportLightPos(target, i * inc, target_la);
      la._dir = target_la._pos - la._pos;
      la._dir = la._dir.Normalize();
    }
    imported_light->_data.push_back(la);
  }
  _elements.push_back(imported_light);
}
