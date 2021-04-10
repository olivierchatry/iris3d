/*********************************************************
**  File name : IrisSkeletalManager.hpp
**	Iris Engine V0.9 "alaiii"
**  Date Of Creation: 25/05/2003
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/

//!	IrisSkeletalManager class.
/*!
The IrisSkeletalManager class handle IrisSkeletal loading / managing. 
It allow you to load IrisSkeletal without carrying if this skeletal is already loaded in memory.
You can acces skeletal manager by calling IrisContext::GetSkeltalManager. 
*/
class IrisSkeletalManager : public IrisManager<IrisSkeletal>
{
public:
  //! LoadSkeletal function.
  /*!
	\param file_name : file name with path of the imd bones file.
	\return a pointer to a new IrisSkeletal.
	*/
  IrisSkeletal *LoadSkeletal(char *file_name);
};
