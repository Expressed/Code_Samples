#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <initializer_list>
#include <utility>
#include <cstring>
#include <stdexcept>
#include <type_traits>
/*
Templated Map
by Matthew Matyus
Red Black Tree code modelled after pseudo code from CLRS Algorithms book.
Guarenteed O(1) increment and decrement of Iterators and O(lgn) search.
*/



namespace cs540{
	template<typename T1,typename T2>
	class Map{
		public:

    typedef struct link{
			link():parent(NULL),left(NULL),right(NULL),color(0), succ(NULL), pred(NULL){};
      link(link* p,  link*l,  link* r, int c):parent(p),left(l),right(r), color(c),succ(NULL),pred(NULL){};
      link *parent;
      link *left;
      link *right;
      int color;
			link* succ;
			link* pred;
    }link;
    
    typedef struct rbNode: public link{
      rbNode(std::pair<const T1, T2> pr, link* p, link*l, link* r, int c):link(p,l,r,c),kv(pr){};
      std::pair<const T1,T2> kv;
    }rbNode;
    
    typedef struct rbTree{
      link *root;
    }rbTree;
    
//Iterator Begin
		class Iterator{
			public:
				
				Iterator(){current_ = NULL;end_ = NULL;endb =false;};
				Iterator(const Iterator& i):current_(i.current_), end_(i.end_), endb(i.endb){};
				Iterator(link* y): current_(y){};
				Iterator( link*y, link* z, bool end): current_(y), end_(z), endb(end){};
					link* current_;
					link* end_;	
					bool		endb;
	        std::pair<const T1, T2>& operator*() const{
					return static_cast<rbNode *>(current_)->kv;}
				
				Iterator& operator++(){
					current_ = current_->succ;
				  return *this;
				}
				Iterator operator++(int){
					Iterator temp = *this;
					++(*this);
				  return temp;
        }
				Iterator& operator--(){
					if(endb){current_ = end_;endb=false; return *this;}
					current_ = current_->pred;
				return *this;
				}
				Iterator operator--(int){
					Iterator temp= *this;
					--(*this);
					return temp;
				}
	 			Iterator& operator=(const Iterator& other){
					if(this == &other){return *this;}
					endb = other.endb;
					current_ = other.current_;
					end_ = other.end_;
					return *this;
				}
				Iterator& operator=(Iterator&& other){
					if(this == &other){return *this;}
					endb = other.endb;
					current_ = other.current_;
					end_ = other.end_;
					return *this;
				}
				Iterator(Iterator&& other){
					endb = other.endb;
					current_ = other.current_;
					end_ = other.end_;
					other.current_ = NULL;
					other.end_ = NULL;
				}
				
			};
//Const_It_Begin
    class ConstIterator{
			public:
				ConstIterator(){};
				ConstIterator(const ConstIterator& i):current_(i.current_), end_(i.end_), endb(i.endb){};
				ConstIterator(const Iterator& i):current_(i.current_),end_(i.end_),endb(i.endb){ };
				ConstIterator( link* y): current_(y){};
				ConstIterator(link *y, link* z, bool end): current_(y), end_(z), endb(end){};
			 	 link* current_;
				 link* end_;	
				bool endb;
        std::pair<const T1, T2>& operator*() const{
					return ((rbNode*)(current_))->kv;

				};
				
        ConstIterator& operator++(){
					current_ = current_->succ;
          return *this;
				}
				
        ConstIterator operator++(int){
          ConstIterator temp = *this;
					++(*this);
					return temp;}
				
        ConstIterator& operator--(){
					if(endb){current_ = end_;endb=false; return *this;}
					current_ = current_->pred;
          return *this;
				}
				ConstIterator operator--(int){
					ConstIterator temp= *this;
					--(*this);
					return temp;
				}
			  ConstIterator& operator=(const ConstIterator& other){
					if(this == &other){return *this;}
					endb = other.endb;
					current_ = other.current_;
					end_ = other.end_;
					return *this;
				}	
				ConstIterator(ConstIterator&& other){
					endb = other.endb;
					current_ = other.current_;
					end_ = other.end_;
					other.current_ = NULL;
					other.end_ = NULL;
				}
};
//Const_It_End
    
