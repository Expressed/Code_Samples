#include "uniqueptr.hpp"
#include <set>
#include <cstdlib>
#include <cassert>
#include <type_traits>

std::set<size_t *> counts;

template<typename T> using UP = cs540::UniquePtr<T>;

void count_check() { /*for (auto c : counts) assert(!*c);*/ }

template<typename T>
inline void unique_check() {
	static_assert(std::is_default_constructible<UP<T>>{}, "");
	static_assert(std::is_constructible<UP<T>, T*>{}, "");
	static_assert(!std::is_copy_constructible<UP<T>>{}, "");
	static_assert(std::is_move_constructible<UP<T>>{}, "");
	static_assert(!std::is_copy_assignable<UP<T>>{}, "");
	static_assert(std::is_move_assignable<UP<T>>{}, "");
	static_assert(std::is_constructible<bool, UP<T>>{}, "");
	static_assert(!std::is_convertible<UP<T>, bool>{}, "");
	static_assert(std::is_destructible<UP<T>>{}, "");
}

template<typename T, typename U>
inline void convert_check() {
	static_assert(std::is_constructible<UP<U>, T*>{}, "");
	static_assert(!std::is_constructible<UP<U>, const UP<T>&>{}, "");
	static_assert(std::is_constructible<UP<U>, UP<T>&&>{}, "");
	static_assert(!std::is_assignable<UP<U>, const UP<T>&>{}, "");
	static_assert(std::is_assignable<UP<U>, UP<T>&&>{}, "");
}

struct Base {
	size_t n, m;
	Base(size_t n = 0) : n{n}, m{n} {
		unique_check<Base>();
		counts.insert(&++count);
	}
	~Base() { assert(count--); }
	static size_t count;
};

struct Derived : Base {
	Derived(size_t n = 1) : Base{n} {
		unique_check<Derived>();
		convert_check<Derived, Base>();
		counts.insert(&++count);
	}
	~Derived() { assert(count--); }
	static size_t count;
};

template<size_t N>
struct Chain : Chain<N-1> {
	Chain(size_t n = N) : Chain<N-1>{n} {
		unique_check<Chain>();
		convert_check<Chain, Chain<N-1>>();
		counts.insert(&++count);
	}
	~Chain() { assert(count--); }
	static size_t count;
};

template<>
struct Chain<0> {
	size_t n, m;
	Chain(size_t n = 0) : n{n}, m{n} {
		unique_check<Chain>();
		counts.insert(&++count);
	}
	~Chain() { assert(count--); }
	static size_t count;
};

template<size_t N, size_t M>
struct Grid : Grid<N, M-1>, Grid<N-1,M> {
	Grid(size_t n = N, size_t m = M)
		: Grid<N, M-1>{n, m}, Grid<N-1, M>{n, m} {
		unique_check<Grid>();
		convert_check<Grid, Grid<N, M-1>>();
		convert_check<Grid, Grid<N-1, M>>();
		counts.insert(&++count);
	}
	~Grid() { assert(count--); }
	static size_t count;
};

template<size_t N>
struct Grid<N, 0> : Grid<N-1, 0> {
	Grid(size_t n = N, size_t m = 0) : Grid<N-1, 0>{n, m} {
		unique_check<Grid>();
		convert_check<Grid, Grid<N-1, 0>>();
		counts.insert(&++count);
	}
	~Grid() { assert(count--); }
	static size_t count;
};

template<size_t M>
struct Grid<0, M> : Grid<0, M-1> {
	Grid(size_t n = 0, size_t m = M) : Grid<0, M-1>{n, m} {
		unique_check<Grid>();
		convert_check<Grid, Grid<0, M-1>>();
		counts.insert(&++count);
	}
	~Grid() { assert(count--); }
	static size_t count;
};

template<>
struct Grid<0, 0> {
	size_t n, m;
	Grid(size_t n = 0, size_t m = 0) : n{n}, m{m} {
		unique_check<Grid>();
		counts.insert(&++count);
	}
	~Grid() { assert(count--); }
	static size_t count;
};

size_t Base::count = 0,
	   Derived::count = 0,
	   Chain<0>::count = 0,
	   Grid<0, 0>::count = 0;
template<size_t N>
size_t Chain<N>::count = 0;
template<size_t N, size_t M>
size_t Grid<N, M>::count = 0;
template<size_t N>
size_t Grid<N, 0>::count = 0;
template<size_t M>
size_t Grid<0, M>::count = 0;

using BP = UP<Base>;
using DP = UP<Derived>;
template<size_t N>
using CP = UP<Chain<N>>;
template<size_t N, size_t M>
using GP = UP<Grid<N, M>>;

template class cs540::UniquePtr<Base>;
template class cs540::UniquePtr<Derived>;
template class cs540::UniquePtr<Chain<20>>;
template class cs540::UniquePtr<Grid<5,5>>;

