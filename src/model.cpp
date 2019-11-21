
#include "figure.h"
#include <math.h>
#include <sstream>

namespace ipegenerator{

void Figure::draw_float(const double &x, const double &y, const double &piston, const double &compressibility, const double &zoom)
{
    ipe::Group *group = new ipe::Group();

    // ********************** Pipe **********************
    ipe::Curve *curve_pipe=new ipe::Curve();
    const double pipe_compressibility = 0.1-(0.05*compressibility);

    ipe::Vector zero(s_t_x(x), s_t_y(y));

    ipe::Vector pipe00(s_t_x(-0.1*zoom), s_t_y(0)); pipe00 += zero;
    ipe::Vector pipe01(s_t_x(-0.1*zoom), s_t_y(1*zoom)); pipe01 += zero;
    ipe::Vector pipe11(s_t_x(0.1*zoom), s_t_y(1*zoom)); pipe11 += zero;
    ipe::Vector pipe10(s_t_x(0.1*zoom), s_t_y(0)); pipe10 += zero;

    std::vector<ipe::Vector> spline1{pipe00, ipe::Vector(s_t_x(-pipe_compressibility*zoom), s_t_y(0.5*zoom))+zero,pipe01};
    curve_pipe->appendSpline(spline1);
    curve_pipe->appendSegment(pipe01, pipe11);
    std::vector<ipe::Vector> spline2{pipe11, ipe::Vector(s_t_x(pipe_compressibility*zoom), s_t_y(0.5*zoom))+zero,pipe10};
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
    ipe::Vector piston00(s_t_x(-0.05*zoom), s_t_y(piston_position*zoom)); piston00 += zero;
    ipe::Vector piston01(s_t_x(-0.05*zoom), s_t_y((piston_position+size_piston)*zoom)); piston01 += zero;
    ipe::Vector piston11(s_t_x(0.05*zoom),  s_t_y((piston_position+size_piston)*zoom)); piston11 += zero;
    ipe::Vector piston10(s_t_x(0.05*zoom), s_t_y(piston_position*zoom)); piston10 += zero;
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

    m_page->append(ipe::TSelect::ENotSelected, m_current_layer, group);
}

}

