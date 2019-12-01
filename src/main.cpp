#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "omp.h"

#include "ipelib.h"
#include "ibex_IntervalVector.h"

#include "figure.h"

using namespace std;


void test1()
{
    ibex::IntervalVector frame_data(2);
    frame_data[0] = ibex::Interval(0.0, 1000);
    frame_data[1] = ibex::Interval(-0.3, 2.0);

    ipegenerator::Figure fig(frame_data, 120, 50);

    fig.set_graduation_parameters(0.0, 100.0, 0.0, 0.5);
    fig.draw_axis("x_1", "x_2");

    ibex::IntervalVector box(2);
    box[0] = ibex::Interval(100.0, 300.0);
    box[1] = ibex::Interval(1.0, 1.5);
    fig.draw_box(box);

    fig.draw_text("test", 100, 1.0);

    vector<double> x, y;
    for(double t=0.0; t<1000.0; ++t)
    {
        x.push_back(t);
        y.push_back(sin(t/100.));
    }
    fig.draw_curve(x, y);

    fig.set_color_fill("black");
    fig.set_color_stroke("red");
    fig.set_color_type(ipegenerator::STROKE_AND_FILL);
    fig.set_opacity(30);
    fig.draw_circle(10, 1.0, 0.5);

    fig.draw_circle_radius_final(100.0, 0.5, 10.0);

    fig.set_dashed("dotted");
    fig.draw_sector(500.0, 1.0, 100.0, 1.0, 0.0, M_PI_2);

    fig.add_layer("test");
    fig.set_visible("test");

    fig.save_ipe("test.ipe");
    fig.save_pdf("test.pdf");
}

void test2()
{
    ibex::IntervalVector frame_data(2);
    frame_data[0] = ibex::Interval(0.0, 10.0);
    frame_data[1] = ibex::Interval(0.0, 10.0);

    ipegenerator::Figure fig(frame_data, 200, 200);

    fig.draw_float(0.0, 0, 0.0, 0.0);
    fig.draw_float(1.0, 0, 1.0, 0.0);
    fig.draw_float(2.0, 0, 0.0, 1.0, ipegenerator::FLOAT_PISTON_DOWN);
    fig.draw_float(3.0, 0, 1.0, 1.0);
    fig.draw_float(5.0, 0, 1.0, 1.0, ipegenerator::FLOAT_PISTON_UP, 0.2);

    fig.draw_symbol(2.0, 2.0, "disk(sx)", 5.0);

    fig.save_ipe("test.ipe");
    fig.save_pdf("test.pdf");
}

void test3(){
    ibex::IntervalVector frame_data(2);
    frame_data[0] = ibex::Interval(0.0, 1000);
    frame_data[1] = ibex::Interval(-10.0, 2.0);

    ipegenerator::Figure fig(frame_data, 120, 50);

    fig.set_inverted_y();
    fig.set_number_digits_axis_x(0);
    fig.set_number_digits_axis_y(1);
    fig.set_graduation_parameters(0.0, 100.0, -10.0, 2.0);
    fig.draw_axis("x_1", "x_2");

    ibex::IntervalVector box(2);
    box[0] = ibex::Interval(100.0, 300.0);
    box[1] = ibex::Interval(1.0, 1.5);
    fig.draw_box(box);

    fig.save_ipe("test.ipe");
}

int main(int argc, char *argv[])
{
//    test1();
//    test2();
    test3();
}

