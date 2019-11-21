
#include "figure.h"
#include <math.h>
#include <sstream>

namespace ipegenerator{



void Figure::draw_float(const double &x, const double &y, const double &piston, const double &compressibility, const FLOAT_PISTON_MVT &mvt,const double &zoom)
{
    ipe::Group *group = new ipe::Group();
    ipe::Matrix zoom_translate_operator(ipe::Linear(zoom, 0.0, 0.0, zoom), ipe::Vector(x,y));
    ipe::Matrix final_operator=m_transform_global*zoom_translate_operator;

    // ********************** Pipe **********************
    ipe::Curve *curve_pipe=new ipe::Curve();
    const double pipe_compressibility = 0.1-(0.05*compressibility);

    ipe::Vector pipe00 = final_operator * ipe::Vector(-0.1, 0);
    ipe::Vector pipe01 = final_operator * ipe::Vector(-0.1, 1);
    ipe::Vector pipe11 = final_operator * ipe::Vector(0.1, 1);
    ipe::Vector pipe10 = final_operator * ipe::Vector(0.1, 0);

    std::vector<ipe::Vector> spline1{pipe00, final_operator * ipe::Vector(-pipe_compressibility, 0.5),pipe01};
    curve_pipe->appendSpline(spline1);
    curve_pipe->appendSegment(pipe01, pipe11);
    std::vector<ipe::Vector> spline2{pipe11, final_operator * ipe::Vector(pipe_compressibility,0.5),pipe10};
    curve_pipe->appendSpline(spline2);
    curve_pipe->setClosed(true);
    ipe::Shape shape_pipe;
    shape_pipe.appendSubPath(curve_pipe);

    ipe::AllAttributes attr_pipe;
    attr_pipe.iFill = m_steel_sheet->find(ipe::EColor,ipe::Attribute(true, "gray"));;
    attr_pipe.iStroke = ipe::Attribute::BLACK();
    attr_pipe.iPathMode  = ipe::EStrokedAndFilled;
    ipe::Path *path_pipe = new ipe::Path(attr_pipe, shape_pipe);
    group->push_back(path_pipe);

    // ********************** Piston **********************
    const double size_piston = 0.2;
    const double piston_position = -size_piston*abs(piston);
    ipe::Vector piston00 = final_operator * ipe::Vector(-0.05, piston_position);
    ipe::Vector piston01 = final_operator * ipe::Vector(-0.05, piston_position+size_piston);
    ipe::Vector piston11 = final_operator * ipe::Vector(0.05, piston_position+size_piston);
    ipe::Vector piston10 = final_operator * ipe::Vector(0.05, piston_position);
    ipe::Curve *curve_piston=new ipe::Curve();
    curve_piston->appendSegment(piston01, piston00);
    curve_piston->appendSegment(piston00, piston10);
    curve_piston->appendSegment(piston10, piston11);
    ipe::Shape shape_piston;
    shape_piston.appendSubPath(curve_piston);

    ipe::AllAttributes attr_piston;
    attr_piston.iFill = m_steel_sheet->find(ipe::EColor,ipe::Attribute(true, "darkgray"));;
    attr_piston.iStroke = ipe::Attribute::BLACK();
    attr_piston.iPathMode  = ipe::EStrokedAndFilled;
    ipe::Path *path_piston = new ipe::Path(attr_piston, shape_piston);
    group->push_back(path_piston);

    // ********************** Piston Movement **********************

    set_color_fill("black");
    set_color_stroke("black");
    set_color_type(STROKE_AND_FILL);
    if(mvt != FLOAT_PISTON_EQUAL)
    {
        ipe::Segment seg;
        if(mvt == FLOAT_PISTON_UP)
        {
            ipe::Vector arrow0 = final_operator * ipe::Vector(0.0, piston_position);
            ipe::Vector arrow1 = final_operator * ipe::Vector(0.0, 0.0);
            seg = ipe::Segment(arrow0, arrow1);
        }
        else
        {
            ipe::Vector arrow0 = final_operator * ipe::Vector(0.0, piston_position);
            ipe::Vector arrow1 = final_operator * ipe::Vector(0.0, -size_piston);
            seg = ipe::Segment(arrow0, arrow1);
        }
        ipe::AllAttributes attr(m_current_attr);
        attr.iFArrow = true;
        attr.iFArrowSize = ipe::Attribute::NORMAL();
        attr.iFArrowShape = ipe::Attribute::ARROW_NORMAL();

        ipe::Shape shape(seg);
        ipe::Path *path = new ipe::Path(attr, shape, true);
        group->push_back(path);
    }

    m_page->append(ipe::TSelect::ENotSelected, m_current_layer, group);
}

}

