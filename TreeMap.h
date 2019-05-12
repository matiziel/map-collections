#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace aisdi
{

template <typename KeyType, typename ValueType>
class TreeMap
{
	class Node;
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
  
  

  TreeMap()
  {
	  root = nullptr;
	  size = 0;
  }

  TreeMap(std::initializer_list<value_type> list) : TreeMap()
  {
	  for (auto it = list.begin(); it != list.end(); ++it) 
	  {
		  this->operator[](it->first) = it->second;
      }
        
    /*(void)list; // disables "unused argument" warning, can be removed when method is implemented.
    throw std::runtime_error("TODO");*/
  }
  void clean(Node *n)
  {
        Node *tmp;
        if (n==nullptr || size == 0)
            return;
        clean(n->right);
        clean(n->left);
        size--;
        tmp = removeElement(n);
        delete tmp;
   }
  
   ~TreeMap()
   {
	   clean(root);
	   root = nullptr;
   }

  TreeMap(const TreeMap& other) : TreeMap()
  {
	  for(const_iterator it = other.begin(); it != other.cend(); ++it)
          this->operator[](it->first) = it->second;
      size = other.size;

    /*(void)other;
    throw std::runtime_error("TODO");*/
  }

  TreeMap(TreeMap&& other) : root(other.root), size(other.size)
  {
		other.root = nullptr;
		other.size = 0;
    /*(void)other;
    throw std::runtime_error("TODO");*/
  }

  TreeMap& operator=(const TreeMap& other)
  {
	   if (this ==&other)
            return *this;
       if(!isEmpty())
       {
		   clean(root);
		   root = nullptr;
	   }
       
       for(iterator it = other.begin(); it !=other.end(); ++it)
            this->operator[](it->first) = it->second;
       size = other.size;
       return *this;


    /*(void)other;
    throw std::runtime_error("TODO");*/
  }

  TreeMap& operator=(TreeMap&& other)
  {
		clean(root);
		root = nullptr;
        root = other.root;
        size = other.size;
        other.root = nullptr;
        other.size = 0;
        return *this;
    /*(void)other;
    throw std::runtime_error("TODO");*/
  }

  bool isEmpty() const
  {
	  return size == 0;
    //throw std::runtime_error("TODO");
  }

  mapped_type& operator[](const key_type& key)
  {
	  Node *tmp = root;
	  Node *parent = nullptr;
    Node *newElement = nullptr;
    Node *rtt = nullptr;
    bool check;


	  while(tmp!=nullptr)
	  {
		  parent = tmp;
		  if(tmp->getValueType().first == key)
			return tmp->getValueType().second;
			
		  else if(tmp->getValueType().first < key)
			tmp = tmp->right;
			
		  else if(tmp->getValueType().first > key)
			tmp = tmp->left;
	  }
	  
	  newElement = new Node(parent, key, mapped_type{});
	  if(parent != nullptr)
	  {
		  if(key > parent->getValueType().first)
			  parent->right = newElement;
		  else
			  parent->left = newElement;
	  }
	  else 
    {
		  root = newElement;
      ++size;
      return newElement->getValueType().second;
      
	  }
	  ++size;
    //*************************************//
    // ********** ROTACJA *****************//
    //*************************************//

    if(parent->balance) parent->balance = 0; // jesli parent ma współczynnik wywazenia 1 lub -1 to po dodaniu ma zero i drzewo jest wywazone
    else
    {
      if(parent->left == newElement)   
        parent->balance = 1;
      else
        parent->balance = -1;

      rtt = parent->parent;        // będziemy szli w górę drzewa w kierunku korzenia
                        // rtt i parent wskazują ojca i syna na tej ścieżce
      check = false;
      while(rtt != nullptr)
      {
        if(rtt->balance)    
        {
          check = true;     // ustalamy wynik pętli jezeli wsp rownowagi jest -1 lub 1, poniewaz dodalismy wezel i przy wsp +-1 zwiekszy sie do +-2 
          break;        // przerywamy pętlę poniewaz na tym wezle wykoujemy rotacje
        }
                        // inaczej modyfikujemy rtt->balance, poniewaz dodalismy wezel
        if(rtt->left == parent) 
          rtt->balance =  1;
        else             
          rtt->balance = -1;

        parent = rtt;          // przechodzimy w górę na wyższy poziom
        rtt = rtt->parent;
      }

      if(check)             // jeśli rtt->balance = +/- 1, to musimy
      {                 // równoważyć drzewo
        if(rtt->balance == 1)         //jesli lewe poddrzewo bylo dłuzsze
        {
          if(rtt->right == parent)  //jesli dodany wezel jest w prawym poddrzewie
            rtt->balance = 0;  // gałęzie się równoważą, poniewaz dodalismy do prawej czyli krotszego
          else if(parent->balance == -1) 
            rotate_LR(rtt);
          else                 
            rotate_LL(rtt);
        }
        else               //jesli prawe poddrzewo bylo dłuzsze           
        {              
          if(rtt->left == parent)  //jesli dodany wezel jest w lewym poddrzewie
            rtt->balance = 0;  // gałęzie się równoważą poniewaz dodalismy do lewej czyli krotszego
          else if(parent->balance == 1) 
            rotate_RL(rtt);
          else                
            rotate_RR(rtt);
        }
      }
    }
	  
	  return newElement->getValueType().second;
    /*(void)key;
    throw std::runtime_error("TODO");*/
	  
	  
  }
  
    Node* findElement(const key_type& key) const
    {
	   Node *tmp = root;
       while (tmp != nullptr) 
       {
		  if (tmp->getValueType().first < key) 
		  {
		  	  tmp = tmp->right;
          }
          else if (tmp->getValueType().first > key) 
          {
		  	  tmp = tmp->left;
          } 
          else 
          {
			  return tmp;
          }
       }
     return nullptr;
    }

  const mapped_type& valueOf(const key_type& key) const
  {
	 Node *tmp = findElement(key);
     if(tmp != nullptr)
		return tmp->getValueType().second;
	 else throw std::out_of_range("");  
	  
    /*(void)key;
    throw std::runtime_error("TODO");*/
  }

  mapped_type& valueOf(const key_type& key)
  {
     Node *tmp = findElement(key);
     if(tmp != nullptr)
		return tmp->getValueType().second;
	 else throw std::out_of_range("");  
    /*(void)key;
    throw std::runtime_error("TODO");*/
  }

  const_iterator find(const key_type& key) const
  {
	 Node *tmp = findElement(key);
	  if(tmp != nullptr)
		return const_iterator(tmp, this);
	  else return cend(); 
	  
    /*(void)key;
    throw std::runtime_error("TODO");*/
  }

  iterator find(const key_type& key)
  {
	 Node *tmp = findElement(key);
	  if(tmp != nullptr)
		return iterator(const_iterator(tmp, this));
	  else return end();
	  
    /*(void)key;
    throw std::runtime_error("TODO");*/
  }
  
  Node* removeElement(Node *toDelete)
  {
    Node  *tmp,*y,*z;
    bool nest;

    if(toDelete->left != nullptr && toDelete->right != nullptr)  //jesli wezel ma dwoch synow to usuwamy rekurencyjnie poprzednika i zamieniamy wartości
    {
      y = removeElement(previousNode(toDelete));                 //poprzednik zawsze jest lisciem albo ma jedno dziecko
      nest = false;   //ustawienie zmiennej warunkowej
    }
    else //jezeli ma jednego syna lub wgl
    {
      --size;
      if(toDelete->left != nullptr)     //jesli ma lewego syna to przypisujemy go do y
      {
        y = toDelete->left; 
        toDelete->left = nullptr;
      }
      else                              //jesli ma prawego to przypisujemy do y a jak nie ma wgl to ustawiamy na nullptr
      {
        y = toDelete->right; 
        toDelete->right = nullptr;
      }
      toDelete->balance = 0;
      nest  = true;
    }

    if(y != nullptr)       //zamiana węzłów jesli usuwany ma jakies dziecko
    {
      y->parent = toDelete->parent;
      y->left = toDelete->left;  

      if(y->left != nullptr) 
        y->left->parent = y;
      y->right = toDelete->right; 

      if(y->right != nullptr) 
        y->right->parent = y;

      y->balance = toDelete->balance;
    }

    if(toDelete->parent != nullptr) //jezeli parent nie jest nullem
    {
      if(toDelete->parent->left == toDelete) //ustaw rodzica usuwanego na y
        toDelete->parent->left = y; 
      else toDelete->parent->right = y;
    }
    else root = y; //jezeli parent usuwanego jest nullem to ustaw y na root

    if(nest == true)
    {
      z = y;
      y = toDelete->parent;
      while(y != nullptr)
      {
        if(!y->balance)
        {              // Przypadek nr 1 zrownowazony wezel, moze zmienic sie na 1 lub -1
          if(y->left == z)  y->balance = -1; 
          else y->balance = 1;
          break;
        }
        else
        {
          if(((y->balance == 1) && (y->left == z)) || ((y->balance == -1) && (y->right == z)))
          {           // Przypadek nr 2 usuwamy z dluzeszego wezla, balance ustawiamy na 0
            y->balance = 0;
            z = y; 
            y = y->parent;
          }
          else  // usuwamy z krótszego poddrzewa, konieczna rotacja
          {
            if(y->left == z)     //sprawdzamy czy z nalezy do lewego czy prawego poddrzewa
              tmp = y->right; 
            else 
              tmp = y->left;

            if(!tmp->balance)
            {         // Przypadek 3.1 dziecka węzła y w cięższym poddrzewie jest równy 0, wykonujemy odpowiednią rotację pojedynczą: RR
              if(y->balance == 1) rotate_LL(y); 
              else rotate_RR(y);
              break;
            }
            else if(y->balance == tmp->balance)
            {          // Przypadek 3.2 y.balance jest taki sam jak współczynnik tmp.balance, ykonujemy odpowiednią rotację pojedynczą:  RR (y.balance = -1) lub LL (y.balance = 1).
              if(y->balance == 1) rotate_LL(y); 
              else rotate_RR(y);
              z = tmp; 
              y = tmp->parent;
            }
            else
            {         // Przypadek 3.3 Współczynniki y.balance i tmp.balance są przeciwne. Wykonujemy rotację podwójną: RL (y.balance = -1) lub LR (y.balance = 1). 
              if(y->balance == 1) rotate_LR(y); 
              else rotate_RL(y);
              z = y->parent; 
              y = z->parent;
            }
          }
        }
      }
    }
    return toDelete;
  }

  void remove(const key_type& key)
  {
	  Node *toDelete, *tmp = findElement(key);
	  if(root == nullptr || tmp == nullptr)
		  throw std::out_of_range("");
	  toDelete = removeElement(tmp);
    delete toDelete;
	  
	  
    /*(void)key;
    throw std::runtime_error("TODO");*/
  }

  void remove(const const_iterator& it)
  {
	  Node *toDelete, *tmp = it.ptr;
	  if(root == nullptr || tmp == nullptr)
		  throw std::out_of_range("");
	  toDelete = removeElement(tmp);
    delete toDelete;


    /*(void)it;
    throw std::runtime_error("TODO");*/
  }

  size_type getSize() const
  {
	  return (size_type)size;
    //throw std::runtime_error("TODO");
  }

  bool operator==(const TreeMap& other) const
  {
	  if(this->size != other.size)
		return false;
	  auto it = cbegin();
	  auto itOther = other.cbegin();
	  while (it != this->cend() && itOther != other.cend())
	  {
		if ((*it).first != ((*itOther).first) || ((*it).second != (*itOther).second))
		{
            return false;
        }
                ++it;
                ++itOther;
      }
      return true;
    /*(void)other;
    throw std::runtime_error("TODO");*/
  }

  bool operator!=(const TreeMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
	return iterator(cbegin());
	  
   // throw std::runtime_error("TODO");
  }

  iterator end()
  {
	 return iterator(cend());
    //throw std::runtime_error("TODO");
  }

  const_iterator cbegin() const
  {
	  if(size == 0)
		return cend();
		
	  Node *tmp = root;
	  while(tmp->left != nullptr)
	  {
		  tmp = tmp->left;
	  }
	  return const_iterator(tmp, this);
    //throw std::runtime_error("TODO");
  }

  const_iterator cend() const
  {
	 return const_iterator(nullptr, this);
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
private:
	Node* root;
	int size;
	
	void rotate_RR(Node *A)
  {
    Node *B = A->right, *p = A->parent;

    A->right = B->left;
    if(A->right) 
      A->right->parent = A;

    B->left = A;
    B->parent = p;
    A->parent = B;

    if(p)
    {
      if(p->left == A) p->left = B; 
      else p->right = B;
    }
    else root = B;

    if(B->balance == -1)
      A->balance = B->balance = 0;
    else
    {
      A->balance = -1; 
      B->balance = 1;
    }
  }
	void rotate_LL(Node *A)
	{
    Node *B = A->left, *p = A->parent;

    A->left = B->right;
    if(A->left) 
      A->left->parent = A;

    B->right = A;
    B->parent = p;
    A->parent = B;

    if(p)
    {
      if(p->left == A) p->left = B; 
      else p->right = B;
    }
    else root = B;

    if(B->balance == 1) 
      A->balance = B->balance = 0;
    else
    {
      A->balance = 1; B->balance = -1;
    }
	}
	void rotate_RL(Node *A)
	{
    Node * B = A->right, * C = B->left, * p  = A->parent;

    B->left = C->right;
    if(B->left) 
      B->left->parent = B;

    A->right = C->left;
    if(A->right) 
      A->right->parent = A;

    C->left = A;
    C->right = B;
    A->parent = B->parent = C;
    C->parent = p;

    if(p)
    {
      if(p->left == A) p->left = C; 
      else p->right = C;
    }
    else root = C;

    if(C->balance == -1) A->balance =  1; 
    else A->balance = 0;

    if(C->balance ==  1) B->balance = -1; 
    else B->balance = 0;

    C->balance = 0;
	}
	void rotate_LR(Node *A)
	{
    Node * B = A->left, * C = B->right, * p = A->parent;

    B->right = C->left;
    if(B->right) 
      B->right->parent = B;

    A->left = C->right;
    if(A->left) 
      A->left->parent = A;

    C->right = A;
    C->left = B;
    A->parent = B->parent = C;
    C->parent = p;

    if(p)
    {
      if(p->left == A) p->left = C; 
      else p->right = C;
    }
    else root = C;

    if(C->balance ==  1) A->balance = -1; 
    else A->balance = 0;

    if(C->balance == -1) B->balance =  1; 
    else B->balance = 0;

    C->balance = 0;
	}
	Node* previousNode(Node *tmp)
	{
    Node * r;

    if(tmp)
    {
      if(tmp->left)
      {
        tmp = tmp->left;
        while(tmp->right) tmp = tmp->right;
      }
      else
        do
        {
          r = tmp;
          tmp = tmp->parent;
        } while(tmp && tmp->right != r);
    }
  return tmp;	
	}
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename TreeMap::value_type;
  using pointer = const typename TreeMap::value_type*;
  
  TreeMap::Node *ptr;
  const TreeMap *base;
  

  explicit ConstIterator()
  {
	  ptr = nullptr;
	  base = nullptr;
  }
  
  ConstIterator(Node *node, const TreeMap *map)
  {
	  ptr = node;
	  base = map;
  }

  ConstIterator(const ConstIterator& other)
  {
	  ptr = other.ptr;
	  base = other.base;
    /*(void)other;
    throw std::runtime_error("TODO");*/
  }
 

  ConstIterator& operator++()
  {
	  if(ptr == nullptr)
		  throw std::out_of_range("");
	  if(ptr->right != nullptr)
	  {
		  ptr = ptr->right;
		  while (ptr->left != nullptr)
			  ptr = ptr->left;
	  }
	  else
	  {
		  while(ptr->parent != nullptr && ptr->parent->right == ptr)
			  ptr = ptr->parent;
		  
		  if(ptr->parent != nullptr)
			  ptr = ptr->parent;
		  else ptr = nullptr;
	  }
	  
	  return *this;
    //throw std::runtime_error("TODO");
  }

  ConstIterator operator++(int)
  {
	  ConstIterator it(this->ptr, this->base);
	  this->operator++();
	  return it;
	  
    //throw std::runtime_error("TODO");
  }

  ConstIterator& operator--()
  {
	  if(*this == base->cbegin() || base->size == 0) 
		throw std::out_of_range("");
	  
	  if(ptr == nullptr)
	  {
		  Node* tmp = base->root;
		  while(tmp->right != nullptr)
			tmp = tmp->right;
		  this->ptr = tmp;
		  return *this;
	  }
	  
	  if(ptr->left != nullptr)
	  {
		  ptr = ptr->left;
		  while (ptr->left != nullptr)
			ptr = ptr->left;
	  }
	  else
	  {
		  while(ptr->parent != nullptr && ptr->parent->left == ptr)
			ptr = ptr->parent;
		  
		  if(ptr->parent != nullptr)
			ptr = ptr->parent;
		  else ptr = nullptr;
	  }
	  
	  return *this;
	  
	  
    //throw std::runtime_error("TODO");
  }

  ConstIterator operator--(int)
  {
	  ConstIterator it(this->ptr, this->base);
	  this->operator--();
	  return it;
    //throw std::runtime_error("TODO");
  }

  reference operator*() const
  {
	  if(ptr == nullptr)
		throw std::out_of_range("");
      return ptr->getValueType();
    /*throw std::runtime_error("TODO");*/
  }

  pointer operator->() const
  {
	  return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
	  if( base->root != other.base->root )
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
class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::reference;
  using pointer = typename TreeMap::value_type*;

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
class TreeMap<KeyType, ValueType>::Node
{	
public:
	Node *parent;
	Node *right, *left;
	value_type *value;
	int balance;
	
	Node()
	{
		this->parent = nullptr;
		this->left = nullptr;
		this->right = nullptr;
		this->value = nullptr;
		balance = 0;
	}
	
	~Node()
	{
		if(value != nullptr) delete value;
	}
		

	Node(Node* parent,const key_type first, mapped_type second)
	{
		this->parent = parent;
		this->left = nullptr;
		this->right = nullptr;
		value = new value_type(first, second);
		balance = 0;
	}
	
	bool hasChildren()
	{ 
		return (right != nullptr) || (left != nullptr);
	}
	
	value_type &getValueType() { return *value; }
	
};


}

#endif /* AISDI_MAPS_MAP_H */