 //Reverse IT Begin
    class ReverseIterator{
    public:
      
      ReverseIterator(){};
      ReverseIterator( link* y): current_(y){};
			ReverseIterator( link*y, link* z, bool end): current_(y), end_(z), endb(end){};
				link* current_;
				link* end_;
				bool		endb;
      std::pair<const T1, T2>& operator*() const{
        return static_cast<rbNode *>(current_)->kv;
      };
      ReverseIterator& operator++(){
        if(endb){current_ = end_;endb=false; return *this;}
        current_ = current_->pred;
				return *this;
      }
      ReverseIterator operator++(int){
        ReverseIterator temp = *this;
        ++(*this);
				return temp;}
      ReverseIterator& operator--(){
        current_ = current_->succ;
				return *this;
      }
      ReverseIterator operator--(int){
        ReverseIterator temp= *this;
        --(*this);
        return temp;
      }
		  
   
    };

//Reverse IT End
    
  //Essentials
  	Map();
		~Map();
		Map(const Map&);
		Map& operator=(const Map&);
		Map& operator=(const Map&&);
		Map(std::initializer_list<std::pair<const T1, T2>>);
		Map(Map&&);	
		void printTree();
		bool empty() const;
		T2& at(const T1&);
		const T2& at(const T1&) const;	
		link *getMin(link*);

		link *getMin(link*) const;
		link *getMax(link*);
		link *getMax(link*) const;
		void deleteNode(link*);
    void rbDelete(link*);
    
  //Iterators
		Iterator find(const T1&);
		Iterator begin();
		Iterator end();
		Iterator insert(const std::pair<const T1, T2>&);	
		Iterator insert(std::pair<const T1, T2>&&);
		ConstIterator begin() const;
		ConstIterator end() const;
		ConstIterator find(const T1&) const;
    ReverseIterator rbegin();
    ReverseIterator rend();
    
  //Utilities
		bool operator==(const Map&) const;
		bool operator!=(const Map&) const;
		typename Map<T1,T2>::Iterator insertP( std::pair<const T1,T2>);		
		void erase(Iterator);
		void clear();
		void remove(const T1&);
	 	T2& operator[](const T1&);
    
  //Members
    link nil = {&nil, &nil, &nil, 0};
    std::size_t size() const;
    rbTree tree;

    //iterator equality operators
    friend bool operator==(const Iterator &it1, const Iterator &it2){
      return (it1.current_ == it2.current_);
    }
    friend bool operator!=(const Iterator &it1, const Iterator &it2){
      return !(it1.current_ == it2.current_);
    }					


    friend bool operator==(const ConstIterator &it1, const ConstIterator &it2){
			return it1.current_ == it2.current_;    
		}		

      
    friend bool operator!=(const ConstIterator &it1, const ConstIterator &it2){
   		return !(it1.current_ == it2.current_);
		 }		

        
    friend bool operator==(const Iterator &it1, const ConstIterator &it2){
			return it1.current_ == it2.current_;
		}	
      
    friend bool operator!=(const Iterator &it1, const ConstIterator &it2){
			return !(it1.current_ == it2.current_);
		}
          
    friend bool operator==(const ConstIterator &it1, const Iterator &it2){
   		return it1.current_ == it2.current_;
		 }		

      
    friend bool operator!=(const ConstIterator &it1, const Iterator &it2){
   		return !(it1.current_ == it2.current_);
		 }
    
    friend bool operator==(const ReverseIterator &it1, const ReverseIterator &it2){
			return it1.current_ == it2.current_;
    }

    
    friend bool operator!=(const ReverseIterator &it1, const ReverseIterator &it2){
      return !(it1.current_  == it2.current_);
    }

