#ifndef FIGURE_H
#define FIGURE_H

#include "../ipe/ipelib.h"
#include "ibex_IntervalVector.h"
#include "codac_TubeVector.h"
#include "codac_Figure.h"
#include "codac_ColorMap.h"

#define TUBE_MAX_NB_DISPLAYED_SLICES 2000
#define IPE_BOUNDED_INFINITY 99999.

namespace ipegenerator {

#define MM_TO_BP 2.83467
enum FLOAT_PISTON_MVT{FLOAT_PISTON_EQUAL,FLOAT_PISTON_DOWN,FLOAT_PISTON_UP};
enum PATH_TYPE{STROKE_ONLY,STROKE_AND_FILL,FILL_ONLY};

class Figure
{
public:
    /**
     * @brief Figure
     * @param frame_data size of the frame of data
     * @param width in mm (default A4=210)
     * @param height in mm (default A4=297)
     */
    Figure(const ibex::IntervalVector &frame_data, const double &width=210, const double &height=297, const bool &keep_ratio=false);

    /**
     * @brief Figure constructor with the load of an existing Figure
     * @param filename
     * @param frame_data
     * @param width
     * @param height
     * @param keep_ratio
     */
    Figure(const std::string &filename, const ibex::IntervalVector &frame_data, const double &width=210, const double &height=297, const bool &keep_ratio=false);

    ~Figure();

    void save_pdf(const std::string &file_name);
    void save_ipe(const std::string &file_name);

    // Ipe functions
    void add_layer(const std::string &layer_name);
    void set_visible(const std::string &layer_name, bool visible=true);

    // Drawing functions
    void draw_axis(const std::string &name_x, const std::string &name_y, const bool& enable_numbers=true);
    size_t draw_arrow(const double &x0, const double &y0, const double &x1, const double &y1);
    size_t draw_arrow(const ipe::Vector &v1, const ipe::Vector &v2);
    size_t draw_text(const std::string &text, const double &x, const double &y, const bool& math_mode=false, const ipe::THorizontalAlignment& horizontal_align=ipe::EAlignHCenter);

    /**
    * \brief Draws a box on the figure. The drawing style (color, dashing, opacity...) should be set
    * before calling this function
    *
    * \param box the box to be drawn
    * \return the index of the figure on the page
    */
    size_t draw_box(const ibex::IntervalVector &box); // Painting style has to be defined before drawing
    /**
    * \brief Draws a box on the figure. The drawing style (color, dashing, opacity...) should be set
    * before calling this function
    *
    * \param box the box to be drawn
    * \param color_stroke color the edges of the box
    * \param color_fill color of the  inside of the box
    * \return the index of the figure on the page
    */
    size_t draw_box(const ibex::IntervalVector &box, const std::string& color_stroke, const std::string& color_fill,
                    const PATH_TYPE &type=STROKE_AND_FILL);
    /**
    * \brief Draws a box on the figure. The drawing style (color, dashing, opacity...) should be set
    * before calling this function
    *
    * \param box the box to be drawn
    * \param color_stroke color the edges of the box
    * \param color_fill color of the  inside of the box
    * \return the index of the figure on the page
    */
    size_t draw_box(const ibex::IntervalVector &box, const ipe::Color& color_stroke, const ipe::Color& color_fill,
                    const PATH_TYPE &type=STROKE_AND_FILL);

    size_t draw_box(const ipe::Rect& box);
    size_t draw_box(const ipe::Vector &center, const double &width, const bool& keep_ratio=false);
    size_t draw_curve(const std::vector<double> &x, const std::vector<double> &y);
    size_t draw_segment(const double &x0, const double &y0, const double &x1, const double &y1);
    size_t draw_polygon(const std::vector<double>& x, const std::vector<double>& y, const bool& closed=true);
    size_t draw_polygon(const std::vector<double>& x, const std::vector<double>& y, const string& color_stroke, const string& color_fill,
                        const PATH_TYPE &type=STROKE_AND_FILL, const bool &closed=true);
    size_t draw_polygon(const std::vector<double>& x, const std::vector<double>& y, const ipe::Color& color_stroke, const ipe::Color& color_fill,
                        const PATH_TYPE &type=STROKE_AND_FILL, const bool &closed=true);
    size_t draw_ellipse(const double& x, const double& y, const double& r1, const double& r2);
    size_t draw_circle(const double &x, const double &y, const double &r);
    size_t draw_circle_radius_final(const double &x, const double &y, const double &r);
    size_t draw_symbol(const double& x, const double& y, const std::string &name, const double& size=1.0);

