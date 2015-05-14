#include <cstddef>
#include <utility>
#include <iostream>
#include <typeinfo>
namespace cs540{

	class BaseHandler{
			public:
				BaseHandler(){};
				virtual ~BaseHandler(){};
			private:
		};
			
			template <typename A>
			class Handler: public BaseHandler{
				public:
				Handler(A* p):ptr(p){};	
				~Handler(){ delete this->ptr;};
				private:
				A* ptr;
			};

	template <class T>
	class UniquePtr{		
		public:
			UniquePtr();
			template <typename U> explicit UniquePtr(U *p);
			UniquePtr(UniquePtr&&);
			template <typename U> UniquePtr(UniquePtr<U>&&);
			~UniquePtr();
			T* get() const;
			template <typename U> void reset(U*);
			void reset(std::nullptr_t);
			UniquePtr& operator=(UniquePtr&&);
			template <typename U> UniquePtr& operator=(UniquePtr<U>&&);
			T& operator*() const;
			T* operator->() const;
			explicit operator bool() const;
			T* ptr;
			cs540::BaseHandler* bhptr;
			//Delete Copy-Constructor and Assignment
			template <typename U> UniquePtr& operator=(UniquePtr<U>&) = delete;
			UniquePtr& operator=(UniquePtr&) = delete;
			template <typename U> UniquePtr(const UniquePtr<U>&) = delete;
			UniquePtr(const UniquePtr&) = delete;		

	};//End Class UniquePtr
	
	template <typename T>
	UniquePtr<T>::UniquePtr():ptr(nullptr),bhptr(nullptr){
		

	}
	template <typename T> 
	UniquePtr<T>::~UniquePtr(){
		
		delete  bhptr;
		ptr = nullptr;
	
	}

	template <typename T> template<typename U>
	UniquePtr<T>::UniquePtr(U *p):ptr(p), bhptr( new cs540::Handler<U>(p)){


	}
	
	template <typename T>
	UniquePtr<T>::UniquePtr(UniquePtr&& o):ptr(o.ptr), bhptr(o.bhptr){
			o.ptr = nullptr;
			o.bhptr = nullptr;
	}
	template <typename T>template <typename U> 
	UniquePtr<T>::UniquePtr(UniquePtr<U>&& o):ptr(o.ptr), bhptr(o.bhptr){
		o.ptr = nullptr;
		o.bhptr = nullptr;
	}
	
	template<typename T>
	T*
	UniquePtr<T>::get() const{
		return ptr;
	}
	
	template<typename T> template <typename U>
	void UniquePtr<T>::reset(U *o){
	    if(ptr){delete bhptr;}
			ptr = o;
			bhptr = new Handler<U>(o);
	}

	template<typename T>
  void 
	UniquePtr<T>::reset(std::nullptr_t n){
		if(ptr){delete bhptr;}
		ptr = n;
		bhptr = n;
  }
	template<typename T>
	UniquePtr<T>&
	UniquePtr<T>::operator=(UniquePtr&& o){
	
	if(*this != o){
				if(bhptr != nullptr ){delete bhptr;}
				ptr = o.ptr;
				bhptr = o.bhptr;	
				o.ptr = nullptr;
				o.bhptr = nullptr;
		}
		return *this;

	}
	template <typename T>template <typename U> 
	UniquePtr<T>&
	UniquePtr<T>::operator=(UniquePtr<U>&& o){

		if(*this != o){
			if(bhptr != nullptr){delete bhptr;}
			ptr = o.ptr;
			bhptr = o.bhptr;
			o.ptr = nullptr;
			o.bhptr = nullptr;
		}		
		return *this;

	}


	template <typename T>
	T& UniquePtr<T>::operator*() const{
		return *ptr;
	}

	template <typename T>
	T* UniquePtr<T>::operator->() const{
		return ptr;
	}

	template <typename T>
	UniquePtr<T>::operator bool() const{
			return ptr != nullptr;
	}


   template<typename T1, typename T2> 
		bool operator==(const UniquePtr<T1>& up1, const UniquePtr<T2>& up2){		
			return up1.ptr == up2.ptr;
		}

		 template <typename T1>  
		bool operator==(const UniquePtr<T1> & up1, std::nullptr_t n){
			return up1.ptr == n;
		}	
		template <typename T1>
		bool operator==(std::nullptr_t n, const UniquePtr<T1> & up1){
			return up1 == n;
		}

		template <typename T1, typename T2>
		bool operator!=(const UniquePtr<T1>& up1, const UniquePtr<T2> &up2){
			return !(up1==up2);
		}
		template <typename T1>
		bool operator!=(const UniquePtr<T1> &up1, std::nullptr_t n){
			return !(up1==n);
		}

		template <typename T1>
		bool operator!=(std::nullptr_t n, const UniquePtr<T1> & up1){
			return !(up1 == n);
		}


}//End540
