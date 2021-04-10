/*********************************************************
** File name : InputManager.hpp
** Iris Engine V0.7 "presque"
** Date of creation: 25/06/2002
** Author : Olivier Chatry - Epitech Console Laboratory
**          (http://www.epitech.net/labconsole/)
*********************************************************/

#ifndef __INPUTMANAGER_HPP__
#define __INPUTMANAGER_HPP__

//! IrisController class.
/*!
	The IrisController controller class describe a paddle from dreamcast. It can be acceded by using InputManager::GetController()

	Source code : Simple input handling (sample/input/).
	Html source code : Simple input handling (sample/input/). 
*/
class IrisController
{
public:
  //! Constructor of the Paddle class.
  /*!		
	*/
  IrisController() { _id = 0; }
  //! Constructor of the Paddle class.
  /*!	
		\param id an uint8 : id of the paddle
	*/
  IrisController(uint8 id) { _id = id; }
  //! Up function.
  /*!		
	\return a boolean, true if up is pressed, false if not.
	*/
  const bool Up() const { return (_cond.buttons & CONT_DPAD_UP) != CONT_DPAD_UP; }

  //! OldUp function.
  /*!		
	\return a boolean, true if up is pressed, false if not.(the frame before current frame).
	*/
  const bool OldUp() const { return (_old_cond.buttons & CONT_DPAD_UP) != CONT_DPAD_UP; }

  //! Down function.
  /*!		
	\return a boolean, true if down is pressed, false if not.
	*/
  const bool Down() const { return (_cond.buttons & CONT_DPAD_DOWN) != CONT_DPAD_DOWN; }
  //! OldDown function.
  /*!		
	\return a boolean, true if down is pressed, false if not.(the frame before current frame).
	*/
  const bool OldDown() const { return (_old_cond.buttons & CONT_DPAD_DOWN) != CONT_DPAD_DOWN; }

  //! Right function.
  /*!		
	\return a boolean, true if right is pressed, false if not. 
	*/
  const bool Right() const { return (_cond.buttons & CONT_DPAD_RIGHT) != CONT_DPAD_RIGHT; }

  //! OldRight function.
  /*!		
	\return a boolean, true if right is pressed, false if not. (the frame before current frame).
	*/
  const bool OldRight() const { return (_old_cond.buttons & CONT_DPAD_RIGHT) != CONT_DPAD_RIGHT; }

  //! Left function.
  /*!		
	\return a boolean, true if left is pressed, false if not.
	*/
  const bool Left() const { return (_cond.buttons & CONT_DPAD_LEFT) != CONT_DPAD_LEFT; }

  //! OldLeft function.
  /*!		
	\return a boolean, true if left is pressed, false if not.(the frame before current frame).
	*/
  const bool OldLeft() const { return (_old_cond.buttons & CONT_DPAD_LEFT) != CONT_DPAD_LEFT; }
  //! RightTrigger function.
  /*!		
	\return a float value beetwwen 0.0f and 1.0f representing how the right trigger is pushed. (non pushed = 0.0f, full pushed = 1.0f)
	*/
  const float RightTrigger() const { return (_cond.rtrig / 255.0f); } // 0 -> 1.0

  //! OldRightTrigger function.
  /*!		
	\return a float value beetwwen 0.0f and 1.0f representing how the right trigger is pushed. (non pushed = 0.0f, full pushed = 1.0f) (the frame before current frame).
	*/
  const float OldRightTrigger() const { return (_old_cond.rtrig / 255.0f); } // 0 -> 1.0
  //! LeftTrigger function.
  /*!		
	\return a float value beetwwen 0.0f and 1.0f representing how the left trigger is pushed. (non pushed = 0.0f, full pushed = 1.0f).
	*/
  const float LeftTrigger() const { return (_cond.ltrig / 255.0f); } // 0 -> 1.0

