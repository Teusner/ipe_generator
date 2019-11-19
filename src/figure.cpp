#include "figure.h"
#include <math.h>
#include <sstream>

namespace ipegenerator{

Figure::Figure(const ibex::IntervalVector &frame_data, const double width, const double height, const bool keep_ratio):
    m_frame_data(2)
{
    m_frame_data = frame_data;
    init_scale(width, height, keep_ratio);

    // Create new document
    ipe::Platform::initLib(ipe::IPELIB_VERSION); // init ipe
    m_document = new ipe::Document();

    // Load style from reference document
    load_style();

    // Create a new page
    m_page = new ipe::Page();

    // Add layers
    m_page->addLayer("axis");
    m_page->addLayer("data");

    // Create a view and set layers visibility
    m_page->insertView(0, "");
    m_page->setVisible(0, "axis", true);
    m_page->setVisible(0, "data", true);

    // Add the page to the document
    m_document->push_back(m_page);

    // Set the layout of the document
    set_layout();

    // Set the general style of the drawing (define standard size)
    style_size();
}

Figure::Figure(const std::string &filename, const ibex::IntervalVector &frame_data, const double width, const double height, const bool keep_ratio):
    m_frame_data(2)
{
    m_frame_data = frame_data;

    init_scale(width, height, keep_ratio);

    // Create new document
    ipe::Platform::initLib(ipe::IPELIB_VERSION); // init ipe
    int reason;
    m_document = ipe::Document::load(filename.c_str(),reason);
    m_page = m_document->page(0);
}

Figure::~Figure(){
    delete(m_document);
}

void Figure::init_scale(const double width, const double height, const bool keep_ratio)
{

    m_output_width = width*MM_TO_BP;
    m_output_height = height*MM_TO_BP;

    m_scale_x = m_output_width/m_frame_data[0].diam();
    m_scale_y = m_output_height/m_frame_data[1].diam();

    // (Not tested yet)
    if(keep_ratio)
    {
        m_scale_x = std::max(m_scale_x, m_scale_y);
        m_scale_y = m_scale_x;
        m_output_width = m_scale_x*m_frame_data[0].diam();
        m_output_height = m_scale_y*m_frame_data[1].diam();
    }

    m_offset_x = -m_frame_data[0].lb()*m_scale_x;
    m_offset_y = -m_frame_data[1].lb()*m_scale_y;

    m_offset_drawing_x = m_distance_axis_text+m_size_axis_graduation+5*m_arrow_size;
    m_offset_drawing_y = m_distance_axis_text+m_size_axis_graduation+5*m_arrow_size;
}

void Figure::style_size()
{
    // Add pen size and arrow size (in function of size of the layout ?
    double size_normal = m_thickness_pen_factor*std::max(m_layout.paper().height(),m_layout.paper().width());
    m_steel_sheet->add(ipe::EPen, ipe::Attribute(true, "normal"), ipe::Attribute(ipe::Fixed::fromDouble(size_normal)));
    m_steel_sheet->add(ipe::EPen, ipe::Attribute(true, "axis"), ipe::Attribute(ipe::Fixed::fromDouble(size_normal*1.5)));
    m_steel_sheet->add(ipe::EPen, ipe::Attribute(true, "axis_segment"), ipe::Attribute(ipe::Fixed::fromDouble(size_normal)));
    m_steel_sheet->add(ipe::EArrowSize, ipe::Attribute(true, "normal"), ipe::Attribute(ipe::Fixed::fromDouble(m_arrow_size)));
    m_steel_sheet->add(ipe::ETextSize, ipe::Attribute(true, "normal"), ipe::Attribute(true, "\\normalsize"));
}

void Figure::set_thickness_pen_factor(const double val)
{
    m_thickness_pen_factor = val;
    double size_normal = m_thickness_pen_factor*std::max(m_layout.paper().height(),m_layout.paper().width());
    m_steel_sheet->remove(ipe::EPen, ipe::Attribute(true, "normal"));
    m_steel_sheet->add(ipe::EPen, ipe::Attribute(true, "normal"), ipe::Attribute(ipe::Fixed::fromDouble(size_normal)));
}

void Figure::set_thickness_axis(const double val)
{
    double size_normal = val*std::max(m_layout.paper().height(),m_layout.paper().width());
    m_steel_sheet->remove(ipe::EPen, ipe::Attribute(true, "axis"));
    m_steel_sheet->remove(ipe::EPen, ipe::Attribute(true, "axis_segment"));
    m_steel_sheet->add(ipe::EPen, ipe::Attribute(true, "axis"), ipe::Attribute(ipe::Fixed::fromDouble(size_normal*1.5)));
    m_steel_sheet->add(ipe::EPen, ipe::Attribute(true, "axis_segment"), ipe::Attribute(ipe::Fixed::fromDouble(size_normal)));
}

void Figure::set_layout()
{
    m_layout.iPaperSize = ipe::Vector(m_output_width+3*m_offset_drawing_x,m_output_height+2*m_offset_drawing_y);
    m_layout.iFrameSize = ipe::Vector(m_output_width+3*m_offset_drawing_x,m_output_height+2*m_offset_drawing_y);
    m_layout.iOrigin = ipe::Vector(0, 0);
    m_layout.iCrop = true;
    m_steel_sheet->setLayout(m_layout);
}

void Figure::reset_scale(const double width, const double height, const bool keep_ratio){
    init_scale(width, height, keep_ratio);
    set_layout();
}

void Figure::load_style()
{
    int reason;
    ipe::Document *doc_ref = ipe::Document::load(m_ref_document.c_str(),reason);
    m_cascade_ref = new ipe::Cascade(*doc_ref->cascade());
    ipe::Cascade *old_cascade = m_document->cascade();
    m_document->replaceCascade(m_cascade_ref);
    delete(old_cascade);
    m_steel_sheet = m_document->cascade()->sheet(0);
    delete(doc_ref);
}

void Figure::save_pdf(const std::string &file_name)
{
    m_document->runLatex("~/.main.tex");
    m_document->save(file_name.c_str(),ipe::FileFormat::Pdf,ipe::SaveFlag::SaveNormal);

}
void Figure::save_ipe(const std::string &file_name)
{
//    m_document->runLatex("~/.main.tex");
    m_document->save(file_name.c_str(),ipe::FileFormat::Xml,ipe::SaveFlag::SaveNormal);
}

void Figure::draw_axis(const std::string &name_x, const std::string &name_y)
{
    ipe::Vector pt_x(s_t_x(m_frame_data[0].ub())+3*m_arrow_size, m_offset_drawing_y);
    ipe::Vector pt_y(m_offset_drawing_x, s_t_y(m_frame_data[1].ub())+3*m_arrow_size);
    draw_arrow_axis(ipe::Vector(m_offset_drawing_x, m_offset_drawing_y), pt_x);
    draw_arrow_axis(ipe::Vector(m_offset_drawing_x, m_offset_drawing_y), pt_y);

    double width;
    // Horizontal
    ipe::Vector offset_h(m_distance_axis_text, 0.0);
    ipe::AllAttributes attr_h;
    attr_h.iStroke = ipe::Attribute::BLACK();
    ipe::Text *text_h = new ipe::Text(attr_h, name_x.c_str(), pt_x+offset_h, ipe::Text::ELabel, width);
    text_h->setHorizontalAlignment(ipe::EAlignLeft); // for scale_number ?
    text_h->setVerticalAlignment(ipe::EAlignVCenter); // EAlignBaseline
    text_h->setSize(ipe::Attribute::NORMAL());
    text_h->setStyle(ipe::Attribute(true, "math"));
    m_page->append(ipe::TSelect::ENotSelected, 0, text_h);

    // Vertical
    ipe::Vector offset_v(0.0, m_distance_axis_text);
    ipe::AllAttributes attr_v;
    attr_v.iStroke = ipe::Attribute::BLACK();
    ipe::Text *text_v = new ipe::Text(attr_v, name_y.c_str(), pt_y+offset_v, ipe::Text::ELabel, width);
    text_v->setHorizontalAlignment(ipe::EAlignHCenter); // for scale_number ?
    text_v->setVerticalAlignment(ipe::EAlignBottom); // EAlignBaseline
    text_v->setSize(ipe::Attribute::NORMAL());
    text_v->setStyle(ipe::Attribute(true, "math"));
    m_page->append(ipe::TSelect::ENotSelected, 0, text_v);

    // Draw numbers
    draw_axis_numbers();
}

void Figure::draw_arrow_axis(const ipe::Vector &pt1, const ipe::Vector &pt2)
{
    ipe::Segment seg(pt1, pt2);
    ipe::AllAttributes attr;
    attr.iStroke = ipe::Attribute::BLACK();
    attr.iFArrow = true;
    attr.iFArrowSize = ipe::Attribute::NORMAL();
    attr.iFArrowShape = ipe::Attribute::ARROW_NORMAL();
    attr.iPen = m_steel_sheet->find(ipe::EPen, ipe::Attribute(true, "axis"));

    ipe::Shape shape(seg);
    ipe::Path *path = new ipe::Path(attr, shape, true);
    m_page->append(ipe::TSelect::ENotSelected, 0, path);
}

void Figure::draw_axis_number(const double number, const ipe::Vector& pos, const AXIS_SENS sens)
{
    ipe::Group *group = new ipe::Group();
    ipe::Vector offset_text, offset_graduation;

    if(sens==AXIS_VERTICAL)
    {
        offset_text = ipe::Vector(-(m_distance_number_graduation+m_distance_axis_text), 0.0);
        offset_graduation = ipe::Vector(-(m_distance_number_graduation), 0.0);
    }
    else
    {
        offset_text = ipe::Vector(0.0, -(m_distance_number_graduation+m_distance_axis_text));
        offset_graduation = ipe::Vector(0.0, -(m_distance_number_graduation));
    }

    // Text
    ipe::AllAttributes attr;
    attr.iStroke = ipe::Attribute::BLACK();
    std::stringstream number_string;
    number_string << number;
    double width;
    ipe::Text *text = new ipe::Text(attr, number_string.str().c_str(), pos+offset_text, ipe::Text::ELabel, width);
    if(sens==AXIS_VERTICAL)
    {
        text->setHorizontalAlignment(ipe::EAlignRight);
        text->setVerticalAlignment(ipe::EAlignVCenter);
    }
    else
    {
        text->setHorizontalAlignment(ipe::EAlignHCenter);
        text->setVerticalAlignment(ipe::EAlignTop);
    }
    text->setStyle(ipe::Attribute(true, "math"));

    // Segment
    ipe::Segment seg(pos, pos+offset_graduation);
    ipe::AllAttributes attr_seg;
    attr_seg.iStroke = ipe::Attribute::BLACK();
    attr.iPen = m_steel_sheet->find(ipe::EPen, ipe::Attribute(true, "axis_segment"));
    ipe::Shape shape(seg);
    ipe::Path *path = new ipe::Path(attr, shape, true);

    group->push_back(text);
    group->push_back(path);
    m_page->append(ipe::TSelect::ENotSelected, 0, group);
}

void Figure::draw_axis_numbers()
{
    for(double x=std::max(m_start_number_graduation_x,m_frame_data[0].lb()); x<=m_frame_data[0].ub(); x+=m_inter_graduation_x)
        draw_axis_number(x, ipe::Vector(s_t_x(x), m_offset_drawing_y), AXIS_HORIZONTAL);
    for(double y=std::max(m_start_number_graduation_y,m_frame_data[1].lb()); y<=m_frame_data[1].ub(); y+=m_inter_graduation_y)
        draw_axis_number(y, ipe::Vector(m_offset_drawing_x, s_t_y(y)), AXIS_VERTICAL);
}

void Figure::draw_arrow(const double x0, const double y0, const double x1, const double y1, const std::string &color_stroke, const std::string &layer_name)
{
    ipe::Segment seg(ipe::Vector(s_t_x(x0), s_t_y(y0)), ipe::Vector(s_t_x(x1), s_t_y(y1)));
    ipe::AllAttributes attr;
    if(color_stroke!="")
        attr.iStroke = m_steel_sheet->find(ipe::EColor,ipe::Attribute(true, color_stroke.c_str()));
    else
        attr.iStroke = ipe::Attribute::BLACK();
    attr.iFArrow = true;
    attr.iFArrowSize = ipe::Attribute::NORMAL();
    attr.iFArrowShape = ipe::Attribute::ARROW_NORMAL();

    ipe::Shape shape(seg);
    ipe::Path *path = new ipe::Path(attr, shape, true);
    m_page->append(ipe::TSelect::ENotSelected, m_page->findLayer(layer_name.c_str()), path);
}

void Figure::draw_text(const std::string& text, const double x, const double y, const std::string &color, const std::string& layer_name)
{
    ipe::AllAttributes attr_h;
    double width;
    if(color!="")
        attr_h.iStroke = m_steel_sheet->find(ipe::EColor,ipe::Attribute(true, color.c_str()));
    else
        attr_h.iStroke = ipe::Attribute::BLACK();
    ipe::Text *obj = new ipe::Text(attr_h, text.c_str(), ipe::Vector(s_t_x(x), s_t_y(y)), ipe::Text::ELabel, width);
    obj->setHorizontalAlignment(ipe::EAlignLeft);
    obj->setVerticalAlignment(ipe::EAlignBaseline);
    obj->setSize(ipe::Attribute::NORMAL());
    //    obj->setStyle(ipe::Attribute(true, "math"));
    m_page->append(ipe::TSelect::ENotSelected, m_page->findLayer(layer_name.c_str()), obj);

}

void Figure::draw_box(const ibex::IntervalVector& box, const std::string &color_stroke, const std::string &color_fill, const ipe::TPathMode fill_rule, const std::string& layer_name)
{
    ipe::Rect rec(ipe::Vector(s_t_x(box[0].lb()), s_t_y(box[1].lb())), ipe::Vector(s_t_x(box[0].ub()), s_t_y(box[1].ub())));
    ipe::AllAttributes attr;

    if(color_stroke!="")
        attr.iStroke = m_steel_sheet->find(ipe::EColor,ipe::Attribute(true, color_stroke.c_str()));
    else
        attr.iStroke = ipe::Attribute::BLACK();

    if(color_fill!="")
        attr.iFill = m_steel_sheet->find(ipe::EColor,ipe::Attribute(true, color_fill.c_str()));
    else
        attr.iFill = ipe::Attribute::WHITE();
    attr.iPathMode = fill_rule;

    ipe::Shape shape(rec);
    ipe::Path *path = new ipe::Path(attr, shape);
    m_page->append(ipe::TSelect::ENotSelected, m_page->findLayer(layer_name.c_str()), path);
}

void Figure::draw_curve(const std::vector<double>& x, const std::vector<double>& y, const std::string& color_stroke, const std::string& layer_name)
{
    draw_polygon(x, y, color_stroke, "", ipe::EStrokedOnly, layer_name, false);
}

void Figure::draw_polygon(const std::vector<double>& x, const std::vector<double>& y, const std::string& color_stroke, const std::string &color_fill, const ipe::TPathMode fill_rule,const std::string& layer_name, const bool closed){
    ipe::Curve *curve=new ipe::Curve();
    for(size_t i=0; i<x.size()-1; ++i)
        curve->appendSegment(ipe::Vector(s_t_x(x[i]), s_t_y(y[i])),ipe::Vector(s_t_x(x[i+1]), s_t_y(y[i+1])));
    ipe::AllAttributes attr;
    if(color_stroke!="")
        attr.iStroke = m_steel_sheet->find(ipe::EColor,ipe::Attribute(true, color_stroke.c_str()));
    else
        attr.iStroke = ipe::Attribute::BLACK();

    if(color_fill!="")
        attr.iFill = m_steel_sheet->find(ipe::EColor,ipe::Attribute(true, color_fill.c_str()));
    else
        attr.iFill = ipe::Attribute::WHITE();
    attr.iPathMode = fill_rule;
    curve->setClosed(closed);
    ipe::Shape shape;
    shape.appendSubPath(curve);
    ipe::Path *path = new ipe::Path(attr, shape);
    m_page->append(ipe::TSelect::ENotSelected, m_page->findLayer(layer_name.c_str()), path);
}

void Figure::draw_ellipse(const double x, const double y, const double r1, const double r2,  const std::string &color_stroke, const std::string &color_fill, const ipe::TPathMode fill_rule, const int opacity, const std::string& layer_name)
{
    ipe::Matrix m(ipe::Linear(s_t_x(r1), 0, 0, s_t_y(r2)), ipe::Vector(s_t_x(x), s_t_y(y)));
    ipe::Ellipse *ellipse = new ipe::Ellipse(m);
    ipe::AllAttributes attr;

    if(color_stroke!="")
        attr.iStroke = m_steel_sheet->find(ipe::EColor,ipe::Attribute(true, color_stroke.c_str()));
    else
        attr.iStroke = ipe::Attribute::BLACK();

    if(color_fill!="")
        attr.iFill = m_steel_sheet->find(ipe::EColor,ipe::Attribute(true, color_fill.c_str()));
    else
        attr.iFill = ipe::Attribute::WHITE();
    attr.iPathMode = fill_rule;
    std::string opacity_value = std::to_string(opacity)+"\%";
    attr.iOpacity = m_steel_sheet->find(ipe::EOpacity,ipe::Attribute(true, opacity_value.c_str()));

    ipe::Shape shape;
    shape.appendSubPath(ellipse);
    ipe::Path *path = new ipe::Path(attr, shape);
    m_page->append(ipe::TSelect::ENotSelected, m_page->findLayer(layer_name.c_str()), path);
}

void Figure::draw_circle(const double x, const double y, const double r,  const std::string &color_stroke, const std::string &color_fill, const ipe::TPathMode fill_rule, const int opacity, const std::string& layer_name)
{
    draw_ellipse(x, y, r, r,  color_stroke, color_fill, fill_rule, opacity, layer_name);
}

void Figure::draw_circle_radius_final(const double x, const double y, const double r,  const std::string &color_stroke, const std::string &color_fill, const ipe::TPathMode fill_rule, const int opacity, const std::string& layer_name)
{
    draw_ellipse(x, y, s_t_x_inv(r), s_t_y_inv(r),  color_stroke, color_fill, fill_rule, opacity, layer_name);
}

}