    size_t draw_sector(const double &x, const double &y, const double &r1, const double &r2, const double &alpha_start, const double& alpha_end);

    size_t draw_float(const double &x, const double &y, const double &piston, const double &compressibility, const FLOAT_PISTON_MVT &mvt=FLOAT_PISTON_EQUAL, const double &zoom=0.1);

    // Tube drawings
    /**
    * \brief Draws a single slice of a tube on the figure. The drawing style (color, dashing, opacity...) should be set
    * before calling this function
    *
    * \param slice the slice to be drawn
    */
    void draw_slice(const codac::Slice& slice);

    /**
    * \brief Draws a single slice of a tube on the figure. The drawing style (dashing, opacity...) should be set
    * before calling this function
    *
    * \param slice the slice to be drawn
    * \param color_stroke color the envelope of slices
    * \param color_fill color of the  inside of slices
    * \param type type of pen to use
    */
    void draw_slice(const codac::Slice& slice, const std::string& color_stroke, const std::string& color_fill,
                    const PATH_TYPE &type=STROKE_AND_FILL);

    /**
    * \brief Draws a single slice of a tube on the figure. The drawing style (dashing, opacity...) should be set
    * before calling this function
    *
    * \param slice the slice to be drawn
    * \param color_stroke color the envelope of slices
    * \param color_fill color of the  inside of slices
    * \param type type of pen to use
    */
    void draw_slice(const codac::Slice& slice, const ipe::Color& color_stroke, const ipe::Color& color_fill,
                    const PATH_TYPE &type=STROKE_AND_FILL);

    /**
    * \brief Draws a single gate of a tube on the figure. The drawing style (color, dashing, opacity...) should be set
    * before calling this function
    *
    * \param slice the slice to be drawn
    */
    void draw_gate(const ibex::Interval& gate, double t);

    /**
    * \brief Draws a single gate of a tube on the figure. The drawing style (dashing, opacity...) should be set
    * before calling this function
    *
    * \param slice the slice to be drawn
    * \param color_stroke color the envelope of slices
    * \param color_fill color of the  inside of slices
    * \param type type of pen to use
    */
    void draw_gate(const ibex::Interval& gate, double t,const string& color_stroke, const string& color_fill,
                   const PATH_TYPE &type=STROKE_AND_FILL);

    /**
    * \brief Draws a single gate of a tube on the figure. The drawing style (dashing, opacity...) should be set
    * before calling this function
    *
    * \param slice the slice to be drawn
    * \param color_stroke color the envelope of slices
    * \param color_fill color of the  inside of slices
    * \param type type of pen to use
    */
    void draw_gate(const ibex::Interval& gate, double t,const ipe::Color& color_stroke, const ipe::Color& color_fill,
                   const PATH_TYPE &type=STROKE_AND_FILL);

    void draw_codac_polygon(const codac::Polygon& p);
    void draw_codac_polygon(const codac::Polygon& p,const string& color_stroke, const string& color_fill, const PATH_TYPE& type=STROKE_AND_FILL);
    void draw_codac_polygon(const codac::Polygon& p,const ipe::Color& color_stroke, const ipe::Color& color_fill, const PATH_TYPE& type=STROKE_AND_FILL);


    /**
    * \brief Draws a tube on the figure. The drawing style (color, dashing, opacity...) should be set
    * before calling this function
    *
    * \param tube the tube to be drawn
    */
    void draw_tube(const codac::Tube *tube);

    /**
    * \brief Draws a tube on the figure. The drawing style (dashing, opacity...) should be set
    * before calling this function
    *
    * \param tube the tube to be drawn
    * \param color_stroke color the envelope of slices
    * \param color_fill color of the  inside of slices
    */
    void draw_tube(const codac::Tube *tube, const string& color_stroke, const string& color_fill,
                   const PATH_TYPE &type=STROKE_AND_FILL);

    /**
    * \brief Draws a tube on the figure. The drawing style (dashing, opacity...) should be set
    * before calling this function
    *
    * \param tube the tube to be drawn
    * \param color_stroke color the envelope of slices
    * \param color_fill color of the  inside of slices
    * \param type type of pen to use
    */
    void draw_tube(const codac::Tube *tube, const ipe::Color& color_stroke, const ipe::Color& color_fill,
                   const PATH_TYPE &type=STROKE_AND_FILL);