  //! OldLeftTrigger function.
  /*!		
	\return a float value beetwwen 0.0f and 1.0f representing how the left trigger is pushed. (non pushed = 0.0f, full pushed = 1.0f) (the frame before current frame).
	*/
  const float OldLeftTrigger() const { return (_old_cond.ltrig / 255.0f); } // 0 -> 1.0
  //! AnalogVertical function.
  /*!		
	\return a float value beetwwen -0.5f and 0.5f representing how the vertical analog joypad is pushed. (non pushed = 0.0f, full pushed up = 0.5f, full pushed down = -0.5f)
	*/
  const float AnalogVertical() const { return (_cond.joyy / 255.0f) - 0.5f; } // -0.5 --> 0.5

  //! OldAnalogVertical function.
  /*!		
	\return a float value beetwwen -0.5f and 0.5f representing how the vertical analog joypad is pushed. (non pushed = 0.0f, full pushed up = 0.5f, full pushed down = -0.5f) (the frame before current frame).
	*/
  const float OldAnalogVertical() const { return (_old_cond.joyy / 255.0f) - 0.5f; } // -0.5 --> 0.5
  //! AnalogHorizontal function.
  /*!		
	\return a float value beetwwen -0.5f and 0.5f representing how the horizontal analog joypad is pushed. (non pushed = 0.0f, full pushed up = 0.5f, full pushed down = -0.5f)
	*/
  const float AnalogHorizontal() const { return (_cond.joyx / 255.0f) - 0.5f; } // -0.5 --> 0.5

  //! OldAnalogHorizontal function.
  /*!		
	\return a float value beetwwen -0.5f and 0.5f representing how the horizontal analog joypad is pushed. (non pushed = 0.0f, full pushed up = 0.5f, full pushed down = -0.5f) (the frame before current frame).
	*/
  const float OldAnalogHorizontal() const { return (_old_cond.joyx / 255.0f) - 0.5f; } // -0.5 --> 0.5
  //! a function.
  /*!		
	\return a boolean, true if the button a is pressed, false if not.
	*/
  const bool a() const { return (_cond.buttons & CONT_A) != CONT_A; }

  //! Olda function.
  /*!		
	\return a boolean, true if the button a is pressed, false if not (the frame before current frame).
	*/
  const bool Olda() const { return (_old_cond.buttons & CONT_A) != CONT_A; }
  //! b function.
  /*!		
	\return a boolean, true if the button b is pressed, false if not.
	*/
  const bool b() const { return (_cond.buttons & CONT_B) != CONT_B; }
  //! oldb function.
  /*!		
	\return a boolean, true if the button b is pressed, false if not (the frame before current frame).
	*/
  const bool Oldb() const { return (_old_cond.buttons & CONT_B) != CONT_B; }
  //! x function.
  /*!		
	\return a boolean, true if the button x is pressed, false if not.
	*/
  const bool x() const { return (_cond.buttons & CONT_X) != CONT_X; }
  //! Oldx function.
  /*!		
	\return a boolean, true if the button x is pressed, false if not (the frame before current frame).
	*/
  const bool Oldx() const { return (_old_cond.buttons & CONT_X) != CONT_X; }
  //! y function.
  /*!		
	\return a boolean, true if the button y is pressed, false if not.
	*/
  const bool y() const { return (_cond.buttons & CONT_Y) != CONT_Y; }
  //! Oldy function.
  /*!		
	\return a boolean, true if the button y is pressed, false if not (the frame before current frame).
	*/
  const bool Oldy() const { return (_old_cond.buttons & CONT_Y) != CONT_Y; }
  //! Start function.
  /*!		
	\return a boolean, true if the button Start is pressed, false if not.
	*/
  const bool Start() const { return (_cond.buttons & CONT_START) != CONT_START; }
  //! Start function.
  /*!		
	\return a boolean, true if the button Start is pressed, false if not (the frame before current frame).
	*/
  const bool OldStart() const { return (_old_cond.buttons & CONT_START) != CONT_START; }
  //! HaveVMU function.
  /*!
		\return a boolean, true if the paddle have a VMU (Visual Memory Unit) inserted, false if not.

		Note :This function is not implemeted. It return false in any case. 			
	*/
  const bool HaveVMU() const { return false; }
  //! GetCond function.
  /*!		
		\return the KOS cont_cond_t structure.
	*/
  cont_cond_t &GetCond() { return _cond; }
  cont_cond_t &GetOldCond() { return _old_cond; }
  //! Id function.
  /*!
		\return a uint8, the maple adress of paddle.
	*/
  const uint8 Id() const { return _id; }

protected:
  uint8 _id;
  cont_cond_t _cond;
  cont_cond_t _old_cond;
};

