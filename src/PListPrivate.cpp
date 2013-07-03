/*$Id: PeakListPrivate.cpp 2029 2009-06-29 14:48:06Z tkroeger $*/

/*
 * PeakListPrivate.cpp
 *
 * Copyright (c) 2008 Thorben Kroeger <thorben.kroeger@iwr.uni-heidelberg.de>
 *
 * This file is part of ms++.
 *
 * ms++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ms++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with ms++. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <ms++/private/PListPrivate.h>

#include <ms++/Error.h>
#include <ms++/Log.h>


using namespace ms;

//////////////////////////////////////////////////////////////////////////////
// PListPrivate::Ref //////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//
//Constructors
//

//Default Constructor
PListPrivate::Ref::Ref()
        : ptr(0)
{
}

//Construct as a reference to already existing data
PListPrivate::Ref::Ref(PListPrivate* p) : ptr(p)
{
    mspp_invariant(ptr != 0, "PListPrivate::Ref::Ref(): ptr should not be zero");
    ++ptr->refCount_;
}

//Construct as referencing the same data as already existing reference ref
PListPrivate::Ref::Ref(const Ref& ref) : ptr(ref.ptr)
{
    mspp_invariant(ptr != 0, "PListPrivate::Ref::Ref(): ptr should not be zero");
    ++ptr->refCount_;
}

//Assignment operator
PListPrivate::Ref& PListPrivate::Ref::operator= (const PListPrivate::Ref & ref)
{
    if (ptr == 0) {
        //We reference no data.
        //Just count us as another p.ptr user
        ptr = ref.ptr;
        ++ptr->refCount_;
        return *this;
    }
    if (&ref == this) {
        //test for self-assignment
        return *this;
    }

    //Make us use p's data instead,
    //delete our old data if necessary
    PListPrivate* const old = ptr;
    ptr = ref.ptr;
    ++ptr->refCount_;
    if (--old->refCount_ == 0) {
        delete old;
    }
    return *this;
}

//Get a reference to a unique copy of the data referenced by this object
PListPrivate::Ref PListPrivate::Ref::getRefToUniqueCopy()
{
    mspp_invariant(ptr != 0, "PListPrivate::Ref::Ref(): ptr should not be zero.");

    PListPrivate* p = new PListPrivate;
    *p = *ptr;
    p->refCount_ = 0;
    return Ref( p );
}

//
//Destructor
//

PListPrivate::Ref::~Ref()
{
    assert(ptr != 0);
    if (--ptr->refCount_ == 0) {
        delete ptr;
    }
}

std::vector<Peak>* PListPrivate::Ref::peaks() const
{
    mspp_invariant(ptr != 0, "PListPrivate::Ref::peaks(): ptr should not be zero.");
    return &(ptr->peaks_);
}


//////////////////////////////////////////////////////////////////////////////
// PeakListPrivate      //////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//
// Constructor
//

//Default constructor (public)
PListPrivate::Ref PListPrivate::create()
{
    return Ref( new PListPrivate );
}

//Default constructor (protected)
PListPrivate::PListPrivate()
        :refCount_(0)
{
}

//
// Destructor
//

PListPrivate::~PListPrivate()
{
}

//
// Loading and saving
//
