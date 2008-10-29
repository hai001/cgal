// TODO: Add licence
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL:$
// $Id: $
// 
//
// Author(s)     : Pavel Emeliyanenko <asm@mpi-sb.mpg.de>
//
// ============================================================================

/*!\file CGAL/IO/Qt_widget_Curve_renderer_2.h
 * \brief
  * provides \c CGAL::Qt_widget interface for the curve renderer
 */

#ifndef CGAL_QT_WIDGET_CURVE_RENDERER_2_H
#define CGAL_QT_WIDGET_CURVE_RENDERER_2_H

#include <qpainter.h>
#include <CGAL/IO/Qt_widget.h>
#include <CGAL/Curved_kernel_via_analysis_2/Curve_renderer_facade.h>

CGAL_BEGIN_NAMESPACE

/*! \brief
 * outputs a curve arc to \c Qt_widget
 */
template <class CKvA_2>
Qt_widget& operator << (Qt_widget& ws, const CGALi::Arc_2< CKvA_2 >& arc) {
    
    typedef Curve_renderer_facade<CKvA_2> Facade;

    typedef std::pair< int, int > Coord_2;
    typedef std::vector< Coord_2 > Coord_vec_2;

    boost::optional < Coord_2 > p1, p2;
    std::list<Coord_vec_2> points;
   
    //Facade::setup(CGAL::Bbox_2(-100, -100, 10, -10),
      //      330, 270);

    Facade::setup(CGAL::Bbox_2(ws.x_min(), ws.y_min(), ws.x_max(), ws.y_max()),
               ws.width(), ws.height());

    Facade::instance().draw(arc, points, &p1, &p2);
    if(points.empty()) 
        return ws;
        
    QPainter *ppnt = &ws.get_painter();
    int height = ws.height();

   // std::cerr << ws.width() << " and " <<  ws.height() << "\n";
    typename std::list<Coord_vec_2>::const_iterator lit = points.begin();
    //ppnt->moveTo((*p1).first, height - (*p1).second);
    while(lit != points.end()) {

        const Coord_vec_2& vec = *lit;
        typename Coord_vec_2::const_iterator vit = vec.begin();
        //std::cerr << "(" << vit->first << "; " << vit->second << ")\n";
//         if(lit == points.begin() &&*/ vit != vec.end()) {
//             ppnt->lineTo(vit->first, height - vit->second);
//             vit++;
//         }
        if(vit != vec.end()) 
            ppnt->moveTo(vit->first, height - vit->second);
        
        while(vit != vec.end()) {
            ppnt->lineTo(vit->first, height - vit->second);
            vit++;
            //std::cerr << "(" << vit->e0 << "; " << vit->e1 << "\n";
        }
        lit++;
    }
    //ppnt->lineTo((*p2).first, height - (*p2).second);
        
    QPen old_pen = ppnt->pen();
    ppnt->setPen(QPen(Qt::NoPen)); // avoid drawing outlines
    // draw with the current brush attributes

    //std::cerr << "endpts1: (" << (*p1).first << "; " << (*p1).second << "\n";
    //std::cerr << "endpts2: (" << (*p2).first << "; " << (*p2).second << "\n";

    ppnt->drawEllipse((*p1).first-3, height-(*p1).second-3, 6, 6);
    ppnt->drawEllipse((*p2).first-3, height-(*p2).second-3, 6, 6);
    ppnt->setPen(old_pen);

    return ws;
}

/*! \brief
 *  outputs a curve point to \c Qt_widget
 */
template <class CKvA_2>
Qt_widget& operator << (Qt_widget& ws, const CGALi::Point_2< CKvA_2 >& pt) {
    
    typedef Curve_renderer_facade<CKvA_2> Facade;
   
    std::pair< int, int > coord;
    Facade::setup(CGAL::Bbox_2(ws.x_min(), ws.y_min(), ws.x_max(), ws.y_max()),
            ws.width(), ws.height());

    if(!Facade::instance().draw(pt, coord)) {
        return ws;
    }
       
    QPainter *ppnt = &ws.get_painter();
    QPen old_pen = ppnt->pen();
    ppnt->setPen(QPen(Qt::NoPen));
    ppnt->drawEllipse(coord.first - 3, ws.height() - coord.second - 3, 6, 6);
    ppnt->setPen(old_pen);
    return ws;
}

CGAL_END_NAMESPACE

#endif // CGAL_QT_WIDGET_CURVE_RENDERER_2_H

