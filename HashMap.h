#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <iostream>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <functional>


namespace aisdi
{

template <typename KeyType, typename ValueType>
class HashMap
{
	class Basket;
public:
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<const key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

  HashMap()
  {
    hashTable = new Basket*[allocSize]();
    basketSizes = new int[allocSize]();
  }
    ~HashMap()
    {
       cleanMap();
       delete [] hashTable;
       delete [] basketSizes;
    }

  HashMap(std::initializer_list<value_type> list) : HashMap()
  {
      for(auto it = list.begin(); it != list.end() ; ++it)
      {
		  this->operator[](it->first) = it->second;
      }
  }

  HashMap(const HashMap& other) : HashMap()
  {
      *this = other;
  }

  HashMap(HashMap&& other) : HashMap()
  {
      Basket **atcual = hashTable;
      int *act_size = basketSizes;

      basketSizes = other.basketSizes;
      hashTable = other.hashTable;
      other.basketSizes = act_size;
      other.hashTable = atcual;
  }

  HashMap& operator=(const HashMap& other)
  {
    if(&other != this)
    {
        cleanMap();
        for(auto it = other.begin(); it != other.end() ; ++it)
        {
            this->operator[](it->first) = it->second;
        }

    }
      return *this;
  }

  HashMap& operator=(HashMap&& other)
  {
      if(&other != this)
      {
          cleanMap();
		  Basket **atcual = hashTable;
		  int *act_size = basketSizes;

          basketSizes = other.basketSizes;
          hashTable = other.hashTable;
          other.basketSizes = act_size;
          other.hashTable = atcual;
      }
      return *this;
  }

 

  bool isEmpty() const
  {
	  return getSize() == 0;
    //throw std::runtime_error("TODO");
  }

  mapped_type& operator[](const key_type& key)
  {
	  int index = getHash(key);
	  //std::cout<<index<<std::endl;
	  if(hashTable[index] == nullptr)
	  {
		  hashTable[index] = new Basket(nullptr, nullptr, key, mapped_type{});
		  ++(basketSizes[index]);
		  return hashTable[index]->getValueType().second;
	  }
	  else
	  {
		  Basket *tmp = hashTable[index];
		  Basket *tmp2 = tmp;
		  while(tmp != nullptr)
		  {
			  if(tmp->getValueType().first == key) return tmp->getValueType().second;
			  tmp2 = tmp;
			  tmp = tmp->next;
		  }
		  tmp2 = new Basket(tmp, nullptr, key, mapped_type{});
		  ++(basketSizes[index]);
		  return tmp2->getValueType().second;
	  }
    /*(void)key;
    throw std::runtime_error("TODO");*/
  }
  
  Basket *findElement(const key_type& key) const
  {
	  int index = getHash(key);
	  if(hashTable[index] == nullptr) return nullptr;
	  else
	  {
		  Basket *tmp = hashTable[index];
		  while(tmp != nullptr)
		  {
			  if(tmp->getValueType().first == key) return tmp;
			  tmp = tmp->next;
		  }
	  }
	  return nullptr;
	  
  }

  const mapped_type& valueOf(const key_type& key) const
  {
	  Basket *tmp = findElement(key);
	  if(tmp == nullptr)
		  throw std::out_of_range("");
	  else return tmp->getValueType().second;
    /*(void)key;
    throw std::runtime_error("TODO");*/
  }

  mapped_type& valueOf(const key_type& key)
  {
	  Basket *tmp = findElement(key);
	  if(tmp == nullptr)
		  throw std::out_of_range("");
	  else return tmp->getValueType().second;
    /*(void)key;
    throw std::runtime_error("TODO");*/
  }

  const_iterator find(const key_type& key) const
  {
	  int index = getHash(key);
	  if(hashTable[index] != nullptr)
	  {
		  Basket *tmp = hashTable[index];
		  while(tmp != nullptr)
		  {
			  if(tmp->getValueType().first == key) 
				  return const_iterator(tmp, index, this);
			  tmp = tmp->next;
		  }
	  }

	  return cend();
    /*(void)key;
    throw std::runtime_error("TODO");*/
  }

  iterator find(const key_type& key)
  {
	  int index = getHash(key);
	  if(hashTable[index] != nullptr)
	  {
		  Basket *tmp = hashTable[index];
		  while(tmp != nullptr)
		  {
			  if(tmp->getValueType().first == key) 
				  return iterator(const_iterator(tmp, index, this));

			  tmp = tmp->next;
		  }
	  }
    
	  return end();
    /*(void)key;
    throw std::runtime_error("TODO");*/
  }
  

  void remove(const key_type& key)
  {
	  int index = getHash(key);

      if (!hashTable[index]) throw std::out_of_range("");

      Basket *tmp = hashTable[index];

      if (tmp == nullptr)
      {
		  throw std::out_of_range("");
      }

      if (tmp->previous)
      {
		  tmp->previous->next = tmp->next;
      }

      if (!tmp->previous)
      {
		  hashTable[index] = tmp->next;
      }

      if (tmp->next)
      {
		  tmp->next->previous = tmp->previous;
      }

       --basketSizes[index];
       delete tmp;
    /*(void)key;
    throw std::runtime_error("TODO");*/
  }

  void remove(const const_iterator& it)
  {
	  remove(it->first);
    /*(void)it;
    throw std::runtime_error("TODO");*/
  }

  size_type getSize() const
  {
	  size_type sum = 0;
	  for(int i = 0; i<allocSize; i++)
		sum += basketSizes[i];
	  return sum;
    //throw std::runtime_error("TODO");
  }

