#ifndef FIGURE_H
#define FIGURE_H

#include "ipelib.h"
#include "ibex_IntervalVector.h"

namespace ipegenerator {

#define MM_TO_BP 2.83467

class Figure
{
public:
    /**
     * @brief Figure
     * @param frame_data size of the frame of data
     * @param width in mm (default A4=210)
     * @param height in mm (default A4=297)
     */
    Figure(const ibex::IntervalVector &frame_data, const double width=210, const double height=297, const bool keep_ratio=false);

    /**
     * @brief Figure constructor with the load of an existing Figure
     * @param filename
     * @param frame_data
     * @param width
     * @param height
     * @param keep_ratio
     */
    Figure(const std::string &filename, const ibex::IntervalVector &frame_data, const double width, const double height, const bool keep_ratio);

    ~Figure();

    void save_pdf(const std::string &file_name);
    void save_ipe(const std::string &file_name);

    // Ipe functions
    void add_layer(const std::string &layer_name);

    // Drawing functions
    void draw_axis(const std::string &name_x, const std::string &name_y);
    void draw_arrow(const double x0, const double y0, const double x1, const double y1, const std::string &color_stroke="", const std::string &layer_name="data");
    void draw_text(const std::string &text, const double x, const double y, const std::string& color="", const std::string &layer_name="data");
    void draw_box(const ibex::IntervalVector &box, const std::string &color_stroke="", const std::string &color_fill="", const ipe::TPathMode fill_rule=ipe::EStrokedOnly, const std::string &layer_name="data");
    void draw_curve(const std::vector<double> &x, const std::vector<double> &y, const std::string &color_stroke="", const std::string &layer_name="data");
    void draw_ellipse(const double x, const double y, const double r1, const double r2,  const std::string &color_stroke="", const std::string &color_fill="", const ipe::TPathMode fill_rule=ipe::EStrokedOnly, const int opacity=100, const std::string& layer_name="data");
    void draw_circle(const double x, const double y, const double r,  const std::string &color_stroke="", const std::string &color_fill="", const ipe::TPathMode fill_rule=ipe::EStrokedOnly, const int opacity=100, const std::string& layer_name="data");
    void draw_circle_radius_final(const double x, const double y, const double r,  const std::string &color_stroke="", const std::string &color_fill="", const ipe::TPathMode fill_rule=ipe::EStrokedOnly, const int opacity=100, const std::string& layer_name="data");

    // Style functions
    void set_thickness_pen_factor(const double val);
    void set_distance_axis_text(const double val);
    void set_arrow_size(const double val);

    void set_distance_number_graduation(const double distance_number_graduation);
    void set_size_axis_graduation(const double size_axis_graduation);
    void set_graduation_parameters(const double start_x, const double inter_x, const double start_y, const double inter_y);

    void reset_scale(const double width, const double height, const bool keep_ratio);

private:
    void load_style();
    void set_layout();
    void style_size();
    void init_scale(const double width, const double height, const bool keep_ratio);

    void draw_arrow_axis(const ipe::Vector &pt1, const ipe::Vector &pt2);

    enum AXIS_SENS{AXIS_VERTICAL,AXIS_HORIZONTAL};
    void draw_axis_number(const double number, const ipe::Vector &pos, const AXIS_SENS sens);
    void draw_axis_numbers();


    // scale and translate in x axis
    double s_t_x(const double &val);
    double s_t_x_inv(const double &val);

    // scale and translate in x axis
    double s_t_y(const double &val);
    double s_t_y_inv(const double &val);

private:

    // Parameters
    ibex::IntervalVector m_frame_data;
    double m_output_width, m_output_height; // in bp
    double m_scale_x=1.0, m_scale_y=1.0; // Scale factor
    double m_offset_x, m_offset_y; // Adding offset to (0,0)
    double m_offset_drawing_x, m_offset_drawing_y;

    // Ipe objects
    ipe::Document   * m_document;
    ipe::Cascade    * m_cascade_ref;
    ipe::Page       * m_page;
    ipe::Layout     m_layout;
    ipe::StyleSheet * m_steel_sheet;

    // Ipe parameters
    std::string m_ref_document = "/usr/local/etc/ipegenerator/basic.ipe";
    double m_thickness_pen_factor = 1e-3; // thickness of pen
    double m_arrow_size = 4.294; // Corresponds to /normalsize in latex
    double m_distance_axis_text = 3.0;
    double m_distance_number_graduation = 2.0;
    double m_size_axis_graduation = 3.0;
    double m_general_offset = 0.0;

    double m_start_number_graduation_x = 0.0;
    double m_start_number_graduation_y = 0.0;
    double m_inter_graduation_x = 1.0;
    double m_inter_graduation_y = 1.0;

    // Latex
    bool m_contain_latex = false;
};

inline void Figure::add_layer(const std::string &layer_name)
{
    m_page->addLayer(layer_name.c_str());
}

inline double Figure::s_t_x(const double &val)
{
    return val*m_scale_x+m_offset_x+m_offset_drawing_x;
}

inline double Figure::s_t_y(const double &val)
{
    return val*m_scale_y+m_offset_y+m_offset_drawing_y;
}

inline double Figure::s_t_x_inv(const double &val)
{
    return (val-m_offset_x-m_offset_drawing_x)/m_scale_x;
}

inline double Figure::s_t_y_inv(const double &val)
{
    return (val-m_offset_y-m_offset_drawing_y)/m_scale_y;
}

inline void Figure::set_thickness_pen_factor(const double val)
{
    m_thickness_pen_factor = val;
}

inline void Figure::set_distance_axis_text(const double val)
{
    m_distance_axis_text = val;
}

inline void Figure::set_arrow_size(const double val)
{
    m_arrow_size = val;
}

inline void Figure::set_distance_number_graduation(const double distance_number_graduation)
{
    m_distance_number_graduation = distance_number_graduation;
}

inline void Figure::set_size_axis_graduation(const double size_axis_graduation)
{
    m_size_axis_graduation = size_axis_graduation;
}

inline void Figure::set_graduation_parameters(const double start_x, const double inter_x, const double start_y, const double inter_y)
{
    m_start_number_graduation_x = start_x;
    m_inter_graduation_x = inter_x;
    m_start_number_graduation_y = start_y;
    m_inter_graduation_y = inter_y;
}

}
#endif // FIGURE_H
