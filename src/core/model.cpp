
#include "figure.h"
#include <math.h>
#include <sstream>

namespace ipegenerator{



size_t Figure::draw_float(const double &x, const double &y, const double &piston, const double &compressibility, const FLOAT_PISTON_MVT &mvt,const double &zoom)
{
    ipe::Group *group = new ipe::Group();
    const double min_size_frame = std::min(m_frame_data[0].diam(), m_frame_data[1].diam());
    ipe::Matrix zoom_operator(ipe::Linear(zoom*min_size_frame, 0.0, 0.0, zoom*min_size_frame)*m_transform_global_keep_dimension_keep_y.linear(), ipe::Vector(0.0, 0.0));
    ipe::Matrix translate_operator(ipe::Linear(), m_transform_global*ipe::Vector(x, y));
    ipe::Matrix offset_operator(ipe::Linear(), ipe::Vector(0, -0.5));
    ipe::Matrix final_operator=translate_operator*(zoom_operator*offset_operator);

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
    const double piston_position = -size_piston*(1.+piston)/2.;
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
    return m_page->count()-1;
}


size_t Figure::draw_auv(const double &x, const double &y, const double& yaw, const double &zoom, const bool &custom_color)
{
    ipe::Group *group = new ipe::Group();
    const double min_size_frame = std::min(m_frame_data[0].diam(), m_frame_data[1].diam());
    ipe::Matrix zoom_operator(ipe::Linear(zoom*min_size_frame, 0.0, 0.0, zoom*min_size_frame)*m_transform_global_keep_dimension_keep_y.linear(), ipe::Vector(0.0, 0.0));
    ipe::Matrix translate_operator(ipe::Linear(), m_transform_global*ipe::Vector(x, y));
    ipe::Matrix rotate_operator(cos(yaw),sin(yaw), -sin(yaw),cos(yaw),0,0);
    ipe::Matrix final_operator=translate_operator*rotate_operator*zoom_operator;

    // ********************** Main Body **********************
    ipe::Curve *curve_auv=new ipe::Curve();
    ipe::Vector auv0 = final_operator * ipe::Vector(0, 1);
    ipe::Vector auv1 = final_operator * ipe::Vector(3, 1);
    curve_auv->appendSegment(auv0, auv1);
    ipe::Vector auv2 = final_operator * ipe::Vector(4, 0);
    ipe::Vector auv3 = final_operator * ipe::Vector(3, -1);
    std::vector<ipe::Vector> spline1{auv1, auv2,auv3};
    curve_auv->appendSpline(spline1);
    ipe::Vector auv4 = final_operator * ipe::Vector(0, -1);
    curve_auv->appendSegment(auv3, auv4);
    ipe::Vector auv5 = final_operator * ipe::Vector(-0.7, -0.7);
    ipe::Vector auv6 = final_operator * ipe::Vector(-1, -0.1);
    std::vector<ipe::Vector> spline2{auv4, auv5,auv6};
    curve_auv->appendSpline(spline2);
    ipe::Vector auv7 = final_operator * ipe::Vector(-1.3, -0.7);
    ipe::Vector auv8 = final_operator * ipe::Vector(-1.5, -1);
    std::vector<ipe::Vector> spline3{auv6, auv7,auv8};
    curve_auv->appendSpline(spline3);
    ipe::Vector auv9 = final_operator * ipe::Vector(-2, -1);
    curve_auv->appendSegment(auv8, auv9);
    ipe::Vector auv10 = final_operator * ipe::Vector(-2, 1);
    curve_auv->appendSegment(auv9, auv10);
    ipe::Vector auv11 = final_operator * ipe::Vector(-1.5, 1);
    curve_auv->appendSegment(auv10, auv11);
    ipe::Vector auv12 = final_operator * ipe::Vector(-1.3, 0.7);
    ipe::Vector auv13 = final_operator * ipe::Vector(-1, 0.1);
    std::vector<ipe::Vector> spline4{auv11, auv12,auv13};
    curve_auv->appendSpline(spline4);
    ipe::Vector auv14 = final_operator * ipe::Vector(-0.7, 0.7);
    std::vector<ipe::Vector> spline5{auv13, auv14,auv0};
    curve_auv->appendSpline(spline5);
    curve_auv->setClosed(true);
    ipe::Shape shape_auv;
    shape_auv.appendSubPath(curve_auv);

    ipe::AllAttributes attr_auv;

    ipe::Path *path_auv;
    if(!custom_color)
    {
        attr_auv.iFill = m_steel_sheet->find(ipe::EColor,ipe::Attribute(true, "yellow"));
        attr_auv.iStroke = ipe::Attribute::BLACK();
        attr_auv.iPathMode  = ipe::EStrokedAndFilled;
        path_auv = new ipe::Path(attr_auv, shape_auv);
    }
    else
    {
        path_auv = new ipe::Path(this->m_current_attr, shape_auv);
    }
    group->push_back(path_auv);


    m_page->append(ipe::TSelect::ENotSelected, m_current_layer, group);
    return m_page->count()-1;

}

size_t Figure::draw_simple_auv(const double &x, const double &y, const double& yaw, const double &zoom, const bool &custom_color)
    {
        ipe::Group *group = new ipe::Group();
        const double min_size_frame = std::min(m_frame_data[0].diam(), m_frame_data[1].diam());
        ipe::Matrix zoom_operator(ipe::Linear(zoom*min_size_frame, 0.0, 0.0, zoom*min_size_frame)*m_transform_global_keep_dimension_keep_y.linear(), ipe::Vector(0.0, 0.0));
        ipe::Matrix translate_operator(ipe::Linear(), m_transform_global*ipe::Vector(x, y));
        ipe::Matrix rotate_operator(cos(yaw),sin(yaw), -sin(yaw),cos(yaw),0,0);
        ipe::Matrix final_operator=translate_operator*rotate_operator*zoom_operator;

        // ********************** Main Body **********************
        ipe::Curve *curve_auv=new ipe::Curve();
        ipe::Vector auv0 = final_operator * ipe::Vector(-1, 1);
        ipe::Vector auv1 = final_operator * ipe::Vector(-1, -1);
        curve_auv->appendSegment(auv0, auv1);
        ipe::Vector auv2 = final_operator * ipe::Vector(2, 0);
        curve_auv->appendSegment(auv1, auv2);
        curve_auv->appendSegment(auv2, auv0);
        curve_auv->setClosed(true);
        ipe::Shape shape_auv;
        shape_auv.appendSubPath(curve_auv);

        ipe::AllAttributes attr_auv;

        ipe::Path *path_auv;
        if(!custom_color)
        {
            attr_auv.iFill = m_steel_sheet->find(ipe::EColor,ipe::Attribute(true, "yellow"));
            attr_auv.iStroke = ipe::Attribute::BLACK();
            attr_auv.iPathMode  = ipe::EStrokedAndFilled;
            path_auv = new ipe::Path(attr_auv, shape_auv);
        }
        else
        {
            path_auv = new ipe::Path(this->m_current_attr, shape_auv);
        }
        group->push_back(path_auv);


        m_page->append(ipe::TSelect::ENotSelected, m_current_layer, group);
        return m_page->count()-1;

    }

size_t Figure::draw_shape(const double &x, const double &y, const double& rotation,const std::vector<std::vector<double>> shape, const double &zoom, const bool &custom_color)
{
    const double min_size_frame = std::min(m_frame_data[0].diam(), m_frame_data[1].diam());
    ipe::Matrix zoom_operator(ipe::Linear(zoom*min_size_frame, 0.0, 0.0, zoom*min_size_frame)*m_transform_global_keep_dimension_keep_y.linear(), ipe::Vector(0.0, 0.0));
    ipe::Matrix translate_operator(ipe::Linear(), m_transform_global*ipe::Vector(x, y));
    ipe::Matrix rotate_operator(cos(rotation),sin(rotation), -sin(rotation),cos(rotation),0,0);
    ipe::Matrix final_operator=translate_operator*rotate_operator*zoom_operator;

    ipe::Curve *curve=new ipe::Curve();
    std::vector<ipe::Vector> spline;
    for (size_t i= 0; i<shape.size();i++ )
    {
        spline.push_back(final_operator*ipe::Vector(shape[i][0],shape[i][1]));
    }
    spline.push_back(final_operator*ipe::Vector(shape[0][0],shape[0][1]));
    curve->appendSpline(spline);
    ipe::Shape shape_ipe;
    shape_ipe.appendSubPath(curve);

    ipe::AllAttributes attr;

    ipe::Path *path;
    if(!custom_color)
    {
        attr.iFill = m_steel_sheet->find(ipe::EColor,ipe::Attribute(true, "yellow"));
        attr.iStroke = ipe::Attribute::BLACK();
        attr.iPathMode  = ipe::EStrokedAndFilled;
        path = new ipe::Path(attr, shape_ipe);
    }
    else
    {
        path = new ipe::Path(this->m_current_attr, shape_ipe);
    }


    m_page->append(ipe::TSelect::ENotSelected, m_current_layer, path);
    return m_page->count()-1;
}

size_t Figure::draw_boat(const double &x, const double &y, const double& yaw, const double &zoom, const bool &custom_color)
{
    ipe::Group *group = new ipe::Group();
    const double min_size_frame = std::min(m_frame_data[0].diam(), m_frame_data[1].diam());
    ipe::Matrix zoom_operator(ipe::Linear(zoom*min_size_frame, 0.0, 0.0, zoom*min_size_frame)*m_transform_global_keep_dimension_keep_y.linear(), ipe::Vector(0.0, 0.0));
    ipe::Matrix translate_operator(ipe::Linear(), m_transform_global*ipe::Vector(x, y));
    ipe::Matrix rotate_operator(cos(yaw),sin(yaw), -sin(yaw),cos(yaw),0,0);
    ipe::Matrix final_operator=translate_operator*rotate_operator*zoom_operator;

    // ********************** Main Body **********************
    ipe::Curve *curve_boat = new ipe::Curve();
    ipe::Vector p0 = final_operator * ipe::Vector(-1, 0.7);
    ipe::Vector p1 = final_operator * ipe::Vector(-0.5, 1);
    ipe::Vector p2 = final_operator * ipe::Vector(2.3, 0.85);
    ipe::Vector p3 = final_operator * ipe::Vector(3, 0);
    ipe::Vector m0 = final_operator * ipe::Vector(-1, -0.7);
    ipe::Vector m1 = final_operator * ipe::Vector(-0.5, -1);
    ipe::Vector m2 = final_operator * ipe::Vector(2.3, -0.85);
    ipe::Vector m3 = final_operator * ipe::Vector(3, 0);
    std::vector<ipe::Vector> spline1{p1, p2, p3};
    std::vector<ipe::Vector> spline2{m3, m2, m1};
    curve_boat->appendSegment(p0, p1);
    curve_boat->appendSpline(spline1);
    curve_boat->appendSpline(spline2);
    curve_boat->appendSegment(m1, m0);
    curve_boat->appendSegment(m0, p0);
    curve_boat->setClosed(true);
    ipe::Shape shape_boat;
    shape_boat.appendSubPath(curve_boat);

    ipe::AllAttributes attr_boat;
    ipe::Path *path_boat;
    if(!custom_color) {
        attr_boat.iFill = m_steel_sheet->find(ipe::EColor,ipe::Attribute(true, "yellow"));
        attr_boat.iStroke = ipe::Attribute::BLACK();
        attr_boat.iPathMode  = ipe::EStrokedAndFilled;
        path_boat = new ipe::Path(attr_boat, shape_boat);
    }
    else
    {
        path_boat = new ipe::Path(this->m_current_attr, shape_boat);
    }
    group->push_back(path_boat);

    m_page->append(ipe::TSelect::ENotSelected, m_current_layer, group);
    return m_page->count()-1;
}
}