   private:
    link* search(link*,const T1& );
    void clear_Aux(link*);
    link* getPred(link*);
    link* getSucc(link*);
    const link* getSucc(const link*) const;
    void printTreeAux(link*);
    void rightRotate( link*);
    void leftRotate(link*);
    void insertFixup(link*);
    void rbDeleteFixup(link* x);
    void rbTransplant(link* u,link* v);
    void copyAux(const link*, const link*);
    void copyAuxM(const link*, const link*); 
    std::size_t numNodes;
  };
	template<typename T1, typename T2>
  Map<T1,T2>::Map(){
		tree =  rbTree();	
		tree.root = &nil;
		numNodes = 0;
	
  }

	template<typename T1, typename T2>
	Map<T1,T2>::~Map(){
			clear();	
	}
  
	template<typename T1, typename T2>
	Map<T1, T2>::Map(std::initializer_list<std::pair<const T1, T2>> myList){
		
		numNodes = 0;
		tree =  rbTree();
		tree.root = &nil;

		for(std::pair<const T1,T2>p: myList){insertP(p);}
  }

	template<typename T1, typename T2>
	Map<T1, T2>::Map(const Map& other){
		tree = rbTree();
		nil = other.nil;
		tree.root = &nil;
		nil.parent = &nil;
		nil.left = &nil;
		nil.right = &nil;
		numNodes = 0;
		copyAux(other.tree.root, &other.nil);
	}
			template<typename T1, typename T2>
			void
			Map<T1,T2>::copyAux(const link* n, const link* null){
				if(n != null){
						copyAux(n->left, null);
						insertP( ((rbNode*)(n))->kv );						
						copyAux(n->right, null);
				}	
			}

  
  //Move Functions R-Value References
	template<typename T1, typename T2>
	typename Map<T1, T2>::Map&
	Map<T1,T2>::operator=(const Map&& other){
		if(*this != other){
			numNodes = 0;
			nil = other.nil;
			tree = rbTree();
			tree.root = &nil;
			nil.parent = &nil;
			nil.left = &nil;
			nil.right = &nil;
			copyAuxM(other.tree.root, &other.nil);
		}
    return *this;
	}

  
  template<typename T1, typename T2>
	Map<T1, T2>::Map(Map&& other){
		tree = rbTree();	
		tree.root = &nil;
		nil.parent = &nil;
		nil.left = &nil;
		nil.right = &nil;
		numNodes = 0;
		copyAuxM(other.tree.root, &other.nil);
		other.tree.root = &other.nil;
		
	}
  template<typename T1, typename T2>
  void
  Map<T1,T2>::copyAuxM(const link* n, const link* null){
    if(n != null){
        copyAuxM(n->left, null);
        insert(std::move( ((rbNode*)(n))->kv   ) );										
        copyAuxM(n->right, null);
        delete static_cast<rbNode*>(const_cast<link*>(n));	
        
    }
   }

  template<typename T1, typename T2>
  typename Map<T1,T2>::Iterator
  Map<T1,T2>::insert(std::pair<const T1, T2>&& p){
    rbNode z= rbNode(std::move(p),NULL,NULL,NULL,1);
    link* zz= new rbNode(z);
		link* x = tree.root;
    link* y = &nil;
    while(x!=&nil){
      y = x;
      if(static_cast<rbNode *>(zz)->kv.first == static_cast<rbNode *>(x)->kv.first){
					delete static_cast<rbNode*>(zz);
					return Iterator(x);}
      if( static_cast<rbNode *>(zz)->kv.first < static_cast<rbNode *>(x)->kv.first ){
        x = x->left;
      }else{x = x->right;}
    }
    zz->parent = y;
    if(y == &nil){
      tree.root = zz;
    }else if(static_cast<rbNode *>(zz)->kv.first < static_cast<rbNode *>(y)->kv.first){
      y->left = zz;
    }else{
      y->right = zz;
    }
    zz->left = &nil;
    zz->right = &nil;
    zz->color = 1;//1 is red
    numNodes++;
    insertFixup(zz);
		zz->succ = getSucc(zz);
		zz->pred = getPred(zz);
		link* zSucc = zz->succ;
		link* zPred = zz->pred;
		zPred->succ = getSucc(zPred);
		zSucc->pred = getPred(zSucc);
    return Iterator(zz);
  }
//Move -End


  
	template<typename T1, typename T2>
	bool	
	Map<T1,T2>::operator==(const Map& other) const{
		if(other.numNodes != numNodes){return false;}
		ConstIterator thisIterator = this->begin();
		ConstIterator thisEndIterator = this->end();
		ConstIterator otherIterator = other.begin();
		while(thisIterator != thisEndIterator){
				if( !((*thisIterator).first == (*otherIterator).first) 
						||
						!((*thisIterator).second == (*otherIterator).second)	
				){return false;}
				thisIterator++;
				otherIterator++;
		}		
		return true;
	}