template<typename T>
inline bool null(const UP<T> &u) {
	return
		(u == u) && (!(u != u)) &&
		(u == nullptr) && (!(u != nullptr)) &&
		(nullptr == u) && (!(nullptr != u)) &&
		(u.get() == nullptr);
}

template<typename T>
inline bool not_null(const UP<T> &u) {
	return
		(u == u) && (!(u != u)) &&
		(u != nullptr) && (!(u == nullptr)) &&
		(nullptr != u) && (!(nullptr == u)) &&
		(u.get() != nullptr);
}

template<typename T, typename U>
inline bool equal(const UP<T> &t, const UP<U> &u) {
	return
		(t == t) && (!(t != t)) &&
		(u == u) && (!(u != u)) &&
		(t == u) && (!(t != u)) &&
		(u == t) && (!(u != t)) &&
		(t.get() == u.get());
}

template<typename T, typename U>
inline bool not_equal(const UP<T> &t, const UP<U> &u) {
	return
		(t == t) && (!(t != t)) &&
		(u == u) && (!(u != u)) &&
		(t != u) && (!(t == u)) &&
		(u != t) && (!(u == t)) &&
		(t.get() != u.get());
}

template<typename T>
inline bool val_equal(const UP<T> &t, size_t n = 0, size_t m = 0) {
	return
		not_null(t) &&
		(t->n == n) && ((*t).n == n) && (t.get()->n == n) &&
		(t->m == m) && ((*t).m == m) && (t.get()->m == m);
}

template<typename T, typename U>
inline bool val_equal(const UP<T> &t, const UP<U> &u) {
	return
		not_null(t) && not_null(u) && equal(t, u) &&
		val_equal(t, t->n, t->m) && val_equal(t, u->n, u->m) &&
		val_equal(u, t->n, t->m) && val_equal(u, u->n, u->m);
}

template<typename T, typename U>
inline bool val_not_equal(const UP<T> &t, const UP<U> &u) {
	return
		not_null(t) && not_null(u) && not_equal(t, u) &&
		val_equal(t, t->n, t->m) && !val_equal(t, u->n, u->m) &&
		!val_equal(u, t->n, t->m) && val_equal(u, u->n, u->m);
}

int main() {
	atexit(count_check);
	{
		DP dp;
		assert(null(dp));
		DP dp2(std::move(dp));
		assert(null(dp2));
		dp = std::move(dp2);
		assert(null(dp));
		dp2.reset(nullptr);
		assert(null(dp2));
		assert(equal(dp, dp2));
	}
	{
		DP dp{new Derived};
		assert(not_null(dp));
		assert(val_equal(dp, 1, 1));
		dp.reset(new Derived{2});
		assert(not_null(dp));
		assert(val_equal(dp, 2, 2));
		dp.reset(nullptr);
		assert(null(dp));
		dp = DP{new Derived{3}};
		assert(not_null(dp));
		assert(val_equal(dp, 3, 3));
		dp = std::move(dp);
		dp = DP{new Derived};
		assert(not_null(dp));
		assert(val_equal(dp, 1, 1));
	}
	{
		BP bp{new Base}, dp{new Derived};
		assert(not_null(bp));
		assert(not_null(dp));
		assert(val_equal(bp));
		assert(val_equal(dp, 1, 1));
		assert(val_not_equal(bp, dp));
		{
			BP mp{std::move(bp)};
			assert(val_equal(mp));
			mp = std::move(dp);
			assert(val_equal(mp, 1, 1));
		}
		bp.reset(new Base);
		dp.reset(new Derived);
		DP dp2{new Derived};
		assert(val_not_equal(bp, dp));
		assert(val_not_equal(bp, dp2));
		assert(not_equal(dp, dp2));
	}
	{
		CP<0> cp0{new Chain<2>};
		assert(val_equal(cp0, 2, 2));
		cp0 = CP<3>{new Chain<5>};
		assert(val_equal(cp0, 5, 5));
		cp0 = CP<2>{CP<5>{CP<8>{new Chain<12>}}};
		assert(val_equal(cp0, 12, 12));
	}
	{
		CP<0> cp0;
		CP<2> cp2;
		CP<5> cp5;
		cp5.reset(new Chain<10>);
		assert(val_equal(cp5, 10, 10));
		cp2 = std::move(cp5);
		assert(val_equal(cp2, 10, 10));
		cp0 = std::move(cp2);
		assert(val_equal(cp0, 10, 10));
	}
	{
		GP<0, 0> gp00{GP<0,3>{GP<4,3>{GP<4,4>{GP<4,5>{new Grid<5, 5>}}}}};
		assert(val_equal(gp00, 5, 5));
	}
	{
		GP<0, 0> gp00; GP<1, 0> gp10;
		GP<1, 1> gp11;
		GP<1, 2> gp12; GP<2, 2> gp22;
		gp22.reset(new Grid<2, 5>);
		gp12 = std::move(gp22);
		gp11 = std::move(gp12);
		gp10 = std::move(gp11);
		gp00 = std::move(gp10);
		assert(val_equal(gp00, 2, 5));
	}
}