//! InputManager class.
/*!
	Iris3D Input manager handle common function developper needs for using input. In it current state, only paddle are handled. 
	You can acces InputManger thru IrisContext class using IrisContext::Get().GetInputManager();
	Source code : <u>Simple input handling (samples/input/)</u>.
	Html source code : <u>Simple input handling (sample/input/)</u>.
*/
class IrisInputManager
{
public:
  //!  Constructor of the InputManager class.
  /*!
	*/
  IrisInputManager();
  //!  Destructor of the InputManager class.
  /*!
	*/
  ~IrisInputManager() { _controller.resize(0); };
  //!	 Update function.
  /*!
		Update controller state.
		Automatically called when you use the method IrisContext::BeginScene();.
		\return nothing.
	*/
  void Update();
  //!	 Init function.
  /*!
		Search for controller on mapple bus. This function is automatically called when you use IrisContext::InitRender();, 
		but you can call it whenever you wan't.

		Note : Controller are only searched when this function is called, Iris3D do not handle pluggable device, so you have to call this function for know if new paddle have been plugged.
		\return nothing. 
	*/
  void Init();
  /*
	**	return the number of paddle found on mapple bus
	*/
  //!	 ControllerCount function.
  /*!
		\return an int : the number of paddle found on mapple bus
	*/
  uint ControllerCount() { return _controller.size(); }
  /*
	**	return the paddle (in connected order) 
	*/
  //!	GetController function.
  /*!
		\return a reference to the paddle represented by the value num. If num equal to zero, GetController return the first paddle found on maple bus. The reference is of type Paddle
	*/
  IrisController &GetController(uint i) { return *_controller[i]; }

protected:
  std::vector<IrisController *> _controller;
};

inline IrisInputManager::IrisInputManager()
{
  _controller.resize(0);
}

/*
**	Enumerate all device in mapple port en get controller
*/
inline void IrisInputManager::Init()
{
  uint32 func;

  std::vector<IrisController *>::iterator it;
  for (it = _controller.begin(); it != _controller.end(); ++it)
    delete *it;
  _controller.resize(0);
  for (int p = 0; p < MAPLE_PORT_COUNT; ++p)
    for (int u = 0; u < MAPLE_UNIT_COUNT; ++u)
    {
      func = maple_device_func(p, u);
      if (func & MAPLE_FUNC_CONTROLLER)
      {
        IrisController *paddle = new IrisController(maple_addr(p, u));
        _controller.push_back(paddle);
      }
    }
}

/*
** Update data of all controller in the list
*/
inline void IrisInputManager::Update()
{
  std::vector<IrisController *>::iterator it;
  for (it = _controller.begin(); it != _controller.end(); ++it)
  {
    cont_cond_t &old_cont = (*it)->GetOldCond();
    cont_cond_t &cont = (*it)->GetCond();
    old_cont.buttons = cont.buttons;
    old_cont.joyy = cont.joyy;
    old_cont.joyx = cont.joyx;
    old_cont.ltrig = cont.ltrig;
    old_cont.rtrig = cont.rtrig;
    cont_get_cond((*it)->Id(), &cont);
  }
}
#endif //__INPUTMANAGER_HPP__
