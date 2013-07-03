/*$Id: PeakListPrivate.h 2029 2009-06-29 14:48:06Z tkroeger $*/

/*
 * PeakListPrivate.h
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

#ifndef __PLISTPRIVATE_H__
#define __PLISTPRIVATE_H__

#include <ms++/config.h>

#include <cassert>
#include <vector>

#include "ms++/Peak.h"

namespace ms
{

typedef std::vector<Peak> Peaks;

class MSPP_EXPORT PListPrivate
{
public:
    class MSPP_EXPORT Ref
    {
    public:
        friend std::ostream& operator<<(std::ostream& o, Ref& r);

        ///Default constructor.
        Ref();
        ///Copy constructor. \a ref will reference the same data as this object.
        Ref(const Ref& ref);
        ///Destructor. If the last reference to a PeakListPrivate instance is destroyed,
        ///that data is deleted.
        ~Ref();
        ///Assignment operator. Reference the same data as \a ref does.
        Ref& operator= (const Ref& ref);

        std::vector<Peak>* peaks() const;


    protected:
        ///Constructor. Reference the data pointed to by \a p as well.
        Ref(PListPrivate* p);

        ///Get a reference to a unique copy of the data referenced by this object
        Ref getRefToUniqueCopy();

        ///Pointer to the data this objects references
        PListPrivate* ptr;

        friend class PListPrivate;
        friend class PeakListBase;
        friend class PeakList;
    };

    ///Default constructor (public)
    static Ref create();

protected:
    PListPrivate();
    ~PListPrivate();
    void store();
    void load(int id);

    mutable Peaks peaks_;


    ///number of references to this object
    int refCount_;

    friend class Ref;
};

inline std::ostream& operator<<(std::ostream& o, PListPrivate::Ref& r) {
    o << "[PListPrivate=" << r.ptr << "]";
    return o;
}

} /*namespace ms*/

#endif /*__PEAKLISTPRIVATE_H__*/
