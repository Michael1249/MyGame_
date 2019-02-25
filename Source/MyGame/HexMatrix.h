#pragma once

//направления навигации
enum HexDirection
{
	NW,
	W,
	SW,
	SE,
	E,
	NE,
	DirCount
};
/*
контейнер для хранения данных в гексагональной сетке
с возможностью навигации как по двумерному массиву, 
так и перескакивая от елемента к элементу с помощью  HexDirection
*/
template<typename T>
class HexMatrix
{
public:
	HexMatrix() :
		matrix(nullptr)
	{};
	
	HexMatrix(size_t width, size_t height, bool chain);
	
	template<typename T2>
	HexMatrix(const HexMatrix<T2>& original);
	//HexMatrix(const HexMatrix<T>& original);

	~HexMatrix();
	void Clear();
	void Create(size_t width, size_t height, bool chain);
	T* const GetHex(size_t indx) const;
	T* const GetHex(int x, int y) const;
	T* const GetHex(int x, int y, HexDirection dir) const;
	T* const GetHex(T* hex, HexDirection dir) const;
	T* GetHex(size_t indx);
	T* GetHex(int x, int y);
	T* GetHex(int x, int y, HexDirection dir);
	T* GetHex(T* hex, HexDirection dir);
	T* const operator[](size_t indx) const;
	T* operator[](size_t indx);
	size_t GetWidth() const { return matrix_width; }
	size_t GetHeight() const { return matrix_height; }
	size_t GetCount() const { return matrix_count; }
	size_t GetChain() const { return chain; }
protected:
	T* matrix;
	size_t matrix_width;
	size_t matrix_height;
	size_t matrix_count;
	bool chain;
};

template<typename T>
HexMatrix<T>::HexMatrix(size_t width, size_t height, bool chain)
{
	Create(width, height, chain);
}

template<typename T>
HexMatrix<T>::~HexMatrix()
{
	Clear();
}

template<typename T>
inline void HexMatrix<T>::Clear()
{
	if (matrix)
		delete[] matrix;
}

template<typename T>
inline void HexMatrix<T>::Create(size_t width, size_t height, bool chain)
{
	Clear();
	matrix_count = width * height;
	matrix = new T[matrix_count];
	matrix_width = width;
	matrix_height = height;
	this->chain = chain;
}

template<typename T>
inline T* const HexMatrix<T>::GetHex(size_t indx) const
{
	return const_cast<HexMatrix<T>*>(this)->GetHex(indx);
}

template<typename T>
inline T* const HexMatrix<T>::GetHex(int x, int y) const
{
	return const_cast<HexMatrix<T>*>(this)->GetHex(x, y);
}

template<typename T>
inline T* const HexMatrix<T>::GetHex(int x, int y, HexDirection dir) const
{
	return const_cast<HexMatrix<T>*>(this)->GetHex(x, y, dir);
}

template<typename T>
inline T* const HexMatrix<T>::GetHex(T * hex, HexDirection dir) const
{
	return const_cast<HexMatrix<T>*>(this)->GetHex(hex, dir);
}

template<typename T>
inline T* HexMatrix<T>::GetHex(size_t indx)
{
	if (indx >= matrix_count)
		return nullptr;
	return matrix + indx;
}

template<typename T>
T* HexMatrix<T>::GetHex(int x, int y)
{
	if (chain)
		x %= matrix_width;
	if (x < 0 || x > matrix_width - 1 || y < 0 || y > matrix_height - 1)
	{
		return nullptr;
	}
	return matrix + x + y * matrix_width;
}

template<typename T>
inline T* HexMatrix<T>::GetHex(int x, int y, HexDirection dir)
{
	int offset = 1 - y % 2;
	switch (dir)
	{
	case E: return GetHex(x + 1, y);
		break;
	case W: return GetHex(x - 1, y);
		break;
	case NE: return GetHex(x + offset, y - 1);
		break;
	case NW: return GetHex(x + offset - 1, y - 1);
		break;
	case SW: return GetHex(x + offset - 1, y + 1);
		break;
	case SE: return GetHex(x + offset, y + 1);
		break;
	default: return nullptr;
		break;
	}
	return nullptr;
}

template<typename T>
inline T* HexMatrix<T>::GetHex(T * hex, HexDirection dir)
{
	size_t indx = hex - matrix;
	return GetHex(indx % matrix_width, indx / matrix_width, dir);
}

template<typename T>
inline T * const HexMatrix<T>::operator[](size_t indx) const
{
	return GetHex(indx);
}

template<typename T>
inline T * HexMatrix<T>::operator[](size_t indx)
{
	return GetHex(indx);
}

template<typename T>
template<typename T2>
inline HexMatrix<T>::HexMatrix(const HexMatrix<T2>& original):
	HexMatrix()
{
	Create(original.GetWidth(), original.GetHeight(), original.GetChain());	
}

//template<typename T>
//inline HexMatrix<T>::HexMatrix(const HexMatrix<T>& original) :
//	HexMatrix()
//{
//	Create(original.GetWidth(), original.GetHeight(), original.GetChain());
//	for (size_t i; i < matrix_count; i++)
//	{
//		matrix[i] = &original.GetHex(i);
//	}
//}