  bool operator==(const HashMap& other) const
  {
	  if (getSize() != other.getSize()) return false;
      else 
      {
		  auto it = cbegin();
          auto it2 = other.cbegin();

          while (it != cend() && it2 != other.cend()) 
          {
			  if (it->first != it2->first || it->second != it2->second) return false;
              ++it;
              ++it2;
          }
       return it == cend() && it2 == other.cend();
       }
    /*(void)other;
    throw std::runtime_error("TODO");*/
  }

  bool operator!=(const HashMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
	  return iterator(cbegin());
    //throw std::runtime_error("TODO");
  }

  iterator end()
  {
	  return iterator(cend());
    //throw std::runtime_error("TODO");
  }

  const_iterator cbegin() const
  {
	  Basket *tmp;
	  for(int i = 0; i<allocSize; ++i)
	  {
		  if(hashTable[i] != nullptr)
		  {
			  tmp = hashTable[i];
			  return const_iterator(tmp, i, this);
		  }
	  }
	  return cend();
  }

  const_iterator cend() const
  {
	  return const_iterator(nullptr, allocSize, this);
    //throw std::runtime_error("TODO");
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
//**********************************************************************
//**********************************************************************
//**********************************************************************

private:

	
	Basket **hashTable;	//array od baskets		
	int *basketSizes; // how much items in one of baskets, the same basket == the same hash
	static const int allocSize = 40;	// how much baskets we can use

	
	unsigned int getHash(const KeyType& key) const
	{
	  unsigned int ret = (unsigned int)abs(std::hash<key_type>{}(key));
	  ret = ret % allocSize;
	  return ret ;
	}
	
	void cleanMap()
	{
		if(!isEmpty())
		{
			for (int i = 0; i < allocSize; ++i) 
			{
				Basket *temp = hashTable[i];
				Basket *temp2;

				while (temp != nullptr) 
				{
					temp2 = temp->next;
					delete temp;
					temp = temp2;
				}
				hashTable[i] = nullptr;
				basketSizes[i] = 0;
			}
		}
	}


};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::ConstIterator
{
	HashMap::Basket *ptr;
	const HashMap *base;
	int index;
public:
  using reference = typename HashMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename HashMap::value_type;
  using pointer = const typename HashMap::value_type*;

  explicit ConstIterator()
  {
	  ptr = nullptr;
	  base = nullptr;
	  index = 0;
  }

  ConstIterator(const ConstIterator& other)
  {
	  ptr = other.ptr;
	  base = other.base;
	  index = other.index;
    /*(void)other;
    throw std::runtime_error("TODO");*/
  }
  
  ConstIterator(Basket *ptr, int index, const HashMap *base)
  {
	  this->ptr = ptr;
	  this->base = base;
	  this->index = index;
  }

  ConstIterator& operator++()
  {
	  if(index >= HashMap::allocSize || ptr == nullptr)
		throw std::out_of_range("");
	  if(ptr->next != nullptr)
		ptr = ptr->next;
	  else
	  {
		  ++index;
		  for(; index < HashMap::allocSize; ++index)
		  {
			  if (base->hashTable[index] != nullptr) 
			  {
				  ptr = base->hashTable[index];
                  return *this;
			  }
		  }
		  index = HashMap::allocSize;
		  ptr = nullptr;
	  }
	  return *this;
    //throw std::runtime_error("TODO");
  }

  ConstIterator operator++(int)
  {
	  ConstIterator it(this->ptr, this->index, this->base);
	  this->operator++();
	  return it;
    //throw std::runtime_error("TODO");
  }

  ConstIterator& operator--()
  {
	  if(*this == base->cbegin())
		throw std::out_of_range("");
      if (!ptr || !ptr->previous)
      {
		  for (--index; index >= 0; --index)
		  {
			  if (base->hashTable[index] != nullptr) 
			  {
				  Basket *tmp = base->hashTable[index];
                  while (tmp->next != nullptr)
                  {
					  tmp = tmp->next;
                  }

                  ptr = tmp;
                  return *this;
               }
		  }
       }
       else 
       {
		   ptr = ptr->previous;
       }
       return *this;
    //throw std::runtime_error("TODO");
  }

  ConstIterator operator--(int)
  {
	  ConstIterator it(this->ptr, this->index, this->base);
	  this->operator--();
	  return it;
    //throw std::runtime_error("TODO");
  }

  reference operator*() const
  {
	  if(ptr == nullptr)
		throw std::out_of_range("");
      return ptr->getValueType();
    //throw std::runtime_error("TODO");
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
	  if( base != other.base || index != other.index )
		return false;
	  return ptr == other.ptr;
    /*(void)other;
    throw std::runtime_error("TODO");*/
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator
{

public:
  using reference = typename HashMap::reference;
  using pointer = typename HashMap::value_type*;

  explicit Iterator()
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Basket
{
public:
	Basket *next;
	Basket *previous;
	value_type *value;
	
	Basket()
	{
		next = nullptr;
		previous = nullptr;
		value = nullptr;
	}
	
	Basket(Basket *previous, Basket *next, const key_type first, mapped_type second)
	{
		this->next = next;
		this->previous = previous;
		this->value = new value_type(first, second);
	}
	
	~Basket()
	{
		if(value != nullptr) delete value;
	}
	
	value_type &getValueType() { return *value; }
};

}

#endif /* AISDI_MAPS_HASHMAP_H */