    /**
    * \brief Draws a tube on the figure. The drawing style (dashing, opacity...) should be set
    * before calling this function
    *
    * http://codac.io/manual/07-graphics/04-colormaps.html?highlight=color%20map#color-maps
    *
    * \param tube the tube to be drawn
    * \param color_map custom color map for tube drawing
    * \param traj_colorMap custom color trajectory (see codac library documentation for more info http://codac.io/manual/07-graphics/04-colormaps.html?highlight=color%20map#color-maps)
    * \param type type of pen to use
    */
    void draw_tube(const codac::Tube *tube, const codac::ColorMap* color_map, const codac::Trajectory* traj_colorMap=NULL,
                   const PATH_TYPE &type=STROKE_AND_FILL);

    /**
     * \brief Draws a 2D projection of a TubeVector. The drawing style (color, dashing, opacity...) should be set
     * before calling this function
     *
     * \param tube_v the tubeVector to be drawn
     * \param index_x the vector dimension to be put as abscissa
     * \param index_y the vector dimension to be put as ordinate
     * \param from_first_to_last boolean to say in which order the slices are drawn
     * \param smooth_drawing use polygons for drawing (TO DO)
     */
    void draw_tubeVector(const codac::TubeVector *tube_v, const int index_x, const int index_y,
                         const bool from_first_to_last=false, const bool smooth_drawing=false);

    /**
    * \brief Draws a 2D projection of a TubeVector. The drawing style (dashing, opacity...) should be set
    * before calling this function
    *
    * \param tube_v the tubeVector to be drawn
    * \param index_x the vector dimension to be put as abscissa
    * \param index_y the vector dimension to be put as ordinate
    * \param color_stroke color the envelope of slices
    * \param color_fill color of the  inside of slices
    * \param type type of pen to use
    * \param from_first_to_last boolean to say in which order the slices are drawn
    * \param smooth_drawing use polygons for drawing (TO DO)
     */
    void draw_tubeVector(const codac::TubeVector *tube_v, const int index_x, const int index_y, const string& color_stroke,
                         const string& color_fill,const PATH_TYPE& type = STROKE_AND_FILL,
                         const bool from_first_to_last= false, const bool smooth_drawing=false);

    /**
    * \brief Draws a 2D projection of a TubeVector. The drawing style (dashing, opacity...) should be set
    * before calling this function
    *
    * \param tube_v the tubeVector to be drawn
    * \param index_x the vector dimension to be put as abscissa
    * \param index_y the vector dimension to be put as ordinate
    * \param color_stroke color the envelope of slices
    * \param color_fill color of the  inside of slices
    * \param type type of pen to use
    * \param from_first_to_last boolean to say in which order the slices are drawn
    * \param smooth_drawing use polygons for drawing (TO DO)
    */
    void draw_tubeVector(const codac::TubeVector *tube_v, const int index_x, const int index_y, const ipe::Color& color_stroke,
                         ipe::Color& color_fill, const PATH_TYPE& type = STROKE_AND_FILL,
                         const bool from_first_to_last=false, const bool smooth_drawing=false);

    /**
    * \brief Draws a 2D projection of a TubeVector. The drawing style (dashing, opacity...) should be set
    * before calling this function
    *
    * \param tube_v the tubeVector to be drawn
    * \param index_x the vector dimension to be put as abscissa
    * \param index_y the vector dimension to be put as ordinate
    * \param color_map custom color map
    * \param type type of pen to use
    * \param from_first_to_last boolean to say in which order the slices are drawn
    * \param smooth_drawing use polygons for drawing (TO DO)
    */
    void draw_tubeVector(const codac::TubeVector *tube_v, const int index_x, const int index_y, const codac::ColorMap* color_map,
                         const codac::Trajectory* traj_coloMap=NULL, const PATH_TYPE& type = STROKE_AND_FILL,
                         const bool from_first_to_last=false, const bool smooth_drawing=false);

    // Style functions
    void set_thickness_pen_factor(const double &val=1e-3);
    void set_thickness_axis(const double &val=1e-3);
    void set_distance_axis_text(const double &val);
    void set_arrow_axis_size(const double &val);

    void set_distance_number_graduation(const double &distance_number_graduation);
    void set_size_axis_graduation(const double &size_axis_graduation);
    void set_graduation_parameters(const double &start_x, const double &inter_x, const double &start_y, const double &inter_y);

    void reset_scale(const double &width, const double &height, const bool &keep_ratio=false);
    void set_scale_offset(const bool &enable);
    void set_number_digits_axis_x(const size_t &val);
    void set_number_digits_axis_y(const size_t &val);
    void set_inverted_y();

    void set_color_stroke(const std::string &color_stroke="");
    void set_color_stroke(const int r=0, const int g=0, const int b=0);
    void set_color_stroke(const ipe::Color& color);
    void set_color_fill(const std::string &color_fill="");
    void set_color_fill(const int r=0, const int g=0, const int b=0);
    void set_color_fill(const ipe::Color& color);

