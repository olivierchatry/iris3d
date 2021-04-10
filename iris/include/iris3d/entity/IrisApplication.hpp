/*********************************************************
**  File name : IrisApplication.hpp
**  Iris Engine V0.9 "alllaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/
#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__
using namespace std;
const uint exit_value = ~0;

typedef set<IrisApplicationPart *> AppPartSet;
typedef set<IrisApplicationPart *>::iterator AppPartSetIt;

class IrisApplication
{
public:
  IrisApplication() { _initial = NULL; }
  virtual ~IrisApplication() {}
  /*
	**	Main	fonction, called once
	*/
  virtual int main();
  /*
	**	Register a new part in the application
	*/
  virtual void RegisterPart(IrisApplicationPart *part, bool part_initial = false)
  {
    _part.insert(part);
    part_initial ? _initial = part : 0;
  }
  virtual void DoInitialInit(void);
  /**
	 * Return an application part
	 * @param part_id id of application part that you want.
	 */
  IrisApplicationPart *GetPart(unsigned part_id);

protected:
  /*
	**	Return intial part (first part to be used)
	*/
  virtual IrisApplicationPart *InitialPart();
  /*
	**	Search for part with id (code_id), if not found
	**	return initial part;
	*/
  virtual IrisApplicationPart *ChangePart(unsigned code_id);
  AppPartSet _part;
  IrisApplicationPart *_initial;
  IrisApplicationPart *_current;
};

inline IrisApplicationPart *IrisApplication::InitialPart()
{
  assert(_initial);
  return _initial;
}

inline IrisApplicationPart *IrisApplication::ChangePart(unsigned code_id)
{
  AppPartSetIt it;
  for (it = _part.begin(); it != _part.end(); ++it)
  {
    if ((*it)->Id() == code_id)
      return (*it);
  }
  // by default, return initial part
  return InitialPart();
};

inline void IrisApplication::DoInitialInit(void)
{
  AppPartSetIt it;
  for (it = _part.begin(); it != _part.end(); ++it)
    (*it)->InitialInit();
}

inline int IrisApplication::main()
{
  uint id = 0;
  IrisContext &c = IrisContext::Get();
  c.InitContext(1024 * 1024);
  // printf("Number of paddle = %d\n",c.GetInputManager().PaddleCount());
  _current = InitialPart();
  _current->Init();
  while (id != exit_value)
  {
    id = _current->Run(*this);
    if (id != exit_value)
    {
      _current->Desinit();
      _current = ChangePart(id);
      _current->Init();
    }
  }
  return (0);
}

inline IrisApplicationPart *IrisApplication::GetPart(unsigned code_id)
{
  AppPartSetIt it;
  for (it = _part.begin(); it != _part.end(); ++it)
  {
    if ((*it)->Id() == code_id)
      return (*it);
  }
  return NULL;
}

#define DECLARE_MAIN_BEGIN(App) \
  int main(void)                \
  {                             \
    App the_app;                \
    fabs(0.0);
#define DECLARE_PART(part, def) the_app.RegisterPart(new part, def);

#define DECLARE_MAIN_END() \
  the_app.DoInitialInit(); \
  return the_app.main();   \
  }

#endif