	template<typename T1, typename T2>
	bool	
	Map<T1,T2>::operator!=(const Map& other) const{
		return !(*this == other);
	}


	template<typename T1, typename T2>
	typename Map<T1, T2>::Map&
	Map<T1,T2>::operator=(const Map& other){
		if(this != &other){
			numNodes = 0;
			tree = other.tree;
			nil = other.nil;
			tree.root = &nil;
			copyAux(other.tree.root, &other.nil);
		}
		return *this;
		
	}

	template<typename T1, typename T2>
	typename Map<T1,T2>::Iterator
	Map<T1,T2>::insertP(std::pair<const T1, T2> p){
		rbNode z= {p,NULL,NULL,NULL,1};
		link* zz= new rbNode(z);
    link* x = tree.root;
    link* y = &nil;
    while(x!=&nil){
      y = x;
      if(static_cast<rbNode *>(zz)->kv.first == static_cast<rbNode *>(x)->kv.first){
        delete static_cast<rbNode*>(zz);
        return Iterator(x);}
      if(static_cast<rbNode *>(zz)->kv.first < static_cast<rbNode *>(x)->kv.first ){
        x = x->left;
      }else{x = x->right;}	
    }
    zz->parent = y;
    if(y == &nil){
      tree.root = zz;   
    }else if(static_cast<rbNode *>(zz)->kv.first < static_cast<rbNode *>(y)->kv.first){
      y->left = zz;
    }else{
      y->right = zz;
    }
    zz->left = &nil;
    zz->right = &nil;
    zz->color = 1;//1 is red
    numNodes++;
    insertFixup(zz);
		zz->succ = getSucc(zz);
		zz->pred = getPred(zz);
		link* zSucc = zz->succ;
		link* zPred = zz->pred;
		zPred->succ = getSucc(zPred);
		zSucc->pred = getPred(zSucc);
    return Iterator(zz);
 }

	template<typename T1, typename T2>
	void Map<T1, T2>::insertFixup(link* z){
    link* y;
		while(z->parent->color == 1){
			if( z->parent == z->parent->parent->left ){
				y = z->parent->parent->right;
				if(y->color == 1){
					z->parent->color = 0;
					y->color = 0;
					z->parent->parent->color = 1;
					z = z->parent->parent;
				}else{
						if(z == z->parent->right){
							z = z->parent;
							leftRotate(z);
						
						}
						z->parent->color = 0;
						z->parent->parent->color = 1;
					rightRotate(z->parent->parent);
				
				}
			}else{	
				y = z->parent->parent->left;
				if(y->color == 1){
					z->parent->color = 0;
					y->color = 0;
					z->parent->parent->color = 1;
					z = z->parent->parent;
				}else{
						if(z == z->parent->left){
							z = z->parent;
							rightRotate(z);
			
						}
						z->parent->color = 0;
						z->parent->parent->color = 1;
					leftRotate(z->parent->parent);	
		
				}
			}
		}
		tree.root->color = 0;//0 is black
	}
	template<typename T1, typename T2>
	void Map<T1,T2>::leftRotate(link *z){
	link* y = z->right;
		z->right = y->left;
		if(y->left != &nil){
			y->left->parent = z;
		}	
		y->parent = z->parent;
		if(z->parent == &nil){
			tree.root = y;
		}else if( z==z->parent->left){
			z->parent->left = y;
		}
		else{z->parent->right = y;}
		y->left = z;
		z->parent = y;		
	}
	template<typename T1, typename T2>
	void Map<T1,T2>::rightRotate(link *z){
	link* y = z->left;
		z->left = y->right;
		if(y->right != &nil){
			y->right->parent = z;
		}	
		y->parent = z->parent;
		if(z->parent == &nil ){
			tree.root = y;
		}else if( z==z->parent->right){
			z->parent->right = y;	
		}
		else{z->parent->left = y;}
		y->right = z;
		z->parent = y;
	}

template<typename T1, typename T2>
auto Map<T1,T2>::insert(const std::pair<const T1, T2> &pr)-> Iterator{
	return insertP(pr);
}

