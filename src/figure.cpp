#include "figure.h"
#include <math.h>
#include <sstream>
#include <iomanip>

namespace ipegenerator{

Figure::Figure(const ibex::IntervalVector &frame_data, const double &width, const double &height, const bool &keep_ratio):
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
    m_page->addLayer("outer");
    m_page->addLayer("inner");
    m_page->addLayer("uncertain");

    // Create a view and set layers visibility
    m_page->insertView(0, "");
    m_page->setVisible(0, "axis", true);
    m_page->setVisible(0, "data", true);
    m_page->setVisible(0, "outer", true);
    m_page->setVisible(0, "inner", true);
    m_page->setVisible(0, "uncertain", true);

    // Add the page to the document
    m_document->push_back(m_page);

    // Set the layout of the document
    set_layout();

    // Set the general style of the drawing (define standard size)
    style_size();
}

Figure::Figure(const std::string &filename, const ibex::IntervalVector &frame_data, const double &width, const double &height, const bool &keep_ratio):
    m_frame_data(2)
{
    m_frame_data = frame_data;

    init_scale(width, height, keep_ratio);

    // Create new document
    ipe::Platform::initLib(ipe::IPELIB_VERSION); // init ipe
    int reason;
    m_document = ipe::Document::load(filename.c_str(),reason);
    m_page = m_document->page(0);
    m_steel_sheet = m_document->cascade()->sheet(0);
}

Figure::~Figure(){
    delete(m_document);
}

void Figure::init_scale(const double &width, const double &height, const bool &keep_ratio)
{
    m_width = width;
    m_height = height;
    m_keep_ratio = keep_ratio;

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

    if(m_scale_offset)
    {
        m_offset_drawing_x = m_distance_axis_text+m_size_axis_graduation+5*m_arrow_axis_size;
        m_offset_drawing_y = m_distance_axis_text+m_size_axis_graduation+5*m_arrow_axis_size;
    }
    else
    {
        m_offset_drawing_x = 0.0;
        m_offset_drawing_y = 0.0;
    }


    m_transform_global = ipe::Matrix(ipe::Linear(m_scale_x, 0.0, 0.0, m_scale_y), ipe::Vector(m_offset_x+m_offset_drawing_x, m_offset_y+m_offset_drawing_y));
    m_transform_global_keep_dimension = ipe::Matrix(ipe::Linear(std::max(m_scale_x,m_scale_y), 0.0, 0.0, std::max(m_scale_x,m_scale_y)),m_transform_global.translation());
    m_transform_global_keep_y = m_transform_global;
    m_transform_global_keep_dimension_keep_y = m_transform_global_keep_dimension;

    if(m_inversion_y)
    {
        m_transform_global = ipe::Matrix(ipe::Linear(m_scale_x, 0.0, 0.0, -m_scale_y), ipe::Vector(m_offset_x+m_offset_drawing_x, m_output_height-m_offset_y+m_offset_drawing_y));
        m_transform_global_keep_dimension = ipe::Matrix(ipe::Linear(std::max(m_scale_x,m_scale_y), 0.0, 0.0, std::max(m_scale_x,m_scale_y)),m_transform_global.translation());
    }
}

void Figure::style_size()
{
    // Add pen size and arrow size (in function of size of the layout ?
    double size_normal = m_thickness_pen_factor*std::max(m_layout.paper().height(),m_layout.paper().width());
    m_steel_sheet->add(ipe::EPen, ipe::Attribute(true, "normal"), ipe::Attribute(ipe::Fixed::fromDouble(size_normal)));
    m_steel_sheet->add(ipe::EPen, ipe::Attribute(true, "axis"), ipe::Attribute(ipe::Fixed::fromDouble(size_normal*1.5)));
    m_steel_sheet->add(ipe::EPen, ipe::Attribute(true, "axis_segment"), ipe::Attribute(ipe::Fixed::fromDouble(size_normal)));
    m_steel_sheet->add(ipe::EArrowSize, ipe::Attribute(true, "normal"), ipe::Attribute(ipe::Fixed::fromDouble(m_arrow_axis_size)));
    m_steel_sheet->add(ipe::ETextSize, ipe::Attribute(true, "normal"), ipe::Attribute(true, "\\normalsize"));
}

void Figure::set_thickness_pen_factor(const double &val)
{
    m_thickness_pen_factor = val;
    double size_normal = m_thickness_pen_factor*std::max(m_layout.paper().height(),m_layout.paper().width());
    m_steel_sheet->remove(ipe::EPen, ipe::Attribute(true, "normal"));
    m_steel_sheet->add(ipe::EPen, ipe::Attribute(true, "normal"), ipe::Attribute(ipe::Fixed::fromDouble(size_normal)));
}

