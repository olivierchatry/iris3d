/*********************************************************
**  File name : IrisContext.hpp
**	Iris Engine V0.9 "alaiii"
**  Date Of Creation: 18/06/2002
**  Author : Olivier Chatry - Epitech Console Laboratory
**           (http://www.epitech.net/labconsole/)
*********************************************************/

//! Template class for manager, basically it just a simple reference counter.
template <typename TYPE>
class IrisManager
{
public:
  //!	Constructor of the IrisManager class.
  /*!
	*/
  IrisManager();
  //!	Destructor of the IrisManager class
  /*!
	Delete all data loaded via the manager.
	Reset reference counter.
	*/
  ~IrisManager();

  //! Free object from manager
  /*!
	Free an data from the manager regarding if it loaded more than one time.
	\param data a pointer to loaded data.
	\return a boolean, true if data have been freed, false if not. 
	*/
  bool Unload(TYPE *data);

  /*
	* Unload all object
	*/
  //!	DeleteAll function.
  /*!
	Free all data regardless how many time data in list have been instancied.
	\return a boolean.
	*/
  bool DeleteAll();
  //!	Count function.
  /*!
	\return an uint32 representating how many object are loaded.
	*/
  size_t Count();
  //!	Get function.
  /*!
	\param index an index value.
	\return a pointer to an IrisImportObject using index value.
	*/
  TYPE *Get(uint index);
  typedef std::vector<TYPE *>::iterator data_it;
  typedef std::vector<uint>::iterator reference_it;

protected:
  //! Data list.
  std::vector<TYPE *> _pointers;
  //! Reference list.
  std::vector<uint> _references;
};

template <typename TYPE>
IrisManager<TYPE>::IrisManager()
{
  _pointers.resize(0);
  _references.resize(0);
}

template <typename TYPE>
IrisManager<TYPE>::~IrisManager()
{
  printf("[IrisManager::~IrisManager] Cleaning data ...\n");
  DeleteAll();
}

template <typename TYPE>
bool IrisManager<TYPE>::Unload(TYPE *data)
{
  uint idx = 0;
  reference_it r = _references.begin();
  for (data_it i = _pointers.begin(); i != _pointers.end(); ++i)
  {
    TYPE *data_test = *i;
    if (data_test == data)
    {
      if (--_references[idx] == 0)
      {
        printf("[INFO] Unloading data %p\n", data);
        delete data;
        _pointers.erase(i);
        _references.erase(r);
      }
      else
      {
        printf("[INFO] Rest %d reference on %p\n", _references[idx], data);
      }
      return (true);
    }
    idx++;
    r++; // go next referenece iterator
  }
  return (false);
}

template <typename TYPE>
bool IrisManager<TYPE>::DeleteAll()
{
  data_it i;
  for (i = _pointers.begin(); i != _pointers.end(); ++i)
  {
    TYPE *data = *i;
    delete data;
  }
  _pointers.resize(0);
  _references.resize(0);
  return true;
}

template <typename TYPE>
size_t IrisManager<TYPE>::Count()
{
  return _pointers.size();
}

template <typename TYPE>
TYPE *IrisManager<TYPE>::Get(uint index)
{
  return _pointers[index];
}