    void set_color_type(const PATH_TYPE &type);
    void set_opacity(const int &opacity);
    void set_current_layer(const std::string &layer_name);
    void set_dashed(const std::string &dashed);
    void set_line_width(const double &val);
    void set_arrow_size(const double &val);
    void reset_attribute();

    void remove_object(const int &id);

private:
    void load_style();
    void set_layout();
    void style_size();
    void init_scale(const double &width, const double &height, const bool &keep_ratio);

    void draw_arrow_axis(const ipe::Vector &pt1, const ipe::Vector &pt2);

    enum AXIS_SENS{AXIS_VERTICAL,AXIS_HORIZONTAL};
    void draw_axis_number(const double &number, const ipe::Vector &pos, const AXIS_SENS &sens);
    void draw_axis_numbers();

    double trunc_inf(double x)
    {
        return (x == POS_INFINITY ? IPE_BOUNDED_INFINITY : (x == NEG_INFINITY ? -IPE_BOUNDED_INFINITY : x));
    }


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
    double m_offset_x=0.0, m_offset_y=0.0; // Adding offset to (0,0)
    double m_offset_drawing_x=0.0, m_offset_drawing_y=0.0;
    ipe::Matrix m_transform_global; // transformation offset + zoom
    ipe::Matrix m_transform_global_keep_dimension; // transformation offset without zoom
    ipe::Matrix m_transform_global_keep_y; // transformation offset + zoom
    ipe::Matrix m_transform_global_keep_dimension_keep_y; // transformation offset without zoom
    bool m_inversion_y = false;
    double m_width, m_height;
    bool m_keep_ratio;

    // Ipe objects
    ipe::Document   * m_document;
    ipe::Cascade    * m_cascade_ref;
    ipe::Page       * m_page;
    ipe::Layout     m_layout;
    ipe::StyleSheet * m_steel_sheet;

    ipe::AllAttributes m_current_attr;
    int                 m_current_layer=1;

    // Ipe parameters
    std::string m_ref_document = "/home/julien-damers/Libraries/dev/Debug/ipegenerator/style/basic.ipe";
    double m_thickness_pen_factor = 1e-3; // thickness of pen
    double m_arrow_axis_size = 4.294; // Corresponds to /normalsize in latex
    double m_distance_axis_text = 3.0;
    double m_distance_number_graduation = 2.0;
    double m_size_axis_graduation = 3.0;
    double m_general_offset = 0.0;
    bool   m_scale_offset = true;
    size_t m_number_digits_axis_x = 3;
    size_t m_number_digits_axis_y = 3;

    double m_start_number_graduation_x = 0.0;
    double m_start_number_graduation_y = 0.0;
    double m_inter_graduation_x = 1.0;
    double m_inter_graduation_y = 1.0;

    // Latex
    bool m_contain_latex = false;

    //tube display
    codac::Interval m_restricted_tdomain = codac::Interval(codac::Interval::ALL_REALS);
    unsigned int m_tube_max_nb_disp_slices = TUBE_MAX_NB_DISPLAYED_SLICES;
};

inline void Figure::add_layer(const std::string &layer_name)
{
    m_page->addLayer(layer_name.c_str());
    m_page->setVisible(0, layer_name.c_str(), true);
}

inline void Figure::set_visible(const std::string &layer_name, bool visible)
{
    m_page->setVisible(0, layer_name.c_str(), visible);
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

inline void Figure::set_distance_axis_text(const double &val)
{
    m_distance_axis_text = val;
}

inline void Figure::set_arrow_axis_size(const double &val)
{
    m_arrow_axis_size = val;
}

inline void Figure::set_distance_number_graduation(const double &distance_number_graduation)
{
    m_distance_number_graduation = distance_number_graduation;
}

inline void Figure::set_size_axis_graduation(const double& size_axis_graduation)
{
    m_size_axis_graduation = size_axis_graduation;
}

inline void Figure::set_graduation_parameters(const double &start_x, const double &inter_x, const double &start_y, const double &inter_y)
{
    m_start_number_graduation_x = start_x;
    m_inter_graduation_x = inter_x;
    m_start_number_graduation_y = start_y;
    m_inter_graduation_y = inter_y;
}

inline void Figure::set_scale_offset(const bool &enable)
{
	m_scale_offset = enable;
}

inline void Figure::set_number_digits_axis_x(const size_t &val)
{
    m_number_digits_axis_x = val;
}

inline void Figure::set_number_digits_axis_y(const size_t &val)
{
    m_number_digits_axis_y = val;
}

}
#endif // FIGURE_H
