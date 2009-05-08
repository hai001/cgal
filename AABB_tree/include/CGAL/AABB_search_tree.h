// Copyright (c) 2009  INRIA Sophia-Antipolis (France)
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: https://scm.gforge.inria.fr/svn/cgal/trunk/AABB_tree/include/CGAL/AABB_tree.h $
// $Id: AABB_tree.h 48894 2009-04-24 14:11:17Z palliez $
//
//
// Author(s) : Pierre Alliez

#ifndef CGAL_AABB_SEARCH_TREE_H
#define CGAL_AABB_SEARCH_TREE_H

#include <CGAL/Orthogonal_k_neighbor_search.h>
#include <CGAL/Search_traits_3.h>

namespace CGAL 
{
        template <class Underlying, class Id>
        class Add_decorated_point: public Underlying 
        {
                class Decorated_point: public Underlying::Point_3 
                {
                public:
                    Id id;
                    Decorated_point(): Underlying::Point_3(), id(){}
                    // Allows the user not to provide the id
                    // so that we don't break existing code
                    Decorated_point(const typename Underlying::Point_3& p): Underlying::Point_3(p), id(){}
                };
        public:
                typedef Decorated_point Point_3;
        };

        template <class Traits>
        class AABB_search_tree
        {
        public:
                typedef typename Traits::FT FT;
                typedef typename Traits::Point_3 Point;
                typedef typename Traits::Point_and_primitive_id Point_and_primitive_id;
                typedef typename CGAL::Search_traits_3<Add_decorated_point<Traits, typename Traits::Primitive::Id> > TreeTraits;
                typedef typename CGAL::Orthogonal_k_neighbor_search<TreeTraits> Neighbor_search;
                typedef typename Neighbor_search::Tree Tree;
        private:
                Tree* m_tree;
        public:
                template <class ConstPointIterator>
                AABB_search_tree(ConstPointIterator begin, ConstPointIterator beyond)
                {
                        typedef typename Add_decorated_point<Traits, typename Traits::Primitive::Id>::Point_3 Decorated_point;
                        std::vector<Decorated_point> points;
                        while(begin != beyond) {
                                points.push_back(Decorated_point(begin->first));
                                points.back().id = begin->second;
                                ++begin;
                        }
                        m_tree = new Tree(points.begin(), points.end());
                }
                
                ~AABB_search_tree() {
                        delete m_tree;
                }

                // TOFIX: make it const
                Point_and_primitive_id closest_point(const Point& query) 
                {
                        typedef typename Add_decorated_point<Traits, typename Traits::Primitive::Id>::Point_3 Decorated_point;
                        Neighbor_search search(*m_tree, query, 1);
                        return Point_and_primitive_id(static_cast<Point>(search.begin()->first), search.begin()->first.id);
                }
        };

}

#endif // CGAL_AABB_SEARCH_TREE_H

