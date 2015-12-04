/* Copyright (C) 2016 Doubango Telecom <https://www.doubango.org>
*
* This file is part of Open Source Panorama 360 (a.k.a pan360) project.
* Source code hosted at https://github.com/DoubangoTelecom/pan360
* Website hosted at http://pan360.org
*
* PAN360 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* PAN360 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with PAN360.
*/
#if !defined(_PAN360_OBJECT_H_)
#define _PAN360_OBJECT_H_

#include "pan360/p360_config.h"
#include "pan360/p360_debug.h"

#define P360ObjSafeRelease(pObject)	(pObject) = NULL
#define P360ObjSafeFree				P360ObjSafeRelease

class P360_API P360Obj
{
public:
	P360Obj() :m_nRefCount(0) {}
	P360Obj(const P360Obj &) :m_nRefCount(0) {}
	virtual ~P360Obj() {}

public:
	virtual P360_INLINE const char* getObjectId() = 0;
#if !defined(SWIG)
	P360_INLINE long getRefCount() const {
		return m_nRefCount;
	}
	void operator=(const P360Obj &) {}
#endif


public:
	P360_INLINE long takeRef() { /*const*/
		p360_atomic_inc(&m_nRefCount);
		return m_nRefCount;
	}
	P360_INLINE long releaseRef() { /*const*/
		if (m_nRefCount) { // must never be equal to zero
			p360_atomic_dec(&m_nRefCount);
		}
		return m_nRefCount;
	}

private:
	volatile long m_nRefCount;
};

//
//	P360ObjWrapper declaration
//
template<class P360ObjType>
class P360ObjWrapper
{

public:
	P360_INLINE P360ObjWrapper(P360ObjType obj
#if !defined(SWIG)
		= NULL
#endif
		);
	P360_INLINE P360ObjWrapper(const P360ObjWrapper<P360ObjType> &obj);
	virtual ~P360ObjWrapper();

public:
#if defined(SWIG)
	P360ObjType unWrap() { return getWrappedObject(); }
#else
	P360_INLINE P360ObjWrapper<P360ObjType>& operator=(const P360ObjType other);
	P360_INLINE P360ObjWrapper<P360ObjType>& operator=(const P360ObjWrapper<P360ObjType> &other);
	P360_INLINE bool operator ==(const P360ObjWrapper<P360ObjType> other) const;
	P360_INLINE bool operator!=(const P360ObjWrapper<P360ObjType> &other) const;
	P360_INLINE bool operator <(const P360ObjWrapper<P360ObjType> other) const;
	P360_INLINE P360ObjType operator->() const;
	P360_INLINE P360ObjType operator*() const;
	P360_INLINE operator bool() const;
#endif

protected:
	long takeRef();
	long releaseRef();

	P360ObjType getWrappedObject() const;
	void wrapObject(P360ObjType obj);

private:
	P360ObjType m_WrappedObject;
};

//
//	P360ObjWrapper implementation
//
template<class P360ObjType>
P360ObjWrapper<P360ObjType>::P360ObjWrapper(P360ObjType obj)
{
	wrapObject(obj), takeRef();
}

template<class P360ObjType>
P360ObjWrapper<P360ObjType>::P360ObjWrapper(const P360ObjWrapper<P360ObjType> &obj)
{
	wrapObject(obj.getWrappedObject()),
		takeRef();
}

template<class P360ObjType>
P360ObjWrapper<P360ObjType>::~P360ObjWrapper()
{
	releaseRef(),
		wrapObject(NULL);
}


template<class P360ObjType>
long P360ObjWrapper<P360ObjType>::takeRef()
{
	if (m_WrappedObject /*&& m_WrappedObject->getRefCount() At startup*/) {
		return m_WrappedObject->takeRef();
	}
	return 0;
}

template<class P360ObjType>
long P360ObjWrapper<P360ObjType>::releaseRef()
{
	if (m_WrappedObject && m_WrappedObject->getRefCount()) {
		if (m_WrappedObject->releaseRef() == 0) {
			delete m_WrappedObject, m_WrappedObject = NULL;
		}
		else {
			return m_WrappedObject->getRefCount();
		}
	}
	return 0;
}

template<class P360ObjType>
P360ObjType P360ObjWrapper<P360ObjType>::getWrappedObject() const
{
	return m_WrappedObject;
}

template<class P360ObjType>
void P360ObjWrapper<P360ObjType>::wrapObject(const P360ObjType obj)
{
	if (obj) {
		if (!(m_WrappedObject = dynamic_cast<P360ObjType>(obj))) {
			P360_DEBUG_ERROR("Trying to wrap an object with an invalid type");
		}
	}
	else {
		m_WrappedObject = NULL;
	}
}

template<class P360ObjType>
P360ObjWrapper<P360ObjType>& P360ObjWrapper<P360ObjType>::operator=(const P360ObjType obj)
{
	releaseRef();
	wrapObject(obj), takeRef();
	return *this;
}

template<class P360ObjType>
P360ObjWrapper<P360ObjType>& P360ObjWrapper<P360ObjType>::operator=(const P360ObjWrapper<P360ObjType> &obj)
{
	releaseRef();
	wrapObject(obj.getWrappedObject()), takeRef();
	return *this;
}

template<class P360ObjType>
bool P360ObjWrapper<P360ObjType>::operator ==(const P360ObjWrapper<P360ObjType> other) const
{
	return getWrappedObject() == other.getWrappedObject();
}

template<class P360ObjType>
bool P360ObjWrapper<P360ObjType>::operator!=(const P360ObjWrapper<P360ObjType> &other) const
{
	return getWrappedObject() != other.getWrappedObject();
}

template<class P360ObjType>
bool P360ObjWrapper<P360ObjType>::operator <(const P360ObjWrapper<P360ObjType> other) const
{
	return getWrappedObject() < other.getWrappedObject();
}

template<class P360ObjType>
P360ObjWrapper<P360ObjType>::operator bool() const
{
	return (getWrappedObject() != NULL);
}

template<class P360ObjType>
P360ObjType P360ObjWrapper<P360ObjType>::operator->() const
{
	return getWrappedObject();
}

template<class P360ObjType>
P360ObjType P360ObjWrapper<P360ObjType>::operator*() const
{
	return getWrappedObject();
}

#endif /* _PAN360_OBJECT_H_ */