void Figure::set_thickness_axis(const double &val)
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

void Figure::reset_scale(const double &width, const double &height, const bool &keep_ratio){
    init_scale(width, height, keep_ratio);
    set_layout();
}

void Figure::load_style()
{
    int reason;
    ipe::Document *doc_ref = ipe::Document::load(m_ref_document.c_str(),reason);

    if(doc_ref!=nullptr)
    {
        m_cascade_ref = new ipe::Cascade(*doc_ref->cascade());
        ipe::Cascade *old_cascade = m_document->cascade();
        m_document->replaceCascade(m_cascade_ref);
        delete(old_cascade);
    }
    else
    {
        std::cout << "Error while opening the style sheet " << reason << " " << m_ref_document << std::endl;
    }
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

void Figure::draw_axis(const std::string &name_x, const std::string &name_y, const bool &enable_numbers)
{
    ipe::Vector pt_origin = m_transform_global*ipe::Vector(m_frame_data[0].lb(), m_frame_data[1].lb());
    ipe::Vector pt_x = m_transform_global*ipe::Vector(m_frame_data[0].ub(), m_frame_data[1].lb())+ipe::Vector(3*m_arrow_axis_size,0.);
    ipe::Vector pt_y = m_transform_global*ipe::Vector(m_frame_data[0].lb(), m_frame_data[1].ub())+ipe::Vector(0.,3*m_arrow_axis_size*(m_inversion_y?-1.:1.));

    draw_arrow_axis(pt_origin, pt_x); // X
    draw_arrow_axis(pt_origin, pt_y); // Y

    double width = 0.0;
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
    ipe::Vector offset_v(0.0, m_distance_axis_text*(m_inversion_y?-1.:1.));
    ipe::AllAttributes attr_v;
    attr_v.iStroke = ipe::Attribute::BLACK();
    ipe::Text *text_v = new ipe::Text(attr_v, name_y.c_str(), pt_y+offset_v, ipe::Text::ELabel, width);
    text_v->setHorizontalAlignment(ipe::EAlignHCenter); // for scale_number ?
    text_v->setVerticalAlignment((m_inversion_y)?(ipe::EAlignTop):(ipe::EAlignBottom)); // EAlignBaseline
    text_v->setSize(ipe::Attribute::NORMAL());
    text_v->setStyle(ipe::Attribute(true, "math"));
    m_page->append(ipe::TSelect::ENotSelected, 0, text_v);

    // Draw numbers
    if(enable_numbers)
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

void Figure::draw_axis_number(const double &number, const ipe::Vector& pos, const AXIS_SENS &sens)
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
        offset_text = ipe::Vector(0.0, -(m_distance_number_graduation+m_distance_axis_text)*(m_inversion_y?-1.:1.));
        offset_graduation = ipe::Vector(0.0, -(m_distance_number_graduation)*(m_inversion_y?-1.:1.));
    }

    // Text
    ipe::AllAttributes attr;
    attr.iStroke = ipe::Attribute::BLACK();
    std::stringstream number_string;
    std::fesetround(FE_TONEAREST);
    number_string << std::fixed << std::setprecision((sens==AXIS_HORIZONTAL)?m_number_digits_axis_x:m_number_digits_axis_y) << number;
    double width = 0.0;
    ipe::Text *text = new ipe::Text(attr, number_string.str().c_str(), pos+offset_text, ipe::Text::ELabel, width);
    if(sens==AXIS_VERTICAL)
    {
        text->setHorizontalAlignment(ipe::EAlignRight);
        text->setVerticalAlignment(ipe::EAlignVCenter);
    }
    else
    {
        text->setHorizontalAlignment(ipe::EAlignHCenter);
        text->setVerticalAlignment((m_inversion_y)?(ipe::EAlignBottom):(ipe::EAlignTop));
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
    {
        ipe::Vector pt = m_transform_global*ipe::Vector(x,m_frame_data[1].lb());
        draw_axis_number(x, pt, AXIS_HORIZONTAL);
    }
    for(double y=std::max(m_start_number_graduation_y,m_frame_data[1].lb()); y<=m_frame_data[1].ub(); y+=m_inter_graduation_y)
    {
        ipe::Vector pt = m_transform_global*ipe::Vector(m_frame_data[0].lb(),y);
        draw_axis_number(y, pt, AXIS_VERTICAL);
    }
}

size_t Figure::draw_arrow(const ipe::Vector& v1, const ipe::Vector& v2)
{
    ipe::Segment seg(m_transform_global*v1, m_transform_global*v2);
    ipe::AllAttributes attr(m_current_attr);
    attr.iFArrow = true;
    //    attr.iFArrowSize = ipe::Attribute::NORMAL();
    attr.iFArrowShape = ipe::Attribute::ARROW_NORMAL();

    ipe::Shape shape(seg);
    ipe::Path *path = new ipe::Path(attr, shape, true);
    m_page->append(ipe::TSelect::ENotSelected, m_current_layer, path);
    return m_page->count()-1;
}

size_t Figure::draw_arrow(const double &x0, const double &y0, const double &x1, const double &y1)
{
    return draw_arrow(ipe::Vector(x0, y0), ipe::Vector(x1, y1));
}

size_t Figure::draw_text(const std::string& text, const double &x, const double &y, const bool &math_mode, const ipe::THorizontalAlignment &horizontal_align)
{
    double width = 0.0;
    ipe::Text *obj = new ipe::Text(m_current_attr, text.c_str(), m_transform_global*ipe::Vector(x, y), ipe::Text::ELabel, width);
    obj->setHorizontalAlignment(horizontal_align);
    obj->setVerticalAlignment(ipe::EAlignBaseline);
    obj->setSize(ipe::Attribute::NORMAL());
    if(math_mode)
        obj->setStyle(ipe::Attribute(true, "math"));
    m_page->append(ipe::TSelect::ENotSelected, m_current_layer, obj);
    return m_page->count()-1;
}

size_t Figure::draw_box(const ibex::IntervalVector& box)
{
    ipe::Rect rec(m_transform_global*ipe::Vector(box[0].lb(), box[1].lb()), m_transform_global*ipe::Vector(box[0].ub(), box[1].ub()));

    ipe::Shape shape(rec);
    ipe::Path *path = new ipe::Path(m_current_attr, shape);
    m_page->append(ipe::TSelect::ENotSelected, m_current_layer, path);
    return m_page->count()-1;
}

size_t Figure::draw_box(const ipe::Rect& box)
{
    ipe::Rect rec(m_transform_global*box.bottomLeft(), m_transform_global*box.topRight());
    ipe::Shape shape(rec);
    ipe::Path *path = new ipe::Path(m_current_attr, shape);
    m_page->append(ipe::TSelect::ENotSelected, m_current_layer, path);
    return m_page->count()-1;
}

size_t Figure::draw_box(const ipe::Vector &center, const double &width, const bool &keep_ratio)
{
    ipe::Vector offset(width/2.0, width/2.0);
    ipe::Rect rec;
    if(keep_ratio)
    {
        ipe::Matrix zoom(m_transform_global_keep_dimension.linear(), ipe::Vector(0.0, 0.0));
        rec = ipe::Rect(m_transform_global*center-zoom*offset, m_transform_global*center+zoom*offset);
    }
    else
        rec = ipe::Rect(m_transform_global*(center-offset), m_transform_global*(center+offset));
    ipe::Shape shape(rec);
    ipe::Path *path = new ipe::Path(m_current_attr, shape);
    m_page->append(ipe::TSelect::ENotSelected, m_current_layer, path);
    return m_page->count()-1;
}

size_t Figure::draw_curve(const std::vector<double>& x, const std::vector<double>& y)
{
    return draw_polygon(x, y, false);
}

size_t Figure::draw_segment(const double &x0, const double &y0, const double &x1, const double &y1){
    return draw_curve(std::vector<double>{x0, x1}, std::vector<double>{y0, y1});
}

size_t Figure::draw_polygon(const std::vector<double>& x, const std::vector<double>& y,const bool &closed){
    ipe::Curve *curve=new ipe::Curve();
    if(x.size()==0)
        return 0;
    else if(x.size()==1)
        curve->appendSegment(m_transform_global*ipe::Vector(x[0], y[0]), m_transform_global*ipe::Vector(x[0], y[0]));
    else
    {
        for(size_t i=0; i<x.size()-1; ++i)
            curve->appendSegment(m_transform_global*ipe::Vector(x[i], y[i]),m_transform_global*ipe::Vector(x[i+1], y[i+1]));
    }

    curve->setClosed(closed);
    ipe::Shape shape;
    shape.appendSubPath(curve);
    ipe::Path *path = new ipe::Path(m_current_attr, shape);
    m_page->append(ipe::TSelect::ENotSelected,m_current_layer, path);
    return m_page->count()-1;
}

size_t Figure::draw_ellipse(const double &x, const double &y, const double &r1, const double &r2)
{
    ipe::Matrix m = m_transform_global*ipe::Matrix(ipe::Linear(r1, 0, 0, r2), ipe::Vector(x, y));
    ipe::Ellipse *ellipse = new ipe::Ellipse(m);

    ipe::Shape shape;
    shape.appendSubPath(ellipse);
    ipe::Path *path = new ipe::Path(m_current_attr, shape);
    m_page->append(ipe::TSelect::ENotSelected, m_current_layer, path);
    return m_page->count()-1;
}

size_t Figure::draw_circle(const double &x, const double &y, const double &r)
{
    return draw_ellipse(x, y, r, r);
}

size_t Figure::draw_circle_radius_final(const double &x, const double &y, const double &r)
{
    ipe::Vector r_inv = m_transform_global.linear().inverse()*ipe::Vector(r,r);
    return draw_ellipse(x, y, r_inv.x, r_inv.y);
}

size_t Figure::draw_sector(const double& x, const double& y, const double& r1, const double& r2, const double& alpha_start, const double& alpha_end){
    ipe::Curve *curve=new ipe::Curve();

    ipe::Matrix m = m_transform_global*ipe::Matrix(ipe::Linear(r1, 0, 0, r2), ipe::Vector(x, y));
    ipe::Arc arc(m, ipe::Angle(alpha_start), ipe::Angle(alpha_end));
    curve->appendSegment(m.translation(), arc.beginp());
    curve->appendArc(m, arc.beginp(), arc.endp());
    curve->setClosed(true);

    ipe::Shape shape;
    shape.appendSubPath(curve);
    ipe::Path *path = new ipe::Path(m_current_attr, shape);
    m_page->append(ipe::TSelect::ENotSelected, m_current_layer, path);
    return m_page->count()-1;
}

size_t Figure::draw_symbol(const double& x, const double& y, const std::string &name, const double& size){
    ipe::Reference *ref = new ipe::Reference(m_current_attr,ipe::Attribute(true, ("mark/"+ name).c_str()),m_transform_global*ipe::Vector(x, y));
    ref->setSize(ipe::Attribute(ipe::Fixed::fromDouble(size)));
    m_page->append(ipe::TSelect::ENotSelected, m_current_layer, ref);
    return m_page->count()-1;
}

void Figure::set_color_stroke(const std::string &color_stroke)
{
    if(color_stroke!="")
        m_current_attr.iStroke = m_steel_sheet->find(ipe::EColor,ipe::Attribute(true, color_stroke.c_str()));
    else
        m_current_attr.iStroke = ipe::Attribute::BLACK();
}

void Figure::set_color_fill(const std::string &color_fill)
{
    m_current_attr.iFill = m_steel_sheet->find(ipe::EColor,ipe::Attribute(true, color_fill.c_str()));
}

void Figure::set_color_type(const PATH_TYPE& type)
{
    m_current_attr.iPathMode = (ipe::TPathMode)type; // To be checked
}

void Figure::set_opacity(const int &opacity){
    if(opacity != 100)
    {
        std::string opacity_value = std::to_string(opacity)+"\%";
        m_current_attr.iOpacity = ipe::Attribute(true, opacity_value.c_str());
    }
    else
    {
        m_current_attr.iOpacity = ipe::Attribute::OPAQUE();
    }
}

void Figure::set_current_layer(const std::string &layer_name)
{
    m_current_layer = m_page->findLayer(layer_name.c_str());
}

void Figure::set_dashed(const std::string &dashed){
    if(dashed=="")
    {
        m_current_attr.iDashStyle = ipe::Attribute::NORMAL();
    }
    else
    {
        m_current_attr.iDashStyle = m_steel_sheet->find(ipe::EDashStyle, ipe::Attribute(true, dashed.c_str()));
    }
}

void Figure::remove_object(const int &id)
{
    m_page->remove(id);
}

void Figure::set_line_width(const double &val)
{
    m_current_attr.iPen = ipe::Attribute(ipe::Fixed::fromDouble(val));
}

void Figure::set_arrow_size(const double &val){
    m_current_attr.iFArrowSize = ipe::Attribute(ipe::Fixed::fromDouble(val));
}

void Figure::reset_attribute()
{
    m_current_attr = ipe::AllAttributes();
}

void Figure::set_inverted_y()
{
    m_inversion_y = true;
    init_scale(m_width, m_height, m_keep_ratio);
}

}