  template<typename T1, typename T2>
	void Map<T1,T2>::printTree(){
		printTreeAux(tree.root);

	}
	template<typename T1, typename T2>
	 void Map<T1,T2>::printTreeAux(link* N){
		if(N == &nil){return;}
			printTreeAux(N->left);				
			std::cout <<  N->color << std::endl;		
			printTreeAux(N->right);
	}	


template<typename T1, typename T2>
bool Map<T1,T2>::empty() const{
	return tree.root == &nil;
}

template<typename T1, typename T2>
typename Map<T1,T2>::link* Map<T1,T2>::search(link* node,const T1& key) {	
	link* current = node;	
	while(current!=&nil){
     if(key == static_cast<rbNode *>(current)->kv.first){
						return current;
			}
			if(key < static_cast<rbNode *>(current)->kv.first){
				current = current->left;
			}
			else{
				current = current->right;
			}	
	}		
	return current;
}

template<typename T1, typename T2>
typename Map<T1,T2>::link*
Map<T1,T2>::getMin(link* n){
	
	link *current = n;//= tree.root;
	if(current != &nil){
		while(current->left != &nil){
			current = current->left;
	}	
		return current;
	}
	return &nil;
}
//const_min

template<typename T1, typename T2>
 typename Map<T1,T2>::link*
Map<T1,T2>::getMin(link* n) const{
	
	link *current = n;//= tree.root;
	if(current != &nil){
		while(current->left != &nil){
			current = current->left;
	}	
		return current;
	}
	current = const_cast<link*>(&nil);
	return current;
}


template<typename T1, typename T2>
typename Map<T1,T2>::link*
Map<T1,T2>::getMax(link* n) {	
		link *current = n;//tree.root;
	if(current != &nil){
		while(current->right != &nil){
			current = current->right;
		}	
		return current;
	}
 current = &nil;
	return current;
}
//const_max

template<typename T1, typename T2>
 typename Map<T1,T2>::link*
Map<T1,T2>::getMax(link* n)const {	
		link *current = n;//tree.root;
	if(current != &nil){
		while(current->right != &nil){
			current = current->right;
		}	
		return current;
	}
current = const_cast<link*>(&nil);
	return current;
}


template<typename T1, typename T2>
typename Map<T1, T2>::link*
Map<T1,T2>::getPred(link* x) {	
	if(x==&nil){return x;}
	
if(x->left != &nil){
		return getMax(x->left);
	}
	link* y = x->parent;
	while( y != &nil && x== y->left){
		x = y;
		y =	y->parent; 
	}
	return y;
}

template<typename T1, typename T2>
typename Map<T1, T2>::link*
Map<T1,T2>::getSucc(link* x) {	
if(x==&nil){return x;}	
if(x->right != &nil){
		return getMin(x->right);
	}
	link* y = x->parent;
	while( y != &nil && x== y->right){
		x = y;
		y =	y->parent; 
	}
	return y;
}

//const_succ

template<typename T1, typename T2>
const typename Map<T1, T2>::link*
Map<T1,T2>::getSucc(const link* x) const {	
if(x==&nil){return x;}	
if(x->right != &nil){
		return getMin(x->right);
	}
	link* y = x->parent;
	while( y != &nil && x== y->right){
		x = y;
		y =	y->parent; 
	}
	return y;
}



template<typename T1, typename T2>
void
Map<T1,T2>::rbTransplant(link* u,link* v) {	
	if(u->parent == &nil){
		tree.root = v;
	}else if(u == u->parent->left){
		u->parent->left = v;
	}else{
		u->parent->right = v;
	}
	v->parent = u->parent;
}	

template<typename T1, typename T2>
void
Map<T1,T2>::rbDelete(link* z) {	
	link* zPred = z->pred;
	link* zSucc = z->succ;
	link *y = z;
	int colorTemp = y->color;
	link *x;
	if(z->left == &nil){
		x = z->right;
		rbTransplant(z, z->right);
	}else if (z->right == &nil){
		x = z->left;
		rbTransplant(z, z->left);
	}else{
		y = getMin(z->right);
		colorTemp = y->color;
		x = y->right;
		if(y->parent == z){
			x->parent = y;
		}else{
			rbTransplant(y,y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		rbTransplant(z, y);
		y->left =z->left;
		y->left->parent = y;
		y->color = z->color;
	
	}if(colorTemp == 0){
		rbDeleteFixup(x);
	}
	delete static_cast<rbNode*>(z);
	zPred->succ = getSucc(zPred);
	zSucc->pred = getPred(zSucc);
	numNodes--;
}


template<typename T1, typename T2>
void
Map<T1,T2>::rbDeleteFixup(link* x) {	
	link* y;
	while( (x!=tree.root) && (x->color == 0) ){
		if(x==x->parent->left){
			y = x->parent->right;
			if(y->color == 1){
				y->color = 0;
				x->parent->color = 1;
				leftRotate(x->parent);
				y = x->parent->right;
			}if( (y->left->color==0) && (y->right->color == 0)){
				y->color = 1;
				x = x->parent;
			}else {if(y->right->color == 0){
				y->left->color = 0;
				y->color = 1;
				rightRotate(y);
				y = x->parent->right;
			}
			y->color = x->parent->color;
			x->parent->color = 0;
			y->right->color = 0;
			leftRotate(x->parent);
			x = tree.root;}
		}else{
			y = x->parent->left;
			if(y->color == 1){
				y->color = 0;
				x->parent->color = 1;
				rightRotate(x->parent);
				y = x->parent->left;
			}if((y->right->color==0) && (y->left->color == 0) ){
				y->color = 1;
				x = x->parent;
			}else{ if(y->left->color == 0){
				y->right->color = 0;
				y->color = 1;
				leftRotate(y);
				y = x->parent->left;
			}
			y->color = x->parent->color;
			x->parent->color = 0;
			y->left->color = 0;
			rightRotate(x->parent);
			x = tree.root;
		  }
		}
  }
	x->color = 0;
}

template<typename T1, typename T2>
typename Map<T1,T2>::ConstIterator
Map<T1,T2>::find(const T1& key)const{		
//
bool found = false;
	link* current = tree.root;
	while(current!=&nil && !found){
     if(key == static_cast<rbNode *>(current)->kv.first){
						found = true;
			}
			if(key < static_cast<rbNode *>(current)->kv.first){
				current = current->left;
			}
			else{
				current = current->right;
			}	
	}		

typename Map<T1,T2>::link* rb = current;

//
	if(rb!=&nil){
		return ConstIterator(rb);
	}
  link* end = const_cast<Map*>(this)->getMax(tree.root);
  current = const_cast<Map*>(this)->getSucc(const_cast<link*>(end));
  ConstIterator  itr =  ConstIterator( current);
  itr.end_ = end;
  itr.endb = true;
  itr.current_ = current;

 return itr; 

}

template<typename T1, typename T2>
typename Map<T1,T2>::Iterator
Map<T1,T2>::find(const T1& key){
	typename Map<T1,T2>::link* rb = search(tree.root,key);
	if(rb!=&nil){
		return Iterator(rb);
	}
  link* end =getMax(tree.root);
  link* current = getSucc(end);
  Iterator  itr =  Iterator( current);
  itr.end_ = end;
  itr.endb = true;
  itr.current_ = current;
  return itr;
}
//ITerator
  template<typename T1, typename T2>
  typename Map<T1, T2>::Iterator
  Map<T1, T2>::begin(){
  return Iterator(getMin(tree.root), NULL, false);}

  template<typename T1, typename T2>
  typename Map<T1, T2>::Iterator
  Map<T1, T2>::end(){
	link* end = getMax(tree.root);
 	link* current  = getSucc(end);
  return Iterator(current, end, true);}
//
//Const
  template<typename T1, typename T2>
  typename Map<T1, T2>::ConstIterator
  Map<T1, T2>::begin() const{
   link* current = const_cast<Map*>(this)->getMin(tree.root);
	return ConstIterator(current, NULL, false);} 

  template<typename T1, typename T2>
  typename Map<T1, T2>::ConstIterator
  Map<T1, T2>::end() const{


 link* end = const_cast<Map*>(this)->getMax(tree.root);
link* current= const_cast<Map*>(this)->getSucc(end);
return  ConstIterator(current, end, true);}
//

//Reverse
  template<typename T1, typename T2>
  typename Map<T1, T2>::ReverseIterator
  Map<T1, T2>::rbegin(){
    link* current = getMax(tree.root);    
    return ReverseIterator(current, NULL, false);}
  
  template<typename T1, typename T2>
  typename Map<T1, T2>::ReverseIterator
  Map<T1, T2>::rend(){
    link* end = getMin(tree.root);
    link* current = getPred(end);
    return ReverseIterator(current,end,true);}
//


template<typename T1, typename T2>
std::size_t 
Map<T1, T2>::size() const{
	return numNodes;
}

template<typename T1, typename T2>
void
Map<T1,T2>::erase(typename Map<T1,T2>::Iterator it){
	rbDelete(it.current_);
}

template<typename T1, typename T2>
void Map<T1,T2>::clear(){
	clear_Aux(tree.root);
		tree.root = &nil;
		numNodes = 0;
}
	template<typename T1,typename T2>
	void Map<T1,T2>::clear_Aux(link* n){
			
				if(n!=&nil){	
				clear_Aux(n->left);
				clear_Aux(n->right);
				delete static_cast<rbNode*>(n);
			}
	}

template<typename T1, typename T2>
T2& 
Map<T1,T2>::at(const T1& key){
	 
  typename Map<T1,T2>::link* current = tree.root;

	while(current!=&nil){
    
			if(key == static_cast<rbNode *>(current)->kv.first){
						return static_cast<rbNode *>(current)->kv.second;
			}
			if(key < static_cast<rbNode *>(current)->kv.first){
				current = current->left;
			}
			else{
				current = current->right;
			}	
	}		
	throw std::out_of_range("Key Not Member of Map");

}

template<typename T1, typename T2>
const T2& 
Map<T1,T2>::at(const T1& key) const{
	 
	
	link* current = tree.root;
	bool found = false;	
	while(current!=&nil && !found){
     if(key == static_cast<rbNode *>(current)->kv.first){
						found = true;
			}
			if(key < static_cast<rbNode *>(current)->kv.first){
				current = current->left;
			}
			else{
				current = current->right;
			}	
	}
	typename Map<T1,T2>::link* rb = static_cast<rbNode*>(current);
if(rb!=&nil){
		return static_cast<rbNode *>(rb)->kv.second;
	}else{
	throw std::out_of_range("Key Not Member of Map");
	}

	

}

template<typename T1, typename T2>
void Map<T1, T2>::remove(const T1& key){
 
	 typename Map<T1,T2>::link* rb = search(tree.root,key);
	if(rb!=&nil){
		rbDelete(rb);
	}else{
	throw std::out_of_range("Key Not Member of Map");
	}

}

template<typename T1, typename T2>
T2& 
Map<T1,T2>::operator[](const T1& key){
	 
	 typename Map<T1,T2>::link* rb = search(tree.root,key);
	if(rb!=&nil){
		return static_cast<rbNode *>(rb)->kv.second;
	}
  T2 value = static_cast<T2>(65);//65==A
  std::pair<const T1, T2>pair(key, value);
  return ( static_cast<rbNode *>(insertP(pair).current_)->kv.second);
}

}	//cs540closing bracket
